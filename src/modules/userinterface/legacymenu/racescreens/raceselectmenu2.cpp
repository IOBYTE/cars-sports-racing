/***************************************************************************

    file        : raceselectmenu2.cpp
    created     : Sat Jan 27 00:36:29 CET 2024
    copyright   : (C) 2024 Xavier BERTAUX
    email       : bertauxx@gmail.com
    version     : $Id: raceselectmenu2.cpp 7827 2024-01-27 00:57:48Z shadow72 $

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
#include<iostream>

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
void       *RmRaceSelectMenuHandle2 = NULL;

// Menu control Ids
static int Endurance2ButtonId;
static int Championship2ButtonId;
static int Endurance3ButtonId;
static int Endurance4ButtonId;
static int Championship3ButtonId;

/* Called when the menu is activated */
static void
rmOnActivate(void * /* dummy */)
{
	GfLogTrace("Entering Race Mode Select menu\n");

    LmRaceEngine().reset();
}

/* Exit from Race engine */
/*static void
rmOnRaceSelectShutdown(void *prevMenu)
{
    //GfuiScreenActivate(prevMenu);
	
    //LmRaceEngine().cleanup();
	
    //LegacyMenu::self().shutdownGraphics(bUnloadModule=true);
//}*/

static void
onRaceSelectMenuActivate(void * /* dummy */)
{
    GfuiScreenActivate(RmRaceSelectInit(MenuHandle));
}

static void
rmOnSelectRaceManEndurance2(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(true);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = _pCategory+"endurance2";
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
rmOnSelectRaceManChampionship2(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(true);
    // Get the race managers with the given type
    std::string race = _pCategory+"championship2";
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
rmOnSelectRaceManEndurance3(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(true);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = _pCategory+"endurance3";
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
rmOnSelectRaceManEndurance4(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(true);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = _pCategory+"endurance4";
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
rmOnSelectRaceManChampionship3(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(true);
    // Get the race managers with the given type
    std::string race = _pCategory+"championship3";
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
rmRaceSelectNextPage(void *prevMenu)
{
    GfuiScreenActivate(RmRaceSelectInit3(RmRaceSelectMenuHandle2));
}

/* Initialize the 2th single player menu */
void *
RmRaceSelectInit2(void *prevMenu)
{
    if (RmRaceSelectMenuHandle2)
    {
        RmRaceSelectMenuHandle2 = NULL;
    }

	// Ask the RaceEngine what types of races should be allowed here
    //bool SupportsHumanDrivers = LmRaceEngine().supportsHumanDrivers();

    // Create screen, load menu XML descriptor and create static controls.
    RmRaceSelectMenuHandle2 = GfuiScreenCreate((float*)NULL,
											NULL, rmOnActivate, 
											NULL, (tfuiCallback)NULL, 
											1);
//#if defined(CLIENT_SERVER)
    void *hMenuXMLDesc = GfuiMenuLoad("csraceselectmenu2.xml");
/*#else
    void *hMenuXMLDesc = GfuiMenuLoad("raceselectmenu2.xml");
#endif*/
    GfuiMenuCreateStaticControls(RmRaceSelectMenuHandle2, hMenuXMLDesc);

    // Create the raceman type buttons and sub-type combo-boxes (if any).
    Endurance2ButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle2, hMenuXMLDesc, "Endurance2Button",
                                                        NULL, rmOnSelectRaceManEndurance2);
    Championship2ButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle2, hMenuXMLDesc, "Championship2Button",
                                                        NULL, rmOnSelectRaceManChampionship2);
    Endurance3ButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle2, hMenuXMLDesc, "Endurance3Button",
                                                        NULL, rmOnSelectRaceManEndurance3);
    Endurance4ButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle2, hMenuXMLDesc, "Endurance4Button",
                                                       NULL, rmOnSelectRaceManEndurance4);
    Championship3ButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle2, hMenuXMLDesc, "Championship3Button",
                                                NULL, rmOnSelectRaceManChampionship3);

    /*if(rmRanking[rmIndex] < 5)
        GfuiEnable(RmRaceSelectMenuHandle2, Endurance2ButtonId, GFUI_DISABLE);

    if(rmRanking[rmIndex] < 6)
        GfuiEnable(RmRaceSelectMenuHandle2, Championship2ButtonId, GFUI_DISABLE);

    if(rmRanking[rmIndex] < 8)
        GfuiEnable(RmRaceSelectMenuHandle2, Endurance3ButtonId, GFUI_DISABLE);

    if(rmRanking[rmIndex] < 9)
        GfuiEnable(RmRaceSelectMenuHandle2, Endurance4ButtonId, GFUI_DISABLE);

    if(rmRanking[rmIndex] < 7)
        GfuiEnable(RmRaceSelectMenuHandle2, Championship3ButtonId, GFUI_DISABLE);*/

    if((_pCategory.compare("01GPA-") != 0) || (_pCategory.compare("LS1-") != 0) ||
            (_pCategory.compare("LS1-") != 0) || (_pCategory.compare("LP1-") != 0))
    {
        GfuiVisibilitySet(RmRaceSelectMenuHandle2, Endurance3ButtonId, GFUI_INVISIBLE);
        GfuiVisibilitySet(RmRaceSelectMenuHandle2, Endurance4ButtonId, GFUI_INVISIBLE);
    }

    GfLogDebug("Category in racemanselect2= %s\n", _pCategory.c_str());

    /*if( rmRanking[10] > 4 && (rmRanking[8] > 6 || rmRanking[12] > 6))
    {
        GfuiVisibilitySet(RmRaceSelectMenuHandle2, Endurance3ButtonId, GFUI_VISIBLE);
        GfuiVisibilitySet(RmRaceSelectMenuHandle2, Endurance4ButtonId, GFUI_VISIBLE);
    }*/
	
    // Create Back button
    GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle2, hMenuXMLDesc, "BackButton",
                                prevMenu, onRaceSelectMenuActivate);

    // Create Next button
    GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle2, hMenuXMLDesc, "NextButton",
                                prevMenu, rmRaceSelectNextPage);

    // Close menu XML descriptor.
    GfParmReleaseHandle(hMenuXMLDesc);
    
    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(RmRaceSelectMenuHandle2);
    GfuiAddKey(RmRaceSelectMenuHandle2, GFUIK_ESCAPE, "Back To first race Menu",
               prevMenu, onRaceSelectMenuActivate, NULL);

    // Give the race engine the menu to come back to.
    LmRaceEngine().initializeState(RmRaceSelectMenuHandle2);

    return RmRaceSelectMenuHandle2;
}
