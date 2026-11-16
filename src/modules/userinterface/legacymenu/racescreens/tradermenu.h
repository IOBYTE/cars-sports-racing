/***************************************************************************

    file                 : tradermenu.h
    created              : Fri Aug 16 CET 2024
    copyright            : (C) 2024 Xavier Bertaux
    web                  : www.cars-sports-racing.net
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

#ifndef _TRADERMENU_H_
#define _TRADERMENU_H_

#include <map>

#include <tgfclient.h>

class GfCar;

class RmTraderMenu : public GfuiMenuScreen
{
public:
	RmTraderMenu();
    void runMenu(const char *defaultcar);
	
	std::string resetCarCategoryComboBox(const std::string& strSelCatName = "");
	GfCar* resetCarModelComboBox(const std::string& strCatName,
								 const std::string& strSelCarName = "");
	void resetCarDataSheet(const std::string& strSelCarId);
	void resetSkinComboBox(const std::string& strCarName,
                           const std::string& strPreview);
    void resetCarPreviewImage(const std::string& strPreview);

protected:	
	bool initialize();

	const GfCar* getSelectedCarModel() const;
    //const GfDriverSkin& getSelectedSkin() const;

	// Control callback functions (have to be static, as used as tgfclient controls callbacks).
	static void onActivateCB(void *pGarageMenu);
	static void onChangeCategory(tComboBoxInfo *pInfo);
	static void onChangeModel(tComboBoxInfo *pInfo);
	static void onAcceptCB(void *pGarageMenu);
	static void onCancelCB(void *pGarageMenu);
};

#endif //_TRADERMENU_H_
