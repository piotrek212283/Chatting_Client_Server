#pragma once
#include "net.h"

enum class CustomMessageType : uint8_t
{
    ServerAccept,
    ServerDeny,
    ServerMessage,
    ServerMessageAll,
    ServerPing
};

class SimpleTestServer : public server_interface<CustomMessageType>
{
public:
    SimpleTestServer(uint16_t port) : server_interface<CustomMessageType>(port){};
    ~SimpleTestServer() = default;

public:
    virtual bool OnClientConnect(std::shared_ptr<Connection_I<CustomMessageType>> client) override
    {
        return true;
    }
    virtual void OnClientDisconnect(std::shared_ptr<Connection_I<CustomMessageType>> client) override
    {
        return;
    }
    virtual void OnMessage(std::shared_ptr<Connection_I<CustomMessageType>> client, message<CustomMessageType> &msg) override
    {
        return;
    }
};