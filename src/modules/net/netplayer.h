#ifndef NETPLAYER_H
#define NETPLAYER_H

#if defined(__APPLE__) && !defined(USE_MACPORTS)
#include <enet.h>
#else
#include <enet/enet.h>
#endif

#include <string>
#include "network.h"

class NetPlayer
{
public:
    NetPlayer(const std::string &name, const std::string &car, Network::role role, ENetPeer *peer) :
        name(name),
        car(car),
        role(role),
        peer(peer)
    {}

    bool operator==(const NetPlayer &other)
    {
        return this->peer == other.peer;
    }

    const std::string &getName() const {return name;}
    ENetPeer *getPeer() const {return peer;}
    Network::role getRole() const {return role;}

private:
    const std::string name, car;
    const Network::role role;
    ENetPeer *const peer;
};

#endif /* NETPLAYER_H */
