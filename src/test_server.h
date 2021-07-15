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
    virtual void OnClientConnect(std::shared_ptr<connection_id<CustomMessageType>> client) override
    {
        return;
    }
    virtual void OnClientDisconnect(std::shared_ptr<connection_id<CustomMessageType>> client) override
    {
        return;
    }
    virtual void OnMessage(std::shared_ptr<connection_id<CustomMessageType>> client, message<CustomMessageType> &msg) override
    {
        return;
    }
};