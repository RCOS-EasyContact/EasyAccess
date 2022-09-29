FROM alpine:3

RUN apk add --no-cache build-base

COPY Init.c Init.c

RUN gcc -o Init.out -Ofast Init.c

FROM alpine:3

RUN apk add --no-cache openconnect \
 && apk add --no-cache squid

EXPOSE 3128/tcp

ENV RPI_RCSID=REPLACE_ME \
    RPI_PASSWORD=REPLACE_ME \
    VPN_KEEPALIVE=300

COPY squid.conf /etc/squid/squid.conf

COPY --from=0 /Init.out /usr/bin/init

ENTRYPOINT ["/usr/bin/init"]
