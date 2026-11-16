/***************************************************************************

    file                 : racepitsetupmenu.h
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

#ifndef _RACEPITSETUPMENU_H_
#define _RACEPITSETUPMENU_H_

#include <array>

#include <track.h>
#include <car.h>
#include <raceman.h>
#include <tgfclient.h>

#include "confscreens.h"
#include "raceman.h"

#define ITEMS_PER_PAGE  12

class PitCarSetupMenu : public GfuiMenuScreen
{
public:
    PitCarSetupMenu();

    virtual ~PitCarSetupMenu();

    void initialize(tCarElt *car, const char *track, const char *name, void *prevmenu);
    //void* runMenu();

    struct attribute
    {
        int         labelId;
        int         editId;
        int         comboId;
        int         defaultLabelId;
        bool        exists;
        tdble       value;
        tdble       minValue;
        tdble       defaultValue;
        tdble       maxValue;
        std::string type;
        std::string section;
        std::string param;
        std::string units;
        std::string label;
        std::string strValue;
        std::string defaultStrValue;
        std::vector<std::string> in;
        int         precision;

        attribute() :
            labelId(0), editId(0), comboId(0), defaultLabelId(0), exists(false),
            value(0), minValue(0), defaultValue(0), maxValue(0), precision(0)
        {
        }
    };

    struct ComboCallbackData
    {
        PitCarSetupMenu *menu;
        size_t index;
    };

    // callback functions must be static
    static void onActivateCallback(void *pMenu);
    static void onQualifCallback(void *pMenu);
    static void onRaceCallback(void *pMenu);
    static void onCancelCallback(void *pMenu);
    static void onResetCallback(void *pMenu);
    static void onPreviousCallback(void *pMenu);
    static void onNextCallback(void *pMenu);
    static void onComboCallback(tComboBoxInfo *pInfo);

private:
    void storeSettings1();
    void storeSettings2();
    void loadSettings();
    void updateControls();
    void readCurrentPage();
    void setuppage0(long unsigned i, attribute& att);
    void setuppage1(long unsigned i, attribute& att);
    void setuppage2(long unsigned i, attribute& att);
    void setuppage3(long unsigned i, attribute& att);
    void setuppage4(long unsigned i, attribute& att);
    void setuppage5(long unsigned i, attribute& att);
    void setuppage6(long unsigned i, attribute& att);
    void setuppage7(long unsigned i, attribute& att);
    void setuppage8(long unsigned i, attribute& att);
    void setuppage9(long unsigned i, attribute& att);
    void setuppage10(long unsigned i, attribute& att);

    void onActivate();
    void onAcceptQualif();
    void onAcceptRace();
    void onCancel() const;
    void onReset();
    void onPrevious();
    void onNext();
    void onCombo(tComboBoxInfo *pInfo);

    const char* strCar;
    const char* strTrack;
    const char* strHumanName;
    static tCarSetupItem* rmSetup;
    tCarElt* setupCar;

    std::vector<std::array<attribute, ITEMS_PER_PAGE> > items;
    size_t  currentPage;
    ComboCallbackData comboCallbackData[ITEMS_PER_PAGE];
};

#endif /* _RACEPITSETUPMENU_H_ */
