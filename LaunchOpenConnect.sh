echo -e "$RPI_PASSWORD\npush\n" | openconnect vpn.stu.rpi.edu --user=$RPI_RCSID --quiet --passwd-on-stdin
