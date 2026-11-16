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

//#include <map>
//#include <algorithm>
#include <string>
//#include <memory>

#include <tgfclient.h>
#include <robot.h>
#include <raceman.h>
#include <racemanagers.h>
#include <race.h>
#include <cars.h>
#include <drivers.h>

#include "legacymenu.h"
#include "racescreens.h"
#include "tradermenu.h"

class GfCars;
class GfDrivers;
class GfRaces;

const char* cFame[] = { "fame0", "fame1", "fame2", "fame3", "fame4", "fame5",
                       "fame6", "fame7", "fame8", "fame9", "fame10", "fame11",
                       "fame12", "fame13", "fame14", "fame15", "fame16", "fame17" };

// Menu control Ids
static int   PracticeButtonId;
static int   Challenge1ButtonId;
static int   Challenge2ButtonId;
static int   Challenge3ButtonId;
static int   Challenge4ButtonId;
static int   Challenge5ButtonId;
static int   Challenge6ButtonId;
static int   Challenge7ButtonId;

static int   Endurance1ButtonId;
static int   Endurance2ButtonId;
static int   Endurance3ButtonId;
static int   Endurance4ButtonId;

static int   Championship1ButtonId;
static int   Championship2ButtonId;
static int   Championship3ButtonId;
static int   SpecialButtonId;
static int   GarageButtonId;
static int   UpgradeButtonId;

//bool         rmUpdate = false;
bool         rmCar          = false;
bool         rmLicence      = false;
bool         rmChallenge    = false;
bool         rmEndurance    = false;
int          rmLevel[];
int          rmSpecial[];
int          contracts;
int          special;
unsigned char rmContracts[];
double       rmFame = 0.0;
double       rmSkillMult;

//unsigned int rmCredit = 0;

static const char* CurCarName = "";
static std::string CurCarCategory;
static std::string CurChampionship;
static std::string CurContract;
static std::string CurSpecial;

// The Race Select menu.
void *RmCareerSelectMenuHandle = nullptr;

// The car selection menu.
static RmTraderMenu TraderMenu;
static bool running;

int index2;
int credit2;

/* Called when the menu is activated */
static void
rmOnActivate(void * /* dummy */)
{
	GfLogTrace("Entering Race Mode Select menu\n");

    LmRaceEngine().reset();
}

/* Buy car menu */
static void
rmOnRaceSelectBuy(void *prevMenu)
{
    TraderMenu.setPreviousMenuHandle(prevMenu);
    TraderMenu.runMenu("sc-deckard-conejo");
}

/* garage car menu */
static void
rmOnRaceSelectGarage(void *prevMenu)
{
    GfuiScreenActivate(RmGarageSelectInit(RmCareerSelectMenuHandle));
}

/* upgrade car menu */
static void
rmOnRaceSelectUpgrade(void *prevMenu)
{
    GfuiScreenActivate(RmTuningSelectInit(RmCareerSelectMenuHandle));
}

/* Exit from Race engine */
static void
rmOnRaceSelectShutdown(void *prevMenu)
{
    GfuiScreenActivate(MenuHandle);

    LmRaceEngine().cleanup();
    LegacyMenu::self().setCareerMode(false);

    LegacyMenu::self().shutdownGraphics(/*bUnloadModule=*/true);
}

static void
rmOnRaceSelectShutdown2(void *prevMenu)
{
    std::string strBase(GfLocalDir());
    std::string strPlayersFileName = "drivers/human/human.xml";
    char sstring[256];

    void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
    snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
    GfParmSetNum(hparmHuman, sstring, "contract0", (char*)NULL, rmContracts[rmIndex]);
    GfParmWriteFile(0, hparmHuman, "human");
    GfParmReleaseHandle(hparmHuman);

    GfuiScreenActivate(RmCareerSelectInit(MenuHandle));

    LmRaceEngine().cleanup();
}

