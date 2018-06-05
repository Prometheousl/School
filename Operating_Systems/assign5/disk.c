#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>

signed char *loadPage(int page_num) {
	FILE *disk = fopen("BACKING_STORE.bin","r");
	signed char *page = malloc(sizeof(signed char) * 256);

	// set the file pointer to the frame
	fseek(disk,256 * page_num,SEEK_SET);
	// read 256 bytes from disk at frame & store in page
	fread(page,sizeof(signed char),256,disk);

	fclose(disk);

	//printf("Page from disk is %s\n",page);
	return page;
}

void writePage(int page_num, signed char *page_data) {
	FILE *disk = fopen("BACKING_STORE.bin","w");

	fseek(disk,256 * page_num,SEEK_SET);

	fwrite(page_data,sizeof(signed char),256,disk);

	fclose(disk);
	//printf("Successfully wrote to disk!\n");
}
