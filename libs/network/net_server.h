#pragma once

#include "net_common.h"
#include "net_connection.h"
#include "net_message.h"
#include "tsqueue.h"

template <typename T>
class server_interface
{
public:
    server_interface(uint16_t port) : acceptor(asioContext, asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port))
    {
    }

    virtual ~server_interface()
    {
        Stop();
    }

public:
    /* Start the server */
    void Start()
    {
        try
        {
            WaitForClientConnection();
            thrContext = std::thread([this]()
                                     { asioContext.run(); });
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
        std::cout << "[Server] Started !\n";
    }

    /* Stop the server */
    void Stop()
    {
        asioContext.stop();

        if (thrContext.joinable())
        {
            thrContext.join();
        }

        std::cout << "[Server] Stopped !\n";
    }

    /* Asio asioContext wati for clients connection */
    void WaitForClientConnection()
    {
        acceptor.async_accept(
            [this](std::error_code ec, asio::ip::tcp::socket socket)
            {
                if (!ec)
                {
                    
                    std::cout << "[Server] New Connection from: " << socket.remote_endpoint() << "\n";
                    std::shared_ptr<Connection_I<T>> newConnection = std::make_shared<Connection_I<T>>(Connection_I<T>::ownership::server, asioContext, std::move(socket), inputMessageQueue);
                    if (OnClientConnect(newConnection))
                    {
                        dequeConnections.push_back(std::move(newConnection));
                        dequeConnections.back()->ConnectToClient(clientStartID++);
                        std::cout << "[" << dequeConnections.back()->GetID() << "] Connection Approved \n";
                    }
                    else
                    {
                        std::cout << "[------] Connection Denied \n";
                    }
                    
                }
                else
                {
                    std::cout << "[SERVER] New Connection error" << ec.message() << "\n";
                }
                /* Prime context with work to do */
                WaitForClientConnection();
            });
    }

    /* Send message to specific client clients */
    void MessageClient(std::shared_ptr<Connection_I<T>> client, const message<T> &msg)
    {
        if (client && client->IsConnected())
        {
            client->Send(msg);
        }
        else
        {
            OnClientDisconnect(client);
            client.reset();
            dequeConnections.erase(client);
        }
    }

    /* Send message to all clients */
    void MessageAllClients(const message<T> &msg, std::shared_ptr<Connection_I<T>> ignored_client)
    {
        bool invalidClient = false;
        for (auto &client : dequeConnections)
        {
            if (client && client->IsConnected())
            {
                if (client != ignored_client)
                {
                    client->Send(msg);
                }
            }
            else
            {
                OnClientDisconnect(client);
                /* reset invoked on shared pointer erases original conent and replace it with new instance (nullptr in this scenario)*/
                client.reset();
                invalidClient = true;
            }
        }

        if (invalidClient)
        {
            /* Erase all nullptrs in deque by using std::remove algorithm */
            dequeConnections.erase(std::remove(dequeConnections.begin(), dequeConnections.end(), nullptr), dequeConnections.end());
        }
    }

    void Update(size_t MaxMsgNumber = -1)
    {
        size_t MsgCount = 0;

        while (MsgCount < MaxMsgNumber && !inputMessageQueue.isEmpty())
        {
            auto msg = inputMessageQueue.pop_front();

            OnMessage(msg.client_identifier, msg.msg);

            MsgCount++;
        }
    }

protected:
    /* Method called when client connect to the server */
    virtual bool OnClientConnect(std::shared_ptr<Connection_I<T>> client)
    {
        return true;
    }
    /* Method called when client connect to the server */
    virtual void OnClientDisconnect(std::shared_ptr<Connection_I<T>> client)
    {
    }
    /* Method called when server receive message form client */
    virtual void OnMessage(std::shared_ptr<Connection_I<T>> client, message<T> &msg)
    {
    }

protected:
    /* asio asioContext */
    asio::io_context asioContext;
    /* thread for asio context */
    std::thread thrContext;
    /* thread safe input message queue */
    TSqueue<tagged_message<T>> inputMessageQueue;
    /* asio acceptor object to acccept socket connection */
    asio::ip::tcp::acceptor acceptor;
    /* client start ID number to standardize clients */
    uint32_t clientStartID = 10000;
    /* Deque of all clinets connections */
    std::deque<std::shared_ptr<Connection_I<T>>> dequeConnections;
};