/***************************************************************************

    file                 : netclient.cpp
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

#include "netclient.h"
#include "network.h"
#include <string>
#include <stdexcept>
#include <cstddef>
#include <tgf.h>

#if defined(__APPLE__) && !defined(USE_MACPORTS)
#include <enet.h>
#else
#include <enet/enet.h>
#endif

const char *const NetClient::class_name{"NetClient"};

NetClient::NetClient() :
    NetAsync(),
    server(NULL),
    cond(SDL_CreateCond()),
    sem(SDL_CreateSemaphore(0))
{
    if (!cond)
        throw std::runtime_error("SDL_CreateCond failed");

    if (!sem)
        throw std::runtime_error("SDL_CreateSemaphore failed");
}

NetClient::~NetClient()
{
    if (server)
    {
        enet_peer_disconnect(server, Network::EXITED);

        if (SDL_SemWait(sem))
            GfLogError("%s::%s: SDL_SemWait failed.\n", class_name, __func__);
    }

    SDL_DestroyCond(cond);
    SDL_DestroySemaphore(sem);
}

int
NetClient::process_event(const struct msg &ev)
{
    int ret = NetAsync::process_event(ev);

    if (ret)
        return ret;

    switch (ev.type)
    {
        case CONNECT:
        {
            struct connect *msg = static_cast<struct connect *>(ev.data);
            std::string error;

            if (!msg)
            {
                GfLogError("%s::%s: expected non-NULL payload\n",
                    class_name, __func__);
                return -1;
            }

            int status = connect_sync(*msg, error);

            if (msg->cb)
                msg->cb(status, error, ev.user);

            delete msg;
        }
            break;
    }

    return 0;
}

int
NetClient::on_connect(ENetPeer *peer)
{
    GfLogInfo("Connection accepted\n");

    if (SDL_CondSignal(cond))
    {
        GfLogError("%s::%s: SDL_CondSignal failed.\n", class_name, __func__);
        return -1;
    }

    return 0;
}

int
NetClient::on_disconnect(ENetPeer *peer, Network::disconnect_reason r)
{
    server = NULL;

    if (SDL_SemPost(sem))
    {
        GfLogError("%s::%s: SDL_SemPost failed.\n", class_name, __func__);
        return -1;
    }

    return NetAsync::on_disconnect(peer, r);
}

int
NetClient::connect_sync(const struct connect &c, std::string &error)
{
    ENetAddress address = {0};
    const char *addr = c.addr.c_str();

    if (enet_address_set_host(&address, addr))
    {
        error = "enet_address_set_host failed";
        return -1;
    }

    address.port = c.port;

#if (ENET_VERSION >= 0x010300)
    server = enet_host_connect(host, &address, static_cast<int>(N_CHANNELS), 0);
#else
    server = enet_host_connect(host, &address, static_cast<int>(N_CHANNELS));
#endif

    if (!server)
    {
        error = "enet_host_connect failed";
        return -1;
    }

    static const Uint32 timeout = 5000;
    const int res = SDL_CondWaitTimeout(cond, mutex, timeout);

    if (res < 0)
    {
        error = "SDL_CondWaitTimeout failed";
        enet_peer_reset(server);
        server = NULL;
        return -1;
    }
    else if (res == SDL_MUTEX_TIMEDOUT)
    {
        error = "Connection timed out";
        enet_peer_reset(server);
        server = NULL;
        return -1;
    }

    PackedBuffer buf;

    try
    {
        buf.pack_ubyte(Network::CONNECT);
        buf.pack_ubyte(Network::CLIENT);
        buf.pack_stdstring(c.name);
        buf.pack_stdstring(c.car);
    }
    catch (const PackedBufferException &e)
    {
        GfLogFatal("%s::%s: failed to pack buffer.\n", class_name, __func__);
    }

    sendPacket(buf, true);
    return 0;
}

int
NetClient::connect(const std::string &addr, short port, const std::string &name,
    const std::string &car, connect_cb cb, void *user)
{
    struct msg msg = {0};

    msg.type = CONNECT;
    msg.data = new (struct connect){addr, name, car, port, cb};
    msg.user = user;

    return push_event(msg);
}
