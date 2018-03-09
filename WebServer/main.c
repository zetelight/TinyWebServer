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
#include <unistd.h>
#include <pthread.h>

#define PORT 5000
#define MAX_QUEUE 5 // the size of listen queue
#define BUFF_SIZE 8192
#define TRUE 1
#define FALSE 0
#define ADDRESS "test/trivia.html"

void *connectTread(void *id)
{
    int client_fd = *(int *)&id;
    char buf[BUFF_SIZE]; /* Buffer to store html info */
    pthread_detach(pthread_self());
    while (1)
    {
        /* Send response to client*/
        sprintf(buf, "HTTP/1.0 200 OK\r\n");
        send(client_fd, buf, strlen(buf), 0);
        sprintf(buf, "Content-Type: text/html\r\n");
        send(client_fd, buf, strlen(buf), 0);
        strcpy(buf, "\r\n");
        send(client_fd, buf, strlen(buf), 0);

        /* Send the HTTP content*/
        sprintf(buf, "YES. I AM NOT JOKING! \r\n");
        send(client_fd, buf, strlen(buf), 0);
    }
    /* closen client socket */
    close(client_fd);
    pthread_exit(NULL);
    return NULL;
}

int main(int argc, char *argv[])
{
    int server_fd;                  /* server file descriptor    */
    int client_fd;                  /* client file descriptor    */
    int port;                       /* port number               */
    struct sockaddr_in client_addr; /* client address            */
    struct sockaddr_in server_addr; /* server address            */
    socklen_t client_addr_len = sizeof(client_addr);

    /* Check arguments */
    /*
    Expected arguments: 
    */
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
        exit(1);
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
        exit(1);
    }

    /* Socket listen to request */
    if (listen(server_fd, MAX_QUEUE) < 0)
    {
        printf("Listening socket failed\n");
        exit(1);
    }
    pthread_t thread_id;

    /* Start to listen request forever */
    while (TRUE)
    {
        /* Wait and accept for a request */
        client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_fd < 0)
        {
            printf("Accpeting a request failed\n");
            exit(1);
        }
        // Create a new thread
        int t_id;
        t_id = pthread_create(&thread_id, NULL, connectTread, (void *)&client_fd);
        if (t_id < 0)
        {
            perror("Creating threads failed");
        }
        else
        {
            printf("Thread %d is running \n", t_id);
        }
    }
    /* closen server socket */
    close(server_fd);

    return 0;
}