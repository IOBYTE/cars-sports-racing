#ifndef NETWORK_H
#define NETWORK_H

namespace Network
{
    static const short port = 28500;

    enum packet_type
    {
        CONNECT,
        CHAT
    };

    enum role
    {
        SERVER,
        CLIENT
    };

    enum disconnect_reason
    {
        EXITED,
        NAME_IN_USE
    };
};

#endif /* NETWORK_H */
