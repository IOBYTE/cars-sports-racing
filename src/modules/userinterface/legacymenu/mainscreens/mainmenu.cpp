/***************************************************************************

    file                 : mainmenu.cpp
    created              : Sat Mar 18 23:42:38 CET 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: mainmenu.cpp 5158 2013-02-17 17:06:28Z wdbee $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <string>

#include <tgf.h>
#include <tgfclient.h>
#include <tgfdata.h>
#include <player.h>
#include <robot.h>

#include <racescreens.h>
#include <playerconfig.h>

#include "mainmenu.h"
#include "exitmenu.h"
#include "optionsmenu.h"
#include "creditsmenu.h"
#include "tradermenu.h"

class GfPlayer;

static RmTraderMenu TraderMenu;

void *MenuHandle = 0;
static int ManagerButtonId;
static int CareerButtonId;
static int RacedayButtonId;
static int PracticeButtonId;
static int ChampionshipButtonId;
static int NetworkButtonId;
static int MarketPlaceButtonId;
static int TuningButtonId;
static int GarageButtonId;
static int StatsButtonId;

static const char* mainCar;
static bool mainConnected;
static bool mainSkill;
static unsigned int mainCredit;

static void
onPlayerConfigMenuActivate(void * /* dummy */)
{
    /* Here, we need to call OptionOptionInit each time the firing button
       is pressed, and not only once at the Main menu initialization,
       because the previous menu has to be saved (ESC, Back) and because it can be this menu,
       as well as the Raceman menu */
    GfuiScreenActivate(PlayerConfigMenuInit(MenuHandle));
}

static void
onManagementSelectMenuActivate(void * /* dummy */)
{
    //GfuiScreenActivate(RmRaceSelectInit(MenuHandle));
}

static void
onRaceSelectMenuActivate(void * /* dummy */)
{
    GfuiScreenActivate(RmRaceSelectInit(MenuHandle));
}

static void
onRaceCareerSelectMenuActivate(void * /* dummy */)
{
    GfuiScreenActivate(RmCareerSelectInit(MenuHandle));
}

static void
onChampionshipSelectMenuActivate(void * /* dummy */)
{
    //GfuiScreenActivate(RmChampionshipSelectInit(MenuHandle));
}

static void
onPracticeSelectMenuActivate(void * /* dummy */)
{
    //GfuiScreenActivate(RmPracticeSelectInit(MenuHandle));
}

static void
onMarketPlaceSelectMenuActivate(void * /* dummy */)
{
    TraderMenu.setPreviousMenuHandle(MenuHandle);
    TraderMenu.runMenu("sc-deckard-conejo");
}

static void
onTuningSelectMenuActivate(void * /* dummy */)
{
    GfuiScreenActivate(RmTuningSelectInit(MenuHandle));
}

static void
onGarageSelectMenuActivate(void * /* dummy */)
{
    GfuiScreenActivate(RmGarageSelectInit(MenuHandle));
}

static void
onOptionsMenuActivate(void * /* dummy */)
{
    GfuiScreenActivate(OptionsMenuInit(MenuHandle));
}

static void
onStatsMenuActivate(void * /* dummy */)
{
    //CreditsMenuActivate(MenuHandle);
}

static void
onCreditsMenuActivate(void * /* dummy */)
{
    CreditsMenuActivate(MenuHandle);
}

static void
onExitMenuActivate(void * /*dummy*/)
{
    GfuiScreenActivate(ExitMenuInit(MenuHandle));
}

static void
onMainMenuActivate(void * /* dummy */)
{
}

static void
onMainLoadHuman(void)
{
    GfLogInfo("Start loading Human parameters !!!\n");
    std::string strPlayersFileName = "drivers/human/human.xml";
    std::string strGarageFileName = "drivers/human/garage.xml";
    std::string strAiConfigFileName ="config/raceman/extra/skill.xml";

    mainSkill = false;
    rmCredit = 0;
    //mainCar = "";
    mainConnected = false;

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

        if(strcmp(strSkillLevel, "arcade") == 0)
            playerskill = 0;
        else if (strcmp(strSkillLevel, "semi-rookie") == 0)
            playerskill = 1;
        else if (strcmp(strSkillLevel, "rookie") == 0)
            playerskill = 2;
        else if (strcmp(strSkillLevel, "amateur") == 0)
            playerskill = 3;
        else if (strcmp(strSkillLevel, "semi-pro") == 0)
            playerskill = 4;
        else if (strcmp(strSkillLevel, "pro") == 0)
            playerskill = 5;

        rmCredit = (unsigned long int)GfParmGetNum(hparmHuman, drvSectionPath, "credit", (char*)NULL, 10000);
        GfLogInfo("Main Credit = %i\n", mainCredit);
        mainCar = GfParmGetStr(hparmHuman, drvSectionPath, ROB_ATTR_CAR_RACE, "none");
        GfLogInfo("Car race in Main = %s\n", mainCar);

        void *paramHandle = GfParmReadFileLocal(strAiConfigFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        float aiskill = (float)GfParmGetNum(paramHandle, "skill", "level", 0, 10.0);
        GfParmReleaseHandle(paramHandle);

        if((aiskill > 1.0) || (skill == false))
        {
            mainSkill = false;
        }
        else
            mainSkill = true;
    }

    void *hparmGarage = GfParmReadFileLocal(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmGarage)
        hparmGarage = GfParmReadFile(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmGarage)
    {
        GfLogInfo("No usable Garage (garage.xml not found or not readable)\n");
    }

    if(hparmGarage)
    {
        char sstring[128];
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "cars", "index", 1);
        const char* Name = GfParmGetStr(hparmGarage, sstring, ROB_ATTR_GARAGE1, "none");

        if(strlen(Name) == 0 || strcmp(Name, "none") == 0)
            mainCar = "";
        else
            mainCar = Name;

        GfLogInfo("Cars in Garage main = %s\n", mainCar);
        GfParmReleaseHandle(hparmGarage);
    }
}

