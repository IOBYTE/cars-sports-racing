#ifndef NETWORK_SERVER_MENU_H
#define NETWORK_SERVER_MENU_H

#include <string>
#include <cstddef>
#include <tgfclient.h>
#include <netplayer.h>

class NetworkServerMenu
{
public:
	NetworkServerMenu();

private:
	~NetworkServerMenu();
	static void serverDeactivate(void *arg);
	static void changePort(void *arg);
	static void changeName(void *arg);
	static void changePeers(tComboBoxInfo *info);
	static void hostServer(void *arg);
	void setConnectMenu();
	void setHostMenu();
	void updatePlayerList(const std::string &name);
	void lookupPlayerSetup(std::string &driver, std::string &car) const;

	int port_edit, player_edit;
	int peers_combo;
	int error_label;
	short port;
	size_t peers;
	std::string player_name, car, error;

	static const char *const name;
	static const size_t min_peers, default_peers, max_peers;
};

#endif /* NETWORK_SERVER_MENU_H */
