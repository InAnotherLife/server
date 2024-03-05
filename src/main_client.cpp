#include "client.h"

int main() {
  Client client("127.0.0.1", 8000);
  client.Connect();
  return 0;
}
