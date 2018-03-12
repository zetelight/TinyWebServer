/*
* Author: Adam Chen
* Description: include head files releated about networking programming
*/
#ifndef SERVER_H_
#define SERVER_H_

#define PORT 5000
#define MAX_QUEUE 50 // the size of listen queue
#define BUFF_SIZE 8096
#define MAX_THREADS 200
#define TRUE 1
#define FALSE 0
#include "rio.h"

void serve_static(int fd, char *filename, int filesize);
void serve_dynamic(int fd, char *filename, char *cgiargs);
void clientError(int fd, int errnum, char *shortmsg, char *longmsg, char *cause);
void getFileType(char *filename, char *filetype);
void serve(int fd);
void read_requesthdrs(rio_t *rp);
int parsing(char *uri, char *filename, char *cgiargs);
void php_cgi(char *script_path, int fd);


#endif /* SERVER_H_ */