# Week 7

Progress: implement a very very simple webserver. Only can show plain text

We also encountered some problems below:

1. (SOLVED) So much materials are avaliable online but we didn't learn networking programming systematically so that we cannot orgranize these pieces together

    * For instance, we know that we need create a socket, bind, listen, accpet request from client and then response.
        * But how?
        * which headfiles?
        * What kinds of conventions?

2. (SOLVED) Bascially, we have to understand/imitate others' codes to implement our module, which means that it looks like we copy-paste others' codes. How do we avoid that?
    * People make rules for socket programming, so somwhow we need to follow them.

## Referecence

1. CSAPP chapter 11 & 12
2. http://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html
3. https://github.com/zyearn/zaver/blob/master/src/http_parse.c
4. http://www.paulgriffiths.net/program/c/srcs/echoservsrc.html
