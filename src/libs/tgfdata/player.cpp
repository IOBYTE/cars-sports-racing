/***************************************************************************

    file                 : players.cpp
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

#include <cctype>
#include <map>
#include <sstream>
#include <algorithm>

#include <portability.h>

#include <playerpref.h>
#include <robot.h>

#include "cars.h"
#include "player.h"

// Number of drivers defined in robot's xml-file
static int NBPLAYERS = 0;      // Still unknown
static const int BUFSIZE = 256;

// Private data for GfPlayers
class GfPlayer::Private
{
public:

    // One GfDriver structure for each driver (order = sorted directory one).
    std::vector<GfPlayer*> vecPlayers;

    // Map for quick access to GfDriver by { module name, interface index }
    typedef std::map<std::pair<std::string, int>, GfPlayer*> TMapPlayersByKey;
    TMapPlayersByKey mapPlayersByKey;

    // Vector of driver types.
    std::vector<std::string> vecTypes;

    // Vector of driver car categories.
    std::vector<std::string> vecCarCategoryIds;
};

GfPlayer* GfPlayer::_pSelf = 0;

GfPlayer *GfPlayer::self()
{
    if (!_pSelf)
        _pSelf = new GfPlayers;

    return _pSelf;
}

void GfPlayer::shutdown()
{
    delete _pSelf;
    _pSelf = 0;
}

GfPlayer::~GfPlayer()
{
    clear();

    delete _pPrivate;
    _pPrivate = 0;
}

void GfPlayer::clear()
{
    _pPrivate->mapPlayersByKey.clear();
    _pPrivate->vecTypes.clear();
    _pPrivate->vecCarCategoryIds.clear();

    std::vector<GfPlayer*>::const_iterator itPlayer;

    for (itPlayer = _pPrivate->vecPlayers.begin();
         itPlayer != _pPrivate->vecPlayers.end(); ++itPlayer)
        delete *itPlayer;
    _pPrivate->vecPlayers.clear();
}

void GfPlayer::reload()
{
    GfLogInfo("Start GfPlayers reload\n");

    // Clear all.
    clear();

    // (Re)Load robot modules from the "drivers" installed folder.
    std::string strPlayersFileName(GfLocalDir());
    strPlayersFileName += "drivers/human/human.xml";

    void *hparmPlayer = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmPlayer)
        hparmPlayer = GfParmReadFile(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmPlayer)
    {
        GfLogError("No usable human driver (human.xml not found or not readable)\n");
    }

    if (hparmPlayer) // human settings XML could be read
    {
        NBPLAYERS = 0;
        GfLogInfo("GfPlayers reload read human.xml\n");
        char SectionBuffer[BUFSIZE];

        for (int i = 0; i < 10; ++i)
        {
            snprintf(SectionBuffer, BUFSIZE, "%s/%s/%d", ROB_SECT_ROBOTS, ROB_LIST_INDEX, i+1);
            const char *num = GfParmGetStr(hparmPlayer, SectionBuffer, ROB_ATTR_NAME, "");

            if(strlen(num) == 0)
            {
                break;
            }
            else
            {
                GfLogInfo("GfPlayer loader\n");
                GfPlayer* pPlayer = new GfPlayer( hparmPlayer, i+1);

                if (!pPlayer->getCar())
                {
                    GfCar* pSubstCar = GfCars::self()->getCarsInCategory()[0]; // Should never fail.
                }

                ++NBPLAYERS;
            }
        }  // for i

        GfParmReleaseHandle(hparmPlayer);
        hparmPlayer = 0;
    }



    // Sort the car category ids and driver types vectors.
    std::sort(_pPrivate->vecCarCategoryIds.begin(), _pPrivate->vecCarCategoryIds.end());
    std::sort(_pPrivate->vecTypes.begin(), _pPrivate->vecTypes.end());

    // Trace what we got.
    print();
}

GfPlayers::GfPlayers()
{
    _pPrivate = new GfPlayers::Private;

    reload();
}

unsigned GfPlayers::getCount() const
{
    return _pPrivate->vecPlayers.size();
}

const std::vector<std::string>& GfPlayers::getTypes() const
{
    return _pPrivate->vecTypes;
}

GfPlayer* GfPlayers::getActiveProfile() const
{
    /*std::map<bool pr, GfPlayer*>::const_iterator itPlayer =
        _pPrivate->mapPlayersByKey.find(pr);
    if (itPlayer != _pPrivate->mapPlayersByKey.end())
        return itPlayer->second;*/

    return 0;
}

