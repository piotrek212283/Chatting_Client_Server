#pragma once
#include "net_common.h"
#include "net_message.h"
#include "tsqueue.h"

template <typename T>
class Connection_I : public std::enable_shared_from_this<Connection_I<T>>
{
public:
    enum class ownership
    {
        client,
        server
    };

    Connection_I(ownership connection_owner_, asio::io_context &asioContext_, asio::ip::tcp::socket socket_, TSqueue<tagged_message<T>> &inputMsgQueue_)
        : connection_owner(connection_owner_), asioContext(asioContext_), socket(std::move(socket_)), inputMsgQueue(inputMsgQueue_)
    {
        ;
    }

    virtual ~Connection_I() = default;

    uint32_t GetID() const
    {
        return id;
    }

    void ConnectToClient(uint32_t uid = 0)
    {
        if (connection_owner == ownership::server)
        {
            if (socket.is_open())
            {
                id = uid;
            }
        }
    }

    /* Available only for client */
    bool ConnectToServer();
    /* Available for client and server */
    bool Disconnect();
    /* Available for client */
    bool isConnected() const;

    bool Send(const message<T> &msg);

private:
    void ReadHeader()
    {
        asio::async_read(socket, asio::buffer(&tempMessageIn.header, sizeof(message_header<T>)),
                         [this](std::error_code ec, std::size_t length)
                         {
                             if (!ec)
                             {
                                 if (tempMessageIn.header.size > 0)
                                 {
                                     tempMessageIn.payload.resize(tempMessageIn.header.size);
                                     ReadBody();
                                 }
                                 else
                                 {
                                     AddToIncomingMessageQueue();
                                 }
                             }
                             else
                             {
                                 std::cout << "[" << id << "]"
                                           << "Reading Header Failed. \n";
                                 socket.close();
                             }
                         });
    }

    void ReadBody()
    {
        asio::async_read(socket, asio::buffer(tempMessageIn.payload.data(), tempMessageIn.payload.size()),
                         [this](std::error_code ec, std::size_t length)
                         {
                             if (!ec)
                             {
                                 AddToIncomingMessageQueue();
                             }
                             else
                             {
                                 std::cout << "[" << id << "]"
                                           << "Reading Body Failed. \n";
                                 socket.close();
                             }
                         });
    }

    void WriteHeader()
    {
        asio::async_write(socket, asio::buffer(outputMsgQueue.front().header, sizeof(message_header<T>)),
                          [this](std::error_code ec, std::size_t length)
                          {
                              if (!ec)
                              {
                                  if (outputMsgQueue.front().payload.size() > 0)
                                  {
                                      WriteBody();
                                  }
                                  else
                                  {
                                      outputMsgQueue.pop_front();

                                      if (!outputMsgQueue.isEmpty())
                                      {
                                          WriteHeader();
                                      }
                                  }
                              }
                              else
                              {
                                  std::cout << "[" << id << "]"
                                            << "Writing Header Failed. \n";
                                  socket.close();
                              }
                          });
    }

    void WriteBody()
    {
        asio::async_write(socket, asio::buffer(outputMsgQueue.front().payload.data(), outputMsgQueue.front().payload.size()),
                          [this](std::error_code ec, size_t length)
                          {
                              if (!ec)
                              {
                                  outputMsgQueue.pop_front();

                                  if (!outputMsgQueue.isEmpty())
                                  {
                                      WriteHeader();
                                  }
                              }

                              else
                              {
                                  std::cout << "[" << id << "]"
                                            << "Writing Body Failed. \n";
                                  socket.close();
                              }
                          });
    }

    void AddToIncomingMessageQueue()
    {
        if (connection_owner == ownership::server)
        {
            inputMsgQueue.push_back({this->shared_from_this(), tempMessageIn});
        }
        else
        {
            inputMsgQueue.push_back({nullptr, tempMessageIn});
        }
        ReadHeader();
    }

private:
    uint32_t id = 0;
    /* Define parent ot the connection */
    ownership connection_owner = ownership::server;
    /* Reference to asio context */
    asio::io_context &asioContext;
    /* Asio socket */
    asio::ip::tcp::socket socket;
    /* Output Message queue */
    TSqueue<T> outputMsgQueue;
    /* Input Message queue reference */
    TSqueue<tagged_message<T>> &inputMsgQueue;
    /* Message used to store data in parsing stage */
    message<T> tempMessageIn;
};