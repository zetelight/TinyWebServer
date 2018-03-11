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
#include <pthread.h>

/* thread  routine */
void *connectTread(void *client_fd)
{
    int fd = *((int *)client_fd);
    printf("fd %d is serving\n", fd);
    // detach itself
    //pthread_detach(pthread_self());
    serve(fd);
    close(fd);
    pthread_exit(NULL);
}

int main(int argc, char *argv[])
{
    int server_fd;                  /* server file descriptor    */
    int client_fd;                  /* client file descriptor    */
    int port;                       /* port number               */
    struct sockaddr_in client_addr; /* client address            */
    struct sockaddr_in server_addr; /* server address            */
    int id = 0;
    socklen_t client_addr_len = sizeof(client_addr);

    /* Thread pool */
    pthread_t tid[MAX_THREADS];

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
        //pthread_t thread_id;
        printf("client_fd %d is serving\n", client_fd);
        if (client_fd < 0)
        {
            printf("Accepting a request failed\n");
            exit(EXIT_FAILURE);
        }
        else
        {
            //if (fork() == 0)
            //{
            /* Create a thread */
            if (pthread_create(&tid[id], NULL, connectTread, (void *)&client_fd) < 0)
            {
                printf("Creating a thread failed\n");
                exit(EXIT_FAILURE);
            }
            printf("thread %lu is running\n", (unsigned long)tid[id]);
            id++;
            //serve(client_fd);
            //close(client_fd);
            //pthread_join(thread_id, NULL);
            //exit(0);
            // }
            // else
            // {
            //     wait(NULL);
            // }
        }
    }
    /* closen server socket */
    close(server_fd);

    return 0;
}