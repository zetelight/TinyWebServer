#include <stdio.h>
#include <ctype.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/stat.h>
#include "server.h"

#define PORT 5000
#define MAX_QUEUE 5 // the size of listen queue
#define BUFF_SIZE 1024
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{
    int server_fd;                  /* server file descriptor    */
    int client_fd;                  /* client file descriptor    */
    int port;                       /* port number               */
    struct sockaddr_in client_addr; /* client address            */
    struct sockaddr_in server_addr; /* server address            */
    //char buf[BUFF_SIZE];            /* Buffer to store html info */
    socklen_t client_addr_len = sizeof(client_addr);

    /* Check arguments */
    if (argc == 2)
    {
        port = atoi(argv[1]);
    }
    else if (argc != 2)
    {
        port = PORT;
    }

    /* Create a socket for server*/
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0)
    {
        printf("Creating a server socket failed\n");
        exit(EXIT_FAILURE);
    }
    memset(&server_addr, 0, sizeof(server_addr));

    /* Set hostname, ip and sin_family*/
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    /* Bind socket witht the address */
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        printf("Binding socket failed\n");
        exit(EXIT_FAILURE);
    }

    /* Socket listen to request */
    if (listen(server_fd, MAX_QUEUE) < 0)
    {
        printf("Listening socket failed\n");
        exit(EXIT_FAILURE);
    }

    /* Start to listen request forever */
    while (TRUE)
    {
        /* Wait and accept for a request */
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0)
        {
            printf("Accpeting a request failed\n");
            exit(EXIT_FAILURE);
        }

        /* Send response to client*/
        // hardcode the HTTP request
        rio_t rio;
        struct stat sbuf;
        char buf[MAXLINE], command[MAXLINE], uri[MAXLINE], version[MAXLINE], request[MAXLINE], write_in[MAXBUF];
        char filename[MAXLINE];
        Rio_readinitb(&rio,client_fd);
        Rio_readlineb(&rio,buf,MAXLINE);
        sscanf(buf, "%s %s %s", command, uri, version);
        if (strcasecmp(command,"GET")){
            printf("Not implemented this command\n");
            exit(EXIT_FAILURE);
        }
        Rio_readlineb(&rio, request, MAXLINE);
        while(strcmp(request,"\r\n")){
            Rio_readlineb(&rio, request, MAXLINE);
            printf("%s", request);
        }

        strcpy(filename, ".");
        strcat(filename, uri);
        if (uri[strlen(uri)-1] == '/'){
            strcat(filename, "home.html");
        }

        if (stat(filename,&sbuf) < 0){
            printf("File not found\n");
            close(client_fd);
            continue;
        }

        if (!(S_ISREG(sbuf.st_mode))|| !(S_IRUSR & sbuf.st_mode)){
            printf("no permission to read this file\n");
            close(client_fd);
            continue;
        }
        char filetype_default[MAXLINE];
        strcpy(filetype_default, "text/html");
        sprintf(write_in, "HTTP/1.0 200 OK\r\n");
        sprintf(write_in, "%sServer: Tiny Web Server\r\n", write_in);
        sprintf(write_in, "%sContent-length: %ld\r\n", write_in, sbuf.st_size);
        sprintf(write_in, "%sContent-type: %s\r\n\r\n", write_in, filetype_default);
        Rio_writen(client_fd, write_in, strlen(write_in));

        int source = open(filename, O_RDONLY, 0);
        char *scptr = mmap(0, sbuf.st_size, PROT_READ, MAP_PRIVATE, source, 0);
        close(source);
        Rio_writen(client_fd, scptr, sbuf.st_size);
        munmap(scptr, sbuf.st_size);


        /* closen client socket */
        close(client_fd);
    }
    /* closen server socket */
    close(server_fd);

    return 0;
}