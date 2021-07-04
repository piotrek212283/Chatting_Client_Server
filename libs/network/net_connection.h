#pragma once
#include "net_common.h"
#include "net_message.h"
#include "tsqueue.h"

template <typename T>
class connection_id : public std::enable_shared_from_this<connection_id<T>>
{
public:
    connection_id()
    {

    }
    virtual ~connection_id(){

    }
    /* Available only for client */
    bool ConnectToServer();
    /* Available for client and server */
    bool Disconnect();
    /* Available for client */
    bool isConnected() const;

    bool Send(const message<T> &msg);





private:
    /* Reference to asio context */
    asio::io_context &context;
    /* Asio socket */
    asio::ip::tcp::socket socket;
    /* Output Message queue */
    TSqueue<T> outputMsgQueue;
    /* Input Message queue reference */
    TSqueue<tagged_message<T>> &inputMsgQueue;
};