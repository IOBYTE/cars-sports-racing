/***************************************************************************

    file                 : PitCarSetupMenu.cpp
    created              : April 2024
    copyright            : (C) 2024 Xavier Bertaux
    web                  : cars-sports-racing.sourceforge.net

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


/*
This file deals with car setup
*/

#include <sstream>
#include <iomanip>

#include <race.h>
#include <car.h>
#include <cars.h>
#include <tracks.h>
#include <drivers.h>
#include <playerpref.h>
#include <portability.h>

#include "racepitsetupmenu.h"

// callback functions
void PitCarSetupMenu::onActivateCallback(void *pMenu)
{
    // Get the PitCarSetupMenu instance.
    PitCarSetupMenu *pPitCarSetupMenu = static_cast<PitCarSetupMenu*>(pMenu);

    pPitCarSetupMenu->onActivate();
}

void PitCarSetupMenu::onQualifCallback(void *pMenu)
{
    // Get the PitCarSetupMenu instance.
    PitCarSetupMenu *pPitCarSetupMenu = static_cast<PitCarSetupMenu*>(pMenu);

    pPitCarSetupMenu->onAcceptQualif();
}

void PitCarSetupMenu::onRaceCallback(void *pMenu)
{
    // Get the PitCarSetupMenu instance.
    PitCarSetupMenu *pPitCarSetupMenu = static_cast<PitCarSetupMenu*>(pMenu);

    pPitCarSetupMenu->onAcceptRace();
}


void PitCarSetupMenu::onNextCallback(void *pMenu)
{
    // Get the PitCarSetupMenu instance from call-back user data.
    PitCarSetupMenu *pPitCarSetupMenu = static_cast<PitCarSetupMenu*>(pMenu);

    pPitCarSetupMenu->onNext();
}

void PitCarSetupMenu::onCancelCallback(void *pMenu)
{
    // Get the PitCarSetupMenu instance from call-back user data.
    PitCarSetupMenu *pPitCarSetupMenu = static_cast<PitCarSetupMenu*>(pMenu);

    pPitCarSetupMenu->onCancel();
}

void PitCarSetupMenu::onResetCallback(void *pMenu)
{
    // Get the PitCarSetupMenu instance from call-back user data.
    PitCarSetupMenu *pPitCarSetupMenu = static_cast<PitCarSetupMenu*>(pMenu);

    pPitCarSetupMenu->onReset();
}

void PitCarSetupMenu::onPreviousCallback(void *pMenu)
{
    // Get the PitCarSetupMenu instance from call-back user data.
    PitCarSetupMenu *pPitCarSetupMenu = static_cast<PitCarSetupMenu*>(pMenu);

    pPitCarSetupMenu->onPrevious();
}

void PitCarSetupMenu::onComboCallback(tComboBoxInfo *pInfo)
{
    // Get the PitCarSetupMenu instance from call-back user data.
    PitCarSetupMenu *pPitCarSetupMenu = static_cast<PitCarSetupMenu::ComboCallbackData *>(pInfo->userData)->menu;

    pPitCarSetupMenu->onCombo(pInfo);
}

// member functions

void PitCarSetupMenu::onActivate()
{
    GfLogDebug("Entering Car Setup menu\n");

    // Load settings from XML file.
    loadSettings();

    // Initialize GUI from loaded values.
    updateControls();
}

void PitCarSetupMenu::onAcceptQualif()
{
    // Get the current page values.
    readCurrentPage();

    // Save all page values.
    storeSettings1();

    // Switch back to garage menu.
    GfuiScreenActivate(getPreviousMenuHandle());
}

void PitCarSetupMenu::onAcceptRace()
{
    // Get the current page values.
    readCurrentPage();

    // Save all page values.
    storeSettings2();

    // Switch back to garage menu.
    GfuiScreenActivate(getPreviousMenuHandle());
}

void PitCarSetupMenu::onCancel() const
{
    // Back to previous screen.
    GfuiScreenActivate(getPreviousMenuHandle());
}

void PitCarSetupMenu::onReset()
{
    // Reset all values on current page to their defaults.
    for (size_t index = 0; index < ITEMS_PER_PAGE; index++)
    {
        attribute &att = items[currentPage][index];
        if (att.type == "edit")
        {
            att.value = att.defaultValue;
        }
        else if (att.type == "combo")
        {
            att.strValue = att.defaultStrValue;
        }
    }   

    // Update the GUI.
    updateControls();
}

void PitCarSetupMenu::onPrevious()
{
    // Check if first page.
    if (currentPage == 0)
        return;

    // Get the current page values.
    readCurrentPage();

    // Switch to previous page.
    currentPage--;

    // Update the GUI.
    updateControls();
}

void PitCarSetupMenu::onNext()
{
    // Check if last page.
    if (currentPage == (items.size() - 1))
        return;

    // Get the current page values.
    readCurrentPage();

    // Switch to next page.
    currentPage++;

    // update the GUI.
    updateControls();
}

void PitCarSetupMenu::onCombo(tComboBoxInfo *pInfo)
{
    ComboCallbackData *pData = static_cast<PitCarSetupMenu::ComboCallbackData *>(pInfo->userData);

    // Use currentPage and index in callback data to find item.
    attribute &att = items[currentPage][pData->index];

    att.strValue = pInfo->vecChoices[pInfo->nPos];
}

void PitCarSetupMenu::readCurrentPage()
{
    for (size_t index = 0; index < ITEMS_PER_PAGE; index++)
    {
        attribute &att = items[currentPage][index];
        if (att.exists && att.type == "edit")
        {
            std::string strValue(GfuiEditboxGetString(getMenuHandle(), att.editId));
            std::istringstream issValue(strValue);
            issValue >> att.value;
            GfLogDebug("# Current Page = %lu - Index = %lu\n", currentPage, index);
        }
    }
}