static void
rmOnSelectRaceManPractice(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCat[rmIndex]+"practice";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);
    GfLogDebug("Race Practice = %s\n", race.c_str());
    running = true;

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChallenge1(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge1";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(true);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChallenge2(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge2";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(true);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManchallenge3(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge3";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(true);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChallenge4(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge4";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(true);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChallenge5(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge5";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(true);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChallenge6(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge6";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(true);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChallenge7(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"challenge7";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(true);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManEndurance1(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"endurance1";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(true);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManEndurance2(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"endurance2";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(true);
        LegacyMenu::self().setChampionshipMode(false);
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
    // Get the race managers with the given type
    std::string race = _pCategory+"endurance3";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(true);
        LegacyMenu::self().setChampionshipMode(false);
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
    // Get the race managers with the given type
    std::string race = _pCategory+"endurance4";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(true);
        LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChampionship1(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = _pCategory+"championship1";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);
    GfLogDebug("#Career Championship 1 = %s\n", race.c_str());

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(true);
        std::string strBase(GfLocalDir());
        std::string strPlayersFileName = "drivers/human/human.xml";
        char sstring[256];

        void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetStr(hparmHuman, sstring, "championship", race.c_str());
        GfParmWriteFile(0, hparmHuman, "human");
        GfParmReleaseHandle(hparmHuman);
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
    // Get the race managers with the given type
    std::string race = _pCategory+"championship2";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);
    GfLogDebug("#Career Championship 2 = %s\n", race.c_str());

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(true);
        std::string strBase(GfLocalDir());
        std::string strPlayersFileName = "drivers/human/human.xml";
        char sstring[256];

        void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetStr(hparmHuman, sstring, "championship", race.c_str());
        GfParmWriteFile(0, hparmHuman, "human");
        GfParmReleaseHandle(hparmHuman);
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
    // Get the race managers with the given type
    std::string race = _pCategory+"championship3";
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);
    GfLogDebug("#Career Championship 3 = %s\n", race.c_str());

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(true);
        std::string strBase(GfLocalDir());
        std::string strPlayersFileName = "drivers/human/human.xml";
        char sstring[256];

        void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetStr(hparmHuman, sstring, "championship", race.c_str());
        GfParmWriteFile(0, hparmHuman, "human");
        GfParmReleaseHandle(hparmHuman);

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
rmOnSelectRaceManSpecial(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = CurSpecial;
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(true);
        LegacyMenu::self().setEnduranceMode(false);
        LegacyMenu::self().setChampionshipMode(false);
        std::string strBase(GfLocalDir());
        std::string strPlayersFileName = "drivers/human/human.xml";
        char sstring[256];

        void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetNum(hparmHuman, sstring, "special0", (char*)NULL, rmSpecial[rmIndex]);
        GfParmWriteFile(0, hparmHuman, "human");
        GfParmReleaseHandle(hparmHuman);
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
rmOnSelectRaceManContracts(void *pvRaceManTypeIndex)
{
    // Get the race managers with the given type
    std::string race = CurContract;
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);
    GfLogDebug("#Career Contract = %s\n", race.c_str());

    if (pSelRaceMan)
    {
        LegacyMenu::self().setChallengeMode(false);
        LegacyMenu::self().setEnduranceMode(true);
        LegacyMenu::self().setChampionshipMode(false);
        rmCredit += credit2;
        std::string strBase(GfLocalDir());
        std::string strPlayersFileName = "drivers/human/human.xml";
        char sstring[256];

        void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetStr(hparmHuman, sstring, ROB_ATTR_CAR, CurCarName);
        GfParmSetNum(hparmHuman, sstring, "credit", (char*)NULL, rmCredit);
        GfParmSetNum(hparmHuman, sstring, "contract0", (char*)NULL, rmContracts[rmIndex]);
        GfParmWriteFile(0, hparmHuman, "human");
        GfParmReleaseHandle(hparmHuman);

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
    std::string category = rmCategory;

    if (strcmp(rmCategory, "Supercars Class C") == 0)
    {
        rmIndex    = 0;
        _pCategory = _pCat[rmIndex];

    }
    else if(strcmp(rmCategory, "2009 Grand Prix 5") == 0)
    {
        rmIndex    = 1;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Supercars Class B") == 0)
    {
        rmIndex    = 2;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "2010 Grand Prix 4") == 0)
    {
        rmIndex    = 3;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Supercars Class A") == 0)
    {
        rmIndex    = 4;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "1991 Grand Prix") == 0)
    {
        rmIndex    = 5;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Long Day Series GT2") == 0)
    {
        rmIndex    = 6;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "2001 Grand Prix") == 0)
    {
        rmIndex    = 7;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Long Day Series GT1") == 0)
    {
        rmIndex    = 8;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "2001 GP American") == 0)
    {
        rmIndex    = 9;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Long Day Series P2") == 0)
    {
        rmIndex    = 10;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "2011 GP American") == 0)
    {
        rmIndex    = 11;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Long Day Series P1") == 0)
    {
        rmIndex    = 12;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "2012 GP American") == 0)
    {
        rmIndex    = 13;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "1936 Grand Prix") == 0)
    {
        rmIndex    = 14;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "1967 Grand Prix") == 0)
    {
        rmIndex    = 15;
        _pCategory = _pCat[rmIndex];
    }

    GfLogDebug("Prefix category Race : %s - Ranking index = %i\n", _pCategory.c_str(), rmIndex);
}

static
bool rmValidContract(int ind, int ind2)
{
    bool valid = false;
    GfLogDebug("# Valid contract - Fame = %.2f - index = %i - index2 = %i - "
              "rmContract[%i] = %i\n", rmFame, ind, ind2, rmIndex, rmContracts[rmIndex]);

    if (ind > 3 && rmFame > 1.6)
    {
        if (rmIndex == 0)
        {
            if((ind2 < 10) && (rmFame > 1.6) && (rmFame < 5.0) && rmContracts[rmIndex] == 0)
            {
                GfLogDebug("#select contract 1\n");
                CurContract = "SC-C-contract1";
                CurCarName = "sc-kanagawa-z35gts";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(((ind2 < 10) && (rmFame > 9.0) && (rmContracts[rmIndex] < 2)))
            {
                GfLogDebug("#select contract 2\n");
                rmContracts[rmIndex] = 3;
                CurContract = "SC-C-contract2";
                CurCarName = "sc-deckard-conejo";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmContracts[rmIndex] < 4))
            {
                GfLogDebug("#select contract 3\n");
                rmContracts[rmIndex] = 4;
                CurContract = "SC-C-contract3";
                CurCarName = "sc-deckard-conejo";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 10 && rmFame > 50.0 && (rmContracts[rmIndex] < 8))
            {
                GfLogDebug("#select contract 4\n");
                rmContracts[rmIndex] = 8;
                CurContract = "SC-C-contract4";
                CurCarName = "sc-murasama-nsx";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 90.0 && (rmContracts[rmIndex] < 16))
            {
                GfLogDebug("#select contract championship\n");
                rmContracts[rmIndex] = 16;
                CurContract = "SC-C-contract-championship";
                CurCarName = "sc-murasama-nsx";
                credit2 = 9000;
                contracts = 16;
                valid = true;
            }
        }
        else if (rmIndex == 1)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && rmContracts[rmIndex] == 0)
            {
                GfLogDebug("#select contract 1\n");
                CurContract = "09GP5-contract1";
                CurCarName = "sc-kanagawa-z35gts";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmContracts[rmIndex] < 2))
            {
                GfLogDebug("#select contract 2\n");
                rmContracts[rmIndex] = 3;
                CurContract = "09GP5-contract2";
                CurCarName = "09gp5-fmc";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmContracts[rmIndex] < 4))
            {
                GfLogDebug("#select contract 3\n");
                rmContracts[rmIndex] = 4;
                CurContract = "09GP5-contract3";
                CurCarName = "09gp5-fmc";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 10 && rmFame > 50.0 && (rmContracts[rmIndex] < 8))
            {
                GfLogDebug("#select contract 4\n");
                rmContracts[rmIndex] = 8;
                CurContract = "09GP5-contract4";
                CurCarName = "09gp5-fmc";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 90.0 && (rmContracts[rmIndex] < 16))
            {
                GfLogDebug("#select contract championship\n");
                rmContracts[rmIndex] = 16;
                CurContract = "09GP5-contract-championship";
                CurCarName = "09gp5-fmc";
                credit2 = 9000;
                contracts = 16;
                valid = true;
            }
        }
        else if (rmIndex == 2)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && rmContracts[rmIndex] == 0)
            {
                GfLogDebug("#select contract 1\n");
                CurContract = "SC-B-contract1";
                CurCarName = "sc-cavallo-360";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmContracts[rmIndex] < 2))
            {
                GfLogDebug("#select contract 2\n");
                rmContracts[rmIndex] = 3;
                CurContract = "SC-B-contract2";
                CurCarName = "sc-boxer-96";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmContracts[rmIndex] < 4))
            {
                GfLogDebug("#select contract 3\n");
                rmContracts[rmIndex] = 4;
                CurContract = "SC-B-contract3";
                CurCarName = "sc-fmc-gt4";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 10 && rmFame > 50.0 && (rmContracts[rmIndex] < 8))
            {
                GfLogDebug("#select contract 4\n");
                rmContracts[rmIndex] = 8;
                CurContract = "SC-B-contract4";
                CurCarName = "sc-cavallo-360";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 90.0 && (rmContracts[rmIndex] < 16))
            {
                GfLogDebug("#select contract championship\n");
                rmContracts[rmIndex] = 16;
                CurContract = "09GP5-Contract-championship";
                CurCarName = "sc-boxer-96";
                credit2 = 9000;
                contracts = 16;
                valid = true;
            }
        }
        else if (rmIndex == 3)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && rmContracts[rmIndex] == 0)
            {
                GfLogDebug("#select contract 1\n");
                CurContract = "10GP4-contract1";
                CurCarName = "10gp4-diamond";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmContracts[rmIndex] < 2))
            {
                GfLogDebug("#select contract 2\n");
                rmContracts[rmIndex] = 3;
                CurContract = "10GP4-contract2";
                CurCarName = "10gp4-diamond";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmContracts[rmIndex] < 4))
            {
                GfLogDebug("#select contract 3\n");
                rmContracts[rmIndex] = 4;
                CurContract = "10GP4-contract3";
                CurCarName = "10gp4-diamond";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 10 && rmFame > 50.0 && (rmContracts[rmIndex] < 8))
            {
                GfLogDebug("#select contract 4\n");
                rmContracts[rmIndex] = 8;
                CurContract = "10GP4-contract4";
                CurCarName = "10gp4-diamond";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 90.0 && (rmContracts[rmIndex] < 16))
            {
                GfLogDebug("#select contract championship\n");
                rmContracts[rmIndex] = 16;
                CurContract = "10GP4-Contract-championship";
                CurCarName = "10gp4-diamond";
                credit2 = 9000;
                contracts = 16;
                valid = true;
            }
        }
        else if (rmIndex == 4)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && rmContracts[rmIndex] == 0)
            {
                GfLogDebug("#select contract 1\n");
                CurContract = "SC-A-contract1";
                CurCarName = "sc-lynx-220";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmContracts[rmIndex] < 2))
            {
                GfLogDebug("#select contract 2\n");
                rmContracts[rmIndex] = 3;
                CurContract = "SC-A-contract2";
                CurCarName = "sc-spirit-300";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmContracts[rmIndex] < 4))
            {
                GfLogDebug("#select contract 3\n");
                rmContracts[rmIndex] = 4;
                CurContract = "SC-A-contract3";
                CurCarName = "sc-vieringe-4s";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 10 && rmFame > 50.0 && (rmContracts[rmIndex] < 8))
            {
                GfLogDebug("#select contract 4\n");
                rmContracts[rmIndex] = 8;
                CurContract = "SC-A-contract4";
                CurCarName = "sc-lynx-220";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 90.0 && (rmContracts[rmIndex] < 16))
            {
                GfLogDebug("#select contract championship\n");
                rmContracts[rmIndex] = 16;
                CurContract = "SC-A-Contract-championship";
                CurCarName = "sc-spirit-300";
                credit2 = 9000;
                contracts = 16;
                valid = true;
            }
        }
        else if (rmIndex == 5)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && rmContracts[rmIndex] == 0)
            {
                GfLogDebug("#select contract 1\n");
                CurContract = "91GP-contract1";
                CurCarName = "91gp-boxer-amani";
                rmContracts[rmIndex] = 1;
                credit2 = 6000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmContracts[rmIndex] < 2))
            {
                GfLogDebug("#select contract 2\n");
                rmContracts[rmIndex] = 3;
                CurContract = "91GP-contract2";
                CurCarName = "91gp-deckard-ags01";
                credit2 = 8000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmContracts[rmIndex] < 4))
            {
                GfLogDebug("#select contract 3\n");
                rmContracts[rmIndex] = 4;
                CurContract = "91GP-contract3";
                CurCarName = "91gp-cavallo-642";
                credit2 = 10000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 10 && rmFame > 50.0 && (rmContracts[rmIndex] < 8))
            {
                GfLogDebug("#select contract 4\n");
                rmContracts[rmIndex] = 8;
                CurContract = "91GP-contract4";
                CurCarName = "91gp-deckard-b190";
                credit2 = 15000;
                contracts = 8;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 90.0 && (rmContracts[rmIndex] < 16))
            {
                GfLogDebug("#select contract championship\n");
                rmContracts[rmIndex] = 16;
                CurContract = "91GP-Contract-championship";
                CurCarName = "91gp-diamond-fw14";
                credit2 = 30000;
                contracts = 16;
                valid = true;
            }
        }
    }

    return valid;
}

