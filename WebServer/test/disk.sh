#!/bin/bash
echo "Content-type: text/html"
echo ''
echo 'CGI Bash example to show dynamic contents<br>'
echo `df -h / | grep -v Filesystem`