std::vector<GfPlayer*> GfPlayers::getPlayersWithTypeAndCategory(const std::string& strCarCatId) const
{
    std::vector<GfPlayer*> vecSelPlayers;
    std::vector<GfPlayer*>::iterator itPlayer;
    for (itPlayer = _pPrivate->vecPlayers.begin();
         itPlayer != _pPrivate->vecPlayers.end(); ++itPlayer)
        if ((*itPlayer)->matchesTypeAndCategory(strCarCatId))
            vecSelPlayers.push_back(*itPlayer);

    return vecSelPlayers;
}

void GfPlayers::print() const
{
    GfLogInfo("Driver base : %zu types, %zu car categories, %zu drivers\n",
               _pPrivate->vecTypes.size(), _pPrivate->vecCarCategoryIds.size(),
               _pPrivate->vecPlayers.size());

    std::vector<std::string>::const_iterator itType;

    for (itType = _pPrivate->vecTypes.begin(); itType != _pPrivate->vecTypes.end(); ++itType)
    {
        GfLogInfo("  '%s' type :\n", itType->c_str());
        std::vector<std::string>::const_iterator itCarCatId;

        for (itCarCatId = _pPrivate->vecCarCategoryIds.begin();
             itCarCatId != _pPrivate->vecCarCategoryIds.end(); ++itCarCatId)
        {
            const std::vector<GfPlayer*> vecPlayers =
                getPlayersWithTypeAndCategory(*itCarCatId);

            if (vecPlayers.empty())
                continue;

            GfLogInfo("      '%s' car category :\n", itCarCatId->c_str());
            std::vector<GfPlayer*>::const_iterator itPlayer;
            for (itPlayer = vecPlayers.begin(); itPlayer != vecPlayers.end(); ++itPlayer)
                GfLogInfo("          Name : %s, car : %s, Ranking = %u - Profile = %i\n",
                           (*itPlayer)->getName().c_str(),
                           (*itPlayer)->getCar()->getName().c_str(),
                           (*itPlayer)->getRanking(),
                           (*itPlayer)->getActiveProfile());
            GfLogInfo(" End print\n");
        }
    }
}

// GfPlayerskin class ---------------------------------------------------------------
GfPlayerTeamCar::GfPlayerTeamCar(const std::string& strName)
: _bfTargets(0), _strName(strName)
{
}

int GfPlayerTeamCar::getTargets() const
{
    return _bfTargets;
}

const std::string& GfPlayerTeamCar::getName() const
{
    return _strName;
}

const std::string& GfPlayerTeamCar::getCarPreviewFileName() const
{
    return _strCarPreviewFileName;
}

void GfPlayerTeamCar::setTargets(int bfTargets)
{
    _bfTargets = bfTargets;
}

void GfPlayerTeamCar::addTargets(int bfTargets)
{
    _bfTargets |= bfTargets;
}

void GfPlayerTeamCar::setName(const std::string& strName)
{
    _strName = strName;
}

void GfPlayerTeamCar::setCarPreviewFileName(const std::string& strFileName)
{
    _strCarPreviewFileName = strFileName;
}

