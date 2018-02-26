# Week 6

we didn't implement any codes.

The reasons are:

1. we need learn some knowledage about webserver first (This step takes more time)
2. we have to organzie the project architecture first

The reference we used are including but not limited to these links below:

* https://renenyffenegger.ch/notes/web/webserver/cpp/simple/index
* http://www.aosabook.org/en/500L/a-simple-web-server.html
* https://ruslanspivak.com/lsbaws-part1/
* https://stackoverflow.com/questions/2338775/how-do-i-write-a-web-server-in-c-c-on-linux

Understanding the logic is pretty important.
So far, as we know, we have to implement some parts below:

1. create a socket 
2. bind the socket with host and port
3. socket listens to http request
4. server accept the request and connect with socket 
5. interpret the request and send the response to browser
6. browser display the content