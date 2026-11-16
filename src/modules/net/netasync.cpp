/***************************************************************************

    file                 : netasync.cpp
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

#include "netasync.h"
#include "network.h"
#include "pack.h"
#include <string>
#include <stdexcept>
#include <cstddef>
#include <tgf.h>

#if defined(__APPLE__) && !defined(USE_MACPORTS)
#include <enet.h>
#else
#include <enet/enet.h>
#endif

const char *const NetAsync::class_name{"NetAsync"};

NetAsync::NetAsync(short port, size_t peers) :
    mutex(SDL_CreateMutex()),
    in_sem(SDL_CreateSemaphore(0)),
    players_mutex(SDL_CreateMutex()),
    out_sem(SDL_CreateSemaphore(0)),
    done_sem(SDL_CreateSemaphore(0))
{
    if (!mutex)
        throw std::runtime_error("SDL_CreateMutex (mutex) failed");

    if (!players_mutex)
        throw std::runtime_error("SDL_CreateMutex (players_mutex) failed");

    if (!in_sem)
        throw std::runtime_error("SDL_CreateSemaphore (in_sem) failed");

    if (!out_sem)
        throw std::runtime_error("SDL_CreateSemaphore (out_sem) failed");

    if (!done_sem)
        throw std::runtime_error("SDL_CreateSemaphore failed");

    SDL_AtomicSet(&exit, 0);

    if (enet_initialize())
        throw std::runtime_error("enet_initialize failed");

    ENetAddress addr = {0}, *paddr = port ? &addr : NULL;

    addr.host = ENET_HOST_ANY;
    addr.port = port;

#if ENET_VERSION >= 0x010300
    host = enet_host_create(paddr, peers, static_cast<int>(N_CHANNELS), 0, 0);
#else
    host = enet_host_create(paddr, peers, 0, 0);
#endif

    if (!host)
        throw std::runtime_error("enet_host_create failed");

    // Must be initialized after other members so they are initialized
    // before the thread runs.
    in_thread = SDL_CreateThread(in_thread_fn, "NetAs_in", this);

    if (!in_thread)
        throw std::runtime_error("SDL_CreateThread (in_thread) failed");

    out_thread = SDL_CreateThread(out_thread_fn, "NetAs_out", this);

    if (!out_thread)
        throw std::runtime_error("SDL_CreateThread (out_thread) failed");

    recv_thread = SDL_CreateThread(recv_thread_fn, "NetAs_recv", this);

    if (!recv_thread)
        throw std::runtime_error("SDL_CreateThread (recv_thread) failed");
}

NetAsync::~NetAsync()
{
    int status;
    struct msg msg = {0};

    msg.type = EXIT;
    push_event(msg);

    SDL_WaitThread(in_thread, &status);
    SDL_SemPost(in_sem);

    if (status)
        GfLogError("%s::%s received in_thread status %d\n",
            class_name, __func__, status);

    SDL_WaitThread(out_thread, &status);

    if (status)
        GfLogError("%s::%s received out_thread status %d\n",
            class_name, __func__, status);

    SDL_WaitThread(recv_thread, &status);

    if (status)
        GfLogError("%s::%s received recv_thread status %d\n",
            class_name, __func__, status);

    SDL_DestroyMutex(mutex);
    SDL_DestroyMutex(players_mutex);
    SDL_DestroySemaphore(in_sem);
    SDL_DestroySemaphore(out_sem);
    SDL_DestroySemaphore(done_sem);

    if (host)
        enet_host_destroy(host);

    enet_deinitialize();
}

void
NetAsync::setEventCfg(const EventCfg &cfg)
{
    if (SDL_LockMutex(mutex))
    {
        GfLogError("%s::%s: SDL_LockMutex failed\n", class_name, __func__);
        return;
    }

    this->cfg = cfg;

    if (SDL_UnlockMutex(mutex))
        GfLogError("%s::%s: SDL_UnlockMutex failed\n", class_name, __func__);
}

int
NetAsync::lockPlayers() const
{
    return SDL_LockMutex(players_mutex);
}

int
NetAsync::unlockPlayers() const
{
    return SDL_UnlockMutex(players_mutex);
}

int
NetAsync::getPlayers(std::vector<std::string> &p) const
{
    if (lockPlayers())
    {
        GfLogError("%s::%s: lockPlayers failed\n", class_name, __func__);
        return -1;
    }

    for (NetPlayer_citerator it = players.begin(); it != players.end(); it++)
        p.push_back(it->getName());

    if (unlockPlayers())
    {
        GfLogError("%s::%s: unlockPlayers failed\n", class_name, __func__);
        return -1;
    }

    return 0;
}

int
NetAsync::getOwnPlayer(std::string &p) const
{
    bool found = false;

    if (lockPlayers())
    {
        GfLogError("%s::%s: lockPlayers failed\n", class_name, __func__);
        return -1;
    }

    for (NetPlayer_citerator it = players.begin(); it != players.end(); it++)
        if (ownPlayer(*it))
        {
            p = it->getName();
            found = true;
            break;
        }

    if (unlockPlayers())
    {
        GfLogError("%s::%s: unlockPlayers failed\n", class_name, __func__);
        return -1;
    }

    return found ? 0 : -1;
}

int
NetAsync::in_thread_fn(void *arg)
{
    return static_cast<NetAsync *>(arg)->in_thread_impl();
}

int
NetAsync::out_thread_fn(void *arg)
{
    return static_cast<NetAsync *>(arg)->out_thread_impl();
}

int
NetAsync::recv_thread_fn(void *arg)
{
    return static_cast<NetAsync *>(arg)->recv_thread_impl();
}

int
NetAsync::push_event(const struct msg &ev)
{
    if (SDL_LockMutex(mutex))
    {
        GfLogError("%s::%s: SDL_LockMutex failed\n", class_name, __func__);
        return -1;
    }

    events.push_back(ev);

    if (SDL_SemPost(out_sem))
    {
        GfLogError("%s::%s: SDL_SemPost failed\n", class_name, __func__);
        return -1;
    }

    if (SDL_UnlockMutex(mutex))
    {
        GfLogError("%s::%s: SDL_UnlockMutex failed\n", class_name, __func__);
        return -1;
    }

    return 0;
}

class SendPacket
{
public:
    SendPacket(void *data, size_t len, bool reliable, ENetPeer *peer) :
        data(data),
        len(len),
        reliable(reliable),
        peer(peer)
    {
    }

    void *const data;
    const size_t len;
    const bool reliable;
    ENetPeer *const peer;
};

void
NetAsync::sendPacket(PackedBuffer &buf, bool reliable, ENetPeer *peer)
{
    // PackedBuffer change their length() once buffer_move() is called,
    // sending the whole buffer rather than only the used bytes.
    const size_t len = buf.length();
    struct msg ev = {0};
    ev.type = SEND;
    ev.data = new SendPacket(buf.buffer_move(), len, reliable, peer);
    push_event(ev);
}

void
NetAsync::packet_free(ENetPacket *packet)
{
    delete[] packet->data;
}

int
NetAsync::process_send_event(const struct msg &ev)
{
    SendPacket *s = static_cast<SendPacket *>(ev.data);
    enet_uint32 flags = ENET_PACKET_FLAG_NO_ALLOCATE;

    if (s->reliable)
        flags |= ENET_PACKET_FLAG_RELIABLE;

    ENetPacket *p = enet_packet_create(s->data, s->len, flags);

    if (!p)
        return -1;

    p->freeCallback = packet_free;

    enet_uint8 ch = s->reliable ? RELIABLE : UNRELIABLE;

    if (s->peer)
    {
        if (enet_peer_send(s->peer, ch, p))
            return -1;
    }
    else
        enet_host_broadcast(host, ch, p);

    delete s;

    if (SDL_SemPost(in_sem))
    {
        GfLogError("%s::%s: SDL_SemPost failed\n", class_name, __func__);
        return -1;
    }

    return 0;
}

int
NetAsync::process_event(const struct msg &ev)
{
    switch (ev.type)
    {
        case EXIT:
            SDL_AtomicSet(&exit, 1);
            return 0;

        case SEND:
            return process_send_event(ev);

        default:
            break;
    }

    return 0;
}

int
NetAsync::on_connect(ENetPeer *peer)
{
    return 0;
}

int
NetAsync::on_disconnect(ENetPeer *peer, Network::disconnect_reason r)
{
    if (lockPlayers())
    {
        GfLogError("%s::%s: lockPlayers failed\n", class_name, __func__);
        return -1;
    }

    for (NetPlayer_citerator it = players.begin(); it != players.end(); it++)
    {
        if (it->getPeer() == peer)
        {
            if (cfg.disconnect)
                cfg.disconnect(it->getName(), cfg.arg, r);

            players.remove(*it);
            break;
        }
    }

    if (unlockPlayers())
    {
        GfLogError("%s::%s: unlockPlayers failed\n", class_name, __func__);
        return -1;
    }

    return 0;
}

int
NetAsync::on_new_player(const std::string &player, ENetPeer *peer)
{
    return 0;
}

void
NetAsync::on_new_msg(const std::string &player, const std::string &msg)
{
}

void
NetAsync::createPlayer(const std::string &name, const std::string &car,
    Network::role role, ENetPeer *peer)
{
    if (on_new_player(name, peer))
        return;

    if (lockPlayers())
    {
        GfLogError("%s::%s: lockPlayers failed\n", class_name, __func__);
        return;
    }

    players.push_back(NetPlayer(name, car, role, peer));

    if (unlockPlayers())
    {
        GfLogError("%s::%s: unlockPlayers failed\n", class_name, __func__);
        return;
    }

    if (cfg.new_player)
        cfg.new_player(name, cfg.arg);
}

bool
NetAsync::ownPlayer(const NetPlayer &player) const
{
    return player.getPeer() == NULL;
}

int
NetAsync::on_receive_connect(PackedBuffer &buf, ENetPeer *peer)
{
    std::string new_player, car;
    Network::role role;

    try
    {
        role = static_cast<Network::role>(buf.unpack_ubyte());
        buf.unpack_stdstring(new_player);
        buf.unpack_stdstring(car);
    }
    catch (const PackedBufferException &e)
    {
        GfLogWarning("%s::%s: PackedBuffer::unpack_stdstring failed: %s.\n",
            class_name, __func__, e.what());
        return 0;
    }

    if (!new_player.empty())
    {
        createPlayer(new_player, car, role, peer);
    }
    else
        GfLogWarning("%s::%s: unexpected empty player name.\n", class_name, __func__);

    return 0;
}

int
NetAsync::on_receive_chat(PackedBuffer &buf)
{
    std::string player, msg;

    try
    {
        buf.unpack_stdstring(player);
        buf.unpack_stdstring(msg);
    }
    catch (const PackedBufferException &e)
    {
        GfLogWarning("%s::%s: PackedBuffer::unpack_stdstring failed: %s.\n",
            class_name, __func__, e.what());
        return 0;
    }

    on_new_msg(player, msg);

    if (cfg.new_msg)
        cfg.new_msg(player, msg, cfg.arg);

    return 0;
}

int
NetAsync::on_receive(ENetEvent &ev)
{
    int ret = 0;
    PackedBuffer buf(ev.packet->data, ev.packet->dataLength);

    try
    {
        Network::packet_type type = static_cast<Network::packet_type>(buf.unpack_ubyte());

        switch (type)
        {
            case Network::CONNECT:
                ret = on_receive_connect(buf, ev.peer);
                break;

            case Network::CHAT:
                ret = on_receive_chat(buf);
                break;

            default:
                GfLogError("%s::%s: unexpected packet type %d.\n",
                    class_name, __func__, type);
                break;
        }
    }
    catch (const PackedBufferException &e)
    {
        GfLogError("%s::%s: failed to unpack buffer.\n", class_name, __func__);
        ret = -1;
    }

    enet_packet_destroy(ev.packet);
    return ret;
}

int
NetAsync::process_input()
{
    ENetEvent ev;

    while (enet_host_service(host, &ev, 0))
    {
        switch (ev.type)
        {
            case ENET_EVENT_TYPE_CONNECT:
                if (on_connect(ev.peer))
                    return -1;

                break;

            case ENET_EVENT_TYPE_DISCONNECT:
            {
                Network::disconnect_reason r = static_cast<Network::disconnect_reason>(ev.data);

                if (on_disconnect(ev.peer, r))
                    return -1;

                // Must be explicitly reset, according to the documentation.
                ev.peer->data = NULL;
            }
                break;

            case ENET_EVENT_TYPE_RECEIVE:
                if (on_receive(ev))
                    return -1;

                break;

            case ENET_EVENT_TYPE_NONE:
                break;
        }
    }

    return 0;
}

int
NetAsync::in_thread_impl()
{
    while (!SDL_AtomicGet(&exit))
    {
        if (SDL_SemWait(in_sem))
        {
            GfLogError("%s::%s: SDL_SemWait failed\n", class_name, __func__);
            return -1;
        }

        if (SDL_LockMutex(mutex))
        {
            GfLogError("%s::%s: SDL_LockMutex failed\n", class_name, __func__);
            return -1;
        }

        int ret = process_input();

        if (SDL_UnlockMutex(mutex))
        {
            GfLogError("%s::%s: SDL_UnlockMutex failed\n", class_name, __func__);
            return -1;
        }

        if (ret < 0)
            return ret;

        if (SDL_SemPost(done_sem))
        {
            GfLogError("%s::%s: SDL_SemPost failed\n", class_name, __func__);
            return -1;
        }
    }

    return 0;
}

int
NetAsync::out_thread_impl()
{
    while (!SDL_AtomicGet(&exit))
    {
        if (SDL_SemWait(out_sem))
        {
            GfLogError("%s::%s SDL_SemWait failed\n", class_name, __func__);
            return -1;
        }

        if (SDL_LockMutex(mutex))
        {
            GfLogError("%s::%s: SDL_LockMutex failed\n", class_name, __func__);
            return -1;
        }

        if (events.empty())
        {
            GfLogFatal("%s::%s: semaphore was posted without an event.\n",
                class_name, __func__);
        }

        int ret = process_event(events.front());

        events.pop_front();

        if (SDL_UnlockMutex(mutex))
        {
            GfLogError("%s::%s: SDL_UnlockMutex failed\n", class_name, __func__);
            return -1;
        }

        if (ret)
            return ret;
    }

    return 0;
}

int
NetAsync::recv_thread_impl()
{
    while (!SDL_AtomicGet(&exit))
    {
        enet_uint32 conditions;

        do
        {
            conditions = ENET_SOCKET_WAIT_RECEIVE | ENET_SOCKET_WAIT_INTERRUPT;
            enet_socket_wait(host->socket, &conditions,
                ENET_HOST_BANDWIDTH_THROTTLE_INTERVAL);
        } while (conditions == ENET_SOCKET_WAIT_INTERRUPT);

        if (SDL_SemPost(in_sem))
        {
            GfLogError("%s::%s: SDL_SemPost failed\n", class_name, __func__);
            return -1;
        }

        if (SDL_SemWait(done_sem))
        {
            GfLogError("%s::%s: SDL_SemWait failed\n", class_name, __func__);
            return -1;
        }
    }

    return 0;
}
