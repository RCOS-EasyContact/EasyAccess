#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(void) {
  // Run "ping" to Test Connectivity
  fclose(stdin);
  freopen("/dev/null", "w", stdout);
  freopen("/dev/null", "w", stderr);
  execl("/bin/ping", "ping", "-c", "1", "roundcube.rpi.edu", NULL);

  return EXIT_FAILURE;
}
