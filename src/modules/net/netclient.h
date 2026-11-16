/***************************************************************************

    file                 : netclient.h
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

#ifndef NET_CLIENT_H
#define NET_CLIENT_H

#if defined(__APPLE__) && !defined(USE_MACPORTS)
#include <enet.h>
#else
#include <enet/enet.h>
#endif

#include <string>
#include <stddef.h>
#include "netasync.h"
#include <SDL_mutex.h>
#include "network.h"

class NetClient : public NetAsync
{
public:
    typedef void (*connect_cb)(int status, const std::string &error, void *user);

    NetClient();
    ~NetClient();
    int connect(const std::string &addr, short port, const std::string &name,
                const std::string &car, connect_cb cb = NULL, void *user = NULL);

private:
    struct connect
    {
        std::string addr, name, car;
        short port;
        connect_cb cb;
    };

    enum msg_type
    {
        CONNECT = NetAsync::MAX_EVENT
    };

    int connect_sync(const struct connect &c, std::string &error);
    int process_event(const struct msg &ev);
    int on_connect(ENetPeer *peer);
    int on_disconnect(ENetPeer *peer, Network::disconnect_reason r);

    ENetPeer *server;
    SDL_cond *cond;
    SDL_semaphore *sem;

    static const char *const class_name;
};

#endif /* NET_CLIENT_H */
