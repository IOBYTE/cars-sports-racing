#include <networkclientmenu.h>
#include <networklobbymenu.h>
#include <tgf.h>
#include <tgfclient.h>
#include <localtime.h>
//#include <guistack.h>
#include <netclient.h>
#include <netdriver.h>
#include <network.h>
#include <playerpref.h>
#include <robot.h>
#include <stdexcept>
#include <string>
#include <cerrno>
#include <cstdlib>

const char *const NetworkClientMenu::name{"NetworkClientMenu"};

NetworkClientMenu::NetworkClientMenu() :
    GuiEventLoop(),
    port(0),
    lobby(NULL)
{
    setConnectMenu();
}

NetworkClientMenu::~NetworkClientMenu()
{
    //GuiStack::pop();
}

void
NetworkClientMenu::clientDeactivate(void *arg)
{
    delete static_cast<NetworkClientMenu *>(arg);
}

void
NetworkClientMenu::changeAddress(void *arg)
{
    NetworkClientMenu *client = static_cast<NetworkClientMenu *>(arg);
    const char *address = GfuiEditboxGetString(GuiStack::top(), client->ip_edit);

    if (address)
        client->address = address;
}

void
NetworkClientMenu::changePort(void *arg)
{
    NetworkClientMenu *client = static_cast<NetworkClientMenu *>(arg);
    const char *port = GfuiEditboxGetString(GuiStack::top(), client->port_edit);

    if (port)
    {
        errno = 0;
        unsigned long n = strtoul(port, NULL, 0);

        if (!errno)
            client->port = n;
        else
            GfLogError("Invalid port number %s.\n", port);
    }
}

void
NetworkClientMenu::changeName(void *arg)
{
    NetworkClientMenu *client = static_cast<NetworkClientMenu *>(arg);
    const char *name = GfuiEditboxGetString(GuiStack::top(), client->player_edit);

    if (name)
        client->player_name = name;
}

void
NetworkClientMenu::onConnectFailed(void *arg)
{
    std::string *descr = static_cast<std::string *>(arg);

    error = "Failed to connect to ";
    error += address;
    error += ":";
    error += std::to_string(port);
    error += ": ";
    error += *descr;
    delete descr;

    // Remove loading screen.
    void *menu = GuiStack::pop();

    GfuiLabelSetText(menu, error_label, error.c_str());
    delete client;
}

int
NetworkClientMenu::exec_event(const gui_event &ev)
{
    (*this.*ev.first)(ev.second);
    return 0;
}

// Note: this function is called from a separate thread.
// Since GUI operations must be performed from the GUI thread, some
// extra complexity is required to push the events into a list
// that is later examined by the GUI thread.
void
NetworkClientMenu::onConnect(int status, const std::string &error, void *arg)
{
    NetworkClientMenu *client = static_cast<NetworkClientMenu *>(arg);

    if (!status)
    {
        gui_event ev(&NetworkClientMenu::setClientMenu, NULL);

        client->error.clear();
        client->push_event(ev);
    }
    else
    {
        gui_event ev(&NetworkClientMenu::onConnectFailed, new std::string(error));

        GfLogError("%s\n", error.c_str());
        client->push_event(ev);
    }
}

void
NetworkClientMenu::closeLobby(void *arg)
{
    std::string *reason = static_cast<std::string *>(arg);

    delete lobby;
    GfuiLabelSetText(GuiStack::top(), error_label, reason->c_str());
    delete reason;
}

void
NetworkClientMenu::onDisconnected(const std::string &reason, void *arg)
{
    NetworkClientMenu *client = static_cast<NetworkClientMenu *>(arg);
    gui_event ev(&NetworkClientMenu::closeLobby, new std::string(reason));

    client->push_event(ev);
}

void
NetworkClientMenu::setClientMenu(void *)
{
    union NetworkLobbyMenu::net net{client};

    // Remove loading screen.
    GuiStack::pop();

    GfuiLabelSetText(GuiStack::top(), error_label, "");
    lobby = new NetworkLobbyMenu(player_name, car, Network::CLIENT, net, onDisconnected, this);
}

void
NetworkClientMenu::setWaitingToConnectScreen()
{
    GfLogTrace("Entering Network Wait Connection menu.\n");

    void *menu = GfuiScreenCreate();

    if (!menu)
        throw std::runtime_error("GfuiScreenCreate failed");

    void *mparam = GfuiMenuLoad("networkwaitconnectmenu.xml");

    if (!mparam)
        throw std::invalid_argument("GfuiMenuLoad failed");

    GfuiMenuCreateStaticControls(menu, mparam);
    GfuiMenuDefaultKeysAdd(menu);
    GuiStack::push(menu);

    client = new NetClient;
    client->connect(address, port, player_name, car, onConnect, this);
}

void
NetworkClientMenu::closeClient(void *arg)
{
    GuiStack::pop();
}

void
NetworkClientMenu::joinServer(void *arg)
{
    static_cast<NetworkClientMenu *>(arg)->setWaitingToConnectScreen();
}

void
NetworkClientMenu::setConnectMenu()
{
    void *menu = GfuiScreenCreate();

    if (!menu)
        throw std::runtime_error("GfuiScreenCreate failed");

    void *mparam = GfuiMenuLoad("networkclientconnectmenu.xml");

    if (!mparam)
        throw std::invalid_argument("GfuiMenuLoad failed");

    GfuiMenuCreateStaticControls(menu, mparam);
    ip_edit = GfuiMenuCreateEditControl(menu, mparam, "IPAddrEdit", this, NULL, changeAddress);
    port_edit = GfuiMenuCreateEditControl(menu, mparam, "PortEdit", this, NULL, changePort);
    player_edit = GfuiMenuCreateEditControl(menu, mparam, "PlayerNameEdit", this, NULL, changeName);
    error_label = GfuiMenuCreateLabelControl(menu, mparam, "ErrorMessage");

    lookupPlayerSetup(player_name, car);
    GfuiEditboxSetString(menu, player_edit, player_name.c_str());

    GfuiMenuCreateButtonControl(menu, mparam, "ConnectButton", this, joinServer);
    GfuiMenuCreateButtonControl(menu, mparam, "BackButton", this, clientDeactivate);

    GfuiMenuDefaultKeysAdd(menu);
    GfuiAddKey(menu, GFUIK_ESCAPE, "Back to previous menu", this, NULL, clientDeactivate);

    GfParmReleaseHandle(mparam);
    GuiStack::push(menu);

    GfuiEditboxSetString(menu, port_edit, std::to_string(Network::port).c_str());
    changePort(this);
}

void
NetworkClientMenu::lookupPlayerSetup(std::string &driver, std::string &car) const
{
    void *drvinfo = GfParmReadFileLocal(HM_DRV_FILE, GFPARM_RMODE_REREAD);

    if (!drvinfo)
        throw std::invalid_argument("GfParmReadFileLocal(" HM_DRV_FILE ") failed");

    static const char *robot_idx = ROB_SECT_ROBOTS "/" ROB_LIST_INDEX "/1";

    driver = GfParmGetStr(drvinfo, robot_idx, ROB_ATTR_NAME, "");
    car = GfParmGetStr(drvinfo, robot_idx, ROB_ATTR_CAR, "");
    GfParmReleaseHandle(drvinfo);
}
