/***************************************************************************

    file                 : racepitmenu.cpp
    created              : Mon Apr 24 18:16:37 CEST 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: racepitmenu.cpp 8159 2022-05-14 11:10:46Z beaglejoe $

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
    		Pit menu for human drivers
    @ingroup	racemantools
    @author	<a href=mailto:torcs@free.fr>Eric Espie</a>
    @version	$Id: racepitmenu.cpp 8159 2022-05-14 11:10:46Z beaglejoe $
*/

#include <cstdlib>

#include <portability.h>

#include <tgfclient.h>

#include <car.h>
#include <isoundengine.h>
#include <raceman.h>

#include "legacymenu.h"
#include "racescreens.h"
#include "racepitsetupmenu.h"

class PitCarSetupMenu;

static void             *menuHandle = NULL;

static int              fuelId;
static int              repairId;
static int              changetyreId;
static int              changecompoundId;
static int              changebrakeId;
static int              setupId;
static tCarElt          *rmCar;
static tfuiCallback     rmCallback;
PitCarSetupMenu         *pitsetupmenu = NULL;

const char          *strTire[] = { "NO", "YES" };
static const int    NbTireValues = sizeof(strTire) / sizeof(strTire[0]);

const char          *strBrake[] = { "NO", "YES" };
static const int    NbBrakeValues = sizeof(strBrake) / sizeof(strBrake[0]);

const char          *strCompound[] = { "NO CHANGE", "SOFT", "MEDIUM", "HARD", "WET", "EXTREM WET" };
static const int    NbCompoundValues = sizeof(strCompound) / sizeof(strCompound[0]);

static int          TireIndex = 0;
static int          CompoundIndex = 0;
static int          BrakeIndex = 0;

static int          TireLeftButtonId, TireRightButtonId;
static int          CompoundLeftButtonId, CompoundRightButtonId;
static int          BrakeLeftButtonId, BrakeRightButtonId;
const char *HumanName = NULL;

static void
rmUpdateCompound(void *vp)
{
    const long delta = (long)vp;
    CompoundIndex = (CompoundIndex + NbCompoundValues + delta) % NbCompoundValues;
    GfuiLabelSetText(menuHandle, changecompoundId, strCompound[CompoundIndex]);

    switch (CompoundIndex)
    {
    default:
    case 0:
        rmCar->pitcmd.tiresetChange = CarPitCmd::TireSetChange::NON_CHANGE;
         break;
    case 1:
       rmCar->pitcmd.tiresetChange = CarPitCmd::TireSetChange::SOFT;
        break;
    case 2:
        rmCar->pitcmd.tiresetChange = CarPitCmd::TireSetChange::MEDIUM;
        break;
    case 3:
        rmCar->pitcmd.tiresetChange = CarPitCmd::TireSetChange::HARD;
        break;
    case 4:
        rmCar->pitcmd.tiresetChange = CarPitCmd::TireSetChange::WET;
        break;
    case 5:
        rmCar->pitcmd.tiresetChange = CarPitCmd::TireSetChange::EXTREM_WET;
        break;
    }
}

static void
rmUpdateTyre(void *vp)
{
    const long delta = (long)vp;
    TireIndex = (TireIndex + NbTireValues + delta) % NbTireValues;
    GfuiLabelSetText(menuHandle, changetyreId, strTire[TireIndex]);

    if (TireIndex == 0)
    {
        rmCar->pitcmd.tireChange = CarPitCmd::NONE;
        rmUpdateCompound(0);
        GfuiEnable(menuHandle, CompoundLeftButtonId, GFUI_DISABLE);
        GfuiEnable(menuHandle, CompoundRightButtonId, GFUI_DISABLE);
        GfuiEnable(menuHandle, changecompoundId, GFUI_DISABLE);
    }
    else
    {
        rmCar->pitcmd.tireChange = CarPitCmd::ALL;
        GfuiEnable(menuHandle, CompoundLeftButtonId, GFUI_ENABLE);
        GfuiEnable(menuHandle, CompoundRightButtonId, GFUI_ENABLE);
        GfuiEnable(menuHandle, changecompoundId, GFUI_ENABLE);
    }
}

static void
rmUpdateBrake(void *vp)
{
    const long delta = (long)vp;
    BrakeIndex = (BrakeIndex + NbBrakeValues + delta) % NbBrakeValues;
    GfuiLabelSetText(menuHandle, changebrakeId, strBrake[BrakeIndex]);

    if (BrakeIndex == 0)
    {
        rmCar->pitcmd.brakeChange = CarPitCmd::BNONE;
    }
    else
    {
        rmCar->pitcmd.brakeChange = CarPitCmd::BALL;
    }
}