// GfPlayer class -------------------------------------------------------------------
// Skill level related constants.
static const char *ASkillLevelStrings[] = ROB_VALS_LEVEL;
static const int NbSkillLevels = sizeof(ASkillLevelStrings) / sizeof(ASkillLevelStrings[0]);
static const double ASkillLevelValues[NbSkillLevels] = { 30.0, 20.0, 10.0, 7.0, 3.0, 0.0 };

static void	*PlayerHdle = NULL;

GfPlayer::GfPlayer(void *hparmPlayer, int itfIndex)
{
    load(hparmPlayer, itfIndex);
}

void GfPlayer::load(void* hparmhuman, int itfIndex)
{
    char sstring[128];
    const char *str;

    snprintf(sstring, sizeof(sstring), "%s/%s/%d", ROB_SECT_ROBOTS, ROB_LIST_INDEX, itfIndex);
    _strName = GfParmGetStr(hparmhuman, sstring, ROB_ATTR_NAME, "");;
    _shortName = GfParmGetStr(hparmhuman, sstring, ROB_ATTR_SNAME, "");
    _codeName = GfParmGetStr(hparmhuman, sstring, ROB_ATTR_CODE, "");
    _nationality = GfParmGetStr(hparmhuman, sstring, ROB_ATTR_NATION, "");
    _rank = GfParmGetNum(hparmhuman, sstring, ROB_ATTR_RANKING, (char*)NULL, 0);
    _activeProfile = GfParmGetNum(hparmhuman, sstring, ROB_ATTR_ACTIVEPROFILE, (char*)NULL, 0);
    const char* pszCarId = GfParmGetStr(hparmhuman, sstring, ROB_ATTR_CAR, "");

    GfLogInfo("_strName       = %s\n", _strName.c_str());
    GfLogInfo("_shortName     = %s\n", _shortName.c_str());
    GfLogInfo("_codeName      = %s\n", _codeName.c_str());
    GfLogInfo("_nationality   = %s\n", _nationality.c_str());
    GfLogInfo("_rank          = %u\n", _rank);
    GfLogInfo("_activeProfile = %i\n", _activeProfile);

    str = GfParmGetStr(hparmhuman, sstring, ROB_ATTR_LEVEL, ASkillLevelStrings[0]);
    _Skilllevel = ARCADE;

    for(int j = 0; j < NbSkillLevels; j++)
    {
        if (strcmp(ASkillLevelStrings[j], str) == 0)
        {
            _Skilllevel = ASkillLevelValues[j];
            GfLogInfo("_SkillLevel    = %f\n", _Skilllevel);
            break;
        }
    }

    // Driven car.

    GfLogInfo("_pCar    = %s\n", pszCarId);
    _pCar = GfCars::self()->getCar(pszCarId);
    const std::string category = GfCars::self()->getCar(pszCarId)->getCategoryName();

    GfLogInfo("Player Name : %s - Car Name : %s - Category = %s\n", _strName.c_str(), _pCar->getName().c_str(), category.c_str());


}

const std::string& GfPlayer::getName() const
{
    return _strName;
}

const GfCar* GfPlayer::getCar() const
{
    return _pCar;
}

const std::string& GfPlayer::getCategoryName() const
{
    return _pCar->getCategoryName();
}

const GfPlayerTeamCar& GfPlayer::getTeamCar() const
{
    return _teamcar;
}

double GfPlayer::getSkillLevel() const
{
    return _Skilllevel;
}

void GfPlayer::setSkilllevel(double skill)
{
    _Skilllevel = skill;
}

unsigned int GfPlayer::getRanking() const
{
    return _rank;
}

bool GfPlayer::getActiveProfile() const
{
    return _activeProfile;
}

bool GfPlayer::matchesTypeAndCategory(const std::string& strCarCatId) const
{
    return (strCarCatId.empty() || getCar()->getCategoryId() == strCarCatId);
}

void GfPlayer::setCar(const GfCar* pCar)
{
    _pCar = pCar;
}

