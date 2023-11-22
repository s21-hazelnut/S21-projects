#!/bin/bash

sudo service nginx stop
sudo docker rm -f $(sudo docker ps -aq)
sudo docker image prune -a