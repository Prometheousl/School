#ifndef DISK_H
#define DISK_H

#define BLOCK_SIZE 256

extern signed char * loadPage(int);
extern void writePage(int page_num, signed char *page_data);

#endif
