#include <networkservermenu.h>
#include <networklobbymenu.h>
#include <pack.h>
#include <tgf.h>
#include <tgfclient.h>
#include <guistack.h>
#include <localtime.h>
#include <netdriver.h>
#include <netplayer.h>
#include <netserver.h>
#include <network.h>
#include <playerpref.h>
#include <robot.h>
#include <stdexcept>
#include <string>
#include <cerrno>
#include <cstdlib>
#include <cstring>
#include <ctime>

const char *const NetworkServerMenu::name{"NetworkServerMenu"};
const size_t NetworkServerMenu::min_peers = 1,
    NetworkServerMenu::default_peers = 8,
    NetworkServerMenu::max_peers = 32;

NetworkServerMenu::NetworkServerMenu() :
    peers(default_peers)
{
    setConnectMenu();
}

NetworkServerMenu::~NetworkServerMenu()
{
    GuiStack::pop();
}

void
NetworkServerMenu::serverDeactivate(void *arg)
{
    delete static_cast<NetworkServerMenu *>(arg);
}

void
NetworkServerMenu::changePort(void *arg)
{
    NetworkServerMenu *server = static_cast<NetworkServerMenu *>(arg);
    const char *port = GfuiEditboxGetString(GuiStack::top(), server->port_edit);

    if (port)
    {
        errno = 0;
        unsigned long n = strtoul(port, NULL, 0);

        if (!errno)
            server->port = n;
        else
            GfLogError("Invalid port number %s.\n", port);
    }
}

void
NetworkServerMenu::changeName(void *arg)
{
    NetworkServerMenu *server = static_cast<NetworkServerMenu *>(arg);
    const char *name = GfuiEditboxGetString(GuiStack::top(), server->player_edit);

    if (name)
        server->player_name = name;
}

void
NetworkServerMenu::changePeers(tComboBoxInfo *info)
{
    NetworkServerMenu *server = static_cast<NetworkServerMenu *>(info->userData);
    const char *peers = GfuiComboboxGetText(GuiStack::top(), server->peers_combo);

    if (peers)
    {
        errno = 0;
        unsigned long n = strtoul(peers, NULL, 0);

        if (!errno)
            server->peers = n;
        else
            GfLogError("Invalid number of peers %s.\n", peers);
    }
}

void
NetworkServerMenu::hostServer(void *arg)
{
    NetworkServerMenu *server = static_cast<NetworkServerMenu *>(arg);

    try
    {
        server->error.clear();
        server->setHostMenu();
    }
    catch (const std::exception &e)
    {
        void *menu = GuiStack::top();

        server->error = "Could not host server: ";
        server->error += e.what();
        GfuiLabelSetText(menu, server->error_label, server->error.c_str());
    }
}

void
NetworkServerMenu::setHostMenu()
{
    union NetworkLobbyMenu::net net{new NetServer(port, peers)};

    new NetworkLobbyMenu(player_name, car, Network::SERVER, net);
}

void
NetworkServerMenu::setConnectMenu()
{
    void *menu = GfuiScreenCreate();

    if (!menu)
        throw std::runtime_error("GfuiScreenCreate failed");

    void *mparam = GfuiMenuLoad("networkserverconnectmenu.xml");

    if (!mparam)
        throw std::invalid_argument("GfuiMenuLoad failed");

    GfuiMenuCreateStaticControls(menu, mparam);
    port_edit = GfuiMenuCreateEditControl(menu, mparam, "PortEdit", this, NULL, changePort);
    player_edit = GfuiMenuCreateEditControl(menu, mparam, "PlayerNameEdit", this, NULL, changeName);
    peers_combo = GfuiMenuCreateComboboxControl(menu, mparam, "NumPeersCombo", this, changePeers);

    for (size_t i = min_peers; i <= max_peers; i++)
        GfuiComboboxAddText(menu, peers_combo, std::to_string(i).c_str());

    GfuiComboboxSetSelectedIndex(menu, peers_combo, peers - 1);
    GfuiEditboxSetString(menu, port_edit, std::to_string(Network::port).c_str());
    changePort(this);

    error_label = GfuiMenuCreateLabelControl(menu, mparam, "ErrorMessage");

    lookupPlayerSetup(player_name, car);
    GfuiEditboxSetString(menu, player_edit, player_name.c_str());

    GfuiMenuCreateButtonControl(menu, mparam, "HostButton", this, hostServer);
    GfuiMenuCreateButtonControl(menu, mparam, "BackButton", this, serverDeactivate);

    GfuiMenuDefaultKeysAdd(menu);
    GfuiAddKey(menu, GFUIK_ESCAPE, "Back to previous menu", this, NULL, serverDeactivate);

    GfParmReleaseHandle(mparam);
    GuiStack::push(menu);
}

void
NetworkServerMenu::lookupPlayerSetup(std::string &driver, std::string &car) const
{
    void *drvinfo = GfParmReadFileLocal(HM_DRV_FILE, GFPARM_RMODE_REREAD);

    if (!drvinfo)
        throw std::invalid_argument("GfParmReadFileLocal(" HM_DRV_FILE ") failed");

    static const char *robot_idx = ROB_SECT_ROBOTS "/" ROB_LIST_INDEX "/1";

    driver = GfParmGetStr(drvinfo, robot_idx, ROB_ATTR_NAME, "");
    car = GfParmGetStr(drvinfo, robot_idx, ROB_ATTR_CAR, "");
    GfParmReleaseHandle(drvinfo);
}
