#include <stdlib.h>
#include <stdio.h>
#include <string.h> // needed for memcpy
#include <inttypes.h>

#include "modified_page_table.h"
#include "da.h"
#include "disk.h"
#include "int.h"


static int page_fault_handler(struct page_table *pt, int page);
static int getFreeFrame(struct page_table *pt);
static ENTRY *LRU(PAGE_TABLE *pt);
static void setPageTable(DA *page_table,int page_num,int f, signed char *p);

struct page_table {
  DA *table;
  int npages;
  int nframes;
  int num_page_faults;

  DA *free_frames;
};

PAGE_TABLE *
newPAGE_TABLE(int np,int nf) {
  PAGE_TABLE *pt = malloc(sizeof(PAGE_TABLE));
  if(pt == 0) {
    fprintf(stderr,"out of memory\n");
    exit(1);
  }
  pt->npages = np;
  pt->nframes = nf;
  pt->num_page_faults = 0;
  pt->table = newDA(displayENTRY);

  for(int i = 0; i < np; i++) {
    insertDA(pt->table,newENTRY(-1,0,i));
  }

  pt->free_frames = newDA(displayINT);
  for(int x = 0; x < nf; x++) {
    insertDA(pt->free_frames,newINT(1));
  }

  return pt;
}

int getNumPageFaults(PAGE_TABLE *pt) {
  return pt->num_page_faults;
}

int getFrameForPage(PAGE_TABLE *pt, int page) {
	if( page<0 || page>=pt->npages ) {
		fprintf(stderr,"page_table_get_entry: illegal page #%d\n",page);
		abort();
	}
  //printf("Getting frame for page %d\n",page);
  int page_frame = getFrame(getDA(pt->table,page));
  //printf("Frame is %d\n",page_frame);
  if(page_frame == -1) {
    page_frame = page_fault_handler(pt,page);
    return page_frame;
  }
  else {
     //printf("Found frame for page %d->%d\n",page,page_frame);
     return page_frame;
  }
}

/* This thing is super duper important! */
static int page_fault_handler(PAGE_TABLE *pt, int page) {
	//printf("page fault on page #%d\n",page);
  pt->num_page_faults++;
  // choose a free frame
  int free_frame = getFreeFrame(pt);
  //printf("Free frame is %d\n",free_frame);
  if(free_frame == -100) {
    //printf("NO FRAMES EMPTY. Time to LRU!**********************\n");
    /* b. If there is no free frame, use a page-replacement algorithm to select
       a victim frame.
       c. Write the victim frame to the disk; change the page and frame tables
       accordingly.
       3. Read the desired page into the newly freed frame; change the page and
       frame tables.*/
    ENTRY *lru = LRU(pt);
    //printf("Returned time is %d\n",getTimeLastAccessed(lru));
    free_frame = getFrame(lru);
    //printf("Page is: ");
    //displayENTRY(stdout,lru);
    //printf("\n");
    //writePage(getPageNumber(lru),getPage(lru));
    setPageTable(pt->table,getPageNumber(lru),-1,NULL); // free page
    setValid(lru, 0);
    setDA(pt->free_frames,free_frame,newINT(1)); // free frame
    signed char *page_data = loadPage(page);
    setPageTable(pt->table,page,free_frame,page_data);
    setDA(pt->free_frames,free_frame,newINT(0));
    return free_frame;
  }
  else {
    // map frame to page & load page into memory
    signed char *page_data = loadPage(page);
    setENTRY(getDA(pt->table,page),free_frame,page_data);
    return free_frame;
  }
}

static void setPageTable(DA *page_table,int page_num,int f, signed char *p) {
  for(int x = 0; x < sizeDA(page_table); x++) {
    ENTRY *e = getDA(page_table,x);
    if(getPageNumber(e) == page_num) {
      setENTRY(e,f,p);
    }
  }
}

static ENTRY *LRU(PAGE_TABLE *pt) {
  //printf("In LRU.\n");
  ENTRY *lru = NULL;
  for(int x = 0; x < sizeDA(pt->table);x++) {
    lru = getDA(pt->table,x);
    if(isValid(lru))
      break;
  }
  //printf("Oldest reference time is %d\n",getTimeLastAccessed(lru));
  for(int x = 1; x < sizeDA(pt->table);x++) {
    ENTRY *candidate = getDA(pt->table,x);
    if(isValid(candidate) && getTimeLastAccessed(candidate) < getTimeLastAccessed(lru)) {
      lru = candidate;
      //printf("Found new lru!\n");
      //printf("Oldest reference time is now %d\n",getTimeLastAccessed(lru));
    }
  }
  if(lru == NULL) printf("crap.\n");
  return lru;
}

static int getFreeFrame(PAGE_TABLE *pt) {
  int frame = 0;
  for(int x = 0; x < sizeDA(pt->free_frames); x++) {
    frame = getINT(getDA(pt->free_frames,x));
    //printf("Inside getFreeFrame, frame is %d\n",frame);
    if(frame == 1) {
      //printf("Found free frame!\n");
      setDA(pt->free_frames,x,newINT(0));
      return x;
    }
  }
  //printf("Couldn't find free frame.\n");
  return -100;
}

extern void printPAGE_TABLE(PAGE_TABLE *pt) {
  //printf("Number pages: %d\nNumber frames: %d\n",pt->npages,pt->nframes);
  for(int x = 0; x < sizeDA(pt->table); x++) {
    ENTRY *curr_e = getDA(pt->table,x);
    //printf("Page %d: ",x);
    displayENTRY(stdout,curr_e);
    //printf("\n");
  }
}

extern ENTRY *getEntry(PAGE_TABLE *pt,int page) {
  return getDA(pt->table,page);
}
// values are stored as signed bytes
extern signed int getValueFromDisk(PAGE_TABLE *pt,int page,int offset) {
  ENTRY *e = getEntry(pt,page);
  signed char *page_data = getPage(e);
  int8_t byte_value;
  // copy a byte into value at offset
  memcpy((char *)&byte_value, &page_data[offset], sizeof(int8_t));

  //printf("Got value %" PRId8 "\n",byte_value);
  return (int)byte_value;
}
