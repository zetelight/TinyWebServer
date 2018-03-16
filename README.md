# CIS 330 GROUP PROJECT - Simple Web server

Author: Xin Chen, Mantai Lin

## Basic features

1. handle http request (GET)
2. interpret http request
3. send the http response to server
4. able to display content, including plain text, photos on browser
5. provide the ability to handle single thread, single process, and one connection per thread.

## The advanced features of this webserver

1. interpret simple web pages, including some html or css elements
2. Implement CGI
3. process multi-threads.

## Usage

```shell
git clone https://zetecx@bitbucket.org/zetecx/cis330-project.git
cd CIS330-Project/WebServer
make main
make run
```
## Note

1. The default port is 5000 but user could run the project by 
```
./main 8000(port number)
```
2. We provide links for all testable html pages within index.html
3. We can handle some 404/403 errors, such as invalid url, or urls which have wrong formats. 