static void
rmUpdtFuel(void * /* dummy */)
{
    char	*val;
    char	buf[32];
    
    val = GfuiEditboxGetString(menuHandle, fuelId);
    rmCar->pitcmd.fuel = (tdble)strtod(val, (char **)NULL);
    sprintf(buf, "%.1f", rmCar->pitcmd.fuel);
    GfuiEditboxSetString(menuHandle, fuelId, buf);
}

static void
rmUpdtRepair(void * /* dummy */)
{
    char	*val;
    char	buf[32];
    
    val = GfuiEditboxGetString(menuHandle, repairId);
    rmCar->pitcmd.repair = strtol(val, (char **)NULL, 0);
    sprintf(buf, "%d", rmCar->pitcmd.repair);
    GfuiEditboxSetString(menuHandle, repairId, buf);
}

static void
rmStopAndGo(void * /* dummy */)
{
    rmCar->_pitStopType = RM_PIT_STOPANDGO;

    /*if(pitsetupmenu != NULL)
    {
        delete pitsetupmenu;
        pitsetupmenu = NULL;
    }*/

    rmCallback(rmCar);
}

static void
rmRepair(void* /* dummy */)
{
	rmCar->_pitStopType = RM_PIT_REPAIR;
    rmCar->pitcmd.setupChanged = true;

    if(pitsetupmenu != NULL)
    {
        delete pitsetupmenu;
        pitsetupmenu = NULL;
    }

	rmCallback(rmCar);
}

static void
rmSetup(void* /* dummy */)
{
    pitsetupmenu = new PitCarSetupMenu;
    pitsetupmenu->initialize(LmRaceEngine().outData()->_rePitRequester,
                             LmRaceEngine().outData()->track->sname, HumanName, menuHandle);

    rmCar->pitcmd.setupChanged = true;
    pitsetupmenu->runMenu();

}

/**
 * This function shows the pit menu and let the user fill in the amount
 * of fuel he wants and the number of damage he want to repair
 *
 * @param car The current car (pitcmd is modified on user decisions)
 * @param s The current situation (used to calculate the remaining time)
 * @param callback The function which is called after the user made a decision
 */
