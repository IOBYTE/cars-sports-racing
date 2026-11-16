/***************************************************************************

    file                 : carsinstockmenu.cpp
    created              : Fri Aug 2024
    copyright            : (C) 2024 Xavier Bertaux
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

#include <sys/stat.h>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>

#include <tgfclient.h>

#include <race.h>
#include <cars.h>
#include <drivers.h>

#include "robot.h"
#include "tradermenu.h"
#include "racescreens.h"
#include "mainmenu.h"

static int BuyButtonId;
static std::string cars[35];
static const char* selectedcar;
static unsigned long int SelectedPrice;
static int maxcarsingarage;
static void *hParmHuman = NULL;
static void *hParmGarage = NULL;

void RmTraderMenu::onActivateCB(void *pTraderMenu)
{
	GfLogTrace("Entering Trader menu\n");

    // Get the RmTraderMenu instance.
	RmTraderMenu* pMenu = static_cast<RmTraderMenu*>(pTraderMenu);

	// Get infos about the current car for the current driver
    //const GfDriver* pDriver = pMenu->getDriver();
    const GfCar* pCurCar = GfCars::self()->getCar("sc-deckard-conejo");

	// Initialize the GUI contents.
    std::ostringstream ossSpecValue;
    ossSpecValue << (unsigned long int)rmCredit;
	GfuiLabelSetText(pMenu->getMenuHandle(), pMenu->getDynamicControlId("CreditLabel"),
                     ossSpecValue.str().c_str());
	const std::string strSelCatName = pMenu->resetCarCategoryComboBox(pCurCar->getCategoryName());
	pCurCar = pMenu->resetCarModelComboBox(strSelCatName, pCurCar->getName());
	pMenu->resetCarDataSheet(pCurCar->getId());
    const std::string path = "cars/models/"+pCurCar->getId()+"/"+pCurCar->getId()+"-preview.jpg";
    pMenu->resetCarPreviewImage(path);
    selectedcar = pCurCar->getId().c_str();
    SelectedPrice = pCurCar->getPrice();

    const char *strGarageFileName = "drivers/human/garage.xml";

    hParmGarage = GfParmReadFileLocal(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
    GfLogDebug("Path garage.xml = %s\n", strGarageFileName);

    if (!hParmGarage)
    {
        hParmGarage = GfParmReadFile(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
    }

    if (!hParmGarage)
        GfLogError("No usable Garage (garage.xml not found or not readable)\n");

    if(hParmGarage)
    {
        char sstring[128];
        const char *car;
        std::string garagename ="";

        for (int i = 0; i < 35; i++)
        {
            snprintf(sstring, sizeof(sstring), "%s/%s/%d", "cars", "index", 1);
            garagename = "car name "+ std::to_string(i+1);
            car = GfParmGetStr(hParmGarage, sstring, garagename.c_str(), "");
            GfLogDebug("Reading car in garage = %s\n", car);

            if (strlen(car) == 0 || strcmp(car, "none") == 0)
            {
                maxcarsingarage = i;
                GfParmReleaseHandle(hParmGarage);
                GfLogDebug("Garage end reading !\n");
                hParmGarage = NULL;
                break;
            }
            else
            {
                cars[i+1] = car;
                maxcarsingarage = i+1;
                GfLogDebug("Car [%i] = %s - Max Cars = %i\n", i+1, cars[i+1].c_str(), maxcarsingarage);
            }
        }
    }

    GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_ENABLE);

    if((unsigned long int)pCurCar->getPrice() > rmCredit)
        GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_DISABLE);
    else if (maxcarsingarage > 0)
    {
        for (int i = 0 ; i < maxcarsingarage; i++)
        {
            if((pCurCar->getId() == cars[i + 1]))
            {
                GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_DISABLE);
                break;
            }
        }
    }
}

const GfCar* RmTraderMenu::getSelectedCarModel() const
{
	const char* pszSelCarName =
		GfuiComboboxGetText(getMenuHandle(), getDynamicControlId("ModelCombo"));

	if (pszSelCarName)
		return GfCars::self()->getCarWithName(pszSelCarName);

	return 0;
}

void RmTraderMenu::onChangeCategory(tComboBoxInfo *pInfo)
{
	// Get the RmGarageMenu instance from call-back user data.
    RmTraderMenu* pMenu = static_cast<RmTraderMenu*>(pInfo->userData);

	// Update GUI.
	const GfCar* pSelCar = pMenu->resetCarModelComboBox(pInfo->vecChoices[pInfo->nPos]);
	pMenu->resetCarDataSheet(pSelCar->getId());
    const std::string path = "cars/models/"+pSelCar->getId()+"/"+pSelCar->getId()+"-preview.jpg";
    pMenu->resetCarPreviewImage(path);
    selectedcar = pSelCar->getId().c_str();
    SelectedPrice = pSelCar->getPrice();

    GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_ENABLE);

    if((unsigned long int)pSelCar->getPrice() > rmCredit)
        GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_DISABLE);
    else if (maxcarsingarage > 0)
    {
        for (int i = 0 ; i < maxcarsingarage; i++)
        {
            if((pSelCar->getId() == cars[i + 1]))
            {
                GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_DISABLE);
                break;
            }
        }
    }
}

void RmTraderMenu::onChangeModel(tComboBoxInfo *pInfo)
{
	// Get the RmGarageMenu instance from call-back user data.
    RmTraderMenu* pMenu = static_cast<RmTraderMenu*>(pInfo->userData);

	// Update GUI.
	const GfCar* pSelCar = pMenu->getSelectedCarModel();
	pMenu->resetCarDataSheet(pSelCar->getId());
    const std::string path = "cars/models/"+pSelCar->getId()+"/"+pSelCar->getId()+"-preview.jpg";
    pMenu->resetCarPreviewImage(path);
    selectedcar = pSelCar->getId().c_str();
    SelectedPrice = pSelCar->getPrice();

    GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_ENABLE);

    if((unsigned long int)pSelCar->getPrice() > rmCredit)
        GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_DISABLE);
    else if(maxcarsingarage > 0)
    {
        for (int i = 0 ; i < maxcarsingarage; i++)
        {
            if((pSelCar->getId() == cars[i + 1]))
            {
                GfuiEnable(pMenu->getMenuHandle(), BuyButtonId, GFUI_DISABLE);
                break;
            }
        }
    }
}

void RmTraderMenu::onAcceptCB(void *pTraderMenu)
{
    GfLogDebug("Start Buy function\n");
    const char *strPlayersFileName = "drivers/human/human.xml";
    const char *strGarageFileName = "drivers/human/garage.xml";

    hParmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
    GfLogDebug("Path Human.xml = %s\n", strPlayersFileName);

    if (!hParmHuman)
    {
        hParmHuman = GfParmReadFile(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        GfLogDebug("Human not found in Local dir\n");
    }

    if (!hParmHuman)
        GfLogError("No usable human driver (human.xml not found or not readable)\n");

    hParmGarage = GfParmReadFileLocal(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
    GfLogDebug("Path garage.xml = %s\n", strGarageFileName);

    if (!hParmGarage)
    {
        hParmGarage = GfParmReadFile(strGarageFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
    }

    if (!hParmGarage)
        GfLogError("No usable Garage (garage.xml not found or not readable)\n");

    if(hParmGarage)
    {
        char sstring[128];
        GfLogDebug("Write Garage selected car = %s\n", selectedcar);
        std::string garagename;
        garagename = "car name "+std::to_string(maxcarsingarage + 1);
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "cars", "index", 1);
        GfParmSetStr(hParmGarage, sstring, garagename.c_str(), selectedcar);
        GfParmWriteFile(0, hParmGarage, "garage");
        GfParmReleaseHandle(hParmGarage);
        GfLogDebug("Garage writing !\n");
        //hParmGarage = NULL;
    }

    if(hParmHuman)
    {
        char sstring[128];
        rmCredit -= SelectedPrice;
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetStr(hParmHuman, sstring, ROB_ATTR_CAR_RACE, selectedcar);
        GfParmSetNum(hParmHuman, sstring, "credit", (char*)NULL, rmCredit);
        GfParmWriteFile(0, hParmHuman, "human");
        GfParmReleaseHandle(hParmHuman);
        GfLogDebug("Human writing ! - rmCredit = %lu - SelectedPrice = %lu\n", rmCredit, SelectedPrice);
        //hParmHuman = NULL;
    }
	
	// Back to previous screen.
    GfuiScreenActivate(MainMenuInit(true));
}

void RmTraderMenu::onCancelCB(void *pTraderMenu)
{
	// Back to previous screen.
    GfuiScreenActivate(MainMenuInit(true));
}

RmTraderMenu::RmTraderMenu()
: GfuiMenuScreen("tradermenu.xml")
{
}

std::string RmTraderMenu::resetCarCategoryComboBox(const std::string& strSelCatName)
{
	const int nCatComboId = getDynamicControlId("CategoryCombo");

	// Retrieve the available car categories.
	const std::vector<std::string>& vecCatNames = GfCars::self()->getCategoryNames();
    // Load the combo-box from their names (and determine the requested category index).
	unsigned nCurCatIndex = 0;
	GfuiComboboxClear(getMenuHandle(), nCatComboId);
	for (unsigned nCatIndex = 0; nCatIndex < vecCatNames.size(); nCatIndex++)
	{
        GfuiComboboxAddText(getMenuHandle(), nCatComboId, vecCatNames[nCatIndex].c_str());
        if (!strSelCatName.empty() && vecCatNames[nCatIndex] == strSelCatName)
            nCurCatIndex = nCatIndex;
	}
	
	// Select the requested category in the combo-box.
	GfuiComboboxSetSelectedIndex(getMenuHandle(), nCatComboId, nCurCatIndex);

	// Disable the combo-box for non human drivers (robot drivers can't change their car),
	// or if it contains only 1 category.
	GfuiEnable(getMenuHandle(), nCatComboId,
               true
			   && GfuiComboboxGetNumberOfChoices(getMenuHandle(), nCatComboId) > 1
			   ? GFUI_ENABLE : GFUI_DISABLE);
	
	return vecCatNames[nCurCatIndex];
}

GfCar* RmTraderMenu::resetCarModelComboBox(const std::string& strCatName,
											  const std::string& strSelCarName)
{
	const int nModelComboId = getDynamicControlId("ModelCombo");

	// Retrieve car models in the selected category.
	const std::vector<GfCar*> vecCarsInCat =	
		GfCars::self()->getCarsInCategoryWithName(strCatName);

	// Load the combo-box from their real names (and determine the selected model index).
	unsigned nCurrCarIndexInCat = 0;
	GfuiComboboxClear(getMenuHandle(), nModelComboId);
	for (unsigned nCarIndex = 0; nCarIndex < vecCarsInCat.size(); nCarIndex++)
	{
		GfuiComboboxAddText(getMenuHandle(), nModelComboId,
							vecCarsInCat[nCarIndex]->getName().c_str());
		if (!strSelCarName.empty() && vecCarsInCat[nCarIndex]->getName() == strSelCarName)
			nCurrCarIndexInCat = nCarIndex;
	}

	// Select the right car in the combo-box.
	GfuiComboboxSetSelectedIndex(getMenuHandle(), nModelComboId, nCurrCarIndexInCat);

	// Disable the combo-box for non human drivers (robot drivers can't change their car).
	// or if it contains only 1 model.
	GfuiEnable(getMenuHandle(), nModelComboId,
               true
			   && GfuiComboboxGetNumberOfChoices(getMenuHandle(), nModelComboId) > 1
			   ? GFUI_ENABLE : GFUI_DISABLE);

	// Return the actually selected car (may differ from the requested car).
	return vecCarsInCat[nCurrCarIndexInCat];
}

void RmTraderMenu::resetCarDataSheet(const std::string& strSelCarId)
{
	static const char* pszDriveWheels[3] =
		{ "Rear", "Front", "4" };
	static const char* pszEnginePosition[5] =
		{ "Front", "Front-mid", "Mid", "Rear-mid", "Rear" };
	static const char* pszEngineShape[4] =
		{ "V", "L", "H", "W" };
	
	// Retrieve selected car.
	const GfCar* pSelCar = GfCars::self()->getCar(strSelCarId);
	
	// Update GUI.
	std::ostringstream ossSpecValue;
	
	ossSpecValue << (long)pSelCar->getMass() << " kg ";
	const long nFRMassPercent = (long)(pSelCar->getFrontRearMassRatio() * 100);
	if (nFRMassPercent > 50)
		ossSpecValue << "(" << nFRMassPercent << "% front)";
	else
		ossSpecValue << "(" << 100 - nFRMassPercent << "% rear)";
	GfuiLabelSetText(getMenuHandle(), getDynamicControlId("MassLabel"),
					 ossSpecValue.str().c_str());

	ossSpecValue.str("");
	ossSpecValue << pszDriveWheels[pSelCar->getDriveTrain()] << " WD, "
				 << pSelCar->getGearsCount() << " gears";
	GfuiLabelSetText(getMenuHandle(), getDynamicControlId("DriveTrainLabel"),
					 ossSpecValue.str().c_str());

	ossSpecValue.str("");
	ossSpecValue << std::fixed << std::setprecision(0)
				 << pSelCar->getMaxPower() / 75 / G << " bhp ("
				 << pSelCar->getMaxPowerSpeed() * 30.0 / PI << " rpm)";
	GfuiLabelSetText(getMenuHandle(), getDynamicControlId("MaxPowerLabel"),
					 ossSpecValue.str().c_str());
	
	ossSpecValue.str("");
	ossSpecValue << pSelCar->getMaxTorque() << " N.m ("
				 << pSelCar->getMaxTorqueSpeed() * 30.0 / PI << " rpm)";
	GfuiLabelSetText(getMenuHandle(), getDynamicControlId("MaxTorqueLabel"),
					 ossSpecValue.str().c_str());

	ossSpecValue.str("");
	ossSpecValue << pszEnginePosition[pSelCar->getEnginePosition()] << ' ';
	if (pSelCar->getCylinders() > 1)
		ossSpecValue << pszEngineShape[pSelCar->getEngineShape()];
	if (pSelCar->getCylinders() > 0)
	{
		ossSpecValue << pSelCar->getCylinders() << " ";
		if (pSelCar->getCylinders() == 1)
			ossSpecValue << "cyl. ";
	}
	if (pSelCar->getEngineCapacity() > 0)
	{
		if (pSelCar->getEngineCapacity() >= 1)
			ossSpecValue << std::setprecision(1) << pSelCar->getEngineCapacity() << " l ";
		else
			ossSpecValue << (pSelCar->getEngineCapacity() * 1000.0) << " cc ";
	}
	if (pSelCar->isTurboCharged())
		ossSpecValue << "turbo";
	if (ossSpecValue.str().empty())
		ossSpecValue << "missing information";

	
	GfuiLabelSetText(getMenuHandle(), getDynamicControlId("EngineLabel"),
					 ossSpecValue.str().c_str());

    ossSpecValue.str("");
    ossSpecValue << pSelCar->getPrice();
    GfuiLabelSetText(getMenuHandle(), getDynamicControlId("PriceLabel"),
                     ossSpecValue.str().c_str());
	
	GfuiProgressbarSetValue(getMenuHandle(), getDynamicControlId("TopSpeedProgress"),
							pSelCar->getTopSpeed() * 3.6f);
    GfuiProgressbarSetValue(getMenuHandle(), getDynamicControlId("PowerMassRatioProgress"),
							pSelCar->getMaxPower() / 75 / G / pSelCar->getMass());
    GfuiProgressbarSetValue(getMenuHandle(), getDynamicControlId("LowSpeedGripProgress"),
							pSelCar->getLowSpeedGrip());
    GfuiProgressbarSetValue(getMenuHandle(), getDynamicControlId("HighSpeedGripProgress"),
							pSelCar->getHighSpeedGrip());
    GfuiProgressbarSetValue(getMenuHandle(), getDynamicControlId("CorneringProgress"),
							pSelCar->getInvertedZAxisInertia());
	
	GfLogDebug("%s : topSp=%.1f, powMass=%.2f, lowSpGrip=%.1f, highSpGrip=%.1f, 1/ZInertia=%.5f\n",
			   strSelCarId.c_str(),
			   pSelCar->getTopSpeed()*3.6f, pSelCar->getMaxPower() / 75 / G / pSelCar->getMass(),
			   pSelCar->getLowSpeedGrip(), pSelCar->getHighSpeedGrip(),
			   pSelCar->getInvertedZAxisInertia());
}

void RmTraderMenu::resetCarPreviewImage(const std::string& strPreview)
{
	const int nCarImageId = getDynamicControlId("PreviewImage");

	// Load the preview image.
    if (GfFileExists(strPreview.c_str()))
        GfuiStaticImageSet(getMenuHandle(), nCarImageId, strPreview.c_str());
	else
		GfuiStaticImageSet(getMenuHandle(), nCarImageId, "data/img/nocarpreview.png");
}

void RmTraderMenu::runMenu(const char* defaultcar)
{
	// Initialize if not already done.
    if (!getMenuHandle())
		initialize();

	// Normally expected job.
	GfuiMenuScreen::runMenu();
}

bool RmTraderMenu::initialize()
{
    cars[0] = "";
	// Create the menu and all its controls.
	createMenu(NULL, this, onActivateCB, NULL, (tfuiCallback)NULL, 1);

    openXMLDescriptor();
    
    createStaticControls();
    
    createLabelControl("CreditLabel");
	
	createComboboxControl("CategoryCombo", this, onChangeCategory);
	createComboboxControl("ModelCombo", this, onChangeModel);
	
	createStaticImageControl("PreviewImage");
	
	createLabelControl("DriveTrainLabel");
	createLabelControl("MaxPowerLabel");
	createLabelControl("MaxTorqueLabel");
	createLabelControl("MassLabel");
	createLabelControl("EngineLabel");
    createLabelControl("PriceLabel");
	
	createProgressbarControl("TopSpeedProgress");
	createProgressbarControl("PowerMassRatioProgress");
	createProgressbarControl("HighSpeedGripProgress");
	createProgressbarControl("LowSpeedGripProgress");
	createProgressbarControl("CorneringProgress");

    BuyButtonId = createButtonControl("BuyButton", this, onAcceptCB);
    createButtonControl("CancelButton", this, onCancelCB);

    closeXMLDescriptor();

    // Keyboard shortcuts.
    addDefaultShortcuts();
    addShortcut(GFUIK_ESCAPE, "Cancel", this, onCancelCB, NULL);
    addShortcut(GFUIK_RETURN, "Accept", this, onAcceptCB, NULL);
    selectedcar = "sc-deckard-conejo";

    return true;
}
