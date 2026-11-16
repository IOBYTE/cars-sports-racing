/***************************************************************************

    file                 : carstuningmenu.cpp
    created              : Sat Nov 2025
    copyright            : (C) 2025 Xavier Bertaux
    web                  : cars-sports-racing.sourceforge.net
    version              : $Id: $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <sstream>
#include <iomanip>

#include <tgfclient.h>
#include <cars.h>
#include <robot.h>

#include "racescreens.h"
#include "mainmenu.h"

#define BIT1		(1 << 0)
#define BIT2    	(1 << 1)
#define BIT3		(1 << 2)

static void *RmTuningSelectMenuHandle = NULL;

static int weight1ButtonId;
static int weight2ButtonId;
static int weight3ButtonId;
static int gearButtonId;
static int tiresButtonId;
static int airButtonId;
static int exaustButtonId;
static int ecuButtonId;
static int springs1ButtonId;
static int springs2ButtonId;
static int antiroll1ButtonId;
static int antiroll2ButtonId;
static int wings1ButtonId;
static int wings2ButtonId;

std::string rmCareerCar = "";

int weight1price, weight2price, weight3price = 0;
int gear1price, tire1price = 0;
int airprice, exaustprice , ecuprice = 0;
int springs1price, springs2price, arb1price, arb2price, wing1price, wing2price = 0;

int option1, option2, option3, option4, option5, option6, option7 = 0;

/* Called when the menu is activated */
static void
rmOnActivate(void * /* dummy */)
{
    GfLogTrace("Entering Tuning Car Select menu\n");
}

static void
rmLoadGarage(const char *carselected)
{
    const char *carname = "";
    char sstring[128];
    std::string garagename ="";
    std::string strGarageFileName = "drivers/human/garage.xml";
    void *hparmGarage = GfParmReadFileLocal(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmGarage)
        hparmGarage = GfParmReadFile(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmGarage)
    {
        GfLogError("No usable Garage (garage.xml not found or not readable)\n");
    }

    if(hparmGarage)
    {
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "cars", "index", 1);

        for (int i = 0; i < 35; i++)
        {
            garagename = "car name "+std::to_string(i+1);
            carname = GfParmGetStr(hparmGarage, sstring, garagename.c_str(), "");
            GfLogDebug("Car Name in garage = %s - passed = %s\n", carname, carselected);

            if (strcmp(carname, carselected) == 0)
            {
                GfLogDebug("# Car Found !!!\n");
                option1 = GfParmGetNum(hparmGarage, sstring, "option1", (char*)NULL, 0);
                option2 = GfParmGetNum(hparmGarage, sstring, "option2", (char*)NULL, 0);
                option3 = GfParmGetNum(hparmGarage, sstring, "option3", (char*)NULL, 0);
                option4 = GfParmGetNum(hparmGarage, sstring, "option4", (char*)NULL, 0);
                option5 = GfParmGetNum(hparmGarage, sstring, "option5", (char*)NULL, 0);
                option6 = GfParmGetNum(hparmGarage, sstring, "option6", (char*)NULL, 0);
                option7 = GfParmGetNum(hparmGarage, sstring, "option7", (char*)NULL, 0);

                GfLogDebug("Car : %s - Option1 = %i\n", carname, option1);
                GfLogDebug("Car : %s - Option2 = %i\n", carname, option2);
                GfLogDebug("Car : %s - Option3 = %i\n", carname, option3);
                GfLogDebug("Car : %s - Option4 = %i\n", carname, option4);
                GfLogDebug("Car : %s - Option5 = %i\n", carname, option5);
                GfLogDebug("Car : %s - Option6 = %i\n", carname, option6);
                GfLogDebug("Car : %s - Option7 = %i\n", carname, option7);
                break;
            }
        }

        GfParmReleaseHandle(hparmGarage);
    }
}

