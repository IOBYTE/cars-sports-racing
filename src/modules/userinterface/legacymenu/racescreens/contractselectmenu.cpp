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
#include <robot.h>
#include <raceman.h>
#include <racemanagers.h>
#include <race.h>
#include <cars.h>
#include <drivers.h>

#include "legacymenu.h"
#include "racescreens.h"

class GfCars;
class GfDrivers;
class GfRaces;

// Menu control Ids
static int   Challenge1ButtonId;
static int   Challenge2ButtonId;
static int   Challenge3ButtonId;
static int   Challenge4ButtonId;
static int   Endurance1ButtonId;
static int   Endurance2ButtonId;
static int   Endurance3ButtonId;
static int   Endurance4ButtonId;
static int   Championship1ButtonId;
static int   Championship2ButtonId;
static int   Championship3ButtonId;
static int   GarageButtonId;

static bool  rmCareer;

// The Race Select menu.
void *RmContractSelectMenuHandle = NULL;

static bool running;

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
    GfuiScreenActivate(MenuHandle);

    LmRaceEngine().cleanup();

    LegacyMenu::self().shutdownGraphics(/*bUnloadModule=*/true);
}

static void
rmOnContractRaceManChallenge1(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge1";
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
rmOnContractRaceManChallenge2(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge2";
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
rmOnContractRaceManchallenge3(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge3";
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
rmOnContractRaceManChallenge4(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge4";
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
rmOnContractRaceManEndurance1(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"endurance1";
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
rmOnContractRaceManEndurance2(void *pvRaceManTypeIndex)
{
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
rmOnContractRaceManEndurance3(void *pvRaceManTypeIndex)
{
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
rmOnContractRaceManEndurance4(void *pvRaceManTypeIndex)
{
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
rmOnContractRaceManChampionship1(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"championship1";
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
rmOnContractRaceManChampionship2(void *pvRaceManTypeIndex)
{
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
rmOnContractRaceManChampionship3(void *pvRaceManTypeIndex)
{
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

static
void rmRaceCategoryInit()
{
    GfLogDebug("Prefix category Race : \n");
    if (strcmp(rmCategory, "2011 GP American") == 0)
    {
        rmIndex    = 0;
        _pCategory = _pCat[rmIndex];

    }
    else if(strcmp(rmCategory, "Supercars Class C") == 0)
    {
        rmIndex    = 1;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "2012 GP American") == 0)
    {
        rmIndex    = 2;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Supercars Class B") == 0)
    {
        rmIndex    = 3;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "2001 GP American") == 0)
    {
        rmIndex    = 4;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Supercars Class A") == 0)
    {
        rmIndex    = 5;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Long Day Series GT2") == 0)
    {
        rmIndex    = 6;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Rally Series") == 0)
    {
        rmIndex    = 7;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Long Day Series GT1") == 0)
    {
        rmIndex    = 8;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Hypercars") == 0)
    {
        rmIndex    = 9;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Long Day Series P2") == 0)
    {
        rmIndex    = 10;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "1936 Grand Prix") == 0)
    {
        rmIndex    = 11;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Long Day Series P1") == 0)
    {
        rmIndex    = 12;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "1967 Grand Prix") == 0)
    {
        rmIndex    = 13;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "1991 Grand Prix") == 0)
    {
        rmIndex    = 14;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Stock Cars") == 0)
    {
        rmIndex    = 15;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "2001 Grand Prix") == 0)
    {
        rmIndex    = 16;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Prototype") == 0)
    {
        rmIndex    = 17;
        _pCategory = _pCat[rmIndex];
    }

    GfLogDebug("Prefix category Race : %s - Ranking index = %i\n", _pCategory.c_str(), rmIndex);
}

/* Initialize the single player menu */
void*
RmContractSelectInit(void *prevMenu, const char* contract, int index)
{
    if (RmContractSelectMenuHandle)
    {
        RmContractSelectMenuHandle = NULL;
    }

    rmCareer = true;

    LegacyMenu::self().setCareerMode(rmCareer);

    std::string strBase(GfLocalDir());
    std::string strPlayersFileName = "drivers/human/human.xml";
    std::string strGarageFileName = "drivers/human/garage.xml";
    std::string strContractFilename = "drivers/human/contracts.xml";

    const char* pszCarId = "";

    _pCategory = "";

    void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
        hparmHuman = GfParmReadFile(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
    {
        GfLogError("No usable human driver (human.xml not found or not readable)\n");
    }

    if (hparmHuman)
    {
        char sstring[128];

        GfLogDebug("Loadind contract select init human\n");

        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);

        const char *contract = GfParmGetStr(hparmHuman, sstring, "contract", "");

        rmCredit = (int)GfParmGetNum(hparmHuman, sstring, "credit", (char*)NULL, 6000);

        /*if(rmCar)
        {
            pszCarId = GfParmGetStr(hparmHuman, sstring, "car career", "");
            GfParmSetStr(hparmHuman, sstring, "car name", pszCarId);

            std::string car = GfCars::self()->getCar(pszCarId)->getCategoryName();
            rmCategory = car.c_str();
            GfLogDebug("Category = %s\n", rmCategory);

            GfParmWriteFile(0, hparmHuman, "human");
            GfParmReleaseHandle(hparmHuman);

            rmRaceCategoryInit();
        }*/
    }

	// Ask the RaceEngine what types of races should be allowed here
    //bool SupportsHumanDrivers = LmRaceEngine().supportsHumanDrivers();

    // Create screen, load menu XML descriptor and create static controls.
    RmContractSelectMenuHandle = GfuiScreenCreate((float*)NULL,
											NULL, rmOnActivate, 
											NULL, (tfuiCallback)NULL, 
											1);

    void *hMenuXMLDesc = GfuiMenuLoad("careermenu.xml");
    GfuiMenuCreateStaticControls(RmContractSelectMenuHandle, hMenuXMLDesc);

    int CarImageId = GfuiMenuCreateStaticImageControl(RmContractSelectMenuHandle, hMenuXMLDesc, "PortraitImage");
    GfuiStaticImageSet(RmContractSelectMenuHandle, CarImageId, "data/img/portrait8.png");

    int Text1Label = GfuiMenuCreateLabelControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Text1Label");
    int Text2Label = GfuiMenuCreateLabelControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Text2Label");
    int Text3Label = GfuiMenuCreateLabelControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Text3Label");
    int Text4Label = GfuiMenuCreateLabelControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Text4Label");
    int Race1Label = GfuiMenuCreateLabelControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Race1Label");
    int Race2Label = GfuiMenuCreateLabelControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Race2Label");

    /*if (rmLevel[rmIndex] == 1 || rmLevel[rmIndex] == 11 || rmLevel[rmIndex] == 21 || rmLevel[rmIndex] == 31)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very Good");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "which one you want to run in.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "Good luck !");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Corn Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Good");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "which one you want to run in.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "Good luck !");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Alicante");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        Challenge1ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge1Button",
                                                                NULL, rmOnContractRaceManChallenge1);
        Challenge2ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge2Button",
                                                                NULL, rmOnContractRaceManChallenge2);
    }
    else if (rmLevel[rmIndex] == 2 || rmLevel[rmIndex] == 12 || rmLevel[rmIndex] == 22 || rmLevel[rmIndex] == 32)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "well, very well,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "now the races will get a little more complicated.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the first on a grand prix track, the second on an Arizona Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Karwada Grand Prix");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Good");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "which one you want to run in.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "Good luck !");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Peters");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Karwada");
        }
        Challenge3ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge3Button",
                                                                NULL, rmOnContractRaceManchallenge3);
        Challenge4ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge4Button",
                                                                NULL, rmOnContractRaceManChallenge4);
    }
    else if (rmLevel[rmIndex] == 3 || rmLevel[rmIndex] == 13 || rmLevel[rmIndex] == 23 || rmLevel[rmIndex] == 33)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend.the first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second on an Arizona Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Karwada Grand Prix");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very Good");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend. The first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second is at Auriac track for 160 Km");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Alicante");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        Challenge1ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge1Button",
                                                                NULL, rmOnContractRaceManChallenge1);
        Endurance2ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "endurance2Button",
                                                                NULL, rmOnContractRaceManEndurance2);
    }
    else if (rmLevel[rmIndex] == 4 || rmLevel[rmIndex] == 14 || rmLevel[rmIndex] == 24 || rmLevel[rmIndex] == 34)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend.the first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second on an Arizona Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Karwada Grand Prix");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very Good");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "The first on at Peters track for 15 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second is at Karwada track for 800 Km");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Peters");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Karwada");
        }
        Challenge3ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge3Button",
                                                                NULL, rmOnContractRaceManchallenge3);
        Endurance4ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "endurance4Button",
                                                                NULL, rmOnContractRaceManEndurance4);
    }
    else if (rmLevel[rmIndex] == 5 || rmLevel[rmIndex] == 15 || rmLevel[rmIndex] == 25 || rmLevel[rmIndex] == 35)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend.the first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second on an Arizona Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Karwada Grand Prix");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "You're the best");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "The first on at Brondehach track for 100 Km,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second is at Auriac track for 15 laps");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Brondehach");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        Endurance1ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "endurance1Button",
                                                                NULL, rmOnContractRaceManEndurance1);
        Challenge2ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge2Button",
                                                                NULL, rmOnContractRaceManChallenge2);
    }
    else if (rmLevel[rmIndex] == 6 || rmLevel[rmIndex] == 16 || rmLevel[rmIndex] == 26 || rmLevel[rmIndex] == 36)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend.the first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second on an Arizona Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Karwada Grand Prix");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "You manage to surprise me");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "The first on at Brondehach track for 800 Km,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second is at Karwada track for 25 laps");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Brondehach");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Karwada");
        }
        Endurance3ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "endurance3Button",
                                                                NULL, rmOnContractRaceManEndurance3);
        Challenge4ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge4Button",
                                                                NULL, rmOnContractRaceManChallenge4);
    }
    else if (rmLevel[rmIndex] == 7 || rmLevel[rmIndex] == 17 || rmLevel[rmIndex] == 27 || rmLevel[rmIndex] == 37)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend.the first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second on an Arizona Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Karwada Grand Prix");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Now you can do a Championship season");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "The first on at Peters track for 15 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second is a championship with 6 tracks on 160 Km");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Peters");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Races Championship");
        }
        Challenge3ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Challenge3Button",
                                                                NULL, rmOnContractRaceManchallenge3);
        Championship2ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Championship2Button",
                                                                NULL, rmOnContractRaceManChampionship2);
    }
    else if (rmLevel[rmIndex] == 8 || rmLevel[rmIndex] == 18 || rmLevel[rmIndex] == 28 || rmLevel[rmIndex] == 38)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend.the first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second on an Arizona Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Karwada Grand Prix");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Now you can do a Championship season");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "The first is the Championship with 6 tracks on 80 Km,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second is at Auriac on 160 Km");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Races Championship");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Races at Auriac");
        }
        Championship1ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Championship1Button",
                                                                NULL, rmOnContractRaceManChampionship1);
        Endurance2ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "endurance2Button",
                                                                NULL, rmOnContractRaceManEndurance2);
    }
    else if (rmLevel[rmIndex] == 9 || rmLevel[rmIndex] == 19 || rmLevel[rmIndex] == 29 || rmLevel[rmIndex] == 39)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "Two races are available this weekend.the first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second on an Arizona Speedway");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Race on Karwada Grand Prix");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Race on Arizona Speedway");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmContractSelectMenuHandle, Text1Label, "We are almost at the end of the season");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text3Label, "The first is the Championship with 7 tracks on 320 Km,");
            GfuiLabelSetText(RmContractSelectMenuHandle, Text4Label, "the second is at Karwada on 25 laps");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race1Label, "Races Championship");
            GfuiLabelSetText(RmContractSelectMenuHandle, Race2Label, "Races at Karwada");
        }
        Championship3ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "Championship3Button",
                                                                NULL, rmOnContractRaceManChampionship3);
        Endurance2ButtonId = GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "endurance2Button",
                                                                NULL, rmOnContractRaceManEndurance2);
    }*/

    // Create Back button
    //GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "QuitButton",
                                //prevMenu, rmOnRaceSelectShutdown);

    // Create Next button
    /*NextButtonId =
            GfuiMenuCreateButtonControl(RmContractSelectMenuHandle, hMenuXMLDesc, "NextButton",
                                NULL, rmRaceSelectNextPage);*/

    // Close menu XML descriptor.
    //GfParmReleaseHandle(hMenuXMLDesc);
    
    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(RmContractSelectMenuHandle);
    GfuiAddKey(RmContractSelectMenuHandle, GFUIK_ESCAPE, "Back To Main Menu",
               prevMenu, rmOnRaceSelectShutdown, NULL);

    // Give the race engine the menu to come back to.
    LmRaceEngine().initializeState(RmContractSelectMenuHandle);

    return RmContractSelectMenuHandle;
}
