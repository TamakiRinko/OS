#include "types.h"
#include "utils.h"
#include "lib.h"


union DirEntry {
	uint8_t byte[128];
	struct {
		int32_t inode;
		char name[64];
	};
};

typedef union DirEntry DirEntry;

int ls(char *destFilePath) {
	// STEP 8
    // TODO: ls
   	printf("ls %s\n", destFilePath);
   	int ret = 0;
	int fd = -1;
	DirEntry dirEntry;
	//printf("size = %d\n", sizeof(DirEntry));
	//char buffer[64] = "\0";
	fd = open(destFilePath, O_DIRECTORY | O_READ);
	//printf("fd = %d\n", fd);
	if(fd < 4){
		return -1;
	}
	while((ret = read(fd, (uint8_t*)(&dirEntry), sizeof(DirEntry))) > 0){
		//printf("hahahah\n");
		if(dirEntry.inode == 0){
			continue;
		}
		printf("%s ", dirEntry.name);
	}
	//printf("hohoho\n");
	printf("\n");
	ret = close(fd);
	//printf("last ret = %d\n", ret);
	return 0;
}

int cat(char *destFilePath) {
	// STEP 9
    // TODO: ls
    /** output format
    cat /usr/test
    ABCDEFGHIJKLMNOPQRSTUVWXYZ
    */
   	printf("cat %s\n", destFilePath);
    int ret = 0;
	int fd = -1;
	char buffer[1024] = "\0";
	//printf("size = %d\n", sizeof(DirEntry));
	//char buffer[64] = "\0";
	fd = open(destFilePath, O_READ);
	//printf("fd = %d\n", fd);
	if(fd < 4){
		return -1;
	}
	while((ret = read(fd, (uint8_t*)(buffer), 1024)) > 0){
		//printf("hahahah\n");
		//printf("ret = %d\n", ret);
		for(int i = 0; i < ret; i++){
			printf("%c", buffer[i]);
		}
	}
	close(fd);
	//printf("\n");
	return 0;
}

int uEntry(void) {
    // STEP 2-9
    // TODO: try different test case when you finish a function
    
    /* filesystem test */
/*
	int fd = 0;
	int i = 0;
	char tmp = 0;
	//int fd2 = 0;
	//int fd3 = 0;
	//int fd4 = 0;
	int ret = 0;
	printf("create /usr/test and write alphabets to it\n");
	fd = open("/usr/test", O_WRITE | O_READ | O_CREATE);
	//fd2 = open("/usr/test2", O_WRITE | O_CREATE);
	//fd3 = open("/usr/test3", O_WRITE);
	//fd4 = open("/usr/test2", O_WRITE);
	//printf("fd = %d\nfd2 = %d\nfd3 = %d\nfd4 = %d\n", fd, fd2, fd3, fd4);
	char buffer[20] = "123456789";
	char result[30] = "\0";
	char result2[30] = "\0";
	ret = write(fd, (uint8_t*)buffer, 7);
	printf("ret = %d\n", ret);
	lseek(fd, -3, 1);

	//fd3 = open("/usr/test", O_WRITE | O_READ);
	ret = read(fd, (uint8_t*)result, 2);
	printf("read ret = %d, result = %s\n", ret, result);
	//write(fd, (uint8_t*)"Hello, World!\n", 14);
	//write(fd, (uint8_t*)"This is a demo!\n", 16);
	//for (i = 0; i < 2049; i ++) {
	//for (i = 0; i < 2048; i ++) {
	//for (i = 0; i < 1025; i ++) {
	//for (i = 0; i < 512; i ++) {



	
	
	for (i = 0; i < 26; i ++) {
		tmp = (char)(i % 26 + 'A');
		write(fd, (uint8_t*)&tmp, 1);
	}
	ret = remove("/usr/test");
	printf("in use, remove = %d\n", ret);
	ret = close(fd);
	printf("close, ret = %d\n", ret);
	//ret = remove("/usr/test");
	//printf("not in use, remove = %d\n", ret);

	fd = open("/usr/test", O_WRITE | O_READ);
	printf("last, fd = %d\n", fd);
	ret = read(fd, (uint8_t*)result2, 20);
	printf("read ret = %d, result2 = %s\n", ret, result2);
*/

/*
	ls("/");
	ls("/boot/");
	ls("/dev/");
	ls("/usr/");

	int fd = -1;
	int i = 0;
	char tmp = 0;
	//int ret = 0;
	printf("create /usr/test and write alphabets to it\n");
	fd = open("/usr/test", O_WRITE | O_READ | O_CREATE);
	for (i = 0; i < 26; i ++) {
		tmp = (char)(i % 26 + 'A');
		write(fd, (uint8_t*)&tmp, 1);
	}
	close(fd);
	ls("/usr/");
	cat("/usr/test");
	printf("\n");
	printf("rm /usr/test\n");
	remove("/usr/test");
	ls("/usr/");
	printf("rmdir /usr/\n");
	remove("/usr/");
	//remove("/dev");
	ls("/");
	//ls("/dev");
	printf("create /usr/\n");
	fd = open("/usr/", O_CREATE | O_DIRECTORY);
	//printf("fd = %d\n", fd);
	close(fd);
	ls("/");
	//fd = open("/usr/test", O_WRITE | O_READ);
	//close(fd);
	//ls("/usr");
	//fd = open("/usr/test/", O_CREATE);
	//close(fd);
	//ls("/usr/");
	//fd = open("/usr/test", O_CREATE);
	//close(fd);
	//ls("/usr/");

	
	
	
	exit();
	return 0;
*/

    // STEP 2-9
    // TODO: try different test case when you finish a function
    
    /* filesystem test */
	int fd = 0;
	int i = 0;
	char tmp = 0;
	
	ls("/");
	ls("/boot/");
	ls("/dev/");
	ls("/usr/");

	printf("create /usr/test and write alphabets to it\n");
	fd = open("/usr/test", O_WRITE | O_READ | O_CREATE);
	//write(fd, (uint8_t*)"Hello, World!\n", 14);
	//write(fd, (uint8_t*)"This is a demo!\n", 16);
	//for (i = 0; i < 2049; i ++) {
	//for (i = 0; i < 2048; i ++) {
	//for (i = 0; i < 1025; i ++) {
	//for (i = 0; i < 512; i ++) {
	for (i = 0; i < 26; i ++) {
		tmp = (char)(i % 26 + 'A');
		write(fd, (uint8_t*)&tmp, 1);
	}
	close(fd);
	ls("/usr/");
	cat("/usr/test");
	printf("\n");
	printf("rm /usr/test\n");
	remove("/usr/test");
	ls("/usr/");
	printf("rmdir /usr/\n");
	remove("/usr/");
	//remove("/dev");
	ls("/");
	//ls("/dev");
	printf("create /usr/\n");
	fd = open("/usr/", O_CREATE | O_DIRECTORY);
	close(fd);
	ls("/");
	//fd = open("/usr/test", O_WRITE | O_READ);
	//close(fd);
	//ls("/usr");
	//fd = open("/usr/test/", O_CREATE);
	//close(fd);
	//ls("/usr/");
	//fd = open("/usr/test", O_CREATE);
	//close(fd);
	//ls("/usr/");
	
	exit();
    /**
    Output format can be found in 
    http://114.212.80.195:8170/2019oslab/lab5/
    */
	return 0;
	
}