static
bool rmValidSpecial(int ind, int ind2)
{
    bool valid = false;
    int mask  = 1 << 0;
    int mask1 = 1 << 1;
    int mask2 = 1 << 2;
    int mask3 = 1 << 3;

    GfLogDebug("# Valid Special - Fame = %.2f - index = %i - index2 = %i - "
              "rmSpecial[%i] = %i\n", rmFame, ind, ind2, rmIndex, rmSpecial[rmIndex]);

    if (ind > 4 && rmFame > 1.6)
    {
        if (rmIndex == 0)
        {
            if(ind2 < 20 && rmFame > 5.0 && rmFame < 7.0 && (rmSpecial[rmIndex] & mask) == 0)
            {
                GfLogDebug("#select special 1\n");
                CurSpecial = "SC-C-special1";
                CurCarName = "sc-kanagawa-z35gts";
                rmSpecial[rmIndex] += 1;
                credit2 = 2000;
                special = 1;
                valid = true;
            }
            else if(ind2 < 24 && rmFame > 9.0 && (rmSpecial[rmIndex] & mask1) == 0)
            {
                GfLogDebug("#select special 2\n");
                rmSpecial[rmIndex] += 2;
                CurSpecial = "SC-C-special2";
                CurCarName = "sc-deckard-conejo";
                credit2 = 2500;
                special = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 20 && rmFame > 20.0 && rmFame < 40.0 && (rmSpecial[rmIndex] & mask2) == 0)
            {
                GfLogDebug("#select special 3\n");
                rmSpecial[rmIndex] = 4;
                CurSpecial = "SC-C-special3";
                CurCarName = "sc-deckard-conejo";
                credit2 = 3000;
                special = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 30 && rmFame > 50.0 && (rmSpecial[rmIndex] & mask3) == 0)
            {
                GfLogDebug("#select contract 4\n");
                rmSpecial[rmIndex] = 8;
                CurSpecial = "SC-C-special4";
                CurCarName = "sc-murasama-nsx";
                credit2 = 3500;
                special = 8;
                valid = true;
            }
        }
        else if (rmIndex == 1)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && (rmSpecial[rmIndex] & mask) == 0)
            {
                GfLogDebug("#select special 1\n");
                CurSpecial = "09GP5-special1";
                CurCarName = "09gp5-fmc";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmSpecial[rmIndex] & mask1) == 0)
            {
                GfLogDebug("#select special 2\n");
                rmContracts[rmIndex] = 3;
                CurSpecial = "09GP5-special2";
                CurCarName = "09gp5-fmc";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmSpecial[rmIndex] & mask2) == 0)
            {
                GfLogDebug("#select special 3\n");
                rmContracts[rmIndex] = 4;
                CurSpecial = "09GP5-special3";
                CurCarName = "09gp5-fmc";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 50.0 && (rmSpecial[rmIndex] & mask3) == 0)
            {
                GfLogDebug("#select special 4\n");
                rmContracts[rmIndex] = 8;
                CurSpecial = "09GP5-special4";
                CurCarName = "09gp5-fmc";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
        }
        else if (rmIndex == 2)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && (rmSpecial[rmIndex] & mask) == 0)
            {
                GfLogDebug("#select special 1\n");
                CurSpecial = "SC-B-special1";
                CurCarName = "sc-cavallo-360";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmSpecial[rmIndex] & mask1) == 0)
            {
                GfLogDebug("#select special 2\n");
                rmContracts[rmIndex] = 3;
                CurSpecial = "SC-B-special2";
                CurCarName = "sc-boxer-96";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmSpecial[rmIndex] & mask2) == 0)
            {
                GfLogDebug("#select special 3\n");
                rmContracts[rmIndex] = 4;
                CurSpecial = "SC-B-special3";
                CurCarName = "sc-fmc-gt4";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 50.0 && (rmSpecial[rmIndex] & mask3) == 0)
            {
                GfLogDebug("#select special 4\n");
                rmContracts[rmIndex] = 8;
                CurSpecial = "SC-B-special4";
                CurCarName = "sc-cavallo-360";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
        }
        else if (rmIndex == 3)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && (rmSpecial[rmIndex] & mask) == 0)
            {
                GfLogDebug("#select special 1\n");
                CurSpecial = "10GP4-special1";
                CurCarName = "10gp4-diamond";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmSpecial[rmIndex] & mask1) == 0)
            {
                GfLogDebug("#select special 2\n");
                rmContracts[rmIndex] = 3;
                CurSpecial = "10GP4-special2";
                CurCarName = "10gp4-diamond";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmSpecial[rmIndex] & mask2) == 0)
            {
                GfLogDebug("#select special 3\n");
                rmContracts[rmIndex] = 4;
                CurSpecial = "10GP4-special3";
                CurCarName = "10gp4-diamond";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 50.0 && (rmSpecial[rmIndex] & mask3) == 0)
            {
                GfLogDebug("#select special 4\n");
                rmContracts[rmIndex] = 8;
                CurSpecial = "10GP4-special4";
                CurCarName = "10gp4-diamond";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
        }
        else if (rmIndex == 4)
        {
            if(ind2 < 10 && rmFame > 1.6 && rmFame < 5.0 && (rmSpecial[rmIndex] & mask) == 0)
            {
                GfLogDebug("#select special 1\n");
                CurSpecial = "SC-A-special1";
                CurCarName = "sc-lynx-220";
                rmContracts[rmIndex] = 1;
                credit2 = 1000;
                contracts = 1;
                valid = true;
            }
            else if(ind2 < 10 && rmFame > 9.0 && (rmSpecial[rmIndex] & mask1) == 0)
            {
                GfLogDebug("#select special 2\n");
                rmContracts[rmIndex] = 3;
                CurSpecial = "SC-A-special2";
                CurCarName = "sc-spirit-300";
                credit2 = 2000;
                contracts = 2;
                valid = true;
            }
            else if(ind > 5 && ind2 > 10 && rmFame > 20.0 && rmFame < 40.0 && (rmSpecial[rmIndex] & mask2) == 0)
            {
                GfLogDebug("#select special 3\n");
                rmContracts[rmIndex] = 4;
                CurSpecial = "SC-A-special3";
                CurCarName = "sc-vieringe-4s";
                credit2 = 3000;
                contracts = 4;
                valid = true;
            }
            else if(ind > 7 && ind2 > 20 && rmFame > 50.0 && (rmSpecial[rmIndex] & mask3) == 0)
            {
                GfLogDebug("#select special 4\n");
                rmContracts[rmIndex] = 8;
                CurSpecial = "SC-A-special4";
                CurCarName = "sc-lynx-220";
                credit2 = 6000;
                contracts = 8;
                valid = true;
            }
        }
    }

    return valid;
}

