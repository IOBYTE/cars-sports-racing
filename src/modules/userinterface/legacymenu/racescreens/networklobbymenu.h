/***************************************************************************

    file                 : networklobbymenu.h
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

#ifndef NETWORK_LOBBY_MENU_H
#define NETWORK_LOBBY_MENU_H

#include <list>
#include <string>
#include <utility>
#include <gui_event_loop.h>
#include <hostsettingsmenu.h>
#include <garagemenu.h>
#include <network.h>
#include <netasync.h>
#include <netclient.h>
#include <netserver.h>

class NetworkLobbyMenu : public GuiEventLoop<std::pair<void (NetworkLobbyMenu::*)(void *), void *> >
{
public:
    union net
    {
        NetAsync *common;
        NetServer *server;
        NetClient *client;
    };

    NetworkLobbyMenu(const std::string &name, const std::string &car,
        enum Network::role role, union net net,
        void (*exit)(const std::string &reason, void *arg) = NULL,
        void *arg = NULL);
    ~NetworkLobbyMenu();

private:
    typedef std::pair<void (NetworkLobbyMenu::*)(void *), void *> gui_event;
    typedef std::pair<std::string, std::string> chat_msg;

    struct disconnect
    {
        std::string name;
        Network::disconnect_reason r;
    };

    static void closeLobby(void *arg);
    static void sendChat(void *arg);
    static void openGarage(void *arg);
    static void onNewPlayer(const std::string &player, void *arg);
    static void onNewMessage(const std::string &player, const std::string &msg, void *arg);
    static void onDisconnected(const std::string &player, void *arg, Network::disconnect_reason r);
    void setMenu();
    void writeToChat(const std::string &msg, const std::string &player = "");
    int exec_event(const gui_event &ev);
    void onWriteToChat(void *arg);
    void addPlayer(void *arg);
    void removePlayer(const std::string &name);
    void onDisconnected_impl(void *arg);

    enum Network::role role;
    union net net;
    std::string player_name;
    int chat_edit;
    int track_label, laps_label, car_cat_label;
    int competitors_scroll, chat_scroll;
    int send_button;
    std::list<std::string> chat, competitors;
    void (*exit)(const std::string &, void *);
    void *exit_arg;
    HostSettingsMenu settings;
    RmGarageMenu garage;

    static const char *const class_name;
};

#endif /* NETWORK_LOBBY_MENU_H */
