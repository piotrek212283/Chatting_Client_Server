#include "net.h"
#include <iostream>

enum class TypeID : uint32_t
{
    Mag_data,
    GPS_data,
    Test_data
};

int main()
{

    message<TypeID> msg;

    float x = 0;
    float y = 1;
    int z = 0;
    msg.header.type_id = TypeID::GPS_data;
    std::cout << msg << std::endl;

    std::cout << "x: " <<  x << " y: " << y << " z: " <<  z << std::endl;

    msg << x << y << z;

    x = 11;
    y = 12;
    z = 1;

    msg >> z >> y >> x;
    std::cout << "x: " <<  x << " y: " << y << " z: " <<  z << std::endl;

    x = 1;
    return 0;
}

