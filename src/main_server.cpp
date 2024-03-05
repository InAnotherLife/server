#include "server.h"

int main() {
  Server server(8000);
  server.Start();
  return 0;
}
