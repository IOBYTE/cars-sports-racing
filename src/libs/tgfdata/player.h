/***************************************************************************

    file                 : players.h
    created              : Wen Dec 27 19:00:00 CET 2023
    copyright            : (C) 2023 by Xavier BERTAUX
    web                  : cars-sports-racing.sourceforge.net
    version              : $Id: players.h 3893 2023-12-27 15:52:42Z shadow72 $

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
		Singleton holding information on the available drivers
    @defgroup	tgfdata	Data manager for the client gaming framework.
*/

#ifndef __TGFPLAYERS_H__
#define __TGFPLAYERS_H__

#include <string>
#include <vector>

#include "tgfdata.h"

class GfCar;

class TGFDATA_API GfPlayerTeamCar
{
public:

    GfPlayerTeamCar(const std::string& strName = "");
	
public:
	int getTargets() const;
	const std::string& getName() const;
	const std::string& getCarPreviewFileName() const;

    void setTargets(int bfTargets);          // Overwrite any previous target.
    void addTargets(int nTargets);           // Bit-or.
	void setName(const std::string& strName);
	void setCarPreviewFileName(const std::string& strFileName);

protected:
    int          _bfTargets;                // Skin targets bit-field (see car.h for possible values)
    std::string  _strName;                  // Skin name (empty if standard skin)
    std::string  _strCarPreviewFileName;    // Car preview for this skin name (empty if none)
};

class TGFDATA_API GfPlayer
{
public:
    GfPlayer(void *hparmPlayer, int itfIndex);

    void load(void* hparmhuman, int itfIndex);
	
    const std::string&      getName() const;
    const std::string&      getShortName() const;
    const std::string&      getCodeName() const;
    const std::string&      getNationality() const;
    unsigned int            getRanking() const;
    void                    setRanking( unsigned int ranking);
    bool                    getActiveProfile() const;
    void                    setActiveProfile(bool profile);
    const GfCar*            getCar() const;
    const std::string&      getCategoryName() const;
    const GfPlayerTeamCar&  getTeamCar() const;
    const GfCar*            getCategory() const;

    bool                    matchesTypeAndCategory(const std::string& strCarCatId) const;
	
    double                  getSkillLevel() const;
    void                    setSkilllevel(double skill);

    void setCar(const GfCar* pCar);
    void setTeamCar(const GfPlayerTeamCar& tc);

	//! Get the possible skins using the same search path as the graphics engine.
    std::vector<GfPlayerTeamCar> getPossibleTeamCar(const std::string& strAltCarId = "") const;

	//! Retrieve the skin with given name is the given list
    static std::vector<GfPlayerTeamCar>::iterator findTeamCar(std::vector<GfPlayerTeamCar>& vecTeamCar,
														const std::string& strName);

protected:
	//! Get the possible skins in a given folder.
	void getPossibleSkinsInFolder(const std::string& strCarId,
								  const std::string& strFolderPath,
                                  std::vector<GfPlayerTeamCar>& vecPossTeamCar) const;
protected:
    std::string     _strName;      // User friendly name (ex: "Yuuki Kyousou").
    std::string     _shortName;    // Short Name player
    std::string     _codeName;     // code name player
    std::string     _nationality;  // Nationality player
    unsigned int    _rank;         // ranking player
    double          _Skilllevel;   // skilllevel player
    bool            _activeProfile;// active profile
    const GfCar*    _pCar;         // Car
    GfPlayerTeamCar _teamcar;      // Skin
};

class TGFDATA_API GfPlayers
{
public:
    // Accessor to the unique instance of the singleton.
    static GfPlayers* self();
    static void shutdown();

    // Reload drivers data from files.
    void      reload();

    unsigned  getCount() const;
    //unsigned int getActiveProfile() const;
    const std::vector<std::string>& getTypes() const;

    GfPlayer* get(const std::string& strModName, int nItfIndex) const;
    GfPlayer* getDriverWithName(const std::string& strName) const;
    GfPlayer* getDriverWithProfile(bool profile) const;
    GfPlayer* getActiveProfile() const;

    std::vector<GfPlayer*> getPlayersWithTypeAndCategory(const std::string& strCarCatId = "") const;

    void print() const;

protected:
    // Protected constructor and destructor : clients can not use them.
    GfPlayers();
    ~GfPlayers();

    // Clear all data.
    void clear();

protected:
    // The singleton itself.
    static GfPlayers* _pSelf;

    // Its private data.
    class Private;
    Private* _pPrivate;
};
#endif /* __TGFPLAYERS_H__ */
