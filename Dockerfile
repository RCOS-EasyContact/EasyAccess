FROM alpine:3

LABEL UPSTREAM="https://github.com/RCOS-EasyContact/EasyAccess"

RUN apk add --no-cache build-base

COPY Init.c Init.c

RUN gcc -o Init.out -Ofast Init.c

FROM alpine:3

RUN apk add --no-cache openconnect squid

EXPOSE 3128/tcp

ENV RPI_RCSID=REPLACE_ME \
    RPI_PASSWORD=REPLACE_ME

COPY .FILES/squid.conf /etc/squid/squid.conf

COPY --from=0 /Init.out /usr/bin/init

HEALTHCHECK --interval=5m --timeout=30s --start-period=2m --retries=0 \
        CMD /usr/bin/rtHealthCheck

ENTRYPOINT ["/usr/bin/init"]
