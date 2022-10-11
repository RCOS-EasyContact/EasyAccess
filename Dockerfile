FROM alpine:3

RUN apk add --no-cache build-base

COPY Init.c Init.c

COPY RuntimeHealthCheck.c RuntimeHealthCheck.c

RUN gcc -o Init.out -Ofast Init.c \
 && gcc -o rtHealthCheck.out -Ofast RuntimeHealthCheck.c

FROM alpine:3

RUN apk add --no-cache openconnect squid

EXPOSE 3128/tcp

ENV RPI_RCSID=REPLACE_ME \
    RPI_PASSWORD=REPLACE_ME

COPY .FILES/squid.conf /etc/squid/squid.conf

COPY --from=0 /Init.out /usr/bin/init

COPY --from=0 /rtHealthCheck.out /usr/bin/rtHealthCheck

HEALTHCHECK --interval=5m --timeout=30s --start-period=2m --retries=1 \
        CMD /usr/bin/rtHealthCheck || exit 1

ENTRYPOINT ["/usr/bin/init"]
