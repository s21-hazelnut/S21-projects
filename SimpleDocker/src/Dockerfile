FROM nginx

COPY ./server/nginx/nginx.conf /etc/nginx/
COPY ./server/main.c .
COPY ./server/main.sh .

RUN apt-get update -y; \
    apt-get install -y gcc \
    libfcgi-dev \
    spawn-fcgi \
    && chown -R nginx:nginx /etc/nginx/ \
    && touch /var/run/nginx.pid \
    && chown -R nginx:nginx /var/run/nginx.pid \
    && chown -R nginx:nginx /var/cache/nginx/ \
    && chown -R nginx:nginx /var/run/ \
    && gcc main.c -o server.fcgi -lfcgi \
    && rm -rf /var/lib/apt/lists \
    # part_5
    && chmod u-s /bin/su \
     && chmod u-s /usr/bin/passwd \
     && chmod u-s /usr/bin/chsh \
     && chmod u-s /usr/bin/gpasswd \
     && chmod u-s /usr/bin/chfn \
     && chmod u-s /usr/bin/newgrp \
     && chmod u-s /bin/mount \
     && chmod u-s /bin/umount \
     && chmod g-s /sbin/unix_chkpwd \
     && chmod g-s /usr/bin/wall \
     && chmod g-s /usr/bin/chage \
     && chmod g-s /usr/bin/expiry \
     && chown -R nginx:nginx /etc/nginx/ \
     && touch /var/run/nginx.pid \
     && chown -R nginx:nginx /var/run/nginx.pid \
     && chown -R nginx:nginx /var/cache/nginx/ \
     && chown -R nginx:nginx /var/run/ \
     && gcc main.c -o server.fcgi -lfcgi \
     && rm -rf /var/lib/apt/lists ;


RUN chmod a+x server.fcgi && chown -R nginx:nginx server.fcgi ;

USER nginx

HEALTHCHECK --interval=5m --timeout=3s \
CMD curl -f http://localhost:81/ || exit 1

ENTRYPOINT ["/main.sh"]
