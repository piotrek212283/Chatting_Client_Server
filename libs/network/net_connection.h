#pragma once
#include "net_common.h"
#include "net_message.h"
#include "tsqueue.h"

template <typename T>
class connection_id : public std::enable_shared_from_this<connection_id<T>>
{
public:
    enum class ownership
    {
        client,
        server
    };

    connection_id(ownership connection_owner_, asio::io_context &asioContext_, asio::ip::tcp::socket socket_, TSqueue<tagged_message<T>> &inputMsgQueue_)
        : connection_owner{connection_owner_}, asioContext{asioContext_}, socket{socket_}, inputMsgQueue{inputMsgQueue_}
    {
    }
    virtual ~connection_id()
    {
    }
    /* Available only for client */
    bool ConnectToServer();
    /* Available for client and server */
    bool Disconnect();
    /* Available for client */
    bool isConnected() const;

    bool Send(const message<T> &msg);

private:
    /* Define parent ot the connection */
    ownership connection_owner;
    /* Reference to asio context */
    asio::io_context &asioCOntext;
    /* Asio socket */
    asio::ip::tcp::socket socket;
    /* Output Message queue */
    TSqueue<T> outputMsgQueue;
    /* Input Message queue reference */
    TSqueue<tagged_message<T>> &inputMsgQueue;
};