static void
rmGarage(std::string& car, const char *options, int value)
{
    const char *carname ="";
    char sstring[128];
    std::string garagename ="";
    std::string strGarageFileName = "drivers/human/garage.xml";
    void *hparmGarage = GfParmReadFileLocal(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmGarage)
        hparmGarage = GfParmReadFile(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmGarage)
    {
        GfLogError("No usable Garage (garage.xml not found or not readable)\n");
    }

    if(hparmGarage)
    {
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "cars", "index", 1);

        for (int i = 0; i < 35; i++)
        {
            garagename = "car name "+std::to_string(i+1);
            carname = GfParmGetStr(hparmGarage, sstring, garagename.c_str(), "");
            GfLogDebug("Car Name in garage = %s - Car Career = %s\n", carname, car.c_str());

            if (strcmp(carname, car.c_str()) == 0)
            {
                GfLogDebug("# Car Found !!!\n");
                int now = GfParmGetNum(hparmGarage, sstring, options, (char*)NULL, 0);
                now += value;
                GfParmSetNum(hparmGarage, sstring, options, NULL, now);
                GfParmWriteFile(0, hparmGarage, "garage");
                break;
            }
        }
    }

    GfParmReleaseHandle(hparmGarage);

    std::string strPlayersFileName = "drivers/human/human.xml";

    void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
        hparmHuman = GfParmReadFile(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
    {
        GfLogError("No usable human driver (human.xml not found or not readable)\n");
    }

    if(hparmHuman)
    {
        char sstring[256];
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetNum(hparmHuman, sstring, "credit", (char*)NULL, rmCredit);
        GfLogDebug("# rmCredit = %lu\n", rmCredit);
        GfParmWriteFile(0, hparmHuman, "human");
        GfParmReleaseHandle(hparmHuman);
    }
}

