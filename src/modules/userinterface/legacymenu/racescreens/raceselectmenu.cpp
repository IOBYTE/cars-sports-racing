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
#include <robot.h>

#include "legacymenu.h"
#include "racescreens.h"

class GfCars;
class GfDrivers;
class GfRaces;

// The Race Select menu.
void *RmRaceSelectMenuHandle = NULL;

// Menu control Ids
static int   PracticeButtonId;
static int   Challenge1ButtonId;
static int   Challenge2ButtonId;
static int   Endurance1ButtonId;
static int   ChampionshipButtonId;
static int   QuickraceButtonId;

static int   NextButtonId;

const char*  rmCategory = "";
int          rmRanking[];
int          rmIndex = 0;
int          rmSkilling = 0;
int          rmPosition = 0;
bool         rmUpdate = false;
bool         rmUpdate2 = false;
unsigned long int rmCredit = 0;
const char*  rmGarage = "" ;

const GfCar*    _pCar;
std::string     _pCategory;

const char* rmrank[] = {    "ranking0", "ranking1", "ranking2", "ranking3",
                            "ranking4", "ranking5", "ranking6", "ranking7",
                            "ranking8", "ranking9", "ranking10", "ranking11",
                            "ranking12", "ranking13", "ranking14", "ranking15",
                            "ranking16", "ranking17", "ranking18", "ranking19" };

const char* rmlevel[] = {   "level0", "level1", "level2", "level3",
                            "level4", "level5", "level6", "level7",
                            "level8", "level9", "level10", "level11",
                            "level12", "level13", "level14", "level15",
                            "level16", "level17" };

const char* rmspecial[] = { "special0", "special1", "special2", "special3",
                            "special4", "special5", "special6", "special7",
                            "special8", "special9", "special10", "special11",
                            "special12", "special13", "special14", "special15",
                            "special16", "special17" };

const char* rmcontract[] = { "contract0", "contract1", "contract2", "contract3",
                            "contract4", "contract5", "contract6", "contract7",
                            "contract8", "contract9", "contract10", "contract11",
                            "contract12", "contract13", "contract14", "contract15",
                            "contract16", "contract17" };

std::string  _pCat[] = { "SC-C-", "09GP5-", "SC-B-", "10GP4-", "SC-A-", "91GP-", "LS2-",
                         "01GP-", "LS1-", "01GPA-", "LP2-", "11GPA-", "LP1-", "12GPA-", "36GP-",
                         "67GP-"};

std::string sccTrack[] = { "alicante", "auriac", "brondehach", "chemisay", "forza", "karwada", "peters" };
std::string sccBonus[] = { "espie", "torino" };

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

