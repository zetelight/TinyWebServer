#define _XOPEN_SOURCE
#define _DEFAULT_SOURCE
#include "server.h"
#include "rio.h"
struct
{
    char *ext;
    char *filetype;
} extensions[] = {
    {"gif", "image/gif"},
    {"jpg", "image/jpeg"},
    {"jpeg", "image/jpeg"},
    {"png", "image/png"},
    {"zip", "application/octet-stream"},
    {"gz", "image/gz"},
    {"tar", "image/tar"},
    {"htm", "text/html"},
    {"html", "text/html"},
    {"php", "text/html"},
    {"cgi", "text/cgi"},
    {"asp", "text/asp"},
    {"jsp", "image/jsp"},
    {"xml", "text/xml"},
    {"js", "text/js"},
    {"css", "test/css"},
    {0, 0}};
/*function for serving static content, basic check the filetype then 
  read through this file, then send it to the client socket.
  */
void serve_static(int fd, char *filename, int filesize)
{
    int sourcefd;
    char *sourceptr, filetype[MAXLINE], buf[MAXLINE];
    filetype[0] = 0;
    getFileType(filename, filetype);
    /*check if this file is php, if it's, run the specific function to interpret it*/
    if (strstr(filename, ".php"))
    {
        php_cgi(filename, fd);
    }
    /*check if it's an available file for this server*/
    else if (strlen(filetype) == 0)
    {
        clientError(fd, 415, "Not Supported", "this file is not supported", filename);
        return;
    }
    /*other static contents*/
    else
    {
        printf("HTTP/1.1 200 OK, Content-type: %s\n", filetype);
        sprintf(buf, "HTTP/1.0 200 OK\r\n");
        sprintf(buf, "%sServer: Tiny Web Server\r\n", buf);
        sprintf(buf, "%sContent-type: %s\r\n", buf, filetype);
        sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, filesize);
        Rio_writen(fd, buf, strlen(buf));
    }

    /*open the source file*/
    sourcefd = open(filename, O_RDONLY, 0);
    if (sourcefd < 0)
    {
        printf("Open Failure\n");
        return;
    }
    /*mapping it*/
    sourceptr = mmap(0, filesize, PROT_READ, MAP_PRIVATE, sourcefd, 0);
    if (sourceptr == ((void *)-1))
    {
        printf("Map failure\n");
        return;
    }
    close(sourcefd);
    Rio_writen(fd, sourceptr, filesize);
    munmap(sourceptr, filesize);
}

/*function for dynamic content, using CGI*/
void serve_dynamic(int fd, char *filename, char *cgiargs)
{
    char buf[MAXLINE], *emptylist[] = {NULL};

    /* Return first part of HTTP response */
    sprintf(buf, "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    sprintf(buf, "Server: Tiny Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (fork() == 0)
    { /* child */
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1);
        dup2(fd, STDOUT_FILENO);              /* Redirect stdout to client */
        execve(filename, emptylist, environ); /* Run CGI program */
    }
    wait(NULL); /* Parent waits for and reaps child */
}

/*function for serving client, basic using R_IO function from textbook, which 
  using Rio_writen to write content to a socket*/
void serve(int fd)
{
    char request[MAXLINE], *ptr;
    char method[MAXLINE], uri[MAXLINE], version[MAXLINE];
    char filename[MAXLINE], cgiargs[MAXLINE];
    rio_t rio;
    struct stat sbuf;
    int is_static;
    char *ret;//use for check invalid address
    int status;//use for check stat of the file
    int badread;


    /*check if is a valid request*/
    Rio_readinitb(&rio, fd);
    
    badread = Rio_readlineb(&rio, request, MAXLINE);

    if(request[0] == '\0'){
        return;
    }

    if (badread == 0)
    {
        printf("Bad Read\n");
    }
    printf("%s\n", request);
    ptr = strstr(request, "HTTP/");//check if the request is http request
    if (ptr == NULL)
    {
        printf("NOT HTTP!\n");
    }
    else
    {
        *ptr = 0;
        ptr = NULL;
    }

    /*get the method, uri and version from the request header*/
    sscanf(request, "%s %s %s", method, uri, version);
    if (strcasecmp(method, "GET"))
    {
        printf("501 Method not implemented\n");
        clientError(fd, 501, "Method not implemented", "This tiny web server only implement GET method", method);
        return;
    }

    read_requesthdrs(&rio); //go through the request header

    /*check if the content is static*/
    is_static = (int)parsing(uri, filename, cgiargs);

    ret = strstr(filename,"//");
    if (ret == NULL){
        ret = strstr(filename, "~");
        if (ret == NULL){
            ret = strstr(filename, "..");
        }
    }

    status = stat(filename, &sbuf);

    if (ret != NULL){
        clientError(fd, 403, "No Permission", "Have no permission to read this file or you're not a irregular user for this file", filename);
            stat("./test/403.html", &sbuf);
            serve_static(fd, "./test/403.html", sbuf.st_size);
            return;
    }

    if (status < 0)
    {
        clientError(fd, 404, "File Not Found", "Server couldn't find this file", filename);
        stat("./test/404.html", &sbuf);
        serve_static(fd, "./test/404.html", sbuf.st_size);
        return;
    }

    if (is_static)//serve static
    {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))//check the stats of the file
        {
            clientError(fd, 403, "No Permission", "Have no permission to read this file or you're not a irregular user for this file", filename);
            stat("./test/403.html", &sbuf);
            serve_static(fd, "./test/403.html", sbuf.st_size);
            return;
        }
        serve_static(fd, filename, sbuf.st_size);
    }
    else//serve dynamic 
    {
        if (!(S_ISREG(sbuf.st_mode)) || !(S_IRUSR & sbuf.st_mode))
        {
            clientError(fd, 403, "No Permission", "Have no permission to read this file or you're not a irregular user for this file", filename);
            stat("./test/403.html", &sbuf);
            serve_static(fd, "./test/403.html", sbuf.st_size);
            return;
        }
        serve_dynamic(fd, filename, cgiargs);
    }
}