static void
rmWeight1Update(void *prevmenu)
{
    rmCredit -= weight1price;
    rmGarage(rmCareerCar, "option1", 1);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmWeight2Update(void *prevmenu)
{
    rmCredit -= weight2price;
    rmGarage(rmCareerCar, "option1", 2);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmWeight3Update(void *prevmenu)
{
    rmCredit -= weight3price;
    rmGarage(rmCareerCar, "option1", 3);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmGearUpdate(void *prevmenu)
{
    rmCredit -= gear1price;
    rmGarage(rmCareerCar, "option2", 1);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmTireUpdate(void *prevmenu)
{
    rmCredit -= tire1price;
    rmGarage(rmCareerCar, "option7", 1);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmAirUpdate(void *prevmenu)
{
    rmCredit -= airprice;
    rmGarage(rmCareerCar, "option3", 1);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmExaustUpdate(void *prevmenu)
{
    rmCredit -= exaustprice;
    rmGarage(rmCareerCar, "option3", 2);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmEcuUpdate(void *prevmenu)
{
    rmCredit -= ecuprice;
    rmGarage(rmCareerCar, "option3", 4);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmSpring1Update(void *prevmenu)
{
    rmCredit -= springs1price;
    rmGarage(rmCareerCar, "option4", 1);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmSpring2Update(void *prevmenu)
{
    rmCredit -= springs2price;
    rmGarage(rmCareerCar, "option4", 2);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmAntiroll1Update(void *prevmenu)
{
    rmCredit -= arb1price;
    rmGarage(rmCareerCar, "option5", 1);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmAntiroll2Update(void *prevmenu)
{
    rmCredit -= arb2price;
    rmGarage(rmCareerCar, "option5", 2);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmWing1Update(void *prevmenu)
{
    rmCredit -= wing1price;
    rmGarage(rmCareerCar, "option6", 1);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
rmWing2Update(void *prevmenu)
{
    rmCredit -= wing2price;
    rmGarage(rmCareerCar, "option6", 2);
    GfuiScreenActivate(RmTuningSelectInit(prevmenu));
}

static void
onCancelCB(void * CiSmenu)
{
    GfuiScreenActivate(MainMenuInit(true));
}

void*
RmTuningSelectInit(void *prevMenu)
{
    GfLogDebug("Start Tuning Car menu\n");
	// Create the menu and all its controls.
    if (RmTuningSelectMenuHandle)
    {
        RmTuningSelectMenuHandle = NULL;
    }

    std::string strPlayersFileName = "drivers/human/human.xml";
    unsigned int priceCar = 0;
    bool optionMass = false;
    bool optionGear = false;
    bool optionWheel = false;
    bool optionEngine = false;
    bool optionSprings = false;
    bool optionAntiroll = false;
    bool optionWings = false;

    void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
        hparmHuman = GfParmReadFile(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
    {
        GfLogError("No usable human driver (human.xml not found or not readable)\n");
    }

    if(hparmHuman)
    {
        char sstring[256];
        const char *pszCarId = "";
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        pszCarId = GfParmGetStr(hparmHuman, sstring, ROB_ATTR_CAR_RACE, "none");
        rmCareerCar = pszCarId;
        priceCar =          GfCars::self()->getCar(pszCarId)->getPrice();
        optionMass =        GfCars::self()->getCar(pszCarId)->isMassOption();
        optionGear =        GfCars::self()->getCar(pszCarId)->isGearOption();
        optionWheel =       GfCars::self()->getCar(pszCarId)->isWheelOption();
        optionEngine =       GfCars::self()->getCar(pszCarId)->isEngineOption();
        optionSprings =     GfCars::self()->getCar(pszCarId)->isSpringsOption();
        optionAntiroll =    GfCars::self()->getCar(pszCarId)->isAntirollOption();
        optionWings =       GfCars::self()->getCar(pszCarId)->isWingOption();

        GfLogDebug("# Option Mass            = %i\n", optionMass);
        GfLogDebug("# Option Gear            = %i\n", optionGear);
        GfLogDebug("# Option Wheel           = %i\n", optionWheel);
        GfLogDebug("# Option Engine          = %i\n", optionEngine);
        GfLogDebug("# Option Springs         = %i\n", optionSprings);
        GfLogDebug("# Option Anti-roll Bars  = %i\n", optionAntiroll);
        GfLogDebug("# Option Wings           = %i\n", optionWings);

        //rmCredit = GfParmGetNum(hparmHuman, sstring, "credit", (char*)NULL, 0);
    }

    GfParmReleaseHandle(hparmHuman);

    rmLoadGarage(rmCareerCar.c_str());

    // Create screen, load menu XML descriptor and create static controls.
    RmTuningSelectMenuHandle = GfuiScreenCreate((float*)NULL,
                                            NULL, rmOnActivate,
                                            NULL, (tfuiCallback)NULL,
                                            1);

    void *hmenu = GfuiMenuLoad("carstuningmenu.xml");
    GfuiMenuCreateStaticControls(RmTuningSelectMenuHandle, hmenu);
    int CreditLabel = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "CreditLabel");
    int Weight1Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Weight1Label");
    int Weight2Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Weight2Label");
    int Weight3Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Weight3Label");
    int Gear1Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Gear1Label");
    int Wheel1Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Wheel1Label");
    int AirLabel = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "AirLabel");
    int ExaustLabel = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "ExaustLabel");
    int EcuLabel = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "EcuLabel");
    int Spring1Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Spring1Label");
    int Spring2Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Spring2Label");
    int Arb1Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Arb1Label");
    int Arb2Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Arb2Label");
    int Wing1Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Wing1Label");
    int Wing2Label = GfuiMenuCreateLabelControl(RmTuningSelectMenuHandle, hmenu, "Wing2Label");

    // Initialize the GUI contents.
    std::ostringstream ossSpecValue;
    ossSpecValue << (unsigned long int)rmCredit;
    GfLogDebug("Credit : %lu\n", rmCredit);
    GfuiLabelSetText(RmTuningSelectMenuHandle, CreditLabel, ossSpecValue.str().c_str());

    weight1price = priceCar / 10;
    weight2price = priceCar / 5;
    weight3price = (int)(priceCar / 3);
    gear1price = (int)(priceCar / 2.8);
    tire1price = (int)(priceCar / 10.8);
    airprice = (int)(priceCar / 30);
    exaustprice = (int)(priceCar / 10);
    ecuprice = (int)(priceCar / 2.9);
    springs1price = (int)(priceCar / 5);
    springs2price = (int)(priceCar / 2.7);
    arb1price = (int)(priceCar / 4);
    arb2price = (int)(priceCar / 2.5);
    wing1price = (int)(priceCar / 2.6);
    wing2price = (int)(priceCar / 1.5);
    ossSpecValue.str("");
    ossSpecValue << (int)weight1price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Weight1Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)weight2price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Weight2Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)weight3price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Weight3Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)gear1price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Gear1Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)tire1price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Wheel1Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)airprice << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, AirLabel, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)exaustprice << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, ExaustLabel, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)ecuprice << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, EcuLabel, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)springs1price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Spring1Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)springs2price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Spring2Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)arb1price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Arb1Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)arb2price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Arb2Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)wing1price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Wing1Label, ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << (int)wing2price << " credits";
    GfuiLabelSetText(RmTuningSelectMenuHandle, Wing2Label, ossSpecValue.str().c_str());

    // Create pieces button
    GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "BackButton",
                                prevMenu, onCancelCB);
    weight1ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "loseweight1Button",
                                                 prevMenu, rmWeight1Update);
    weight2ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "loseweight2Button",
                                                 prevMenu, rmWeight2Update);
    weight3ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "loseweight3Button",
                                                 prevMenu, rmWeight3Update);
    gearButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "gearsportsButton",
                                                   prevMenu, rmGearUpdate);
    tiresButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "TiresportsButton",
                                                   prevMenu, rmTireUpdate);
    airButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "airfilterButton",
                                                 prevMenu, rmAirUpdate);
    exaustButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "exaustButton",
                                                 prevMenu, rmExaustUpdate);
    ecuButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "ecuButton",
                                                 prevMenu, rmEcuUpdate);
    springs1ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "springs1Button",
                                                  prevMenu, rmSpring1Update);
    springs2ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "springs2Button",
                                                  prevMenu, rmSpring2Update);
    antiroll1ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "antiroll1Button",
                                                  prevMenu, rmAntiroll1Update);
    antiroll2ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "antiroll2Button",
                                                  prevMenu, rmAntiroll2Update);
    wings1ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "wings1Button",
                                                  prevMenu, rmWing1Update);
    wings2ButtonId = GfuiMenuCreateButtonControl(RmTuningSelectMenuHandle, hmenu, "wings2Button",
                                                  prevMenu, rmWing2Update);

    if((weight1price > (int)rmCredit) || (option1 > 0) || (optionMass == false))
        GfuiEnable(RmTuningSelectMenuHandle, weight1ButtonId, GFUI_DISABLE);

    if((weight2price > (int)rmCredit) || (option1 > 1) || (optionMass == false))
        GfuiEnable(RmTuningSelectMenuHandle, weight2ButtonId, GFUI_DISABLE);

    if((weight3price > (int)rmCredit) || (option1 > 2) || (optionMass == false))
        GfuiEnable(RmTuningSelectMenuHandle, weight3ButtonId, GFUI_DISABLE);

    if((gear1price > (int)rmCredit) || (option2 > 0) || (optionGear == false))
        GfuiEnable(RmTuningSelectMenuHandle, gearButtonId, GFUI_DISABLE);

    if((tire1price > (int)rmCredit) || (option7 > 0) || (optionWheel == false))
        GfuiEnable(RmTuningSelectMenuHandle, tiresButtonId, GFUI_DISABLE);

    if((airprice > (int)rmCredit) || (option3 & BIT1) || (optionEngine == false))
        GfuiEnable(RmTuningSelectMenuHandle, airButtonId, GFUI_DISABLE);

    if((exaustprice > (int)rmCredit) || (option3 & BIT2) || (optionEngine == false))
        GfuiEnable(RmTuningSelectMenuHandle, exaustButtonId, GFUI_DISABLE);

    if((ecuprice > (int)rmCredit) || (option3 & BIT3) || (optionEngine == false))
        GfuiEnable(RmTuningSelectMenuHandle, ecuButtonId, GFUI_DISABLE);

    if((springs1price > (int)rmCredit) || (option4 > 0) || (optionSprings == false))
        GfuiEnable(RmTuningSelectMenuHandle, springs1ButtonId, GFUI_DISABLE);

    if((springs2price > (int)rmCredit) || (option4 > 1) || (optionSprings == false))
        GfuiEnable(RmTuningSelectMenuHandle, springs2ButtonId, GFUI_DISABLE);

    if((arb1price > (int)rmCredit) || (option5 > 0) || (optionAntiroll == false))
        GfuiEnable(RmTuningSelectMenuHandle, antiroll1ButtonId, GFUI_DISABLE);

    if((arb2price > (int)rmCredit) || (option5 > 1) || (optionAntiroll == false))
        GfuiEnable(RmTuningSelectMenuHandle, antiroll2ButtonId, GFUI_DISABLE);

    if((wing1price > (int)rmCredit) || (option6 > 0) || (optionWings == false))
        GfuiEnable(RmTuningSelectMenuHandle, wings1ButtonId, GFUI_DISABLE);

    if((wing2price > (int)rmCredit) || (option6 > 1) || (optionWings == false))
        GfuiEnable(RmTuningSelectMenuHandle, wings2ButtonId, GFUI_DISABLE);

    // Close menu XML descriptor.
    GfParmReleaseHandle(hmenu);

    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(RmTuningSelectMenuHandle);
    GfuiAddKey(RmTuningSelectMenuHandle, GFUIK_ESCAPE, "Back To Career Menu",
               prevMenu, onCancelCB, NULL);

    return RmTuningSelectMenuHandle;
}