static void
rmChangeTrackRace(std::string race, std::string track)
{

}
static void
rmOnSelectRaceManPractice(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = _pCategory+"practice";
    GfLogDebug("Race = %s\n", race.c_str());
    GfRaceManager* pSelRaceMan = GfRaceManagers::self()->getRaceManager(race);
    rmChangeTrackRace(race, sccTrack[0]);
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

static void
rmOnSelectRaceManQuickrace(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(false);
    // Get the race managers with the given type
    std::string race = "quickrace";
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
rmOnSelectRaceManChallenge1(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(true);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChallenge2(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(true);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManEndurance1(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(true);
    LegacyMenu::self().setChampionshipMode(false);
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
rmOnSelectRaceManChampionship1(void *pvRaceManTypeIndex)
{
    LegacyMenu::self().setChallengeMode(false);
    LegacyMenu::self().setEnduranceMode(false);
    LegacyMenu::self().setChampionshipMode(true);
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

static
void rmRaceCategoryInit()
{
    GfLogDebug("Prefix category Race : \n");
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
    /*else if (strcmp(rmCategory, ) == 0)
    {
        rmIndex    = 16;
        _pCategory = _pCat[rmIndex];
    }
    else if (strcmp(rmCategory, "Rally Series") == 0)
    {
        rmIndex    = 17;
        _pCategory = _pCat[rmIndex];
    }*/

    GfLogDebug("Prefix category Race : %s - Ranking index = %i\n", _pCategory.c_str(), rmIndex);
}

static void
rmRaceSelectNextPage(void *prevMenu)
{
    GfuiScreenActivate(RmRaceSelectInit2(RmRaceSelectMenuHandle));
}

/* Initialize the single player menu */
void *
RmRaceSelectInit(void *prevMenu)
{
    if (RmRaceSelectMenuHandle)
    {
        RmRaceSelectMenuHandle = NULL;
    }

    LegacyMenu::self().setCareerMode(false);

    std::string strPlayersFileName(GfLocalDir());
    strPlayersFileName += "drivers/human/human.xml";
    std::string strAiConfigFileName ="config/raceman/extra/skill.xml";
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
        //const char *str;

        GfLogDebug("Loadind race select init human\n");

        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        for (int i = 0; i < 18; i++)
        {
            rmRanking[i] = GfParmGetNum(hparmHuman, sstring, rmrank[i], (char*)NULL, 0);
            GfLogDebug("_rank %i          = %i\n", i, rmRanking[i]);
        }

        const char* pszCarId = GfParmGetStr(hparmHuman, sstring, "car name", "");
        const char* pszCarId2 = GfParmGetStr(hparmHuman, sstring, "car race", "");
        const char *strSkillLevel = GfParmGetStr(hparmHuman, sstring, ROB_ATTR_LEVEL, ROB_VAL_SEMI_PRO);
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

        std::string CurCarName = pszCarId2;
        GfLogDebug("Car Name = %s - Car Race = %s\n", pszCarId, pszCarId2);
        GfParmSetStr(hparmHuman, sstring, "car name", pszCarId2);
        std::string car = GfCars::self()->getCar(pszCarId2)->getCategoryName();
        rmCategory = car.c_str();

        GfLogDebug("Car              = %s\n", rmCategory);
        GfLogDebug("In non Career = %i - Credit = %lu\n", 0, rmCredit);
        GfParmWriteFile(0, hparmHuman, "human");
        GfParmReleaseHandle(hparmHuman);

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

        LegacyMenu::self().setAiSkill(aiskill);
        LegacyMenu::self().setPlayerSkill(playerskill);
        LegacyMenu::self().setSkillMult(rmSkillMult);

        rmRaceCategoryInit();
    }

	// Ask the RaceEngine what types of races should be allowed here
    //bool SupportsHumanDrivers = LmRaceEngine().supportsHumanDrivers();

    // Create screen, load menu XML descriptor and create static controls.
    RmRaceSelectMenuHandle = GfuiScreenCreate((float*)NULL, 
											NULL, rmOnActivate, 
											NULL, (tfuiCallback)NULL, 
											1);
//#if defined(CLIENT_SERVER)
    void *hMenuXMLDesc = GfuiMenuLoad("csraceselectmenu.xml");
/*#else
	void *hMenuXMLDesc = GfuiMenuLoad("raceselectmenu.xml");
#endif*/
    GfuiMenuCreateStaticControls(RmRaceSelectMenuHandle, hMenuXMLDesc);

    // Create the raceman type buttons and sub-type combo-boxes (if any).
    PracticeButtonId =
        GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle, hMenuXMLDesc, "PracticeButton",
                            NULL, rmOnSelectRaceManPractice);
    Challenge1ButtonId =
        GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle, hMenuXMLDesc, "Challenge1Button",
                            NULL, rmOnSelectRaceManChallenge1);
    Challenge2ButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle, hMenuXMLDesc, "Challenge2Button",
                                                        NULL, rmOnSelectRaceManChallenge2);
    Endurance1ButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle, hMenuXMLDesc, "Endurance1Button",
                                                        NULL, rmOnSelectRaceManEndurance1);
    ChampionshipButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle, hMenuXMLDesc, "Championship1Button",
                                                       NULL, rmOnSelectRaceManChampionship1);
    QuickraceButtonId = GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle, hMenuXMLDesc, "QuickraceButton",
                                                NULL, rmOnSelectRaceManQuickrace);

    /*if(rmRanking[rmIndex] < 1 )
        GfuiEnable(RmRaceSelectMenuHandle, Challenge1ButtonId, GFUI_DISABLE);

    if(rmRanking[rmIndex] < 2 )
        GfuiEnable(RmRaceSelectMenuHandle, Challenge2ButtonId, GFUI_DISABLE);

    if(rmRanking[rmIndex] < 3 )
        GfuiEnable(RmRaceSelectMenuHandle, Endurance1ButtonId, GFUI_DISABLE);

    if(rmRanking[rmIndex] < 4 )
        GfuiEnable(RmRaceSelectMenuHandle, ChampionshipButtonId, GFUI_DISABLE);*/

    if (rmRanking[17] < 0)
        GfuiVisibilitySet(RmRaceSelectMenuHandle, QuickraceButtonId, GFUI_INVISIBLE);
	
    // Create Back button
    GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle, hMenuXMLDesc, "BackButton",
                                prevMenu, rmOnRaceSelectShutdown);

    // Create Next button
    NextButtonId =
            GfuiMenuCreateButtonControl(RmRaceSelectMenuHandle, hMenuXMLDesc, "NextButton",
                                NULL, rmRaceSelectNextPage);

    // Close menu XML descriptor.
    GfParmReleaseHandle(hMenuXMLDesc);
    
    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(RmRaceSelectMenuHandle);
    GfuiAddKey(RmRaceSelectMenuHandle, GFUIK_ESCAPE, "Back To Main Menu",
               prevMenu, rmOnRaceSelectShutdown, NULL);

    // Give the race engine the menu to come back to.
    LmRaceEngine().initializeState(RmRaceSelectMenuHandle);

    return RmRaceSelectMenuHandle;
}