void PitCarSetupMenu::setuppage0(long unsigned i, attribute& att)
{
    tCarSetupItem *setupFuel = &(setupCar->setup.fuel);
    tCarSetupItem *setupBrkRep = &(setupCar->setup.brakeRepartition);
    tCarSetupItem *setupBrkPress = &(setupCar->setup.brakePressure);
    tCarSetupItem *setupFRARB = &(setupCar->setup.arbSpring[0]);
    tCarSetupItem *setupRRARB = &(setupCar->setup.arbSpring[1]);
    tCarSetupItem *setupFRWING = &(setupCar->setup.wingAngle[0]);
    tCarSetupItem *setupRRWING = &(setupCar->setup.wingAngle[1]);
    tCarSetupItem *setupSteerLck = &(setupCar->setup.steerLock);
    tCarSetupItem *setupSteerAgl = &(setupCar->setup.steerWheelRot);

    switch(i)
    {
    case 0:
        setupFuel->desired_value = att.value;
        setupFuel->changed = TRUE;
        GfLogDebug("Setup fuel = %.2f\n", setupFuel->desired_value);
        break;
    case 1:
        break;
    case 2:
        setupBrkRep->desired_value = att.value / 100.0;
        setupBrkRep->changed = TRUE;
        GfLogDebug("Setup BrakeRepartition = %.2f\n", setupBrkRep->desired_value);
        break;
    case 3:
        setupBrkPress->desired_value = att.value * 1000.0;
        setupBrkPress->changed = TRUE;
        GfLogDebug("Setup Brake Press = %.2f\n", setupBrkPress->desired_value);
        break;
    case 5:
        setupFRARB->desired_value = att.value * 1000.0;
        setupFRARB->changed = TRUE;
        GfLogDebug("Setup Front Anti-roll bar = %.2f\n", setupFRARB->desired_value);
        break;
    case 6:
        setupRRARB->desired_value = att.value * 1000.0;
        setupRRARB->changed = TRUE;
        GfLogDebug("Setup Rear Anti-roll bar = %.2f\n", setupRRARB->desired_value);
        break;
    case 8:
        setupFRWING->desired_value = DEG2RAD(att.value);
        setupFRWING->changed = TRUE;
        GfLogDebug("Setup Front Wing Angle = %.2f\n", setupFRWING->desired_value);
        break;
    case 9:
        setupRRWING->desired_value = DEG2RAD(att.value);
        setupRRWING->changed = TRUE;
        GfLogDebug("Setup Rear Wing Angle = %.2f\n", setupRRWING->desired_value);
        break;
    case 10:
        setupSteerLck->desired_value = DEG2RAD(att.value);
        setupSteerLck->changed = TRUE;
        GfLogDebug("Setup Steer Lock = %.2f\n", setupSteerLck->desired_value);
        break;
    case 11:
        setupSteerAgl->desired_value = DEG2RAD(att.value);
        setupSteerAgl->changed = TRUE;
        GfLogDebug("Setup Steer Speed = %.2f\n", setupSteerAgl->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage1(long unsigned i, attribute& att)
{
    tCarSetupItem *setupDRatio0 = &(setupCar->setup.differentialRatio[0]);
    tCarSetupItem *setupDMinTB0 = &(setupCar->setup.differentialMinTqBias[0]);
    tCarSetupItem *setupDMaxTB0 = &(setupCar->setup.differentialMaxTqBias[0]);
    tCarSetupItem *setupDVisc0 = &(setupCar->setup.differentialViscosity[0]);
    tCarSetupItem *setupDLT0 = &(setupCar->setup.differentialLockingTq[0]);
    tCarSetupItem *setupDMaxSB0 = &(setupCar->setup.differentialMaxSlipBias[0]);
    tCarSetupItem *setupDCMaxSB0 = &(setupCar->setup.differentialCoastMaxSlipBias[0]);

    switch(i)
    {
    case 0:
        setupDRatio0->desired_value = att.value;
        setupDRatio0->changed = TRUE;
        GfLogDebug("Setup Front Differential ratio = %.2f\n", setupDRatio0->desired_value);
        break;
    case 1:
        break;
    case 2:
        setupDMaxSB0->desired_value = att.value / 100.0;
        setupDMaxSB0->changed = TRUE;
        GfLogDebug("Setup Front Max Slip Bias = %.2f\n", setupDMaxSB0->desired_value);
        break;
    case 3:
        setupDCMaxSB0->desired_value = att.value / 100.0;
        setupDCMaxSB0->changed = TRUE;
        GfLogDebug("Setup Front Coast Max Slip Bias = %.2f\n", setupDCMaxSB0->desired_value);
        break;
    case 4:
        setupDMinTB0->desired_value = att.value / 100.0;
        setupDMinTB0->changed = TRUE;
        GfLogDebug("Setup Front Min Torque Bias = %.2f\n", setupDMinTB0->desired_value);
        break;
    case 5:
        setupDMaxTB0->desired_value = att.value / 100.0;
        setupDMaxTB0->changed = TRUE;
        GfLogDebug("Setup Front Diff Max Torque Bias = %.2f\n", setupDMaxTB0->desired_value);
        break;
    case 6:
        setupDLT0->desired_value = att.value;
        setupDLT0->changed = TRUE;
        GfLogDebug("Setup Front Diff Locking Input Torque = %.2f\n", setupDLT0->desired_value);
        break;
    case 7:
        setupDVisc0->desired_value = att.value;
        setupDVisc0->changed = TRUE;
        GfLogDebug("Setup Front Diff Viscosity Factor = %.2f\n", setupDVisc0->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage2(long unsigned i, attribute& att)
{
    tCarSetupItem *setupDRatio0 = &(setupCar->setup.differentialRatio[2]);
    tCarSetupItem *setupDMinTB0 = &(setupCar->setup.differentialMinTqBias[2]);
    tCarSetupItem *setupDMaxTB0 = &(setupCar->setup.differentialMaxTqBias[2]);
    tCarSetupItem *setupDVisc0 = &(setupCar->setup.differentialViscosity[2]);
    tCarSetupItem *setupDLT0 = &(setupCar->setup.differentialLockingTq[2]);
    tCarSetupItem *setupDMaxSB0 = &(setupCar->setup.differentialMaxSlipBias[2]);
    tCarSetupItem *setupDCMaxSB0 = &(setupCar->setup.differentialCoastMaxSlipBias[2]);

    switch(i)
    {
    case 0:
        setupDRatio0->desired_value = att.value;
        setupDRatio0->changed = TRUE;
        GfLogDebug("Setup Central Differential ratio = %.2f\n", setupDRatio0->desired_value);
        break;
    case 1:
        break;
    case 2:
        setupDMaxSB0->desired_value = att.value / 100.0;
        setupDMaxSB0->changed = TRUE;
        GfLogDebug("Setup Central Max Slip Bias = %.2f\n", setupDMaxSB0->desired_value);
        break;
    case 3:
        setupDCMaxSB0->desired_value = att.value / 100.0;
        setupDCMaxSB0->changed = TRUE;
        GfLogDebug("Setup Central Coast Max Slip Bias = %.2f\n", setupDCMaxSB0->desired_value);
        break;
    case 4:
        setupDMinTB0->desired_value = att.value / 100.0;
        setupDMinTB0->changed = TRUE;
        GfLogDebug("Setup Central Min Torque Bias = %.2f\n", setupDMinTB0->desired_value);
        break;
    case 5:
        setupDMaxTB0->desired_value = att.value / 100.0;
        setupDMaxTB0->changed = TRUE;
        GfLogDebug("Setup Central Diff Max Torque Bias = %.2f\n", setupDMaxTB0->desired_value);
        break;
    case 6:
        setupDLT0->desired_value = att.value;
        setupDLT0->changed = TRUE;
        GfLogDebug("Setup Central Diff Locking Input Torque = %.2f\n", setupDLT0->desired_value);
        break;
    case 7:
        setupDVisc0->desired_value = att.value;
        setupDVisc0->changed = TRUE;
        GfLogDebug("Setup Central Diff Viscosity Factor = %.2f\n", setupDVisc0->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage3(long unsigned i, attribute& att)
{
    tCarSetupItem *setupDRatio0 = &(setupCar->setup.differentialRatio[1]);
    tCarSetupItem *setupDMinTB0 = &(setupCar->setup.differentialMinTqBias[1]);
    tCarSetupItem *setupDMaxTB0 = &(setupCar->setup.differentialMaxTqBias[1]);
    tCarSetupItem *setupDVisc0 = &(setupCar->setup.differentialViscosity[1]);
    tCarSetupItem *setupDLT0 = &(setupCar->setup.differentialLockingTq[1]);
    tCarSetupItem *setupDMaxSB0 = &(setupCar->setup.differentialMaxSlipBias[1]);
    tCarSetupItem *setupDCMaxSB0 = &(setupCar->setup.differentialCoastMaxSlipBias[1]);

    switch(i)
    {
    case 0:
        setupDRatio0->desired_value = att.value;
        setupDRatio0->changed = TRUE;
        GfLogDebug("Setup Rear Differential ratio = %.2f\n", setupDRatio0->desired_value);
        break;
    case 1:
        break;
    case 2:
        setupDMaxSB0->desired_value = att.value / 100.0;
        setupDMaxSB0->changed = TRUE;
        GfLogDebug("Setup Rear Max Slip Bias = %.2f\n", setupDMaxSB0->desired_value);
        break;
    case 3:
        setupDCMaxSB0->desired_value = att.value / 100.0;
        setupDCMaxSB0->changed = TRUE;
        GfLogDebug("Setup Rear Coast Max Slip Bias = %.2f\n", setupDCMaxSB0->desired_value);
        break;
    case 4:
        setupDMinTB0->desired_value = att.value / 100.0;
        setupDMinTB0->changed = TRUE;
        GfLogDebug("Setup Rear Min Torque Bias = %.2f\n", setupDMinTB0->desired_value);
        break;
    case 5:
        setupDMaxTB0->desired_value = att.value / 100.0;
        setupDMaxTB0->changed = TRUE;
        GfLogDebug("Setup Rear Diff Max Torque Bias = %.2f\n", setupDMaxTB0->desired_value);
        break;
    case 6:
        setupDLT0->desired_value = att.value;
        setupDLT0->changed = TRUE;
        GfLogDebug("Setup Rear Diff Locking Input Torque = %.2f\n", setupDLT0->desired_value);
        break;
    case 7:
        setupDVisc0->desired_value = att.value;
        setupDVisc0->changed = TRUE;
        GfLogDebug("Setup Rear Diff Viscosity Factor = %.2f\n", setupDVisc0->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage4(long unsigned i, attribute& att)
{
    tCarSetupItem *setupGearR = &(setupCar->setup.gearRatio[0]);
    tCarSetupItem *setupGear1 = &(setupCar->setup.gearRatio[2]);
    tCarSetupItem *setupGear2 = &(setupCar->setup.gearRatio[3]);
    tCarSetupItem *setupGear3 = &(setupCar->setup.gearRatio[4]);
    tCarSetupItem *setupGear4 = &(setupCar->setup.gearRatio[5]);
    tCarSetupItem *setupGear5 = &(setupCar->setup.gearRatio[6]);
    tCarSetupItem *setupGear6 = &(setupCar->setup.gearRatio[7]);
    tCarSetupItem *setupGear7 = &(setupCar->setup.gearRatio[8]);
    tCarSetupItem *setupGear8 = &(setupCar->setup.gearRatio[9]);

    switch(i)
    {
    case 0:
        setupGearR->desired_value = att.value;
        setupGearR->changed = TRUE;
        GfLogDebug("Setup Gear ration R = %.2f\n", setupGearR->desired_value);
        break;
    case 1:
        setupGear1->desired_value = att.value;
        setupGear1->changed = TRUE;
        GfLogDebug("Setup Gear ration 1 = %.2f\n", setupGear1->desired_value);
        break;
    case 2:
        setupGear2->desired_value = att.value;
        setupGear2->changed = TRUE;
        GfLogDebug("Setup Gear ration 2 = %.2f\n", setupGear2->desired_value);
        break;
    case 3:
        setupGear3->desired_value = att.value;
        setupGear3->changed = TRUE;
        GfLogDebug("Setup Gear ration 3 = %.2f\n", setupGear3->desired_value);
        break;
    case 4:
        setupGear4->desired_value = att.value;
        setupGear4->changed = TRUE;
        GfLogDebug("Setup Gear ration 4 = %.2f\n", setupGear4->desired_value);
        break;
    case 5:
        setupGear5->desired_value = att.value;
        setupGear5->changed = TRUE;
        GfLogDebug("Setup Gear ration 5 = %.2f\n", setupGear5->desired_value);
        break;
    case 6:
        setupGear6->desired_value = att.value;
        setupGear6->changed = TRUE;
        GfLogDebug("Setup Gear ration 6 = %.2f\n", setupGear6->desired_value);
        break;
    case 7:
        setupGear7->desired_value = att.value;
        setupGear7->changed = TRUE;
        GfLogDebug("Setup Gear ration 7 = %.2f\n", setupGear7->desired_value);
        break;
    case 8:
        setupGear8->desired_value = att.value;
        setupGear8->changed = TRUE;
        GfLogDebug("Setup Gear ration 8 = %.2f\n", setupGear8->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage5(long unsigned i, attribute& att)
{
    tCarSetupItem *setupToeR = &(setupCar->setup.toe[0]);
    tCarSetupItem *setupCamberR = &(setupCar->setup.camber[0]);
    tCarSetupItem *setupPressureR = &(setupCar->setup.tirePressure[0]);

    tCarSetupItem *setupToeL = &(setupCar->setup.toe[1]);
    tCarSetupItem *setupCamberL = &(setupCar->setup.camber[1]);
    tCarSetupItem *setupPressureL = &(setupCar->setup.tirePressure[1]);

    switch(i)
    {
    case 3:
        setupToeL->desired_value = DEG2RAD(att.value);
        setupToeL->changed = TRUE;
        GfLogDebug("Setup Wheel Left Toe = %.2f\n", setupToeL->desired_value);
        break;
    case 4:
        setupCamberL->desired_value = DEG2RAD(att.value);
        setupCamberL->changed = TRUE;
        GfLogDebug("Setup Wheel Left Toe Camber = %.2f\n", setupCamberL->desired_value);
        break;
    case 5:
        setupPressureL->desired_value = att.value * 6894.76f;
        setupPressureL->changed = TRUE;
        GfLogDebug("Setup Wheel Left Toe Pressure = %.2f\n", setupPressureL->desired_value);
        break;
    case 8:
        setupToeR->desired_value = DEG2RAD(att.value);
        setupToeR->changed = TRUE;
        GfLogDebug("Setup Wheel Right Toe = %.2f\n", setupToeR->desired_value);
        break;
    case 9:
        setupCamberR->desired_value = DEG2RAD(att.value);
        setupCamberR->changed = TRUE;
        GfLogDebug("Setup Wheel Right Toe Camber = %.2f\n", setupCamberR->desired_value);
        break;
    case 10:
        setupPressureR->desired_value = att.value * 6894.76f;
        setupPressureR->changed = TRUE;
        GfLogDebug("Setup Wheel Right Toe Pressure = %.2f\n", setupPressureR->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage6(long unsigned i, attribute& att)
{
    tCarSetupItem *setupToeR = &(setupCar->setup.toe[2]);
    tCarSetupItem *setupCamberR = &(setupCar->setup.camber[2]);
    tCarSetupItem *setupPressureR = &(setupCar->setup.tirePressure[2]);

    tCarSetupItem *setupToeL = &(setupCar->setup.toe[3]);
    tCarSetupItem *setupCamberL = &(setupCar->setup.camber[3]);
    tCarSetupItem *setupPressureL = &(setupCar->setup.tirePressure[3]);

    switch(i)
    {
    case 3:
        setupToeL->desired_value = DEG2RAD(att.value);
        setupToeL->changed = TRUE;
        GfLogDebug("Setup Wheel Left Toe = %.2f\n", setupToeL->desired_value);
        break;
    case 4:
        setupCamberL->desired_value = DEG2RAD(att.value);
        setupCamberL->changed = TRUE;
        GfLogDebug("Setup Wheel Left Toe Camber = %.2f\n", setupCamberL->desired_value);
        break;
    case 5:
        setupPressureL->desired_value = att.value * 6894.76f;
        setupPressureL->changed = TRUE;
        GfLogDebug("Setup Wheel Left Toe Pressure = %.2f\n", setupPressureL->desired_value);
        break;
    case 8:
        setupToeR->desired_value = DEG2RAD(att.value);
        setupToeR->changed = TRUE;
        GfLogDebug("Setup Wheel Right Toe = %.2f\n", setupToeR->desired_value);
        break;
    case 9:
        setupCamberR->desired_value = DEG2RAD(att.value);
        setupCamberR->changed = TRUE;
        GfLogDebug("Setup Wheel Right Toe Camber = %.2f\n", setupCamberR->desired_value);
        break;
    case 10:
        setupPressureR->desired_value = att.value * 6894.76f;
        setupPressureR->changed = TRUE;
        GfLogDebug("Setup Wheel Right Toe Pressure = %.2f\n", setupPressureR->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage7(long unsigned i, attribute& att)
{
    tCarSetupItem *setupRspring     = &(setupCar->setup.suspSpring[0]);
    tCarSetupItem *setupRcourse     = &(setupCar->setup.suspCourse[0]);
    tCarSetupItem *setupRbellcrank  = &(setupCar->setup.suspBellcrank[0]);
    tCarSetupItem *setupRpacker     = &(setupCar->setup.suspPacker[0]);
    tCarSetupItem *setupLspring     = &(setupCar->setup.suspSpring[1]);
    tCarSetupItem *setupLcourse     = &(setupCar->setup.suspCourse[1]);
    tCarSetupItem *setupLbellcrank  = &(setupCar->setup.suspBellcrank[1]);
    tCarSetupItem *setupLpacker     = &(setupCar->setup.suspPacker[1]);

    switch(i)
    {
    case 1:
        setupRspring->desired_value = att.value * 1000.0f;
        setupRspring->changed = TRUE;
        GfLogDebug("Setup Front Right spring = %.2f\n", setupRspring->desired_value);
        break;
    case 2:
        setupRcourse->desired_value = att.value * 0.001f;
        setupRcourse->changed= TRUE;
        GfLogDebug("Setup Front Left spring = %.2f\n", setupRspring->desired_value);
        break;
    case 3:
        setupRbellcrank->desired_value = att.value;
        setupRbellcrank->changed = TRUE;
        GfLogDebug("Setup Front Left bellcrank = %.2f\n", setupRbellcrank->desired_value);
        break;
    case 4:
        setupRpacker->desired_value = att.value * 0.001f;
        setupRpacker->changed = TRUE;
        GfLogDebug("Setup Brake Press = %.2f\n", setupRpacker->desired_value);
        break;
    case 5:
        setupLspring->desired_value = att.value * 1000.0f;
        setupLspring->changed = TRUE;
        GfLogDebug("Setup Front Left spring = %.2f\n", setupLspring->desired_value);
        break;
    case 6:
        setupLcourse->desired_value = att.value * 0.001f;
        setupLcourse->changed = TRUE;
        GfLogDebug("Setup Front Left course = %.2f\n", setupLcourse->desired_value);
        break;
    case 7:
        setupLbellcrank->desired_value = att.value;
        setupLbellcrank->changed = TRUE;
        GfLogDebug("Setup Front Left ballcrank = %.2f\n", setupLbellcrank->desired_value);
        break;
    case 9:
        setupLpacker->desired_value = att.value * 0.001f;
        setupLpacker->changed = TRUE;
        GfLogDebug("Setup Front Left packer = %.2f\n", setupLpacker->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage8(long unsigned i, attribute& att)
{
    tCarSetupItem *setupRspring     = &(setupCar->setup.suspSpring[2]);
    tCarSetupItem *setupRcourse     = &(setupCar->setup.suspCourse[2]);
    tCarSetupItem *setupRbellcrank  = &(setupCar->setup.suspBellcrank[2]);
    tCarSetupItem *setupRpacker     = &(setupCar->setup.suspPacker[2]);
    tCarSetupItem *setupLspring     = &(setupCar->setup.suspSpring[3]);
    tCarSetupItem *setupLcourse     = &(setupCar->setup.suspCourse[3]);
    tCarSetupItem *setupLbellcrank  = &(setupCar->setup.suspBellcrank[3]);
    tCarSetupItem *setupLpacker     = &(setupCar->setup.suspPacker[3]);

    switch(i)
    {
    case 1:
        setupRspring->desired_value = att.value * 1000.0f;
        setupRspring->changed = TRUE;
        GfLogDebug("Setup Front Right spring = %.2f\n", setupRspring->desired_value);
        break;
    case 2:
        setupRcourse->desired_value = att.value * 0.001f;
        setupRcourse->changed= TRUE;
        GfLogDebug("Setup Front Left spring = %.2f\n", setupRspring->desired_value);
        break;
    case 3:
        setupRbellcrank->desired_value = att.value;
        setupRbellcrank->changed = TRUE;
        GfLogDebug("Setup Front Left bellcrank = %.2f\n", setupRbellcrank->desired_value);
        break;
    case 4:
        setupRpacker->desired_value = att.value * 0.001f;
        setupRpacker->changed = TRUE;
        GfLogDebug("Setup Brake Press = %.2f\n", setupRpacker->desired_value);
        break;
    case 5:
        setupLspring->desired_value = att.value * 1000.0f;
        setupLspring->changed = TRUE;
        GfLogDebug("Setup Front Left spring = %.2f\n", setupLspring->desired_value);
        break;
    case 6:
        setupLcourse->desired_value = att.value * 0.001f;
        setupLcourse->changed = TRUE;
        GfLogDebug("Setup Front Left course = %.2f\n", setupLcourse->desired_value);
        break;
    case 7:
        setupLbellcrank->desired_value = att.value;
        setupLbellcrank->changed = TRUE;
        GfLogDebug("Setup Front Left ballcrank = %.2f\n", setupLbellcrank->desired_value);
        break;
    case 9:
        setupLpacker->desired_value = att.value * 0.001f;
        setupLpacker->changed = TRUE;
        GfLogDebug("Setup Front Left packer = %.2f\n", setupLpacker->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage9(long unsigned i, attribute& att)
{
    tCarSetupItem *setupFRslowbump          = &(setupCar->setup.suspSlowBump[0]);
    tCarSetupItem *setupFRslowrebound       = &(setupCar->setup.suspSlowRebound[0]);
    tCarSetupItem *setupFRfastbump          = &(setupCar->setup.suspFastBump[0]);
    tCarSetupItem *setupFRfastrebound       = &(setupCar->setup.suspFastRebound[0]);
    tCarSetupItem *setupFRbumpvelocity      = &(setupCar->setup.suspBumpLvel[0]);
    tCarSetupItem *setupFRreboundvelocity   = &(setupCar->setup.suspReboundLvel[0]);
    tCarSetupItem *setupFLslowbump          = &(setupCar->setup.suspSlowBump[1]);
    tCarSetupItem *setupFLslowrebound       = &(setupCar->setup.suspSlowRebound[1]);
    tCarSetupItem *setupFLfastbump          = &(setupCar->setup.suspFastBump[1]);
    tCarSetupItem *setupFLfastrebound       = &(setupCar->setup.suspFastRebound[1]);
    tCarSetupItem *setupFLbumpvelocity      = &(setupCar->setup.suspBumpLvel[1]);
    tCarSetupItem *setupFLreboundvelocity   = &(setupCar->setup.suspReboundLvel[1]);

    switch(i)
    {
    case 0:
        setupFRslowbump->desired_value = att.value * 1000.0f;
        setupFRslowbump->changed = TRUE;
        GfLogDebug("Setup Front Left slow bump          = %.2f\n", setupFRslowbump->desired_value);
        break;
    case 1:
        setupFRslowrebound->desired_value = att.value * 1000.0f;
        setupFRslowrebound->changed = TRUE;
        GfLogDebug("Setup Front Left slow rebound       = %.2f\n", setupFRslowrebound->desired_value);
        break;
    case 2:
        setupFRfastbump->desired_value = att.value * 1000.0f;
        setupFRfastbump->changed = TRUE;
        GfLogDebug("Setup Front Left fast bump          = %.2f\n", setupFRfastbump->desired_value);
        break;
    case 3:
        setupFRfastrebound->desired_value = att.value * 1000.0f;
        setupFRfastrebound->changed = TRUE;
        GfLogDebug("Setup Front Left fast rebound       = %.2f\n", setupFRfastrebound->desired_value);
        break;
    case 4:
        setupFRbumpvelocity->desired_value = att.value;
        setupFRbumpvelocity->changed = TRUE;
        GfLogDebug("Setup Front Left bump velocity      = %.2f\n", setupFRbumpvelocity->desired_value);
        break;
    case 5:
        setupFRreboundvelocity->desired_value = att.value;
        setupFRreboundvelocity->changed = TRUE;
        GfLogDebug("Setup Front Left rebound velocity   = %.2f\n", setupFRreboundvelocity->desired_value);
        break;
    case 6:
        setupFLslowbump->desired_value = att.value * 1000.0f;
        setupFLslowbump->changed = TRUE;
        GfLogDebug("Setup Front Right slow bump         = %.2f\n", setupFLslowbump->desired_value);
        break;
    case 7:
        setupFLslowrebound->desired_value = att.value * 1000.0f;
        setupFLslowrebound->changed = TRUE;
        GfLogDebug("Setup Front Right slow rebound      = %.2f\n", setupFLslowrebound->desired_value);
        break;
    case 8:
        setupFLfastbump->desired_value = att.value * 1000.0f;
        setupFLfastbump->changed = TRUE;
        GfLogDebug("Setup Front Right fast bump         = %.2f\n", setupFLfastbump->desired_value);
        break;
    case 9:
        setupFLfastrebound->desired_value = att.value * 1000.0f;
        setupFLfastrebound->changed = TRUE;
        GfLogDebug("Setup Front Right fast rebound      = %.2f\n", setupFLfastrebound->desired_value);
        break;
    case 10:
        setupFLbumpvelocity->desired_value = att.value;
        setupFLbumpvelocity->changed = TRUE;
        GfLogDebug("Setup Front Right bump velocity     = %.2f\n", setupFLbumpvelocity->desired_value);
        break;
    case 11:
        setupFLreboundvelocity->desired_value = att.value;
        setupFLreboundvelocity->changed = TRUE;
        GfLogDebug("Setup Front Right rebound velocity  = %.2f\n", setupFLreboundvelocity->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::setuppage10(long unsigned i, attribute& att)
{
    tCarSetupItem *setupRRslowbump          = &(setupCar->setup.suspSlowBump[2]);
    tCarSetupItem *setupRRslowrebound       = &(setupCar->setup.suspSlowRebound[2]);
    tCarSetupItem *setupRRfastbump          = &(setupCar->setup.suspFastBump[2]);
    tCarSetupItem *setupRRfastrebound       = &(setupCar->setup.suspFastRebound[2]);
    tCarSetupItem *setupRRbumpvelocity      = &(setupCar->setup.suspBumpLvel[2]);
    tCarSetupItem *setupRRreboundvelocity   = &(setupCar->setup.suspReboundLvel[2]);
    tCarSetupItem *setupRLslowbump          = &(setupCar->setup.suspSlowBump[3]);
    tCarSetupItem *setupRLslowrebound       = &(setupCar->setup.suspSlowRebound[3]);
    tCarSetupItem *setupRLfastbump          = &(setupCar->setup.suspFastBump[3]);
    tCarSetupItem *setupRLfastrebound       = &(setupCar->setup.suspFastRebound[3]);
    tCarSetupItem *setupRLbumpvelocity      = &(setupCar->setup.suspBumpLvel[3]);
    tCarSetupItem *setupRLreboundvelocity   = &(setupCar->setup.suspReboundLvel[3]);

    switch(i)
    {
    case 0:
        setupRRslowbump->desired_value = att.value * 1000.0f;
        setupRRslowbump->changed = TRUE;
        GfLogDebug("Setup Rear Left slow bump          = %.2f\n", setupRRslowbump->desired_value);
        break;
    case 1:
        setupRRslowrebound->desired_value = att.value * 1000.0f;
        setupRRslowrebound->changed = TRUE;
        GfLogDebug("Setup Rear Left slow rebound       = %.2f\n", setupRRslowrebound->desired_value);
        break;
    case 2:
        setupRRfastbump->desired_value = att.value * 1000.0f;
        setupRRfastbump->changed = TRUE;
        GfLogDebug("Setup Front Left fast bump          = %.2f\n", setupRRfastbump->desired_value);
        break;
    case 3:
        setupRRfastrebound->desired_value = att.value * 1000.0f;
        setupRRfastrebound->changed = TRUE;
        GfLogDebug("Setup Rear Left fast rebound       = %.2f\n", setupRRfastrebound->desired_value);
        break;
    case 4:
        setupRRbumpvelocity->desired_value = att.value;
        setupRRbumpvelocity->changed = TRUE;
        GfLogDebug("Setup Rear Left bump velocity      = %.2f\n", setupRRbumpvelocity->desired_value);
        break;
    case 5:
        setupRRreboundvelocity->desired_value = att.value;
        setupRRreboundvelocity->changed = TRUE;
        GfLogDebug("Setup Rear Left rebound velocity   = %.2f\n", setupRRreboundvelocity->desired_value);
        break;
    case 6:
        setupRLslowbump->desired_value = att.value * 1000.0f;
        setupRLslowbump->changed = TRUE;
        GfLogDebug("Setup Rear Right slow bump         = %.2f\n", setupRLslowbump->desired_value);
        break;
    case 7:
        setupRLslowrebound->desired_value = att.value * 1000.0f;
        setupRLslowrebound->changed = TRUE;
        GfLogDebug("Setup Rear Right slow rebound      = %.2f\n", setupRLslowrebound->desired_value);
        break;
    case 8:
        setupRLfastbump->desired_value = att.value * 1000.0f;
        setupRLfastbump->changed = TRUE;
        GfLogDebug("Setup Rear Right fast bump         = %.2f\n", setupRLfastbump->desired_value);
        break;
    case 9:
        setupRLfastrebound->desired_value = att.value * 1000.0f;
        setupRLfastrebound->changed = TRUE;
        GfLogDebug("Setup Rear Right fast rebound      = %.2f\n", setupRLfastrebound->desired_value);
        break;
    case 10:
        setupRLbumpvelocity->desired_value = att.value;
        setupRLbumpvelocity->changed = TRUE;
        GfLogDebug("Setup Rear Right bump velocity     = %.2f\n", setupRLbumpvelocity->desired_value);
        break;
    case 11:
        setupRLreboundvelocity->desired_value = att.value;
        setupRLreboundvelocity->changed = TRUE;
        GfLogDebug("Setup Rear Right rebound velocity  = %.2f\n", setupRLreboundvelocity->desired_value);
        break;
    }

    setupCar->pitcmd.setupChanged = true;
}

void PitCarSetupMenu::updateControls()
{
    GfuiEnable(getMenuHandle(), getDynamicControlId("PreviousButton"), currentPage != 0 ? GFUI_ENABLE : GFUI_DISABLE);
    GfuiEnable(getMenuHandle(), getDynamicControlId("NextButton"), currentPage < (items.size() - 1) ? GFUI_ENABLE : GFUI_DISABLE);

    for (size_t index = 0; index < ITEMS_PER_PAGE; ++index)
    {
        attribute &att = items[currentPage][index];

        // Set label text.
        std::ostringstream ossLabel;
        if (!att.label.empty())
        {
            ossLabel << att.label;
            if (!att.units.empty())
                ossLabel << " (" << att.units << ")"; 
            ossLabel << ":";
        }

        GfuiLabelSetText(getMenuHandle(),
                         att.labelId,
                         ossLabel.str().c_str());

        ossLabel.str("");
        ossLabel.clear();

        // Set default label text.
        if (att.exists)
        {
            if (att.type == "edit")
            {
                // Check for missing min and max.
                if (att.minValue == att.maxValue)
                {
                    ossLabel << std::fixed << std::setprecision(att.precision)
                    << "Default: " << att.defaultValue;
                }
                else
                {
                    ossLabel << std::fixed << std::setprecision(att.precision)
                    << "Min: " << att.minValue
                    << "  Default: " << att.defaultValue
                    << "  Max: " << att.maxValue;
                }
            }
            else if (att.type == "combo")
            {
                ossLabel << "Default: " << att.defaultStrValue;
            }
        }
        GfuiLabelSetText(getMenuHandle(),
                         att.defaultLabelId,
                         ossLabel.str().c_str());

        // Update the edit or combo boxes.
        if (att.label.empty())
        {
            GfuiVisibilitySet(getMenuHandle(), att.editId, GFUI_INVISIBLE);
            GfuiVisibilitySet(getMenuHandle(), att.comboId, GFUI_INVISIBLE);
        }
        else
        {
            if (att.type == "edit")
            {
                GfuiVisibilitySet(getMenuHandle(), att.editId, GFUI_VISIBLE);
                GfuiVisibilitySet(getMenuHandle(), att.comboId, GFUI_INVISIBLE);
            }
            else if (att.type == "combo")
            {
                GfuiVisibilitySet(getMenuHandle(), att.editId, GFUI_INVISIBLE);
                GfuiVisibilitySet(getMenuHandle(), att.comboId, GFUI_VISIBLE);
            }
            else
            {
                GfuiVisibilitySet(getMenuHandle(), att.editId, GFUI_INVISIBLE);
                GfuiVisibilitySet(getMenuHandle(), att.comboId, GFUI_INVISIBLE);
            }

            if (att.exists)
            {
                if (att.type == "edit")
                {
                    std::ostringstream ossValue;
                    ossValue << std::fixed << std::setprecision(att.precision) << att.value;
                    GfuiEditboxSetString(getMenuHandle(), att.editId, ossValue.str().c_str());

                    if (att.minValue == att.maxValue)
                        GfuiEnable(getMenuHandle(), att.editId, GFUI_DISABLE);
                    else
                        GfuiEnable(getMenuHandle(), att.editId, GFUI_ENABLE);
                }
                else if (att.type == "combo")
                {
                    GfuiComboboxClear(getMenuHandle(), att.comboId);
                    size_t selected = 0;
                    for (size_t i = 0; i < att.in.size(); ++i)
                    {
                        GfuiComboboxAddText(getMenuHandle(), att.comboId, att.in[i].c_str()); 
                        if (att.in[i] == att.strValue)
                            selected = i;
                    }
                    GfuiComboboxSetSelectedIndex(getMenuHandle(), att.comboId, selected);
                }
            }
            else
            {
                if (att.type == "edit")
                {
                    GfuiEditboxSetString(getMenuHandle(), att.editId, "----");
                    GfuiEnable(getMenuHandle(), att.editId, GFUI_DISABLE);
                }
                else if (att.type == "combo")
                {
                    GfuiComboboxClear(getMenuHandle(), att.comboId);
                }
            }
        }
    }
}

void PitCarSetupMenu::loadSettings()
{
    GfuiLabelSetText(getMenuHandle(), getDynamicControlId("CarNameLabel"), strCar);

    int option1, option2, option3, option4, option5, option6, option7;
    option1 = option2 = option3 = option4 = option5 = option6 = option7 = 0;

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
            GfLogDebug("Car Name in garage = %s - passed = %s\n", carname, strCar);

            if (strcmp(carname, strCar) == 0)
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

    // Open the XML file of the car.
    std::ostringstream ossCarFileName;
    std::string strCarId = strCar;
    ossCarFileName << "cars/models/" << strCarId << '/' << strCarId << PARAMEXT;
    void *hparmCar = GfParmReadFile(ossCarFileName.str(), GFPARM_RMODE_STD);
    if (!hparmCar)
    {
        GfLogError("Car %s (file %s not %s)\n",
                   strCar, ossCarFileName.str().c_str(),
                   GfFileExists(ossCarFileName.str().c_str()) ? "readable" : "found");
        return;
    }

    GfLogDebug("Opened car file: %s\n", ossCarFileName.str().c_str());

    // Open the XML file of the car setup.
    std::ostringstream ossCarSetupFileName;
    std::string strTrackId = strTrack;
    std::string strName = strHumanName;
    ossCarSetupFileName << GfLocalDir() << "drivers/human/cars/" << strName << '/' << strCarId << "/race/" << strTrackId << PARAMEXT;
    void *hparmCarSetup = GfParmReadFile(ossCarSetupFileName.str(), GFPARM_RMODE_STD);
    if (!hparmCarSetup)
    {
        GfLogDebug("Car Setup: %s/%s (file %s not %s)\n",
                   strCar, strTrack, ossCarSetupFileName.str().c_str(),
                   GfFileExists(ossCarSetupFileName.str().c_str()) ? "readable" : "found");
    }
    else
        GfLogDebug("Opened car setup file: %s\n", ossCarSetupFileName.str().c_str());

    void *hparmItems = GfuiMenuLoad("pitcarsetupmenuitems.xml");
    if (!hparmItems)
    {
        GfLogError("Car Setup Items (file %s not %s)\n",
                   "pitcarsetupmenuitems.xml",
                   GfFileExists("pitcarsetupmenuitems.xml") ? "readable" : "found");
    }
    else
    {
        GfLogDebug("Opened car setup menu items file: %s\n", "pitcarsetupmenuitems.xml");
        std::vector<std::string> sections = GfParmListGetSectionNamesList(hparmItems);

        for (size_t i = 0; i < sections.size(); ++i)
        {
            std::string strSection = sections[i];

            GfLogDebug("section %zu: %s\n", i, strSection.c_str());

            size_t page = GfParmGetNum(hparmItems, strSection.c_str(), "page", "", 0);
            size_t index = GfParmGetNum(hparmItems, strSection.c_str(), "index", "", 0);

            if (page <= items.size())
                items.resize(page + 1);

            if (index >= ITEMS_PER_PAGE)
            {
                GfLogError("Invalid index %zu\n", index);
                continue;
            }

            attribute &att = items[page][index];
            std::string strIndex(std::to_string(index));

            att.labelId = getDynamicControlId(std::string("Label" + strIndex).c_str());
            att.editId = getDynamicControlId(std::string("Edit" + strIndex).c_str());
            att.comboId = getDynamicControlId(std::string("Combo" + strIndex).c_str());
            att.defaultLabelId = getDynamicControlId(std::string("DefaultLabel" + strIndex).c_str());
            att.type = GfParmGetStr(hparmItems, strSection.c_str(), "type", "");
            att.section = GfParmGetStr(hparmItems, strSection.c_str(), "section", "");

            if(att.section == "Front Anti-Roll Bar" && option5 == 1)
                att.section = "Front Anti-Roll Bar1";
            else if (att.section == "Front Anti-Roll Bar" && option5 >= 2)
                att.section = "Front Anti-Roll Bar2";

            if(att.section == "Rear Anti-Roll Bar" && option5 == 1)
                att.section = "Rear Anti-Roll Bar1";
            else if (att.section == "Rear Anti-Roll Bar" && option5 >= 2)
                att.section = "Rear Anti-Roll Bar2";

            if(att.section == "Front Wing" && option6 == 1)
                att.section = "Front Wing1";
            else if (att.section == "Front Wing" && option6 >= 2)
                att.section = "Front Wing2";

            if(att.section == "Rear Wing" && option6 == 1)
                att.section = "Rear Wing1";
            else if (att.section == "Rear Wing" && option6 >= 2)
                att.section = "Rear Wing2";

            if(att.section == "Gearbox/gears/r" && option2 == 1)
                att.section = "Gearbox1/gears/r";

            if(att.section == "Gearbox/gears/1" && option2 == 1)
                att.section = "Gearbox1/gears/1";

            if(att.section == "Gearbox/gears/2" && option2 == 1)
                att.section = "Gearbox1/gears/2";

            if(att.section == "Gearbox/gears/3" && option2 == 1)
                att.section = "Gearbox1/gears/3";

            if(att.section == "Gearbox/gears/4" && option2 == 1)
                att.section = "Gearbox1/gears/4";

            if(att.section == "Gearbox/gears/5" && option2 == 1)
                att.section = "Gearbox1/gears/5";

            if(att.section == "Gearbox/gears/6" && option2 == 1)
                att.section = "Gearbox1/gears/6";

            if(att.section == "Gearbox/gears/7" && option2 == 1)
                att.section = "Gearbox1/gears/7";

            if(att.section == "Gearbox/gears/8" && option2 == 1)
                att.section = "Gearbox1/gears/8";

            if(att.section == "Front Right Wheel" && option7 == 1)
                att.section = "Front Right Wheel1";

            if(att.section == "Front Left Wheel" && option7 == 1)
                att.section = "Front Left Wheel1";

            if(att.section == "Rear Right Wheel" && option7 == 1)
                att.section = "Rear Right Wheel1";

            if(att.section == "Rear Left Wheel" && option7 == 1)
                att.section = "Rear Left Wheel1";

            if(att.section == "Front Right Suspension" && option4 == 1)
                att.section = "Front Right Suspension1";
            else if(att.section == "Front Right Suspension" && option4 >= 2)
                att.section = "Front Right Suspension2";

            if(att.section == "Front Left Suspension" && option4 == 1)
                att.section = "Front Left Suspension1";
            else if(att.section == "Front Left Suspension" && option4 >= 2)
                att.section = "Front Left Suspension2";

            if(att.section == "Rear Right Suspension" && option4 == 1)
                att.section = "Rear Right Suspension1";
            else if(att.section == "Rear Right Suspension" && option4 >= 2)
                att.section = "Rear Right Suspension2";

            if(att.section == "Rear Left Suspension" && option4 == 1)
                att.section = "Rear Left Suspension1";
            else if(att.section == "Rear Left Suspension" && option4 >= 2)
                att.section = "Rear Left Suspension2";

            att.param = GfParmGetStr(hparmItems, strSection.c_str(), "param", "");
            att.label = GfParmGetStr(hparmItems, strSection.c_str(), "label", "");

            if (att.type == "edit")
            {
                att.units = GfParmGetStr(hparmItems, strSection.c_str(), "unit", "");
                att.precision = GfParmGetNum(hparmItems, strSection.c_str(), "precision", "", 0);

                // Read values from car.
                att.exists = GfParmGetNumWithLimits(hparmCar, att.section.c_str(), att.param.c_str(), att.units.c_str(),
                                                    &att.defaultValue, &att.minValue, &att.maxValue) == 0;

                // Read value from car setup if avaliable.
                if (hparmCarSetup)
                    att.value = GfParmGetNum(hparmCarSetup, att.section.c_str(), att.param.c_str(),
                                             att.units.c_str(), att.defaultValue);
                else
                    att.value = att.defaultValue;

                GfLogDebug("section: \"%s\" param: \"%s\" units: \"%s\" label: \"%s\" page: %zu "
                           "index: %zu precision: %d labelId: %d editId: %d defaultLabelId: %d "
                           "exists: %d min: %f default %f max: %f value: %f\n",
                           att.section.c_str(), att.param.c_str(), att.units.c_str(),
                           att.label.c_str(), page, index, att.precision, att.labelId, att.editId,
                           att.defaultLabelId, att.exists, att.minValue, att.defaultValue, att.maxValue, att.value);
            }
            else if (att.type == "combo")
            {
                att.defaultStrValue = GfParmGetStr(hparmCar, att.section.c_str(), att.param.c_str(), "");
                att.exists = !att.defaultStrValue.empty();
                att.in = GfParmGetStrIn(hparmCar, att.section.c_str(), att.param.c_str());

                if (hparmCarSetup)
                    att.strValue = GfParmGetStr(hparmCarSetup, att.section.c_str(), att.param.c_str(),
                                                att.defaultStrValue.c_str());
                else
                    att.strValue = att.defaultStrValue;

                GfLogDebug("section: \"%s\" param: \"%s\" label: \"%s\" page: %zu "
                           "index: %zu labelId: %d comboId: %d defaultLabelId: %d "
                           "exists: %d, in: %zu default: \"%s\" value: \"%s\"\n",
                           att.section.c_str(), att.param.c_str(), att.label.c_str(),
                           page, index, att.labelId, att.comboId, att.defaultLabelId,
                           att.exists, att.in.size(), att.defaultStrValue.c_str(),
                           att.strValue.c_str());
            }
        }

        // Save the control id for all items.
        for (size_t page = 0; page < items.size(); ++page)
        {
            for (size_t index = 0; index < ITEMS_PER_PAGE; ++index)
            {
                attribute &att = items[page][index];
                std::string strIndex(std::to_string(index));

                if (!att.labelId)
                    att.labelId = getDynamicControlId(std::string("Label" + strIndex).c_str());
                if (!att.editId)
                    att.editId = getDynamicControlId(std::string("Edit" + strIndex).c_str());
                if (!att.comboId)
                    att.comboId = getDynamicControlId(std::string("Combo" + strIndex).c_str());
                if (!att.defaultLabelId)
                    att.defaultLabelId = getDynamicControlId(std::string("DefaultLabel" + strIndex).c_str());

            }
        }

        // Close the XML file of the menu items.
        GfParmReleaseHandle(hparmItems);
    }

    // Close the XML file of the car.
    GfParmReleaseHandle(hparmCar);

    // Close the XML file of the car setup.
    if (hparmCarSetup)
        GfParmReleaseHandle(hparmCarSetup);
}

// Save car setup qualification to XML file.
void PitCarSetupMenu::storeSettings1()
{
    // Open the XML file of the car setup.
    std::ostringstream ossCarSetupFileName;
    std::string strCarId = strCar;
    std::string strTrackId = strTrack;
    std::string strName = strHumanName;
    ossCarSetupFileName << GfLocalDir() << "drivers/human/cars/" << strName << '/' << strCarId << "/qualif/" << strTrackId << PARAMEXT;
    void *hparmCarSetup = GfParmReadFile(ossCarSetupFileName.str(), GFPARM_RMODE_STD);

    if (!hparmCarSetup)
    {
        // Create the car setup file directory if it doesn't exist.
        std::string strDir = ossCarSetupFileName.str();
        strDir.resize(strDir.find_last_of('/'));
        if (!GfDirExists(strDir.c_str()))
            GfDirCreate(strDir.c_str());

        hparmCarSetup = GfParmReadFile(ossCarSetupFileName.str(),
                                       GFPARM_RMODE_STD | GFPARM_RMODE_CREAT | GFPARM_TEMPLATE);
        if (!hparmCarSetup)
        {
            GfLogError("Car Setup %s/%s (file %s not %s)\n",
                       strCar, strTrack,
                       ossCarSetupFileName.str().c_str(),
                       GfFileExists(ossCarSetupFileName.str().c_str()) ? "readable" : "found");
            return;
        }

        GfLogDebug("Created car setup file: %s\n", ossCarSetupFileName.str().c_str());
    }
    else
    {
        // Remove existing content.
        GfParmClean(hparmCarSetup);

        GfLogDebug("Opened car setup file: %s\n", ossCarSetupFileName.str().c_str());
    }

    // Store all items.
    for (size_t page = 0; page < items.size(); ++page)
    {
        for (size_t index = 0; index < ITEMS_PER_PAGE; ++index)
        {
            attribute &att = items[page][index];
            // Only write items that exist and have been changed.
            if (att.exists)
            {
                if (att.type == "edit")
                {
                    if (att.value != att.defaultValue)
                    {
                        GfParmSetNum(hparmCarSetup, att.section.c_str(), att.param.c_str(),
                                     att.units.c_str(), att.value, att.minValue, att.maxValue);
                    }
                }
                else if (att.type == "combo")
                {
                    if (att.strValue != att.defaultStrValue)
                    {
                        GfParmSetStrAndIn(hparmCarSetup, att.section.c_str(), att.param.c_str(),
                                          att.strValue.c_str(), att.in);
                    }
                }

                switch(page)
                {
                case 0:
                    setuppage0(index, att);
                    break;
                case 1:
                    setuppage1(index, att);
                    break;
                case 2:
                    setuppage2(index, att);
                    break;
                case 3:
                    setuppage3(index, att);
                    break;
                case 4:
                    setuppage4(index, att);
                    break;
                case 5:
                    setuppage5(index, att);
                    break;
                case 6:
                    setuppage6(index, att);
                    break;
                }
            }
        }
    }

    // Write the XML file of the car setup.
    GfParmWriteFile(NULL, hparmCarSetup, strCarId.c_str());

    // Close the XML file of the car setup.
    GfParmReleaseHandle(hparmCarSetup);
}

// Save car setup qualification to XML file.
void PitCarSetupMenu::storeSettings2()
{
    // Open the XML file of the car setup.
    std::ostringstream ossCarSetupFileName;
    std::string strCarId = strCar;
    std::string strTrackId = strTrack;
    std::string strName = strHumanName;
    ossCarSetupFileName << GfLocalDir() << "drivers/human/cars/" << strName << '/' << strCarId << "/race/" << strTrackId << PARAMEXT;
    void *hparmCarSetup = GfParmReadFile(ossCarSetupFileName.str(), GFPARM_RMODE_STD);

    if (!hparmCarSetup)
    {
        // Create the car setup file directory if it doesn't exist.
        std::string strDir = ossCarSetupFileName.str();
        strDir.resize(strDir.find_last_of('/'));
        if (!GfDirExists(strDir.c_str()))
            GfDirCreate(strDir.c_str());

        hparmCarSetup = GfParmReadFile(ossCarSetupFileName.str(),
                                       GFPARM_RMODE_STD | GFPARM_RMODE_CREAT | GFPARM_TEMPLATE);
        if (!hparmCarSetup)
        {
            GfLogError("Car Setup %s/%s (file %s not %s)\n",
                       strCar, strTrack, ossCarSetupFileName.str().c_str(),
                       GfFileExists(ossCarSetupFileName.str().c_str()) ? "readable" : "found");
            return;
        }

        GfLogDebug("Created car setup file: %s\n", ossCarSetupFileName.str().c_str());
    }
    else
    {
        // Remove existing content.
        GfParmClean(hparmCarSetup);

        GfLogDebug("Opened car setup file: %s\n", ossCarSetupFileName.str().c_str());
    }

    // Store all items.
    for (size_t page = 0; page < items.size(); ++page)
    {
        for (size_t index = 0; index < ITEMS_PER_PAGE; ++index)
        {
            attribute &att = items[page][index];
            // Only write items that exist and have been changed.
            if (att.exists)
            {
                if (att.type == "edit")
                {
                    if (att.value != att.defaultValue)
                    {
                        GfParmSetNum(hparmCarSetup, att.section.c_str(), att.param.c_str(),
                                     att.units.c_str(), att.value, att.minValue, att.maxValue);
                    }
                }
                else if (att.type == "combo")
                {
                    if (att.strValue != att.defaultStrValue)
                    {
                        GfParmSetStrAndIn(hparmCarSetup, att.section.c_str(), att.param.c_str(),
                                          att.strValue.c_str(), att.in);
                    }
                }

                switch(page)
                {
                case 0:
                    setuppage0(index, att);
                    break;
                case 1:
                    setuppage1(index, att);
                    break;
                case 2:
                    setuppage2(index, att);
                    break;
                case 3:
                    setuppage3(index, att);
                    break;
                case 4:
                    setuppage4(index, att);
                    break;
                case 5:
                    setuppage5(index, att);
                    break;
                case 6:
                    setuppage6(index, att);
                    break;
                }
            }
        }
    }

    // Write the XML file of the car setup.
    GfParmWriteFile(NULL, hparmCarSetup, strCarId.c_str());

    // Close the XML file of the car setup.
    GfParmReleaseHandle(hparmCarSetup);
}

PitCarSetupMenu::PitCarSetupMenu()
    : GfuiMenuScreen("pitcarsetupmenu.xml"),
    strCar(nullptr),
    strTrack(nullptr),
    strHumanName(nullptr),
    currentPage(0)
{
    // Initialize combo callback user data.
    for (size_t i = 0; i < ITEMS_PER_PAGE; ++i)
    {
        comboCallbackData[i].menu = this;
        comboCallbackData[i].index = i;
    }
}

PitCarSetupMenu::~PitCarSetupMenu()
{
    strCar = NULL;
    strTrack = NULL;
    strHumanName = NULL;
    setupCar = NULL;
    currentPage = 0;
}

void PitCarSetupMenu::initialize(tCarElt *car, const char *track, const char* name, void *prevmenu)
{
    strCar = car->_carName;
    strTrack = track;
    strHumanName = name;
    setupCar = car;
    setPreviousMenuHandle(prevmenu);

    GfLogDebug("Initializing Car Setup menu: \"%s\"\n", strCar);

    createMenu(NULL, this, onActivateCallback, NULL, (tfuiCallback)NULL, 1);

    openXMLDescriptor();
    
    createStaticControls();

    createLabelControl("CarNameLabel");

    // Create items.
    for (size_t index = 0; index < ITEMS_PER_PAGE; ++index)
    {
        std::string strIndex(std::to_string(index));
        createLabelControl(std::string("Label" + strIndex).c_str());
        createEditControl(std::string("Edit" + strIndex).c_str(), this, NULL, NULL);
        createComboboxControl(std::string("Combo" + strIndex).c_str(), &comboCallbackData[index], onComboCallback);
        createLabelControl(std::string("DefaultLabel" + strIndex).c_str());
    }

    // Create buttons.
    createButtonControl("QualifButton", this, onQualifCallback);
    createButtonControl("RaceButton", this, onRaceCallback);
    createButtonControl("CancelButton", this, onCancelCallback);
    createButtonControl("ResetButton", this, onResetCallback);
    createButtonControl("PreviousButton", this, onPreviousCallback);
    createButtonControl("NextButton", this, onNextCallback);

    closeXMLDescriptor();

    // Keyboard shortcuts.
    addDefaultShortcuts();
    addShortcut(GFUIK_ESCAPE, "Cancel", this, onCancelCallback, NULL);
    addShortcut(GFUIK_SPACE, "Qualif", this, onQualifCallback, NULL);
    addShortcut(GFUIK_RETURN, "Race", this, onRaceCallback, NULL);
}
