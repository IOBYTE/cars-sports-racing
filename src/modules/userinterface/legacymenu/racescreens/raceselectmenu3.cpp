/***************************************************************************

    file        : raceselectmenu.cpp
    created     : Sat Nov 16 09:36:29 CET 2002
    copyright   : (C) 2002 by Eric Espie
    email       : eric.espie@torcs.org   
    version     : $Id: raceselectmenu.cpp 7827 2021-12-17 00:57:48Z iobyte $                                  

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

/** @file   
    		Race selection menu
    @author	<a href=mailto:eric.espie@torcs.org>Eric Espie</a>
    @version	$Id: raceselectmenu.cpp 7827 2021-12-17 00:57:48Z iobyte $
*/

#include <map>
#include <algorithm>
#include <string>

#include <tgfclient.h>
#include <raceman.h>
#include <racemanagers.h>
#include <race.h>
#include <cars.h>

#include "legacymenu.h"
#include "racescreens.h"

class GfCars;
class GfDrivers;
class GfRaces;

// The Race Select menu.
void *RmRaceSelectMenuHandle3 = NULL;

// Menu control Ids
#if !defined(OFFICIAL)
static int GarageButtonId;
static int ChampionshipDevButtonId;
#endif

static int OnlineServerLabelId;
static int OnlineClientButtonId;
static int OnlineWatcherButtonId;

/* Called when the menu is activated */
static void
rmOnActivate(void * /* dummy */)
{
	GfLogTrace("Entering Race Mode Select menu\n");

    LmRaceEngine().reset();
}

/* Exit from Race engine */
static void
rmOnRaceSelectShutdown(void *prevMenu)
{
    GfuiScreenActivate(prevMenu);
	
    LmRaceEngine().cleanup();
	
	LegacyMenu::self().shutdownGraphics(/*bUnloadModule=*/true);
}

#ifndef OFFICIAL
static void
rmOnSelectRaceManChampionshipDev(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(true);
    // Get the race managers with the given type
    std::string race = "championshipdev";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        // Give the selected race manager to the race engine.
        LmRaceEngine().selectRaceman(pSelRaceMan);

        // Start the race configuration menus sequence.
        LmRaceEngine().configureRace(/* bInteractive */ true);
    }
    else
    {
        GfLogError("No such race manager (type '%s')\n", race.c_str());
    }
}

static void
rmOnSelecteRacemanGarageDev(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = "garage";
    GfLogDebug("Race = %s\n", race.c_str());
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);
    GfLogDebug("Race Practice = %s\n", race.c_str());

    if (pSelRaceMan)
    {
        // Give the selected race manager to the race engine.
        LmRaceEngine().selectRaceman(pSelRaceMan);

        // Start the race configuration menus sequence.
        LmRaceEngine().configureRace(/* bInteractive */ true);
    }
    else
    {
        GfLogError("No such race manager (type '%s')\n", race.c_str());
    }
}
#endif

static void
rmOnSelectRaceManOnlineServer(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = "netserver";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);
    GfLogDebug("Race Online Server = %s\n", race.c_str());

    if (pSelRaceMan)
    {
        // Give the selected race manager to the race engine.
        LmRaceEngine().selectRaceman(pSelRaceMan);

        // Start the race configuration menus sequence.
        LmRaceEngine().configureRace(/* bInteractive */ true);
    }
    else
    {
        GfLogError("No such race manager (type '%s')\n", race.c_str());
    }
}

static void
rmOnSelectRaceManOnlineClient(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = "netclient";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        // Give the selected race manager to the race engine.
        LmRaceEngine().selectRaceman(pSelRaceMan);

        // Start the race configuration menus sequence.
        LmRaceEngine().configureRace(/* bInteractive */ true);
    }
    else
    {
        GfLogError("No such race manager (type '%s')\n", race.c_str());
    }
}

static void
rmOnSelectRaceManOnlineWatcher(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = "netwatcher";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        // Give the selected race manager to the race engine.
        LmRaceEngine().selectRaceman(pSelRaceMan);

        // Start the race configuration menus sequence.
        LmRaceEngine().configureRace(/* bInteractive */ true);
    }
    else
    {
        GfLogError("No such race manager (type '%s')\n", race.c_str());
    }
}

static void
onRaceSelectMenuActivate(void * /* dummy */)
{
    GfuiScreenActivate(RmRaceSelectInit2(MenuHandle));
}

/* Initialize the single player menu */
void *
RmRaceSelectInit3(void *prevMenu)
{
    if (RmRaceSelectMenuHandle3)
    {
        RmRaceSelectMenuHandle3 = NULL;
    }

	// Ask the RaceEngine what types of races should be allowed here
    //bool SupportsHumanDrivers = LmRaceEngine().supportsHumanDrivers();

    // Create screen, load menu XML descriptor and create static controls.
    RmRaceSelectMenuHandle3 = GfuiScreenCreate((float*)NULL,
											NULL, rmOnActivate, 
											NULL, (tfuiCallback)NULL, 
											1);
#if defined(CLIENT_SERVER)
    void *hMenuXMLDesc = GfuiMenuLoad("csraceselectmenu3.xml");
#else
    void *hMenuXMLDesc = GfuiMenuLoad("raceselectmenu3.xml");
#endif
    GfuiMenuCreateStaticControls(RmRaceSelectMenuHandle3, hMenuXMLDesc);

#if !defined(OFFICIAL)
    ChampionshipDevButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle3, hMenuXMLDesc, "ChampionshipDevButton",
                                                      NULL, rmOnSelectRaceManChampionshipDev);

    GarageButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle3, hMenuXMLDesc, "GarageDevButton",
                                                 NULL, rmOnSelecteRacemanGarageDev);
#endif

    // Create the raceman type buttons and sub-type combo-boxes (if any).
    OnlineServerLabelId =
        GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle3, hMenuXMLDesc, "OnlineServerButton",
                            NULL, rmOnSelectRaceManOnlineServer);
    OnlineClientButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle3, hMenuXMLDesc, "OnlineClientButton",
                                                        NULL, rmOnSelectRaceManOnlineClient);
    OnlineWatcherButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle3, hMenuXMLDesc, "OnlineWatcherButton",
                                                        NULL, rmOnSelectRaceManOnlineWatcher);

    GfuiEnable(RmRaceSelectMenuHandle3, OnlineWatcherButtonId, GFUI_DISABLE);

    // Create Back button
    GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle3, hMenuXMLDesc, "BackButton",
                                prevMenu, onRaceSelectMenuActivate);

    // Close menu XML descriptor.
    GfParmReleaseHandle(hMenuXMLDesc);
    
    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(RmRaceSelectMenuHandle3);
    GfuiAddKey(RmRaceSelectMenuHandle, GFUIK_ESCAPE, "Back To Race Menu 2",
			   prevMenu, rmOnRaceSelectShutdown, NULL);

    // Give the race engine the menu to come back to.
    LmRaceEngine().initializeState(RmRaceSelectMenuHandle3);

    return RmRaceSelectMenuHandle3;
}
