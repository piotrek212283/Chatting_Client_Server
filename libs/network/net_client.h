#pragma once

#include "net_common.h"
#include "net_connection.h"
#include "net_message.h"
#include "tsqueue.h"

template <typename T>
class client_interface
{
public:
    client_interface() : socket(context)
    {
    }

    virtual ~client_interface()
    {
        Disconnect();
    }

public:
    /* Connect to the server via hostname and port */
    bool Connect(const std::string &hostname, const uint16_t port)
    {
        connection = std::make_unique<connection_id<T>>();
        asio::error_code ec;
        endpoint = asio::ip::tcp::resolver(hostname, to_string(port));

        connection->ConnectToServer(endpoint);

        thrContext = std::thread([this]()
                                 { context.run(); });
        return false;
    }

    /* Disconnect from the server */
    bool Disconnect()
    {
        if (isConnected())
        {
            connection->Disconnect();
        }
        context.stop();
        if (thrContext.joinable())
        {
            thrContext.join();
        }
        connection.release();
        return true;
    }

    /* Check if client is still connected to the server via connection */
    bool isConnected()
    {
        return false;
    }

    /* Return reference to protected queue of input messages */
    TSqueue<tagged_message<T>> &GetQueue()
    {
        return inputMsgQueue;
    }

protected:
    /* Client is asio context holder to initialize connection */
    asio::io_context context;
    /* Client also create thread for context */
    std::thread thrContext;
    /* Client hold hardware socket */
    asio::ip::tcp::socket socket;
    /* Endpoint with destination to the server */
    asio::ip::tcp::endpoint endpoint;
    /* Unique pointer to connection_id class which define a connection */
    std::unique_ptr<connection_id<T>> connection;
    /* Client own thread safe queue of input msg */
    TSqueue<tagged_message<T>> inputMsgQueue;
};