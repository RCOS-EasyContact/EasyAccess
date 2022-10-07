#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#define likely(x) __builtin_expect((x), 1)
#define unlikely(x) __builtin_expect((x), 0)

int main(int numArgv, char **arrArgv) {
  // Refuse to Start if Not Invoked bt Pid=1
  if (getppid() != 1) {
    printf("\033[0;31m%s\033[0m%s\n", "ERROR: ", "Must be Invoked by PID 1");
    exit(EXIT_FAILURE);
  }

  // Allocate Space for Variables
  const unsigned int VPN_KEEPALIVE = atoi(arrArgv[1]);

  // Run "ping" to Keep Connection Active
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

  return EXIT_SUCCESS;
}
