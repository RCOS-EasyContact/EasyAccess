FROM ubuntu:jammy

RUN apt update \
 && apt upgrade -y \
 && apt install -y clang

COPY Init.c Init.c

RUN clang -o Init.out -Ofast Init.c

FROM ubuntu:jammy

RUN apt update \
 && apt upgrade -y \
 && apt install -y openconnect \
 && apt install -y squid \
 && apt install -y iputils-ping \
 && apt clean

RUN rm /etc/squid/squid.conf \
 && touch /etc/squid/squid.conf \
 && echo "http_port 3128" | tee -a /etc/squid/squid.conf \
 && echo "http_access allow all" | tee -a /etc/squid/squid.conf

EXPOSE 3128/tcp

ENV RPI_RCSID=REPLACE_ME \
    RPI_PASSWORD=REPLACE_ME \
    VPN_KEEPALIVE=600

COPY --from=0 /Init.out /usr/bin/init

COPY LaunchOpenConnect.sh /.LaunchOpenConnect.sh

ENTRYPOINT ["/usr/bin/init"]
