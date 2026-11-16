#ifndef NETWORK_CLIENT_MENU_H
#define NETWORK_CLIENT_MENU_H

#include <string>
#include <list>
#include <utility>
#include <networklobbymenu.h>
#include <netclient.h>
#include <gui_event_loop.h>
#include <SDL_mutex.h>

class NetworkClientMenu : public GuiEventLoop<std::pair<void (NetworkClientMenu::*)(void *), void *> >
{
public:
	NetworkClientMenu();

private:
	typedef std::pair<void (NetworkClientMenu::*)(void *), void *> gui_event;

	~NetworkClientMenu();
	static void clientDeactivate(void *);
	static void changeAddress(void *);
	static void changePort(void *);
	static void changeName(void *);
	static void joinServer(void *);
	static void recompute(void *);
	static void onConnect(int status, const std::string &error, void *arg);
	static void sendChat(void *arg);
	static void closeClient(void *arg);
	static void onDisconnected(const std::string &reason, void *arg);
	void setWaitingToConnectScreen();
	void setClientMenu(void *);
	void setConnectMenu();
	void onConnectFailed(void *);
	void closeLobby(void *);
	void lookupPlayerSetup(std::string &driver, std::string &car) const;
	int exec_event(const gui_event &ev);
	void writeToChat(const std::string &msg);

	int ip_edit, port_edit, player_edit;
	int error_label;
	short port;
	std::string address, player_name, car;
	std::string error;
	std::list<std::string> chat;
	NetClient *client;
	NetworkLobbyMenu *lobby;

	static const char *const name;
};

#endif /* NETWORK_CLIENT_MENU_H */
