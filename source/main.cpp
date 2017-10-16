#include "proxy.h"
#include <signal.h>

const unsigned short PORT = 7777;

int main() {
    proxy_server proxy(PORT);
    proxy.run();
    return 0;
}