void GfPlayer::setTeamCar(const GfPlayerTeamCar& tc)
{
    _teamcar = tc;
}

void GfPlayer::setActiveProfile(bool profile)
{
    _activeProfile = profile;
}

/*static const char* pszTeamCarDirectory = "";
static const char* pszPreviewTexSufx = "-preview.jpg";

std::vector<GfPlayerTeamCar>::iterator GfPlayer::findTeamCar(std::vector<GfPlayerTeamCar>& vecTeamCars,
                                                       const std::string& strName)
{
    std::vector<GfPlayerTeamCar>::iterator itTeamCar;
    for (itTeamCar = vecTeamCars.begin(); itTeamCar != vecTeamCars.end(); ++itTeamCar)
    {
        if (itTeamCar->getName() == strName)
            return itTeamCar;
    }

    return vecTeamCars.end();
}

void GfPlayer::getPossibleSkinsInFolder(const std::string& strCarId,
                                        const std::string& strFolderPath,
                                        std::vector<GfPlayerTeamCar>& vecPossTeamCars) const
{
    GfLogInfo("  getPossibleSkinsInFolder(%s, %s) ...\n",
               strCarId.c_str(), strFolderPath.c_str());

    // Search for skinned livery files, and associated preview files if any.
    tFList *pLiveryFileList =
        GfDirGetListFiltered(strFolderPath.c_str(), strCarId.c_str(), pszTeamCarDirectory);

    if (pLiveryFileList)
    {
        tFList *pCurLiveryFile = pLiveryFileList;

        do
        {
            pCurLiveryFile = pCurLiveryFile->next;

            // Extract the skin name from the livery file name.
            const int nSkinNameLen = // Expecting "<car name>-<skin name>.png"
                strlen(pCurLiveryFile->name) - strCarId.length() - 1 - strlen(pszTeamCarDirectory);
            std::string strSkinName;

            if (nSkinNameLen > 0) // Otherwise, default/standard "<car name>.png"
            {
                strSkinName =
                    std::string(pCurLiveryFile->name)
                    .substr(strCarId.length() + 1, nSkinNameLen);

                // Ignore skins with an excluded prefix.
                int nExclPrfxInd = 0;

                for (; nExclPrfxInd < nExcludedSkinNamePrefixes; nExclPrfxInd++)
                    if (strSkinName.find(apszExcludedSkinNamePrefixes[nExclPrfxInd]) == 0)
                        break;

                if (nExclPrfxInd < nExcludedSkinNamePrefixes)
                    continue;
            }

            // Ignore skins that are already in the list (path search priority).
            if (findTeamCar(vecPossTeamCars, strSkinName) == vecPossTeamCars.end())
            {
                // Create the new skin.
                GfPlayerTeamCar teamcar(strSkinName);

                // Add the whole car livery to the skin targets.
                teamcar.addTargets(RM_CAR_SKIN_TARGET_WHOLE_LIVERY);

                GfLogDebug("  Found %s%s livery\n",
                           strSkinName.empty() ? "standard" : strSkinName.c_str(),
                           strSkinName.empty() ? "" : "-skinned");

                // Add associated preview image, without really checking file existence
                // (warn only ; up to the client GUI to do what to do if it doesn't exist).
                std::ostringstream ossPreviewName;
                ossPreviewName << strFolderPath << '/' << strCarId;
                if (!strSkinName.empty())
                    ossPreviewName << '-' << strSkinName;
                ossPreviewName << pszPreviewTexSufx;
                skin.setCarPreviewFileName(ossPreviewName.str());

                if (!GfFileExists(ossPreviewName.str().c_str()))
                    GfLogWarning("Preview file not found for %s %s skin (%s)\n",
                                 strCarId.c_str(), strSkinName.c_str(), ossPreviewName.str().c_str());
                //else
                //	GfLogDebug("* found skin=%s, preview=%s\n",
                //			   strSkinName.c_str(), ossPreviewName.str().c_str());

                // Add the new skin to the list.
                vecPossSkins.push_back(skin);
            }
        }
        while (pCurLiveryFile != pLiveryFileList);
    }

    GfDirFreeList(pLiveryFileList, NULL, true, true);
}

std::vector<GfPlayerTeamCar> GfPlayer::getPossibleTeamCar(const std::string& strAltCarId) const
{
    const std::string strCarId = strAltCarId.empty() ? _pCar->getId() : strAltCarId;

    GfLogDebug("Checking skins for %s ...\n", strCarId.c_str());

    // Clear the skin and preview lists.
    std::vector<GfPlayerTeamCar> vecPossTeamCars;

    // Get/check skins/skin targets/previews from the directories in the search path
    // WARNING: Must be consistent with the search paths used in grcar.cpp, grboard.cpp,
    //          grscene.cpp ... etc ... but it is not currently 100% achieved
    //          (pit door logos are not searched by the graphics engine
    //           in the car-dedicated folders ... so they may be "over-detected" here).
    std::ostringstream ossDirPath;
    ossDirPath << GfLocalDir() << "drivers/" << _strModName
               << '/' << _nItfIndex << '/' << strCarId;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    ossDirPath.str("");
    ossDirPath << GfLocalDir() << "drivers/" << _strModName
               << '/' << _nItfIndex;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    ossDirPath.str("");
    ossDirPath << GfLocalDir() << "drivers/" << _strModName
               << '/' << strCarId;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    ossDirPath.str("");
    ossDirPath << GfLocalDir() << "drivers/" << _strModName;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    ossDirPath.str("");
    ossDirPath << "drivers/" << _strModName
               << '/' << _nItfIndex << '/' << strCarId;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    ossDirPath.str("");
    ossDirPath << "drivers/" << _strModName
               << '/' << _nItfIndex;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    ossDirPath.str("");
    ossDirPath << "drivers/" << _strModName
               << '/' << strCarId;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    ossDirPath.str("");
    ossDirPath << "drivers/" << _strModName;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    ossDirPath.str("");
    ossDirPath << "cars/models/" << strCarId;
    getPossibleSkinsInFolder(strCarId, ossDirPath.str(), vecPossTeamCars);

    // If we have at least 1 skin, make sure that, if the standard one is inside,
    // it is the first one.
    if (!vecPossTeamCars.empty())
    {
        std::vector<GfPlayerTeamCar>::iterator itTeamCar;
        for (itTeamCar = vecPossTeamCars.begin(); itTeamCar != vecPossTeamCars.end(); ++itTeamCar)
        {
            if (itTeamCar->getName().empty() && itTeamCar != vecPossTeamCars.begin())
            {
                GfPlayerTeamCar stdTeamCar = *itTeamCar;
                vecPossTeamCars.erase(itTeamCar);
                vecPossTeamCars.insert(vecPossTeamCars.begin(), stdTeamCar);
                break;
            }
        }
    }

    // If no skin was found, add the car's standard one
    // (that way, the skin list will never be empty, and that's safer)
    else
    {
        GfLogError("No skin at all found for '%s/%d/%s' : adding dummy '%s' one\n",
                   _strModName.c_str(), _nItfIndex, strCarId.c_str(), "standard");

        GfPlayerTeamCar stdTeamCar;
        std::ostringstream ossPreviewName;
        ossPreviewName << "cars/models/" << strCarId << '/' << strCarId << pszPreviewTexSufx;
        stdTeamCar.setCarPreviewFileName(ossPreviewName.str());

        if (!GfFileExists(ossPreviewName.str().c_str()))
            GfLogWarning("No preview file %s found for dummy '%s' skin\n",
                         ossPreviewName.str().c_str(), "standard");

        vecPossTeamCars.push_back(stdTeamCar);
    }

    return vecPossTeamCars;
}*/
