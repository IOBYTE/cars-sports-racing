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
#include <deque>

#include <tgfclient.h>

#include "robot.h"
#include "racescreens.h"
#include "mainmenu.h"

static const char* selectedcar;
static std::string car[36];

static void *hParmHuman = NULL;
static void *hParmGarage = NULL;
static void *RmGarageSelectMenuHandle = NULL;

static int Button1Id;
static int Button2Id;
static int Button3Id;
static int Button4Id;
static int Button5Id;
static int Button6Id;
static int Button7Id;
static int Button8Id;
static int Button9Id;
static int Button10Id;
static int Button11Id;
static int Button12Id;
static int Button13Id;
static int Button14Id;
static int Button15Id;
static int Button16Id;
static int Button17Id;
static int Button18Id;
static int Button19Id;
static int Button20Id;
static int Button21Id;
static int Button22Id;
static int Button23Id;
static int Button24Id;
static int Button25Id;
static int Button26Id;
static int Button27Id;
static int Button28Id;
static int Button29Id;
static int Button30Id;
static int Button31Id;
static int Button32Id;
static int Button33Id;
static int Button34Id;
static int Button35Id;
static int maxcars;

/* Called when the menu is activated */
static void
rmOnActivate(void * /* dummy */)
{
    GfLogTrace("Entering Race Mode Select menu\n");
}

static void
onCancelCB(void * CiSmenu)
{
    GfuiScreenActivate(MainMenuInit(true));
}

