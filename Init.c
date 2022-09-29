#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

void runService_Squid(void) {
  // Start Squid Proxy Server
  printf("\033[0;32m%s\033[0m%s\n", "INFO: ", "Starting Squid Proxy Server");

  pid_t pidSquidCache = fork();
  if (unlikely(pidSquidCache == -1)) {
    printf("\033[0;31m%s\033[0m%s\n", "ERROR: ", "Failed to Fork");
    exit(EXIT_FAILURE);
  } else if (pidSquidCache == 0) {
    fclose(stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
    execl("/usr/sbin/squid", "squid", "-z", NULL);
    exit(EXIT_FAILURE);
  }
  waitpid(pidSquidCache, NULL, 0);
  while (access("/var/run/squid.pid", F_OK) == 0) {
    sleep(1);
  }

  pid_t pidSquid = fork();
  if (unlikely(pidSquid == -1)) {
    printf("\033[0;31m%s\033[0m%s\n", "ERROR: ", "Failed to Fork");
    exit(EXIT_FAILURE);
  } else if (pidSquid == 0) {
    fclose(stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
    execl("/usr/sbin/squid", "squid", NULL);
    exit(EXIT_FAILURE);
  }
  waitpid(pidSquid, NULL, 0);
}

void runService_OpenConnect(void) {
  // Allocate Space for Variables
  char *RPI_RCSID = NULL;
  char *RPI_PASSWORD = NULL;

  // Get RCSID & Password from Environment
  RPI_RCSID = getenv("RPI_RCSID");
  if (unlikely(RPI_RCSID == NULL)) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "Failed to get \"RPI_RCSID\" from the Environment");
    exit(EXIT_FAILURE);
  }
  RPI_PASSWORD = getenv("RPI_PASSWORD");
  if (unlikely(RPI_PASSWORD == NULL)) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "Failed to get \"RPI_PASSWORD\" from the Environment");
    exit(EXIT_FAILURE);
  }
  if (unlikely(strcmp(RPI_RCSID, "REPLACE_ME") == 0)) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "\"RPI_RCSID\" not Set, Terminating");
    exit(EXIT_FAILURE);
  } else if (unlikely(strcmp(RPI_PASSWORD, "REPLACE_ME") == 0)) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "\"RPI_PASSWORD\" not Set, Terminating");
    exit(EXIT_FAILURE);
  }

  // Start OpenConnect
  printf("\033[0;32m%s\033[0m%s\n", "INFO: ", "Starting OpenConnect");
  int pipeDescriptor[2] = {};
  if (pipe(pipeDescriptor) == -1) {
    printf("\033[0;31m%s\033[0m%s\n", "ERROR: ", "Failed to Create Pipe");
    exit(EXIT_FAILURE);
  }
  pid_t pidOpenConnect = fork();
  if (unlikely(pidOpenConnect == -1)) {
    printf("\033[0;31m%s\033[0m%s\n", "ERROR: ", "Failed to Fork");
    exit(EXIT_FAILURE);
  } else if (pidOpenConnect == 0) {
    close(pipeDescriptor[1]);
    dup2(pipeDescriptor[0], 0);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);
    execl("/usr/bin/openconnect", "openconnect", "vpn.stu.rpi.edu", "--quiet",
          NULL);
  }
  close(pipeDescriptor[0]);
  dprintf(pipeDescriptor[1], "%s\n%s\npush\n", RPI_RCSID, RPI_PASSWORD);
  close(pipeDescriptor[1]);
}

void runService_VPNKeepAlive(void) {
  // Allocate Space for Variables
  char *ENV_VPN_KEEPALIVE = NULL;
  unsigned int VPN_KEEPALIVE = 120;
  unsigned int USR_KEEPALIVE = 0;

  // Get Interval from Environment
  ENV_VPN_KEEPALIVE = getenv("VPN_KEEPALIVE");
  if (unlikely(ENV_VPN_KEEPALIVE == NULL)) {
    printf("\033[0;31m%s\033[0m%s\n",
           "ERROR: ", "Failed to get \"VPN_KEEPALIVE\" from the Environment");
    exit(EXIT_FAILURE);
  }
  USR_KEEPALIVE = atoi(ENV_VPN_KEEPALIVE);
  if (unlikely(USR_KEEPALIVE < VPN_KEEPALIVE)) {
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
    pid_t pidKeepAlive = fork();
    if (unlikely(pidKeepAlive == -1)) {
      printf("\033[0;31m%s\033[0m%s\n", "ERROR: ", "Failed to Fork");
      exit(EXIT_FAILURE);
    } else if (pidKeepAlive == 0) {
      fclose(stdin);
      freopen("/dev/null", "w", stdout);
      freopen("/dev/null", "w", stderr);
      execl("/bin/ping", "ping", "-c", "1", "roundcube.rpi.edu", NULL);
    }
    waitpid(pidKeepAlive, NULL, 0);
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

  while (1) {
    pause();
  }

  return EXIT_SUCCESS;
}
