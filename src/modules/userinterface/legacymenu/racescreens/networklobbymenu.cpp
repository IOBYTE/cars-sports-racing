/***************************************************************************

    file                 : networklobbymenu.cpp
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

#include <networklobbymenu.h>
#include <network.h>
#include <legacymenu.h>
#include <racescreens.h>
#include <race.h>
#include <guistack.h>
#include <netclient.h>
#include <netplayer.h>
#include <netserver.h>
#include <network.h>
#include <localtime.h>
#include <tgf.h>
#include <tgfclient.h>
#include <vector>
#include <string>

const char *const NetworkLobbyMenu::class_name{"NetworkLobbyMenu"};

NetworkLobbyMenu::NetworkLobbyMenu(const std::string &name, const std::string &car,
    enum Network::role role, union net net,
    void (*exit)(const std::string &, void *), void *arg) :
    role(role),
    net(net),
    player_name(name),
    exit(exit),
    exit_arg(arg)
{
    std::vector<std::string> players;

    if (net.common->lockPlayers())
        throw std::runtime_error("lockPlayers failed");

    if (net.common->getPlayers(players))
        throw std::runtime_error("getPlayers failed");

    std::vector<std::string>::const_iterator it;

    // Add existing players to internal list.
    for (it = players.begin(); it != players.end(); it++)
        onNewPlayer(*it, this);

    struct NetServer::EventCfg cfg;

    cfg.new_player = onNewPlayer;
    cfg.new_msg = onNewMessage;
    cfg.disconnect = onDisconnected;
    cfg.arg = this;
    net.common->setEventCfg(cfg);

    if (net.common->unlockPlayers())
        throw std::runtime_error("unlockPlayers failed");

    setMenu();
    writeToChat("*IMPORTANT NOTE: chats are currently unencrypted.*");
    writeToChat("*DO NOT share sensitive information e.g.: passwords!*");
    net.common->createPlayer(name, car, role);
}

NetworkLobbyMenu::~NetworkLobbyMenu()
{
    GuiStack::pop();

    // Avoid RmGarageMenu from double-freeing the screen.
    garage.setPreviousMenuHandle(NULL);
    delete net.common;
}

void
NetworkLobbyMenu::closeLobby(void *arg)
{
    delete static_cast<NetworkLobbyMenu *>(arg);
}

void
NetworkLobbyMenu::setMenu()
{
    void *menu = GfuiScreenCreate();

    if (!menu)
        throw std::runtime_error("GfuiScreenCreate failed");

    void *mparam = GfuiMenuLoad("networklobbymenu.xml");

    if (!mparam)
        throw std::invalid_argument("GfuiMenuLoad failed");

    GfuiMenuCreateStaticControls(menu, mparam);
    track_label = GfuiMenuCreateLabelControl(menu, mparam, "trackname");
    laps_label = GfuiMenuCreateLabelControl(menu, mparam, "lapcountname");
    car_cat_label = GfuiMenuCreateLabelControl(menu, mparam, "carcatname");
    competitors_scroll = GfuiMenuCreateScrollListControl(menu, mparam,
        "competitorsscrolllist", NULL, NULL);
    chat_scroll = GfuiMenuCreateScrollListControl(menu, mparam,
        "chatscrolllist", NULL, NULL);
    chat_edit = GfuiMenuCreateEditControl(menu, mparam, "chatedit", this, NULL, NULL);
    GfuiMenuCreateTextButtonControl(menu, mparam, "sendbutton", this, sendChat);
    GfuiAddKey(menu, GFUIK_RETURN, "Send message to chat", this, NULL, sendChat);

    GfuiMenuCreateButtonControl(menu, mparam, "BackButton", this, closeLobby);
    GfuiAddKey(menu, GFUIK_ESCAPE, "Back to previous menu", this, NULL, closeLobby);

    if (role == Network::SERVER)
        GfuiMenuCreateTextButtonControl(menu, mparam, "racesetup", this, RmConfigureRace);

    GfuiMenuCreateTextButtonControl(menu, mparam, "garage", this, openGarage);

    GfParmReleaseHandle(mparam);
    GuiStack::push(menu);
}

void
NetworkLobbyMenu::openGarage(void *arg)
{
    NetworkLobbyMenu *lobby = static_cast<NetworkLobbyMenu *>(arg);
    std::string self;

    if (lobby->net.common->getOwnPlayer(self))
    {
        GfLogError("%s::%s: getOwnPlayer failed.\n", class_name, __func__);
        return;
    }

    GfDriver *driver = GfDrivers::self()->getDriverWithName(self);

    if (!driver)
    {
        GfLogError("%s::%s: getDriverWithName failed.\n", class_name, __func__);
        return;
    }

    lobby->garage.setPreviousMenuHandle(GuiStack::top());
    lobby->garage.runMenu(LmRaceEngine().race(), driver);
}

void
NetworkLobbyMenu::writeToChat(const std::string &msg, const std::string &player)
{
    std::string s;

    if (!player.empty())
    {
        s += "<";
        s += player;

        struct tm tm;

        if (LocalTime::get(tm))
            GfLogError("%s::%s: LocalTime::get failed.\n", class_name, __func__);
        else
        {
            char t[sizeof "00:00:00"];

            strftime(t, sizeof t, "%H:%M:%S", &tm);

            if (!t)
                GfLogError("%s::%s: GfTime2Str failed.\n", class_name, __func__);
            else
            {
                s += " [";
                s += t;
                s += "]";
            }
        }

        s += "> ";
    }

    s += msg;
    chat.push_back(s);

    void *menu = GuiStack::top();

    GfuiScrollListInsertElement(menu, chat_scroll, chat.back().c_str(),
        GfuiScrollListGetNumberOfElements(menu, chat_scroll), NULL);
}

void
NetworkLobbyMenu::sendChat(void *arg)
{
    NetworkLobbyMenu *lobby = static_cast<NetworkLobbyMenu *>(arg);
    void *menu = GuiStack::top();
    const char *str = GfuiEditboxGetString(menu, lobby->chat_edit);

    if (str && *str)
    {
        PackedBuffer buf;

        try
        {
            buf.pack_ubyte(Network::packet_type::CHAT);
            buf.pack_stdstring(lobby->player_name);
            buf.pack_stdstring(str);
        }
        catch (const PackedBufferException &e)
        {
            GfLogFatal("%s::%s: %s.\n", class_name, __func__, e.what());
        }

        lobby->net.common->sendPacket(buf, true);
        lobby->writeToChat(str, lobby->player_name);
        GfuiEditboxSetString(menu, lobby->chat_edit, "");
    }
}

void
NetworkLobbyMenu::addPlayer(void *arg)
{
    std::string *player = static_cast<std::string *>(arg);
    void *menu = GuiStack::top();

    competitors.push_back(*player);
    delete player;

    GfuiScrollListInsertElement(menu, competitors_scroll,
        competitors.back().c_str(),
        GfuiScrollListGetNumberOfElements(menu, competitors_scroll),
        NULL);

    std::string msg = competitors.back();

    msg += " joined the room.";
    writeToChat(msg);

#if 0
    GfDriver *driver = GfDrivers::self()->getDriverWithName(competitors.back());

    if (!driver)
    {
        GfLogError("%s::%s: getDriverWithName failed.\n", class_name, __func__);
        return;
    }

    LmRaceEngine().race()->appendCompetitor(driver);
#endif
}

void
NetworkLobbyMenu::removePlayer(const std::string &name)
{
    std::string s = name;

    s += " disconnected.";
    writeToChat(s);

    void *menu = GuiStack::top();
    const size_t n = GfuiScrollListGetNumberOfElements(menu, competitors_scroll);

    for (size_t i = 0; i < n; i++)
    {
        void *user;
        const char *el = GfuiScrollListGetElement(menu, competitors_scroll, i, &user);

        if (name == el)
        {
            GfuiScrollListExtractElement(menu, competitors_scroll, i, &user);
            break;
        }
    }
}

void
NetworkLobbyMenu::onDisconnected_impl(void *arg)
{
    struct disconnect *d = static_cast<struct disconnect *>(arg);

    switch (role)
    {
        case Network::SERVER:
            removePlayer(d->name);
            break;

        case Network::CLIENT:
            if (exit)
            {
                std::string reason = "Disconnected from server: ";

                switch (d->r)
                {
                    case Network::EXITED:
                        reason = "connection closed";
                        break;

                    case Network::NAME_IN_USE:
                        reason = "name is already in use";
                        break;
                }

                exit(reason, exit_arg);
            }

            break;
    }

    delete d;
}

void
NetworkLobbyMenu::onWriteToChat(void *arg)
{
    chat_msg *msg = static_cast<chat_msg *>(arg);

    writeToChat(msg->first, msg->second);
    delete msg;
}

// Note: this function might be called (or not) from a separate thread.
// Since GUI operations must be performed from the GUI thread, some
// extra complexity is required to push the events into a list
// that is later examined by the GUI thread.
void
NetworkLobbyMenu::onNewPlayer(const std::string &player, void *arg)
{
    NetworkLobbyMenu *lobby = static_cast<NetworkLobbyMenu *>(arg);

    lobby->push_event(gui_event(
        &NetworkLobbyMenu::addPlayer,
        new std::string(player)));
}

// Note: this function might be called (or not) from a separate thread.
void
NetworkLobbyMenu::onNewMessage(const std::string &player,
    const std::string &msg, void *arg)
{
    NetworkLobbyMenu *lobby = static_cast<NetworkLobbyMenu *>(arg);

    lobby->push_event(gui_event(&NetworkLobbyMenu::onWriteToChat,
        new chat_msg(msg, player)));
}

// Note: this function is called from a separate thread.
void
NetworkLobbyMenu::onDisconnected(const std::string &name, void *arg, Network::disconnect_reason r)
{
    NetworkLobbyMenu *lobby = static_cast<NetworkLobbyMenu *>(arg);

    lobby->push_event(gui_event(&NetworkLobbyMenu::onDisconnected_impl,
        new (struct disconnect){name, r}));
}

int
NetworkLobbyMenu::exec_event(const gui_event &ev)
{
    (*this.*ev.first)(ev.second);
    return 0;
}
