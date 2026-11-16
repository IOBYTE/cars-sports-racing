/***************************************************************************

    file                 : netserver.h
    created              : August 2022
    copyright            : (C) 2022 Xavier Del Campo
    web                  : speed-dreams.sourceforge.net

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef NET_SERVER_H
#define NET_SERVER_H

#if defined(__APPLE__) && !defined(USE_MACPORTS)
#include <enet.h>
#else
#include <enet/enet.h>
#endif

#include <string>
#include <cstddef>
#include "netasync.h"
#include "network.h"

class NetServer : public NetAsync
{
public:
    NetServer(short port, size_t peers);

private:
    ~NetServer();
    int on_connect(ENetPeer *peer);
    int on_disconnect(ENetPeer *peer, Network::disconnect_reason r);
    void disconnect();
    int on_new_player(const std::string &player, ENetPeer *peer = NULL);
    void on_new_msg(const std::string &player, const std::string &msg);
    void broadcast_new_player(const std::string &player, ENetPeer *peer);
    void send_player_list(ENetPeer *peer);
    bool player_exists(const std::string &player, std::string &error) const;

    static const char *const class_name;
};

#endif /* NET_SERVER_H */
