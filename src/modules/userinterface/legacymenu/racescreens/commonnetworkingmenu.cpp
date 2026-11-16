#include <cstdlib>
#include <cstdio>
#include <string>
#include <stdexcept>

#include <racescreens.h>
#include <tgf.hpp>
#include <tgfclient.h>
//#include <guistack.h>

#include <race.h>
#include <racemanagers.h>
#include <networkclientmenu.h>
#include <networkservermenu.h>
#include "legacymenu.h"
#include "racescreens.h"
#include "garagemenu.h"

static void
NetworkClientConnectMenu(void *arg)
{
    // Pre-C++11 support is a requirement, so C++11 smart pointers
    // cannot be used here, unfortunately.
    // The object is deleted itself when the "Back" button is pressed,
    // so there is no need to keep track of it.
    new NetworkClientMenu;
}

static void
NetworkServerConnectMenu(void *arg)
{
    // Pre-C++11 support is a requirement, so C++11 smart pointers
    // cannot be used here, unfortunately.
    // The object is deleted itself when the "Back" button is pressed,
    // so there is no need to keep track of it.
    new NetworkServerMenu;
}

static void
NetworkMenuExit(void *)
{
    //GuiStack::pop();
    GfuiScreenActivate(RmRaceSelectMenuHandle);
}

void
RmNetworkMenu()
{
    tRmInfo* reInfo = LmRaceEngine().inData();
    void *params = reInfo->params;

    if (!params)
        throw std::invalid_argument("Expected reInfo->params != NULL");

    void *racemanMenuHdle = GfuiScreenCreate();
    void *mparam = GfuiMenuLoad("networkmenu.xml");

    GfuiMenuCreateStaticControls(racemanMenuHdle, mparam);

    const int nTitleLabelId = GfuiMenuCreateLabelControl(racemanMenuHdle, mparam, "TitleLabel");
    const char* pszTitle = GfParmGetStr(params, RM_SECT_HEADER, RM_ATTR_NAME, 0);

    if (pszTitle)
        GfuiLabelSetText(racemanMenuHdle, nTitleLabelId, pszTitle);

    GfuiMenuCreateButtonControl(racemanMenuHdle, mparam, "HostButton",
                                racemanMenuHdle, NetworkServerConnectMenu);
    GfuiMenuCreateButtonControl(racemanMenuHdle, mparam, "JoinButton",
                                racemanMenuHdle, NetworkClientConnectMenu);
    GfuiMenuCreateButtonControl(racemanMenuHdle, mparam, "BackButton",
                                NULL, NetworkMenuExit);

    GfuiMenuDefaultKeysAdd(racemanMenuHdle);
    GfuiAddKey(racemanMenuHdle, GFUIK_ESCAPE, "Back to previous menu",
                                racemanMenuHdle, NULL, NetworkMenuExit);
    GfParmReleaseHandle(mparam);
    //GuiStack::push(racemanMenuHdle);
}