static void
onValidate(void)
{
    const char *strPlayersFileName = "drivers/human/human.xml";
    hParmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
    GfLogDebug("Path Human.xml = %s\n", strPlayersFileName);

    if (!hParmHuman)
    {
        hParmHuman = GfParmReadFile(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
        GfLogDebug("Human not found in Local dir\n");
    }

    if (!hParmHuman)
        GfLogError("No usable human driver (human.xml not found or not readable)\n");

    if(hParmHuman)
    {
        char sstring[128];
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetStr(hParmHuman, sstring, ROB_ATTR_CAR_CAREER, selectedcar);
        GfParmWriteFile(0, hParmHuman, "human");
        GfParmReleaseHandle(hParmHuman);
        GfLogDebug("Human writing !\n");
        hParmHuman = NULL;
    }

    GfuiScreenActivate(MainMenuInit(true));
}

static void
rmOnSelectCarButton1(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[1].c_str();
    GfLogDebug("Selected Car button 1 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton2(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[2].c_str();
    GfLogDebug("Selected Car button 2 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton3(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[3].c_str();
    GfLogDebug("Selected Car button 3 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton4(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[4].c_str();
    GfLogDebug("Selected Car button 4 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton5(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[5].c_str();
    GfLogDebug("Selected Car button 5 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton6(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[6].c_str();
    GfLogDebug("Selected Car button 6 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton7(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[7].c_str();
    GfLogDebug("Selected Car button 7 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton8(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[8].c_str();
    GfLogDebug("Selected Car button 8 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton9(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[9].c_str();
    GfLogDebug("Selected Car button 9 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton10(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[10].c_str();
    GfLogDebug("Selected Car button 10 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton11(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[11].c_str();
    GfLogDebug("Selected Car button 11 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton12(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[12].c_str();
    GfLogDebug("Selected Car button 12 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton13(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[13].c_str();
    GfLogDebug("Selected Car button 13 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton14(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[14].c_str();
    GfLogDebug("Selected Car button 14 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton15(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[15].c_str();
    GfLogDebug("Selected Car button 15 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton16(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[16].c_str();
    GfLogDebug("Selected Car button 16 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton17(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[17].c_str();
    GfLogDebug("Selected Car button 17 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton18(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[18].c_str();
    GfLogDebug("Selected Car button 18 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton19(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[19].c_str();
    GfLogDebug("Selected Car button 19 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton20(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[20].c_str();
    GfLogDebug("Selected Car button 20 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton21(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[21].c_str();
    GfLogDebug("Selected Car button 21 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton22(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[22].c_str();
    GfLogDebug("Selected Car button 22 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton23(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[23].c_str();
    GfLogDebug("Selected Car button 23 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton24(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[24].c_str();
    GfLogDebug("Selected Car button 24 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton25(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[25].c_str();
    GfLogDebug("Selected Car button 25 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton26(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[26].c_str();
    GfLogDebug("Selected Car button 26 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton27(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[27].c_str();
    GfLogDebug("Selected Car button 27 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton28(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[28].c_str();
    GfLogDebug("Selected Car button 28 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton29(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[29].c_str();
    GfLogDebug("Selected Car button 29 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton30(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[30].c_str();
    GfLogDebug("Selected Car button 30 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton31(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[31].c_str();
    GfLogDebug("Selected Car button 31 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton32(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[32].c_str();
    GfLogDebug("Selected Car button 32 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton33(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[33].c_str();
    GfLogDebug("Selected Car button 33 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton34(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[34].c_str();
    GfLogDebug("Selected Car button 34 = %s\n", selectedcar);
    onValidate();
}

static void
rmOnSelectCarButton35(void *pCarsInStockMenu)
{
    GfLogDebug("Start Accepted car function\n");

    selectedcar = car[35].c_str();
    GfLogDebug("Selected Car button 35 = %s\n", selectedcar);
    onValidate();
}

void*
RmGarageSelectInit(void *prevMenu)
{
    GfLogDebug("Start Cars in Stock menu\n");
	// Create the menu and all its controls.
    if (RmGarageSelectMenuHandle)
    {
        RmGarageSelectMenuHandle = NULL;
    }

    char sstring[128];
    std::string defaultbutton;
    std::string disablebutton;
    std::string focusbutton;
    std::string pushbutton;

    int i = 0;
    const char *carname;

    /* Load garage settings from garage.xml file */
    hParmGarage = GfParmReadFileLocal("drivers/human/garage.xml", GFPARM_RMODE_REREAD);

    if (hParmGarage == NULL)
    {
        GfLogError("Error in Garage, file not found !!!\n");
    }

    // Create screen, load menu XML descriptor and create static controls.
    RmGarageSelectMenuHandle = GfuiScreenCreate((float*)NULL,
                                            NULL, rmOnActivate,
                                            NULL, (tfuiCallback)NULL,
                                            1);

    void *hmenu = GfuiMenuLoad("carsinstockmenu.xml");
    GfuiMenuCreateStaticControls(RmGarageSelectMenuHandle, hmenu);

    std::string garagename ="";

    GfLogDebug("Created screen Cars in Stock passed\n");
    snprintf(sstring, sizeof(sstring), "%s/%s/%d", "cars", ROB_LIST_INDEX, i + 1);

    for (i = 0; i < 35; i++)
    {        
        garagename = "car name "+std::to_string(i+1);
        carname = GfParmGetStr(hParmGarage, sstring, garagename.c_str(), "");
        GfLogDebug("Car Name in garage = %s\n", carname);

        if (strlen(carname) == 0)
        {
            GfLogDebug("Car name == 0\n");
            break; // Exit at end of driver list.
        }
        else
        {
            std::string cname = carname;
            maxcars = i+1;
            car[i+1] = carname;
            disablebutton = ("data/img/bg-nocar-disabled.png");
            defaultbutton = "data/img/bg-"+cname+".png";
            focusbutton = "data/img/bg-"+cname+"-focused.png";
            pushbutton = "data/img/bg-"+cname+"-pushed.png";
            GfLogDebug("Created string for buttons passed - Enable button = %s\n", defaultbutton.c_str());

            switch (i + 1)
            {
            case 1:
                GfLogDebug("Menu Create Dynamic Image Button passed !\n");
                Button1Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car1button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton1);
                //selectedcar = cname.c_str();
                GfLogDebug("Menu Create Dynamic Image Button passed !\n");
                break;
            case 2:
                Button2Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car2button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton2);
                break;
            case 3:
                Button3Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car3button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton3);
                break;
            case 4:
                GfLogDebug("Menu Create Dynamic Image Button passed !\n");
                Button4Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car4button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton4);
                //selectedcar = cname.c_str();
                break;
            case 5:
                Button5Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car5button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton5);
                break;
            case 6:
                Button6Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car6button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton6);
                break;
            case 7:
                Button7Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car7button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton7);
                //selectedcar = cname.c_str();
                GfLogDebug("Menu Create Dynamic Image Button passed !\n");
                break;
            case 8:
                Button8Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car8button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton8);
                break;
            case 9:
                Button9Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car9button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton9);
                break;
            case 10:
                Button10Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car10button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton10);
                //selectedcar = cname.c_str();
                break;
            case 11:
                Button11Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car11button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton11);
                break;
            case 12:
                Button12Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car12button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton12);
                break;
            case 13:
                Button13Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car13button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton13);
                //selectedcar = cname.c_str();
                GfLogDebug("Menu Create Dynamic Image Button passed !\n");
                break;
            case 14:
                Button14Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car14button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton14);
                break;
            case 15:
                Button15Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car15button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton15);
                break;
            case 16:
                Button16Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car16button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton16);
                //selectedcar = cname.c_str();
                break;
            case 17:
                Button17Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car17button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton17);
                break;
            case 18:
                Button18Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car18button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton18);
                break;
            case 19:
                Button19Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car19button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton19);
                //selectedcar = cname.c_str();
                GfLogDebug("Menu Create Dynamic Image Button passed !\n");
                break;
            case 20:
                Button20Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car20button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton20);
                break;
            case 21:
                Button21Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car21button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton21);
                break;
            case 22:
                Button22Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car22button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton22);
                //selectedcar = cname.c_str();
                break;
            case 23:
                Button23Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car23button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton23);
                break;
            case 24:
                Button24Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car24button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton24);
                break;
            case 25:
                Button25Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car25button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton25);
                //selectedcar = cname.c_str();
                GfLogDebug("Menu Create Dynamic Image Button passed !\n");
                break;
            case 26:
                Button26Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car26button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton26);
                break;
            case 27:
                Button27Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car27button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton27);
                break;
            case 28:
                Button28Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car28button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton28);
                //selectedcar = cname.c_str();
                break;
            case 29:
                Button29Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car29button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton29);
                break;
            case 30:
                Button30Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car30button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton30);
                break;
            case 31:
                Button31Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car31button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton31);
                //selectedcar = cname.c_str();
                GfLogDebug("Menu Create Dynamic Image Button passed !\n");
                break;
            case 32:
                Button32Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car32button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton32);
                break;
            case 33:
                Button33Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car33button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton33);
                break;
            case 34:
                Button34Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car34button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton34);
                //selectedcar = cname.c_str();
                break;
            case 35:
                Button35Id = GfuiMenuCreateDynamicButtonControl(
                            RmGarageSelectMenuHandle, hmenu, "car35button", disablebutton.c_str(),
                            defaultbutton.c_str(), focusbutton.c_str(),
                            pushbutton.c_str(), cname.c_str(), NULL, rmOnSelectCarButton35);
                break;
            }
        }
    }

    for (i = maxcars+1; i < 36; i++)
    {
        switch (i)
        {
        case 2:
            Button2Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car2button",
                                                            NULL, rmOnSelectCarButton2);
            GfuiEnable(RmGarageSelectMenuHandle, Button2Id, GFUI_DISABLE);
            break;
        case 3:
            Button3Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car3button",
                                                            NULL, rmOnSelectCarButton3);
            GfuiEnable(RmGarageSelectMenuHandle, Button3Id, GFUI_DISABLE);
            break;
        case 4:
            Button4Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car4button",
                                                            NULL, rmOnSelectCarButton4);
            GfuiEnable(RmGarageSelectMenuHandle, Button4Id, GFUI_DISABLE);
            break;
        case 5:
            Button5Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car5button",
                                                            NULL, rmOnSelectCarButton5);
            GfuiEnable(RmGarageSelectMenuHandle, Button5Id, GFUI_DISABLE);
            break;
        case 6:
            Button6Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car6button",
                                                            NULL, rmOnSelectCarButton6);
            GfuiEnable(RmGarageSelectMenuHandle, Button6Id, GFUI_DISABLE);
            break;
        case 7:
            Button7Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car7button",
                                                            NULL, rmOnSelectCarButton7);
            GfuiEnable(RmGarageSelectMenuHandle, Button7Id, GFUI_DISABLE);
            break;
        case 8:
            Button8Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car8button",
                                                            NULL, rmOnSelectCarButton8);
            GfuiEnable(RmGarageSelectMenuHandle, Button8Id, GFUI_DISABLE);
            break;
        case 9:
            Button9Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car9button",
                                                            NULL, rmOnSelectCarButton9);
            GfuiEnable(RmGarageSelectMenuHandle, Button9Id, GFUI_DISABLE);
            break;
        case 10:
            Button10Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car10button",
                                                            NULL, rmOnSelectCarButton10);
            GfuiEnable(RmGarageSelectMenuHandle, Button10Id, GFUI_DISABLE);
            break;
        case 11:
            Button11Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car11button",
                                                            NULL, rmOnSelectCarButton11);
            GfuiEnable(RmGarageSelectMenuHandle, Button11Id, GFUI_DISABLE);
            break;
        case 12:
            Button12Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car12button",
                                                            NULL, rmOnSelectCarButton12);
            GfuiEnable(RmGarageSelectMenuHandle, Button12Id, GFUI_DISABLE);
            break;
        case 13:
            Button13Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car13button",
                                                            NULL, rmOnSelectCarButton13);
            GfuiEnable(RmGarageSelectMenuHandle, Button13Id, GFUI_DISABLE);
            break;
        case 14:
            Button14Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car14button",
                                                            NULL, rmOnSelectCarButton14);
            GfuiEnable(RmGarageSelectMenuHandle, Button14Id, GFUI_DISABLE);
            break;
        case 15:
            Button15Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car15button",
                                                            NULL, rmOnSelectCarButton15);
            GfuiEnable(RmGarageSelectMenuHandle, Button15Id, GFUI_DISABLE);
            break;
        case 16:
            Button16Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car16button",
                                                            NULL, rmOnSelectCarButton16);
            GfuiEnable(RmGarageSelectMenuHandle, Button16Id, GFUI_DISABLE);
            break;
        case 17:
            Button17Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car17button",
                                                            NULL, rmOnSelectCarButton17);
            GfuiEnable(RmGarageSelectMenuHandle, Button17Id, GFUI_DISABLE);
            break;
        case 18:
            Button18Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car18button",
                                                            NULL, rmOnSelectCarButton18);
            GfuiEnable(RmGarageSelectMenuHandle, Button18Id, GFUI_DISABLE);
            break;
        case 19:
            Button19Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car19button",
                                                            NULL, rmOnSelectCarButton19);
            GfuiEnable(RmGarageSelectMenuHandle, Button19Id, GFUI_DISABLE);
            break;
        case 20:
            Button20Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car20button",
                                                            NULL, rmOnSelectCarButton20);
            GfuiEnable(RmGarageSelectMenuHandle, Button20Id, GFUI_DISABLE);
            break;
        case 21:
            Button21Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car21button",
                                                            NULL, rmOnSelectCarButton21);
            GfuiEnable(RmGarageSelectMenuHandle, Button21Id, GFUI_DISABLE);
            break;
        case 22:
            Button22Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car22button",
                                                            NULL, rmOnSelectCarButton22);
            GfuiEnable(RmGarageSelectMenuHandle, Button22Id, GFUI_DISABLE);
            break;
        case 23:
            Button23Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car23button",
                                                            NULL, rmOnSelectCarButton23);
            GfuiEnable(RmGarageSelectMenuHandle, Button23Id, GFUI_DISABLE);
            break;
        case 24:
            Button24Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car24button",
                                                            NULL, rmOnSelectCarButton24);
            GfuiEnable(RmGarageSelectMenuHandle, Button24Id, GFUI_DISABLE);
            break;
        case 25:
            Button25Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car25button",
                                                            NULL, rmOnSelectCarButton25);
            GfuiEnable(RmGarageSelectMenuHandle, Button25Id, GFUI_DISABLE);
            break;
        case 26:
            Button26Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car26button",
                                                            NULL, rmOnSelectCarButton26);
            GfuiEnable(RmGarageSelectMenuHandle, Button26Id, GFUI_DISABLE);
            break;
        case 27:
            Button27Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car27button",
                                                            NULL, rmOnSelectCarButton27);
            GfuiEnable(RmGarageSelectMenuHandle, Button27Id, GFUI_DISABLE);
            break;
        case 28:
            Button28Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car28button",
                                                            NULL, rmOnSelectCarButton28);
            GfuiEnable(RmGarageSelectMenuHandle, Button28Id, GFUI_DISABLE);
            break;
        case 29:
            Button29Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car29button",
                                                            NULL, rmOnSelectCarButton29);
            GfuiEnable(RmGarageSelectMenuHandle, Button29Id, GFUI_DISABLE);
            break;
        case 30:
            Button30Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car30button",
                                                            NULL, rmOnSelectCarButton30);
            GfuiEnable(RmGarageSelectMenuHandle, Button30Id, GFUI_DISABLE);
            break;
        case 31:
            Button31Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car31button",
                                                            NULL, rmOnSelectCarButton31);
            GfuiEnable(RmGarageSelectMenuHandle, Button31Id, GFUI_DISABLE);
            break;
        case 32:
            Button32Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car32button",
                                                            NULL, rmOnSelectCarButton32);
            GfuiEnable(RmGarageSelectMenuHandle, Button32Id, GFUI_DISABLE);
            break;
        case 33:
            Button33Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car33button",
                                                            NULL, rmOnSelectCarButton33);
            GfuiEnable(RmGarageSelectMenuHandle, Button33Id, GFUI_DISABLE);
            break;
        case 34:
            Button34Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car34button",
                                                            NULL, rmOnSelectCarButton34);
            GfuiEnable(RmGarageSelectMenuHandle, Button34Id, GFUI_DISABLE);
            break;
        case 35:
            Button35Id = GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "car35button",
                                                            NULL, rmOnSelectCarButton35);
            GfuiEnable(RmGarageSelectMenuHandle, Button35Id, GFUI_DISABLE);
            break;
        }
    }

    // Create Back button
    GfuiMenuCreateButtonControl(RmGarageSelectMenuHandle, hmenu, "BackButton",
                                prevMenu, onCancelCB);

    // Close menu XML descriptor.
    GfParmReleaseHandle(hmenu);

    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(RmGarageSelectMenuHandle);
    GfuiAddKey(RmGarageSelectMenuHandle, GFUIK_ESCAPE, "Back To Career Menu",
               prevMenu, onCancelCB, NULL);

    return RmGarageSelectMenuHandle;
}
