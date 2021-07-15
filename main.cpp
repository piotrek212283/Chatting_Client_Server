#include "net.h"
#include "test_server.h"
#include <iostream>



int main()
{

    SimpleTestServer server(5000);
    server.Start();

    while(1) {
        server.Update();
    }

    return 0;
}

