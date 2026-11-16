#ifndef NET_DRIVER_H
#define NET_DRIVER_H

#include <string>

class NetDriver
{
public:
    explicit NetDriver(int index);

private:
    std::string name, sname, code, car, type, skill_level;
    int race_number;
    float r, g, b;
};

#endif /* NET_DRIVER_H */
