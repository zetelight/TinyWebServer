# CIS 330 GROUP PROJECT - Simple Web server

Author: Xin Chen, Linman Tai

## Basic features

1. handle http request (GET)
2. interpret http request
3. send the http response to server
4. able to display content on browser
5. provide the ability to handle single thread, single process, and one connection per thread.

## The advanced features of this webserver

1. interpret simple web pages, including some html or css elements
2. process multi-threads.
3. process more complex content, e.g. streaming video.

## Usage

```shell
git clone https://zetecx@bitbucket.org/zetecx/cis330-project.git
cd CIS/330-Project/WebServer
make main
make run

```

## TODO List

* Write a request handler to interpret a HTML page
* Write a method too determine the type of page
* Modulize the project
* Implement CGI
* Multi-threads?