/*function for reporting an error*/
void clientError(int fd, int errnum, char *shortmsg, char *longmsg, char *cause)
{
    char buf[MAXLINE], body[MAXLINE];

    if (errnum == 404){//simply write the 404.html to client, finished in serve
        return;
    }
    else if (errnum == 403){//same as above but using 403.html
        return;
    }
    
    else{
        /*the header*/
        sprintf(body, "<html><head><title>%d %s</head></title>", errnum, shortmsg);
        sprintf(body, "%s<body><p>%d %s (%s): %s</p></body></html>", body, errnum, shortmsg, cause, longmsg);

        /*actual error message*/
        sprintf(buf, "%sHTTP/1.0 %d %s\r\n", buf, errnum, shortmsg);
        sprintf(buf, "%sContent-type: text/html\r\n", buf);
        sprintf(buf, "%sContent-length: %d\r\n\r\n", buf, (int)strlen(body));
        Rio_writen(fd, buf, strlen(buf));
        Rio_writen(fd, buf, strlen(body));

    }
    
    
}

/*copy the correct type to filetype*/
void getFileType(char *filename, char *filetype)
{
    for (int i = 0; extensions[i].ext != 0; i++)
    {
        if (strstr(filename, extensions[i].ext))//go through the list of extensions to check
        {
            strcpy(filetype, extensions[i].filetype);
            break;
        }
    }
}
/*read the request header*/
void read_requesthdrs(rio_t *rp)
{
    char buf[MAXLINE];

    Rio_readlineb(rp, buf, MAXLINE);
    while (strcmp(buf, "\r\n"))
    {
        Rio_readlineb(rp, buf, MAXLINE);
        printf("%s", buf);
    }
    return;
}

/*parsing the request*/
int parsing(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    /*this for static content*/
    if (!strstr(uri, "cgi-bin"))
    {
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
    
        if (uri[strlen(uri) - 1] == '/')//check if it's calling the home page
        {
            strcat(filename, "index.html");
        }
        return TRUE;
    }

    /*for dynamic content*/
    else
    {
        ptr = index(uri, '?');
        if (ptr)
        {
            strcpy(cgiargs, ptr + 1);
            *ptr = '\0';
        }
        else
        {
            strcpy(cgiargs, "");
        }
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}

/*the function for dealing with php content*/
void php_cgi(char *script_path, int fd)
{
    char buf[MAXLINE];
    /*send the header to client*/
    sprintf(buf, "HTTP/1.1 200 OK\n Server: Web Server in C\n Connection: close\n");
    Rio_writen(fd, buf, strlen(buf));

    /*dealing php with CGI*/
    dup2(fd, STDOUT_FILENO);
    char script[500];
    strcpy(script, "SCRIPT_FILENAME=");
    strcat(script, script_path);
    putenv("GATEWAY_INTERFACE=CGI/1.1");
    putenv(script);
    putenv("QUERY_STRING=");
    putenv("REQUEST_METHOD=GET");
    putenv("REDIRECT_STATUS=true");
    putenv("SERVER_PROTOCOL=HTTP/1.1");
    putenv("REMOTE_HOST=127.0.0.1");
    execl("/usr/bin/php-cgi", "php-cgi", NULL);
}
