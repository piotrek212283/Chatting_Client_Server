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
    T message_header;
    std::vector<uint32_t> payload;

    friend std::ostream &operator<<(std::ostream &out, const message<T> &msg)
    {
        /* Print debug info about msg */
        out << "Type ID: " << reinterpret_cast<int>(msg.message_header.type_id) << " Size: " << msg.message_header.size << std::endl;
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
        msg.message_header.size = i + sizeof(GenericDataType);

        return msg;
    }

    template <typename GenericDataType>
    friend message<T> &operator>>(message<T> &msg, const GenericDataType &data)
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
        msg.message_header.size = i;

        return msg;
    }
};
