#!/bin/bash

sudo apt update -y
sudo apt install -y nginx 
sudo apt-get install -y libfcgi libfcgi-dev
sudo apt install -y spawn-fcgi

sudo cp nginx/nginx.conf /etc/nginx/

./main.sh