/*
 * Function
 *	MainMenuInit
 *
 * Description
 *	init the main menu
 *
 * Parameters
 *	none
 *
 * Return
 *	0 ok -1 nok
 *
 * Remarks
 *	
 */

void *
MainMenuInit(bool SupportsHumanDrivers)
{
    // Initialize only once.
    if (MenuHandle)
        MenuHandle = NULL;
        //return MenuHandle;

    onMainLoadHuman();
    //GfPlayer* player;
    //GfPlayer::self()->reload();
    //GfLogInfo("player Name = %s\n", GfPlayer::self()->getName().c_str());

    MenuHandle = GfuiScreenCreate((float*)NULL, 
				    NULL, onMainMenuActivate, 
				    NULL, (tfuiCallback)NULL, 
				    1);

    void *menuDescHdle = GfuiMenuLoad("mainmenu.xml");

    GfuiMenuCreateStaticControls(MenuHandle, menuDescHdle);

     //Add buttons and create based on xml
    RacedayButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "race", NULL, onRaceSelectMenuActivate);
    ChampionshipButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "championship", NULL, onChampionshipSelectMenuActivate);
    PracticeButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "practice", NULL, onPracticeSelectMenuActivate);
    NetworkButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "multiplayer", NULL, onPracticeSelectMenuActivate);
    MarketPlaceButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "marketplace", NULL, onMarketPlaceSelectMenuActivate);
    CareerButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "career", NULL, onRaceCareerSelectMenuActivate);
    ManagerButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "management", NULL, onManagementSelectMenuActivate);
    TuningButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "tuning", NULL, onTuningSelectMenuActivate);
    GarageButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "garage", NULL, onGarageSelectMenuActivate);
    StatsButtonId = GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "stats", NULL, onStatsMenuActivate);
    GfuiEnable(MenuHandle, ManagerButtonId, GFUI_DISABLE);

    if ((strlen(mainCar) == 0 || strcmp(mainCar, "none") == 0))
    {
        GfLogInfo("mainSkill = %i - mainCar = %s\n", mainSkill, mainCar);
        GfuiEnable(MenuHandle, CareerButtonId, GFUI_DISABLE);
        GfuiEnable(MenuHandle, RacedayButtonId, GFUI_DISABLE);
        GfuiEnable(MenuHandle, ChampionshipButtonId, GFUI_DISABLE);
        GfuiEnable(MenuHandle, PracticeButtonId, GFUI_DISABLE);
        GfuiEnable(MenuHandle, NetworkButtonId, GFUI_DISABLE);
        GfuiEnable(MenuHandle, GarageButtonId, GFUI_DISABLE);
        GfuiEnable(MenuHandle, TuningButtonId, GFUI_DISABLE);
    }

    if (!mainSkill)
        GfuiEnable(MenuHandle, CareerButtonId, GFUI_DISABLE);

    GfuiEnable(MenuHandle, StatsButtonId, GFUI_DISABLE);

    if (SupportsHumanDrivers)
        GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "profils", NULL, onPlayerConfigMenuActivate);

    GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "options", NULL, onOptionsMenuActivate);
    GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "credits", NULL, onCreditsMenuActivate);
    GfuiMenuCreateButtonControl(MenuHandle, menuDescHdle, "quit", NULL, onExitMenuActivate);

    GfParmReleaseHandle(menuDescHdle);

    GfuiMenuDefaultKeysAdd(MenuHandle);
    GfuiAddKey(MenuHandle, GFUIK_ESCAPE, "Quit the game", NULL, onExitMenuActivate, NULL);

    return MenuHandle;
}

/*
 * Function
 *	MainMenuRun
 *
 * Description
 *	Activate the main menu
 *
 * Parameters
 *	none
 *
 * Return
 *	0 ok -1 nok
 *
 * Remarks
 *	
 */
int
MainMenuRun(void)
{
    GfuiScreenActivate(MenuHandle);
	
    return 0;
}
