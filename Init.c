#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void runService_Squid(void) {
  // Start Squid Proxy Server
  printf("\033[0;32m%s\033[0m%s\n", "INFO: ", "Starting Squid Proxy Server");
  system("rm -rf /var/spool/squid && service squid start");
}

void runService_OpenConnect(void) {
  // Allocate Space for Variables
  char *RPI_RCSID = NULL;
  char *RPI_PASSWORD = NULL;

  // Get RCSID & Password from Environment
  RPI_RCSID = getenv("RPI_RCSID");
  if (RPI_RCSID == NULL) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "Failed to get \"RPI_RCSID\" from the Environment");
    exit(EXIT_FAILURE);
  }
  RPI_PASSWORD = getenv("RPI_PASSWORD");
  if (RPI_PASSWORD == NULL) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "Failed to get \"RPI_PASSWORD\" from the Environment");
    exit(EXIT_FAILURE);
  }
  if (strcmp(RPI_RCSID, "REPLACE_ME") == 0) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "\"RPI_RCSID\" not Set, Terminating");
    exit(EXIT_FAILURE);
  } else if (strcmp(RPI_PASSWORD, "REPLACE_ME") == 0) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "\"RPI_PASSWORD\" not Set, Terminating");
    exit(EXIT_FAILURE);
  }

  // Start OpenConnect
  printf("\033[0;32m%s\033[0m%s\n", "INFO: ", "Starting OpenConnect");
  system("/.LaunchOpenConnect.sh");
}

void runService_VPNKeepAlive(void) {
  // Allocate Space for Variables
  char *ENV_VPN_KEEPALIVE = NULL;
  unsigned int VPN_KEEPALIVE = 120;
  unsigned int USR_KEEPALIVE = 0;

  // Get Interval from Environment
  ENV_VPN_KEEPALIVE = getenv("VPN_KEEPALIVE");
  if (ENV_VPN_KEEPALIVE == NULL) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "Failed to get \"VPN_KEEPALIVE\" from the Environment");
    exit(EXIT_FAILURE);
  }
  USR_KEEPALIVE = atoi(ENV_VPN_KEEPALIVE);
  if (USR_KEEPALIVE < VPN_KEEPALIVE) {
    printf("\033[0;33m%s\033[0m%s\n", "WARNING: ",
           "\"VPN_KEEPALIVE\" is less than Minimal Value, Ignoring the Value");
  } else {
    VPN_KEEPALIVE = USR_KEEPALIVE;
  }

  // Keep Alive
  printf("\033[0;32m%s\033[0m%s%u%s\n",
         "INFO: ", "Starting Keep Alive Service with Interval of ",
         VPN_KEEPALIVE, " Seconds");
  while (1) {
    system("ping -c 1 roundcube.rpi.edu");
    sleep(VPN_KEEPALIVE);
  }
}

int main(void) {
  // Refuse to Start as Non-Pid=1 Program
  if (getpid() != 1) {
    printf("\033[0;31m%s\033[0m%s\n", "ERROR: ", "Must be Run as PID 1");
    exit(EXIT_FAILURE);
  }

  runService_Squid();

  runService_OpenConnect();

  runService_VPNKeepAlive();

  return EXIT_SUCCESS;
}
