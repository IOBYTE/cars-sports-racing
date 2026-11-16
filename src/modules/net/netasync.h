/***************************************************************************

    file                 : netasync.h
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

#ifndef NET_ASYNC_H
#define NET_ASYNC_H

#if defined(__APPLE__) && !defined(USE_MACPORTS)
#include <enet.h>
#else
#include <enet/enet.h>
#endif

#include <cstddef>
#include <list>
#include <vector>
#include <string>
#include "pack.h"
#include "netplayer.h"
#include "network.h"
#include <SDL_atomic.h>
#include <SDL_mutex.h>
#include <SDL_thread.h>

class NetAsync
{
public:
    class EventCfg
    {
    public:
        EventCfg() : new_player(NULL), new_msg(NULL), disconnect(NULL), arg(NULL) {}
        void (*new_player)(const std::string &name, void *arg);
        void (*new_msg)(const std::string &player, const std::string &msg, void *arg);
        void (*disconnect)(const std::string &player, void *arg, Network::disconnect_reason r);
        void *arg;
    };

    virtual ~NetAsync();
    void sendPacket(PackedBuffer &buf, bool reliable = false, ENetPeer *peer = NULL);
    void setEventCfg(const EventCfg &cfg);
    void createPlayer(const std::string &name, const std::string &car,
        Network::role role, ENetPeer *peer = NULL);
    int lockPlayers() const;
    int unlockPlayers() const;
    int getPlayers(std::vector<std::string> &p) const;
    int getOwnPlayer(std::string &p) const;

protected:
    NetAsync(short port = 0, size_t peers = 1);
    bool ownPlayer(const NetPlayer &player) const;

    struct msg
    {
        int type;
        void *data, *user;
    };

    enum channel
    {
        RELIABLE,
        UNRELIABLE,

        N_CHANNELS
    };

    int push_event(const struct msg &ev);
    virtual int process_event(const struct msg &ev);
    int process_send_event(const struct msg &ev);
    virtual int on_connect(ENetPeer *peer);
    virtual int on_disconnect(ENetPeer *peer, Network::disconnect_reason r);
    virtual int on_new_player(const std::string &player, ENetPeer *peer);
    virtual void on_new_msg(const std::string &player, const std::string &msg);
    int on_receive(ENetEvent &ev);
    int on_receive_connect(PackedBuffer &buf, ENetPeer *peer);
    int on_receive_chat(PackedBuffer &buf);

    enum msg_type
    {
        EXIT,
        SEND,

        MAX_EVENT
    };

    typedef std::list<NetPlayer>::const_iterator NetPlayer_citerator;

    std::list<NetPlayer> players;
    std::list<struct msg> events;
    ENetHost *host;
    SDL_mutex *mutex;
    SDL_semaphore *in_sem;

private:
    static int in_thread_fn(void *);
    static int out_thread_fn(void *);
    static int recv_thread_fn(void *);
    static void packet_free(ENetPacket *);
    int in_thread_impl();
    int out_thread_impl();
    int recv_thread_impl();
    int process_input();

    SDL_mutex *players_mutex;
    EventCfg cfg;
    SDL_semaphore *out_sem, *done_sem;
    SDL_atomic_t exit;
    // Must be placed last - see constructor for further reference.
    SDL_Thread *in_thread, *out_thread, *recv_thread;

    static const char *const class_name;
};

#endif /* NET_ASYNC_H */