void
RmPitMenuStart(tCarElt *car, tSituation *s, tfuiCallback callback)
{
    char buf[128];

    rmCar = car;
    rmCallback = callback;

    for (int i = 0; i < s->_ncars; i++)
    {
        if (s->cars[i]->_driverType == RM_DRV_HUMAN)
                HumanName = s->cars[i]->_name;

        GfLogDebug("Pit Menu Human Name =%s\n", HumanName);
    }


    if (menuHandle)
        GfuiScreenRelease(menuHandle);

    GfLogDebug("Entering Pit menu\n");

    // Create screen, load menu XML descriptor and create static controls.
    menuHandle = GfuiScreenCreate(NULL, NULL, NULL, NULL, NULL, 1);

    void *menuXMLDescHdle = GfuiMenuLoad("pitmenu.xml");

    GfuiMenuCreateStaticControls(menuHandle, menuXMLDescHdle);

    GfuiMenuDefaultKeysAdd(menuHandle);

    // Create variable title label.
    int titleId = GfuiMenuCreateLabelControl(menuHandle, menuXMLDescHdle, "titlelabel");
    snprintf(buf, sizeof(buf), "Pit Stop for %s", car->_name);
    GfuiLabelSetText(menuHandle, titleId, buf);

    TireLeftButtonId =
        GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "tiresleftarrow", (void*)-1, rmUpdateTyre);
    TireRightButtonId =
        GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "tiresrightarrow", (void*)1, rmUpdateTyre);
    changetyreId = GfuiMenuCreateLabelControl(menuHandle, menuXMLDescHdle, "tireschangetext");

    CompoundLeftButtonId =
        GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "compoundsleftarrow", (void*)-1, rmUpdateCompound);
    CompoundRightButtonId =
            GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "compoundsrightarrow", (void*)1, rmUpdateCompound);
    changecompoundId = GfuiMenuCreateLabelControl(menuHandle, menuXMLDescHdle, "compoundschangetext");

    BrakeLeftButtonId =
        GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "brakeleftarrow", (void*)-1, rmUpdateBrake);
    BrakeRightButtonId =
        GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "brakerightarrow", (void*)1, rmUpdateBrake);
    changebrakeId = GfuiMenuCreateLabelControl(menuHandle, menuXMLDescHdle, "brakechangetext");

    rmUpdateTyre(0);
    rmUpdateCompound(0);
    rmUpdateBrake(0);

    GfLogDebug("# Car %s - Skill Level =%u\n", car->_carName, car->_skillLevel);

    if (car->_skillLevel < SEMI_PRO)
    {
        rmUpdateTyre(0);
        rmUpdateCompound(0);
        GfuiEnable(menuHandle, TireLeftButtonId, GFUI_DISABLE);
        GfuiEnable(menuHandle, TireRightButtonId, GFUI_DISABLE);
        GfuiEnable(menuHandle, changetyreId, GFUI_DISABLE);
        GfuiEnable(menuHandle, CompoundLeftButtonId, GFUI_DISABLE);
        GfuiEnable(menuHandle, CompoundRightButtonId, GFUI_DISABLE);
        GfuiEnable(menuHandle, changecompoundId, GFUI_DISABLE);
    }

    if (car->_skillLevel < PRO)
    {
        rmUpdateBrake(0);
        GfuiEnable(menuHandle, BrakeLeftButtonId, GFUI_DISABLE);
        GfuiEnable(menuHandle, BrakeRightButtonId, GFUI_DISABLE);
        GfuiEnable(menuHandle, changebrakeId, GFUI_DISABLE);
    }

    // Create labels for remaining laps and remaining fuel.
    int remainLapsTimeId = GfuiMenuCreateLabelControl(menuHandle, menuXMLDescHdle, "remaininglapstimelabel");
    if( s->_totTime > 0 && s->_totTime > s->currentTime ) // Timed part of the timed session
    {
        GfuiMenuCreateLabelControl(menuHandle, menuXMLDescHdle, "remainingtimelabel");
    	if( s->_extraLaps > 0)
            snprintf(buf, sizeof(buf), "%s + %d laps", GfTime2Str( s->_totTime - s->currentTime, NULL, true, 0 ), s->_extraLaps);
        else
            snprintf(buf, sizeof(buf), "%s", GfTime2Str( s->_totTime - s->currentTime, NULL, true, 0 ) );
    }
    else
    {
        GfuiMenuCreateLabelControl(menuHandle, menuXMLDescHdle, "remaininglapslabel");
        snprintf(buf, sizeof(buf), "%d", car->_remainingLaps); //Laps tot drive to win the race
    }

    GfuiLabelSetText(menuHandle, remainLapsTimeId, buf);

    int remainFuelId = GfuiMenuCreateLabelControl(menuHandle, menuXMLDescHdle, "remainingfuellabel");
    snprintf(buf, sizeof(buf), "%.1f", car->_fuel);
    GfuiLabelSetText(menuHandle, remainFuelId, buf);

    // Create edit boxes for fuel and repair amounts.
    fuelId = GfuiMenuCreateEditControl(menuHandle, menuXMLDescHdle, "fuelamountedit", NULL, NULL, rmUpdtFuel);
    snprintf(buf, sizeof(buf), "%.1f", car->pitcmd.fuel);
    GfuiEditboxSetString(menuHandle, fuelId, buf);

    repairId = GfuiMenuCreateEditControl(menuHandle, menuXMLDescHdle, "repairamountedit", NULL, NULL, rmUpdtRepair);
    snprintf(buf, sizeof(buf), "%d", (int)car->pitcmd.repair);
    GfuiEditboxSetString(menuHandle, repairId, buf);

    // Create Repair and Stop&Go buttons.
    GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "repairbutton", NULL, rmRepair);
    GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "stopgobutton", NULL, rmStopAndGo);
    setupId = GfuiMenuCreateButtonControl(menuHandle, menuXMLDescHdle, "setupbutton", NULL, rmSetup);

    if(s->_raceType == RM_TYPE_QUALIF || s->_raceType == RM_TYPE_RACE)
        GfuiEnable(menuHandle, setupId, GFUI_DISABLE);

    // Close menu XML descriptor.
    GfParmReleaseHandle(menuXMLDescHdle);
    
    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(menuHandle);

    // Activate the created screen.
    GfuiScreenActivate(menuHandle);
}

// Pit menu return callback.
static void
rmOnBackFromPitMenu(void *pvcar)
{
	const tCarElt* car = (tCarElt*)pvcar;
	LmRaceEngine().setPitCommand(car->index, &car->pitcmd);
    //GfParmReleaseHandle(menuHandle);

    if(pitsetupmenu != NULL)
    {
        delete pitsetupmenu;
        pitsetupmenu = NULL;
    }

	LegacyMenu::self().activateGameScreen();
}

bool
RmCheckPitRequest()
{
	// If one (human) driver is in pit, switch the display loop to the pit menu.
	if (LmRaceEngine().outData()->_rePitRequester)
	{
		// Mute sound.
		if (LegacyMenu::self().soundEngine())
			LegacyMenu::self().soundEngine()->mute(true);
		
		// TODO pit music??

		// First, stop the race engine.
		LmRaceEngine().stop();

		// Then open the pit menu (will return in ReCarsUpdateCarPitCmd).
        RmPitMenuStart(LmRaceEngine().outData()->_rePitRequester, LmRaceEngine().outData()->s, rmOnBackFromPitMenu);

		return true;
	}

	return false;
}
