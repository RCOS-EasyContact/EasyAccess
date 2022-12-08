# EasyAccess

Accessing Rensselaer's internal network can be painful and complicated due to the requirement of a VPN connection and DUO verification. EasyAccess is here to simplify this process by acting as a conversion layer that converts the Cisco AnyConnect VPN to a proxy server that can be used to proxy Rensselaer's internal network.

EasyAccess is designed to be used by EasyContact server, allowing it to visit Rensselaer's email server without the need for the server to connect to Cisco AnyConnect VPN. But EasyAccess can also be used standalone, allowing one to access the internal network without the need to alter its host network environment. An example practice is to use EasyAccess with [
SwitchyOmega](https://github.com/FelisCatus/SwitchyOmega) Chrome extension, allowing one to visit Rensselaer's email web page on any network environment.

EasyAccess can have a maximum life span of 7 days, after that, the Cisco AnyConnect VPN will be disconnected and the user must restart the container in order to continue using this.

This container includes a health check function, if the Cisco AnyConnect VPN is active, the container will appear to be `healthy`; when the Cisco AnyConnect VPN is disconnected, the container will appear to be `unhealthy`.

## Usage

1. Pull image

    ```shell
    docker pull rexezuge/easyaccess
    ```

1. Setup container

    > Replace `REPLACE_ME` with actual values

    ```shell
    docker run --name EasyAccess \
            --publish 3128:3128 \
            --cap-add=NET_ADMIN \
            --env RPI_RCSID=REPLACE_ME \
            --env RPI_PASSWORD=REPLACE_ME \
            rexezuge/easyaccess
    ```

1. Press `Confirm` on your DUO Mobile

1. Check container status

    ```shell
    docker ps
    ```

1. EasyAccess is now active and listening on port 3128 of your local machine.

1. This container will be active for 7 days, after that it will be `unhealthy`. To restart, run `docker restart EasyAccess` and press `Confirm` on your DUO Mobile

## Debug

- Run an one-time-only container with console output

    ```shell
    docker run --rm -it
            --name EasyAccess-Debug \
            --publish 13128:3128 \
            --cap-add=NET_ADMIN \
            --env RPI_RCSID=REPLACE_ME \
            --env RPI_PASSWORD=REPLACE_ME \
            rexezuge/easyaccess
    ```

- Attach to a running container

    ```shell
    docker exec -it EasyAccess-Debug /bin/sh
    ```

- View log

    ```shell
    docker logs EasyAccess-Debug
    ```
