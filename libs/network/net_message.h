#pragma once

#include "net_common.h"

template <typename T>
struct message_header
{
    /* Custom generic type ID */
    T type_id;
    /* Total frame size */
    uint32_t size;
};

template <typename T>
struct message
{
    message_header<T> header;
    std::vector<uint8_t> payload;

    uint32_t size()
    {
        return sizeof(message_header<T>) + payload.size();
    }

    friend std::ostream &operator<<(std::ostream &out, const message<T> &msg)
    {
        /* Print debug info about msg */
        out << "Type ID: " << static_cast<int>(msg.header.type_id) << " Size: " << msg.header.size << std::endl;
        return out;
    }

    template <typename GenericDataType>
    friend message<T> &operator<<(message<T> &msg, const GenericDataType &data)
    {
        /* Check if data is standard type */
        static_assert(std::is_standard_layout<GenericDataType>::value, "Not satandard data type");
        /* get starting index of free vector size */
        uint32_t i = msg.payload.size();
        /* Resize vector to load new data */
        msg.payload.resize(msg.payload.size() + sizeof(GenericDataType));
        /* copy msg to payload vector */
        memcpy(msg.payload.data() + i, &data, sizeof(GenericDataType));
        /* update actual msg size */
        msg.header.size = msg.size();

        return msg;
    }

    template <typename GenericDataType>
    friend message<T> &operator>>(message<T> &msg, GenericDataType &data)
    {
        /* Check if data is standard type */
        static_assert(std::is_standard_layout<GenericDataType>::value, "Not satandard data type");
        /* get the beginning of GenericDataType inside payload */
        uint32_t i = msg.payload.size() - sizeof(GenericDataType);
        /* Copy GenericDataType into data */
        memcpy(&data, msg.payload.data() + i, sizeof(GenericDataType));
        /* Resize vector to load new data */
        msg.payload.resize(i);
        /* Update message size */
        msg.header.size = msg.size();

        return msg;
    }
};

template<typename T>
class Connection_I;


template <typename T>
struct tagged_message
{
    /* client identifier by connection id to the server */
    std::shared_ptr<Connection_I<T>> client_identifier = nullptr;
    /* standard message class */
    message<T> msg;

    friend std::ostream &operator<<(std::ostream &os, const tagged_message<T> &msg)
    {
        os << msg.msg;
        return os;
    }
};