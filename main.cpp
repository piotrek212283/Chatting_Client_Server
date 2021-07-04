#include <iostream>

#define ASIO_STANDALONE
#include <asio.hpp>
#include <asio/ts/buffer.hpp>
#include <asio/ts/internet.hpp>
#include "tsqueue.h"

using namespace std;
using namespace asio;
using namespace asio::ip;

std::vector<char> vBuffer(20 * 1024);
void GrabSomeData(tcp::socket &socket)
{
    socket.async_read_some(buffer(vBuffer.data(), vBuffer.size()), 
    [&](std::error_code ec, size_t length) {
        if (!ec) {
            cout << "Read: " << length <<" bytes" << endl;
            for (size_t i = 0; i < length; ++i) {
                cout << vBuffer[i];
            }
            GrabSomeData(socket);
        }
    });
}

int main()
{
    error_code ec;

    /* Create platform specific interface contex*/
    io_context context;

    /* Give fake job for context to doesnt finish */
    io_context::work idleWork(context);

    /* Start Asio context within separate thread */
    std::thread thrContex = std::thread([&]() { context.run();});

    /* Address of somewhere we want to connect */
    tcp::endpoint endpoint(make_address("93.184.216.34", ec), 80);

    /* Create network socket */
    tcp::socket socket(context);

    /* Connect socket to desired ip address */
    socket.connect(endpoint, ec);

    /* Check if connection succeeded */
    if (!ec)
    {
        cout << "Connected!" << endl;
    }
    else
    {
        cout << "Failed to connect: " << ec.message() << endl;
    }

    if (socket.is_open()) {

        GrabSomeData(socket);

        std::string sRequest = 
        "GET /index.html HTTP/1.1\r\n"
        "HOST: example.com \r\n"
        "Connection: close \r\n\r\n";
        socket.write_some(buffer(sRequest.data(), sRequest.size()), ec);
    }

        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);

    system("pause");
    return 0;
}