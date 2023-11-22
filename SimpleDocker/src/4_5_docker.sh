#!/bin/bash
#-----------------dockle insall------------------- #part5
VERSION=$(
    curl --silent "https://api.github.com/repos/goodwithtech/dockle/releases/latest" | \
    grep '"tag_name":' | \
    sed -E 's/.*"v([^"]+)".*/\1/' \
) && curl -L -o dockle.deb https://github.com/goodwithtech/dockle/releases/download/v${VERSION}/dockle_${VERSION}_Linux-64bit.deb
sudo dpkg -i dockle.deb && rm dockle.deb
#--------------------------------------------------
sudo docker build . -t my_name:my_tag -f Dockerfile
sudo docker images
sudo dockle -i CIS-DI-0005 --accept-key NGINX_GPGKEY my_name:my_tag  #part5
sudo docker run -dt -p 80:81 -v $(pwd)/server/nginx/nginx.conf:/etc/nginx/nginx.conf my_name:my_tag