/* Initialize the single player menu */
void*
RmCareerSelectInit(void *prevMenu)
{
    if (RmCareerSelectMenuHandle)
    {
        RmCareerSelectMenuHandle = NULL;
    }

    //std::string strBase(GfLocalDir());
    std::string strPlayersFileName = "drivers/human/human.xml";
    std::string strGarageFileName = "drivers/human/garage.xml";
    std::string strContractFilename = "drivers/human/contracts.xml";
    std::string strAiConfigFileName ="config/raceman/extra/skill.xml";

    void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
        hparmHuman = GfParmReadFile(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
    {
        GfLogError("No usable human driver (human.xml not found or not readable)\n");
    }

    if(hparmHuman)
    {
        char drvSectionPath[128];
        snprintf(drvSectionPath, sizeof(drvSectionPath), "%s/%s/%u", ROB_SECT_ROBOTS, ROB_LIST_INDEX, 1);
        const char *strSkillLevel = GfParmGetStr(hparmHuman, drvSectionPath, ROB_ATTR_LEVEL, ROB_VAL_SEMI_PRO);
        bool skill = (strcmp(strSkillLevel, "semi-pro") == 0) || (strcmp(strSkillLevel, "pro") == 0);
        int playerskill = 0;
        int mult = 0;
        double mult1, dmult;

        if(strcmp(strSkillLevel, "arcade") == 0)
        {
            playerskill = 0;
            mult = 50;
        }
        else if (strcmp(strSkillLevel, "semi-rookie") == 0)
        {
            playerskill = 1;
            mult = 40;
        }
        else if (strcmp(strSkillLevel, "rookie") == 0)
        {
            playerskill = 2;
            mult = 30;
        }
        else if (strcmp(strSkillLevel, "amateur") == 0)
        {
            playerskill = 3;
            mult = 20;
        }
        else if (strcmp(strSkillLevel, "semi-pro") == 0)
        {
            playerskill = 4;
            mult = 10;
        }
        else if (strcmp(strSkillLevel, "pro") == 0)
        {
            playerskill = 5;
            mult = 0;
        }

        void *paramHandle = GfParmReadFileLocal(strAiConfigFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        float aiskill = (float)GfParmGetNum(paramHandle, "skill", "level", 0, 10.0);
        GfParmReleaseHandle(paramHandle);

        if (aiskill == 10)
            mult1 = 50;
        else if (aiskill == 7)
            mult1 = 40;
        else if (aiskill == 3)
            mult1 = 30;
        else if (aiskill == 1.5)
            mult1 = 20;
        else if (aiskill == 1)
            mult1 = 10;
        else
            mult1 = 0;

        dmult = 100 - (mult + mult1);

        if (dmult == 100)
            rmSkillMult = 1.0;
        else if (dmult > 0)
            rmSkillMult = dmult * 0.01;
        else
            rmSkillMult = 0.01;

        if((aiskill > 1.0) || (skill == false))
        {
            return MenuHandle;
        }

        LegacyMenu::self().setAiSkill(aiskill);
        LegacyMenu::self().setPlayerSkill(playerskill);
        LegacyMenu::self().setSkillMult(rmSkillMult);
        GfLogInfo("Career Skill Mult = %.2f\n", rmSkillMult);
    }

    rmCar = false;
    contracts = 0;
    LegacyMenu::self().setCareerMode(true);
    LegacyMenu::self().setChampionshipMode(false);
    GfLogDebug("#careerselectmenu rmCareer = %i - rmChampionship = %i\n", LegacyMenu::self().getCareerMode(),
    LegacyMenu::self().getChampionshipMode());
    _pCategory = "";
    CurCarCategory.clear();
    rmCategory = CurCarCategory.c_str();
    CurChampionship = "none";

    void *hparmGarage = GfParmReadFileLocal(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmGarage)
        hparmGarage = GfParmReadFile(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmGarage)
    {
        GfLogError("No usable Garage (garage.xml not found or not readable)\n");
    }

    if(hparmGarage)
    {
        char sstring[128];
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "cars", "index", 1);
        CurCarName = GfParmGetStr(hparmGarage, sstring, ROB_ATTR_GARAGE1, "none");

        if(strlen(CurCarName) == 0 || strcmp(CurCarName, "none") == 0)
            rmCar = false;
        else
        {
            rmCar = true;
        }

        GfLogDebug("Cars in Garage = %s\n", CurCarName);
    }

    if (hparmHuman)
    {
        char sstring[256];
        const char *Fame = "";

        GfLogDebug("Loadind race select init human\n");

        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        for (int i = 0; i < 18; i++)
        {
            const char *strLevel = rmlevel[i];
            rmLevel[i] = (unsigned int)GfParmGetNum(hparmHuman, sstring, strLevel, (char*)NULL, 0);
            const char *strSpecial = rmspecial[i];
            rmSpecial[i] = (unsigned int)GfParmGetNum(hparmHuman, sstring, strSpecial, (char*)NULL, 0);
            const char *strContract = rmcontract[i];
            rmContracts[i] = (unsigned int)GfParmGetNum(hparmHuman, sstring, strContract, (char*)NULL, 0);
        }

        rmCredit = (unsigned long int)GfParmGetNum(hparmHuman, sstring, "credit", (char*)NULL, 10000);
        rmLicence = (bool)GfParmGetNum(hparmHuman, sstring, "licence", (char*)NULL, 0);

        if(rmCar)
        {
            const std::string carId = GfParmGetStr(hparmHuman, sstring, "car career", "none");
            GfLogDebug("# Car Career = %s\n", carId.c_str());

            if (carId != "none")
                CurChampionship = GfParmGetStr(hparmHuman, sstring, "championship", "none");
            GfLogDebug(" # championship load = %s\n", CurChampionship.c_str());
            GfParmSetStr(hparmHuman, sstring, "car name", carId.c_str());

            GfCar *pCar = GfCars::self()->getCar(carId);
            if (!pCar) {
                GfLogError("No car '%s'\n", carId.c_str());
            } else {
                CurCarCategory = pCar->getCategoryName();
                rmCategory = CurCarCategory.c_str();
                rmRaceCategoryInit();
                Fame = cFame[rmIndex];
                rmFame = (double)GfParmGetNum(hparmHuman, sstring, Fame, (char *)NULL, 0.0);
            }

            GfLogDebug("Career mode Level = %u - Licence = %i - Credit = %lu - Fame = %.2f - sfame =%s\n",
                rmLevel[rmIndex], rmLicence, rmCredit, rmFame, Fame);
            GfLogDebug("Category = %s\n", rmCategory);

            GfParmWriteFile(0, hparmHuman, "human");
            GfParmReleaseHandle(hparmHuman);

            GfParmReleaseHandle(hparmGarage);
        }
    }

    GfLogDebug(" # championship 2 load = %s\n", CurChampionship.c_str());
	// Ask the RaceEngine what types of races should be allowed here
    //bool SupportsHumanDrivers = LmRaceEngine().supportsHumanDrivers();

    // Create screen, load menu XML descriptor and create static controls.
    RmCareerSelectMenuHandle = GfuiScreenCreate((float*)NULL,
											NULL, rmOnActivate, 
											NULL, (tfuiCallback)NULL, 
											1);

    void *hMenuXMLDesc = GfuiMenuLoad("careermenu.xml");
    GfuiMenuCreateStaticControls(RmCareerSelectMenuHandle, hMenuXMLDesc);

    int CarImageId = GfuiMenuCreateStaticImageControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "PortraitImage");
    //GfuiStaticImageSet(RmCareerSelectMenuHandle, CarImageId, "data/img/portrait8.png");

    int Text1Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Text1Label");
    int Text2Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Text2Label");
    int Text3Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Text3Label");
    int Text4Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Text4Label");
    int Text5Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Text5Label");
    int Text6Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Text6Label");
    int Text7Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Text7Label");
    int Text8Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Text8Label");
    int Race1Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Race1Label");
    int Race2Label = GfuiMenuCreateLabelControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Race2Label");

    int val = CurChampionship.compare("none");
    index2 = rmLevel[rmIndex];
    int index = index2 % 10;

    GfLogDebug("# val rmLevel[%i] = %i\n", rmIndex, index);

    bool valid = rmValidContract(index, index2);
    bool valid2= rmValidSpecial(index, index2);

    if ((val < 0) || (val > 0))
    {
        GfLogDebug(" # championship load = %s\n", CurChampionship.c_str());

        auto const pos = CurChampionship.find_last_of('-');
        const auto leaf = CurChampionship.substr(pos + 1);

        GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "");
        GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Load and Continue your championship !");
        GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "");
        GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "");

        GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship in run");

        if(leaf.compare("championship1") == 0)
            Championship1ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Championship1Button",
                                                                NULL, rmOnSelectRaceManChampionship1);
        else if (leaf.compare("championship2") == 0)
            Championship2ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Championship2Button",
                                                                NULL, rmOnSelectRaceManChampionship2);
        else if (leaf.compare("championship3") == 0)
            Championship3ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Championship3Button",
                                                                NULL, rmOnSelectRaceManChampionship3);

        valid = false;

        GfLogDebug("# Championship leaf = %s\n", leaf.c_str());
    }
    else if(valid)
    {
        switch(contracts)
        {
        case 1:
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hi pilot !");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "My team would like to invite you to ride for them in a challenge");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "race in Karwada for 10 laps.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text5Label, "If you agree, we'll offer you a bonus of 1,000 credits regardless");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text6Label, "of your final ranking.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text7Label, "You'll have a Kanagawa Z35gts at your disposal for this race.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text8Label, "");
            GfuiStaticImageSet(RmCareerSelectMenuHandle, CarImageId, "data/img/portrait1.png");
            break;
        case 2:
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hi pilot !");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "My team would like to invite you to ride for them in a challenge");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "race in Forza for 10 laps.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text5Label, "If you agree, we'll offer you a bonus of 2,000 credits regardless");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text6Label, "of your final ranking.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text7Label, "You'll have a Deckard Conejo at your disposal for this race.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text8Label, "");
            GfuiStaticImageSet(RmCareerSelectMenuHandle, CarImageId, "data/img/portrait2.png");
            break;
        case 3:
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hi pilot !");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "My team would like to invite you to ride for them in a Endurance");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "race to Brondehach for 80 KM.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text5Label, "If you agree, we'll offer you a bonus of 3,000 credits regardless");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text6Label, "of your final ranking.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text7Label, "You'll have a Deckard Conejo at your disposal for this race.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text8Label, "");
            GfuiStaticImageSet(RmCareerSelectMenuHandle, CarImageId, "data/img/portrait3.png");
            break;
        case 4:
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hi pilot !");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "My team would like to invite you to ride for them in a Endurance");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "race to Auriac for 160 KM.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text5Label, "If you agree, we'll offer you a bonus of 6,000 credits regardless");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text6Label, "of your final ranking.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text7Label, "You'll have a Murasama NSX at your disposal for this race.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text8Label, "");
            GfuiStaticImageSet(RmCareerSelectMenuHandle, CarImageId, "data/img/portrait6.png");
            break;
        }
    }
    else if(valid2)
    {
        switch(special)
        {
        case 1:
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hello");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "these week, I have a special race");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "10 laps at Torino track.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "You have six others drivers, all with the same car");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text5Label, "the sc-kanagawa-z35gts.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text6Label, "You win 2000 credit as bonus race ");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text7Label, "");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text8Label, "");
            SpecialButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "SpecialButton1",
                                                           NULL, rmOnSelectRaceManSpecial);
            break;
        case 2:
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hi pilot !");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "A special race this weekend");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "10 laps at Torino track");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text5Label, "Seven drivers in race, all with the same car");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text6Label, "the sc-deckard-conejo");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text7Label, "You win 2500 credit as bonus race");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text8Label, "");
            SpecialButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "SpecialButton2",
                                                           NULL, rmOnSelectRaceManSpecial);
            break;
        case 3:
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hey pilot !");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "A special race this weekend");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "10 laps at Espie");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text5Label, "seven driver in race, all with the same car");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text6Label, "the sc-deckard-conejo");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text7Label, "You win 3000 credits as bonus race");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text8Label, "");
            SpecialButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "SpecialButton3",
                                                           NULL, rmOnSelectRaceManSpecial);
            break;
        case 4:
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hi pilot !");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Great special race this weekend");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "10 laps at Dijon");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text5Label, "seven drivers in race, all with the");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text6Label, "sc-murasama-nsx");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text7Label, "You win 3500 credit as bonus race");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text8Label, "");
            SpecialButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "SpecialButton4",
                                                           NULL, rmOnSelectRaceManSpecial);
            break;
        }
    }
    else if(rmCar == false)
    {
        GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hi pilot !");
        GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "To be able to go race");
        GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "you need a car !!!");
        GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Go buy one");
    }
    else if (index == 0)
    {
        if(running)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Sorry guy");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "you have to do better than that !!");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "go back to that track and start again");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "you will succeed !");
        }
        else if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Good");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Now, you have to do a test drive with your new car");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "set a lap record under 1 minute 50 seconds");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Alicante");
        }
        else if(rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Good, you enter in the OpenWheeler category");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Now, you have to do a test drive with your new car");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "set a lap record under 51 seconds");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Pinabashi");
        }
        else if(rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Welcome in Supercars Class B !!");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "you're going to try out your new car");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "set a lap record under 1 minute 43 seconds to Espie");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive on Espie");
        }
        else if(rmIndex == 3)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Ohhh !! Welcome to the last class before Grand Prix Formula");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "you bought yourself a new car ! Let's see what you can do with it");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "set a lap record under 1 minute 50 seconds to Jarama");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive on Jarama");
        }
        else if(rmIndex == 4)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Last Supercars Class before LS");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "would you be as strong as you claim ? let's see this !!");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 20 seconds to Prenzlau");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Prenzlau");
        }
        else if(rmIndex == 5)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Good, now you enter in first class grand prix formula");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "let's see if you're a great pilot");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 20 seconds to Sao-Paulo");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Sao-Paulo");
        }
        else if(rmIndex == 6)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Do you want to run in the big league ?");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "we will now be able to check what you are really worth");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 42 seconds to Forza");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Forza");
        }
        else if(rmIndex == 7)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "You bought yourself a GP One, do you think you're up to it ?");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "you will have to measure yourself against real pilots");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 30 seconds to Melbourne");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Melbourne");
        }
        else if(rmIndex == 8)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "now you are in the last category of LS-GT");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "you'll have to race against the best");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 3 minute 50 seconds to Longday");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Longday");
        }
        else if(rmIndex == 9)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Are you sure you want to race on the Speedways ?");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Be careful, these are dangerous races, accidents are not uncommon.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 30 seconds to Tokyo Speedway");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Tokyo Speedway");
        }
        else if(rmIndex == 10)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Wow !! You're in the Endurance category.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Consistency and mastery is the key");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 50 seconds to Salty");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Salty");
        }
        else if(rmIndex == 11)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Now you are in the 2011 American Grand Prix category");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "the races will be more difficult !!");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 00 seconds to Milky-Five Speedway");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Milky-Five Speedway");
        }
        else if(rmIndex == 12)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "And now the last Endurance serie.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Are you going to finish your career in style ?");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 50 seconds to Salty");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Salty");
        }
        else if(rmIndex == 13)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "the last category of the American Grand Prix");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "retirement is coming soon ...");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 00 seconds to Milky-Five Speedway");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Milky-Five Speedway");
        }
        else if(rmIndex == 14)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "you want to try your hand at the 1936 historic cars");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "good luck driving without insurance");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 50 seconds to Charmey");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Charmey");
        }
        else if(rmIndex == 15)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "The best category to drive without assistance !");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "To drive with finesse");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Set a lap record under 1 minute 00 seconds to Kurpfalzring");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Test drive at Kurpfalzring");
        }

        PracticeButtonId =
                GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "PracticeButton",
                                    NULL, rmOnSelectRaceManPractice);
    }
    else if (index == 1 && index2 < 10)
    {
        if(rmIndex == 0 )
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Very Good");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "which one you want to run in.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Good luck !");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Alicante");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Good");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "which one you want to run in.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Good luck !");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Alicante");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "let's see what you can do with this car");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "First is a challenge with 10 laps on Dijon");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Second is a challenge with 10 laps on Chemisay ... Good luck !");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Dijon");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Chemisay");
        }

        Challenge1ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge1Button",
                                                                NULL, rmOnSelectRaceManChallenge1);
        Challenge2ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge2Button",
                                                                NULL, rmOnSelectRaceManChallenge2);
    }
    else if (index == 1 && index2 > 10)
    {
        if(rmIndex == 0 )
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Welcome to the new season !!!");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "which one you want to run in.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Good luck !");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Alicante");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Hey driver, ready to start a new season ?");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "which one you want to run in.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Good luck !");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Alicante");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Welcome to the new season !!");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "First is a challenge with 10 laps on Dijon");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Second is a challenge with 10 laps on Chemisay ... Good luck !");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Dijon");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Chemisay");
        }

        Challenge1ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge1Button",
                                                                NULL, rmOnSelectRaceManChallenge1);
        Challenge2ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge2Button",
                                                                NULL, rmOnSelectRaceManChallenge2);
    }
    else if (index == 2)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "well, very well,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "First is a challenge at Peters for 10 laps");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Second is an endurance race at Brondehach for 80 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Peters");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Brondehach");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Good");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "There are two races available this weekend, I'll let you choose");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "First is a challenge at Peters for 15 laps");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Second is an endurance race at Brondehach for 80 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Peters");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Brondehach");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Good");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "You can choice first endurance race this weekend");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "First is a challenge at Alicante for 10 laps");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "Second is an endurance race at Espie for 80 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Alicante");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Espie");
        }

        Challenge3ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge3Button",
                                                                NULL, rmOnSelectRaceManchallenge3);
        Endurance1ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Endurance1Button",
                                                                NULL, rmOnSelectRaceManEndurance1);
    }
    else if (index == 3)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Two races are available this weekend. The first on an Karwada for 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is an endurace race at Auriac track for 160 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Karwada");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Very Good");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Two races are available this weekend. The first on an Karwada for 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is an endurace race at Auriac track for 160 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Karwada");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Auriac");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Very Good");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races may be longer in endurance categories.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Two races are available this weekend. The first on Espie for 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is an endurace race on Forza track for 320 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Espie");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Forza");
        }

        Challenge4ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge4Button",
                                                                NULL, rmOnSelectRaceManChallenge4);
        Endurance2ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Endurance2Button",
                                                                NULL, rmOnSelectRaceManEndurance2);
    }
    else if (index == 4)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first a challenge on Brondehach,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a championship with six races on 80 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Brondehach");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship 80 Km");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Very Good");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first a challenge on Brondehach,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a championship with six races on 80 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Brondehach");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship 80 Km");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Tried by a championship ?");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first a challenge on Torino on 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a championship with six races on 80 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Torino");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship 80 Km");
        }

        Challenge5ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge5Button",
                                                                NULL, rmOnSelectRaceManChallenge5);
        Championship1ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Championship1Button",
                                                                NULL, rmOnSelectRaceManChampionship1);
    }
    else if (index == 5)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "You're the best");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is a challenge on Chemisay with 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is an endurance at Peters on 320 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race at Chemisay");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on peters");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "You're the best");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is a challenge on Chemisay with 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is an endurance at Peters on 320 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race at Chemisay");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on peters");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "You're the best");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is a challenge on Karwada with 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is an endurance at Dijon on 600 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race at Karwada");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Dijon");
        }

        Challenge6ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge6Button",
                                                                NULL, rmOnSelectRaceManChallenge6);
        Endurance3ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Endurance3Button",
                                                                NULL, rmOnSelectRaceManEndurance3);
    }
    else if (index == 6)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "You manage to surprise me");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is a challenge at Forza on 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is championship with six races");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Forza Challenge");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship on 160 Km");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "You manage to surprise me");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is a challenge at Newton Speedway,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is championship with six races");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Challenge on Newton Speedway");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship on 160 Km");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "You manage to surprise me");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available this weekend.");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is a challenge at Brondehach with 10 laps");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is championship with ten races on 160 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Challenge Karwada");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship on 160 Km");
        }

        Challenge7ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge7Button",
                                                                NULL, rmOnSelectRaceManChallenge7);
        Championship2ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Championship2Button",
                                                                NULL, rmOnSelectRaceManChampionship2);
    }
    else if (index == 7)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Very good,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first on at Auriac for 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a endurance race at Karwada on 500 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race on Auriac");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race on Karwada");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Now you can do a Championship season");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first on at Auriac for 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a endurance race on Karwada on 500 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Challenge at Auriac");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Endurance at Karwada on 500 Km");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Battle in endurance race on 800 Km ?");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first on at Chemisay for 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is an endurance race at Peters on 800 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Challenge at Chemisay");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Endurance at Peters");
        }

        Challenge2ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge2aButton",
                                                                NULL, rmOnSelectRaceManChallenge2);
        Endurance4ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Endurance4Button",
                                                                NULL, rmOnSelectRaceManEndurance4);
    }
    else if (index == 8)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Now you can do a Championship season");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "Two races are available this weekend.the first on an Alicante track for 5 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a championship with race on 320 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Challenge Alicante");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship 320 Km");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Now you can do a Championship season");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is a challenge at Alicante,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a championship with race on 320 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Challenge Alicante");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Championship");
        }
        else if (rmIndex == 2)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "Now you can do a full Championship season");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is a challenge at Dijon on 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a full championship with races on 320 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Challenge Alicante");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Full Championship");
        }

        Challenge1ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge1Button",
                                                                NULL, rmOnSelectRaceManChallenge1);
        Championship3ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Championship3Button",
                                                                NULL, rmOnSelectRaceManChampionship3);
    }
    else if (index == 9)
    {
        if(rmIndex == 0)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "We are almost at the end of the season");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is the Challenge at Brondehach,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a 320 KM at Peters");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Race at Brondehach");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Race at Peters ");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "We are almost at the end of the season");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is the Challenge at Brondehach on 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a challenge at Forza on 10 laps");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Races at Brondehach");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Races at Forza");
        }
        else if (rmIndex == 1)
        {
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text1Label, "We are almost at the end of the season");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text2Label, "Races available :");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text3Label, "The first is the Challenge at Torino on 10 laps,");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Text4Label, "the second is a endurance race at Dijon on 600 Km");

            GfuiLabelSetText(RmCareerSelectMenuHandle, Race1Label, "Races at Torino");
            GfuiLabelSetText(RmCareerSelectMenuHandle, Race2Label, "Races at Dijon");
        }

        Challenge5ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Challenge5Button",
                                                                NULL, rmOnSelectRaceManChallenge5);
        Challenge7ButtonId = GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "Endurance3Button",
                                                                NULL, rmOnSelectRaceManChallenge7);
    }

    // Create Back button
    if (valid == false)
    {
        GfuiStaticImageSet(RmCareerSelectMenuHandle, CarImageId, "data/img/portrait8.png");
        GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "QuitButton",
                                    prevMenu, rmOnRaceSelectShutdown);
        GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "BuyButton",
                                    prevMenu, rmOnRaceSelectBuy);
        GarageButtonId =
                GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "GarageButton",
                                    prevMenu, rmOnRaceSelectGarage);

        UpgradeButtonId =
                GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "UpgradeButton",
                                    prevMenu, rmOnRaceSelectUpgrade);

        if(!rmCar)
        {
            GfuiEnable(RmCareerSelectMenuHandle, UpgradeButtonId, GFUI_DISABLE);
            GfuiEnable(RmCareerSelectMenuHandle, GarageButtonId, GFUI_DISABLE);
        }
    }
    else
    {
        GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "AcceptButton",
                                    prevMenu, rmOnSelectRaceManContracts);

        GfuiMenuCreateButtonControl(RmCareerSelectMenuHandle, hMenuXMLDesc, "DeclineButton",
                                    NULL, rmOnRaceSelectShutdown2);
    }

    // Close menu XML descriptor.
    GfParmReleaseHandle(hMenuXMLDesc);
    
    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(RmCareerSelectMenuHandle);
    GfuiAddKey(RmCareerSelectMenuHandle, GFUIK_ESCAPE, "Back To Main Menu",
               prevMenu, rmOnRaceSelectShutdown, NULL);

    // Give the race engine the menu to come back to.

    return RmCareerSelectMenuHandle;
}
