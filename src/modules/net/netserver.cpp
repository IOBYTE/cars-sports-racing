/***************************************************************************

    file                 : netserver.cpp
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

#include "netserver.h"
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

const char *const NetServer::class_name{"NetServer"};

NetServer::NetServer(short port, size_t peers) :
    NetAsync(port, peers)
{
}

NetServer::~NetServer()
{
    for (NetPlayer_citerator it = players.begin(); it != players.end(); it++)
        if (!ownPlayer(*it))
            enet_peer_disconnect_now(it->getPeer(), Network::EXITED);
}

int
NetServer::on_connect(ENetPeer *peer)
{
    // As of ENet 1.3.17, only IPv4 is supported.
    // See implementation for enet_socket_create for further reference.
    char ip[sizeof "255.255.255.255"];

    if (enet_address_get_host_ip(&peer->address, ip, sizeof ip))
    {
        GfLogError("enet_address_get_host_ip failed.\n");
        return -1;
    }

    GfLogInfo("%s::%s: incoming connection from %s:%hu.\n",
        class_name, __func__, ip, peer->address.port);
    return 0;
}

int
NetServer::on_disconnect(ENetPeer *peer, Network::disconnect_reason r)
{
    return NetAsync::on_disconnect(peer, r);
}

void
NetServer::send_player_list(ENetPeer *peer)
{
    if (lockPlayers())
    {
        GfLogError("%s::%s: lockPlayers failed\n", class_name, __func__);
        return;
    }

    for (auto it = players.begin(); it != players.end(); it++)
    {
        PackedBuffer buf;

        try
        {
            buf.pack_ubyte(Network::CONNECT);
            buf.pack_ubyte(it->getRole());
            buf.pack_stdstring(it->getName());
        }
        catch (const PackedBufferException &e)
        {
            GfLogFatal("%s::%s: failed to pack buffer.\n",
                class_name, __func__);
        }

        sendPacket(buf, true, peer);
    }

    if (unlockPlayers())
        GfLogError("%s::%s: unlockPlayers failed\n", class_name, __func__);
}

void
NetServer::broadcast_new_player(const std::string &player, ENetPeer *peer)
{
    if (lockPlayers())
    {
        GfLogError("%s::%s: lockPlayers failed\n", class_name, __func__);
        return;
    }

    for (auto it = players.begin(); it != players.end(); it++)
    {
        if (!ownPlayer(*it))
        {
            PackedBuffer buf;

            try
            {
                buf.pack_ubyte(Network::CONNECT);
                buf.pack_ubyte(Network::CLIENT);
                buf.pack_stdstring(player);
            }
            catch (const PackedBufferException &e)
            {
                GfLogFatal("%s::%s: failed to pack buffer.\n",
                    class_name, __func__);
            }

            // Broadcast event to other peers.
            sendPacket(buf, true, it->getPeer());
        }
    }

    if (unlockPlayers())
    {
        GfLogError("%s::%s: unlockPlayers failed\n", class_name, __func__);
        return;
    }
}

bool
NetServer::player_exists(const std::string &player, std::string &error) const
{
    bool result = false;

    if (lockPlayers())
    {
        GfLogError("%s::%s: lockPlayers failed\n", class_name, __func__);
        error = "Internal server error";
        return false;
    }

    for (auto it = players.begin(); it != players.end(); it++)
    {
        if (it->getName() == player)
        {
            error = "Player ";
            error += player;
            result = true;
            break;
        }
    }

    if (unlockPlayers())
    {
        GfLogError("%s::%s: unlockPlayers failed\n", class_name, __func__);
        error = "Internal server error";
    }

    return result;
}

int
NetServer::on_new_player(const std::string &player, ENetPeer *peer)
{
    std::string error;

    if (player_exists(player, error))
    {
        enet_peer_disconnect_later(peer, Network::NAME_IN_USE);
        SDL_SemPost(in_sem);
        return -1;
    }

    broadcast_new_player(player, peer);
    send_player_list(peer);
    return 0;
}

void
NetServer::on_new_msg(const std::string &player, const std::string &msg)
{
    if (lockPlayers())
    {
        GfLogError("%s::%s: lockPlayers failed\n", class_name, __func__);
        return;
    }

    // Broadcast event to other peers.
    for (auto it = players.begin(); it != players.end(); it++)
    {
        if (!ownPlayer(*it) && it->getName() != player)
        {
            PackedBuffer buf;

            try
            {
                buf.pack_ubyte(Network::CHAT);
                buf.pack_stdstring(player);
                buf.pack_stdstring(msg);
            }
            catch (const PackedBufferException &e)
            {
                GfLogFatal("%s::%s: failed to pack buffer.\n", class_name, __func__);
            }

            sendPacket(buf, true, it->getPeer());
        }
    }

    if (unlockPlayers())
    {
        GfLogError("%s::%s: unlockPlayers failed\n", class_name, __func__);
        return;
    }
}
