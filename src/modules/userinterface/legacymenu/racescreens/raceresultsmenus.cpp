/***************************************************************************

    file                 : raceresultsmenus.cpp
    created              : Fri Apr 14 22:36:36 CEST 2000
    copyright            : (C) 2000 by Eric Espie (C) 2024 by shadow72
    email                : torcs@free.fr
    version              : $Id: raceresultsmenus.cpp 8159 2022-05-14 11:10:46Z shadow72 $

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
            This is a set of tools useful for race managers to display results.
    @ingroup	racemantools
    @author	<a href=mailto:bertauxx@gmail.com>Xavier Bertaux</a>
    @version	$Id: raceresultsmenus.cpp 8159 2022-05-14 11:10:46Z shadow72 $
*/

#include <webserver.h>
#include <portability.h>
#include <tgfclient.h>
#include <robot.h>

#include <stdio.h>

#include <drivers.h>
#include <tracks.h>
#include <race.h>
#include <racemanagers.h>

#include "legacymenu.h"
#include "racescreens.h"

const char* rFame[] = { "fame0", "fame1", "fame2", "fame3", "fame4", "fame5",
                        "fame6", "fame7", "fame8", "fame9", "fame10", "fame11",
                        "fame12", "fame13", "fame14", "fame15", "fame16", "fame17" };

static int	rmSaveButtonId;
static int	rmReplayButtonId;
static bool rmEndChampionship = false;
static bool rmCareer = false;
static bool rmChallenge = false;
static bool rmEndurance = false;
static bool rmChampionship = false;
static bool rmUpdate2 = true;
static void	*rmScrHdle = NULL;
static std::string carName = "";

static void rmPracticeResults(void *prevHdle, tRmInfo *info, int start);
static void rmRaceResults(void *prevHdle, tRmInfo *info, int start);
static void rmQualifResults(void *prevHdle, tRmInfo *info, const char*pszTitle, int start);

static const int    DefaultSimuVersion = 1;
static const char   *SimuVersionList[] = { RM_VAL_MOD_SIMU_V5, RM_VAL_MOD_SIMU_REPLAY};
static const int    NbSimuVersions = sizeof(SimuVersionList) / sizeof(SimuVersionList[0]);
static int          CurSimuVersion = DefaultSimuVersion;

typedef struct
{
    void        *prevHdle;
    tRmInfo     *info;
    int         start;
    const char	*title;
} tRaceCall;

tRaceCall       RmNextRace;
tRaceCall       RmPrevRace;

static void
rmUpdateHumanPlayer(tRmInfo *info)
{
    GfLogDebug("# Race Results Update Human Player !!\n");

    std::string strPlayersFileName(GfLocalDir());
    strPlayersFileName += "drivers/human/human.xml";
    void *hparmHuman = GfParmReadFileLocal(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);
    const char *strRank ="";
    const char *strLevel = "";
    char drvSectionPath[128];
    snprintf(drvSectionPath, sizeof(drvSectionPath), "%s/%s/%u", ROB_SECT_ROBOTS, ROB_LIST_INDEX, 1);

    int rmIndex2 = -1;
    int rmIndex3 = -1;

    strRank = rmrank[rmIndex];
    strLevel = rmlevel[rmIndex];

    if (( rmRanking[rmIndex] >= 7 && rmRanking[rmIndex +2] == -1 ) && rmUpdate)
    {
        switch (rmIndex)
        {
        case 0:
        case 1:
            rmRanking[2] = 0;
            rmRanking[3] = 0;
            rmIndex2 = 2;
            rmIndex3 = 3;
            break;
        case 2:
        case 3:
            rmRanking[4] = 0;
            rmRanking[5] = 0;
            rmIndex2 = 4;
            rmIndex3 = 5;
            break;
        case 4:
        case 5:
            rmRanking[6] = 0;
            rmRanking[7] = 0;
            rmIndex2 = 6;
            rmIndex3 = 7;
            break;
        case 6:
        case 7:
            rmRanking[8] = 0;
            rmRanking[9] = 0;
            rmIndex2 = 8;
            rmIndex3 = 9;
            break;
        case 8:
        case 9:
            rmRanking[10] = 0;
            rmRanking[11] = 0;
            rmIndex2 = 10;
            rmIndex3 = 11;
            break;
        case 10:
        case 11:
            rmRanking[12] = 0;
            rmRanking[13] = 0;
            rmIndex2 = 12;
            rmIndex3 = 13;
            break;
        case 12:
        case 13:
            rmRanking[14] = 0;
            rmRanking[15] = 0;
            rmIndex2 = 14;
            rmIndex3 = 15;
            break;
        case 14:
        case 15:
            rmRanking[16] = 0;
            rmRanking[17] = 0;
            rmIndex2 = 16;
            rmIndex3 = 17;
            break;
        case 16:
        case 17:
            rmRanking[18] = 0;
            rmRanking[19] = 0;
            rmIndex2 = 18;
            rmIndex3 = 19;
            break;
        default:
            break;
        }
    }

    if (!hparmHuman)
        hparmHuman = GfParmReadFile(strPlayersFileName, GFPARM_RMODE_STD | GFPARM_RMODE_REREAD);

    if (!hparmHuman)
    {
        GfLogError("No usable human driver (human.xml not found or not readable)\n");
    }

    if (hparmHuman)
    {
        char sstring[128];
        const char *Fame = "";
        Fame = rFame[rmIndex];
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", "Robots", "index", 1);
        GfParmSetNum(hparmHuman, sstring, strRank, (char*)NULL, rmRanking[rmIndex]);
        GfLogDebug(" Added ranking = %i - Index = %s\n", rmRanking[rmIndex], strRank);
        GfParmSetNum(hparmHuman, sstring, strLevel, (char*)NULL, rmLevel[rmIndex]);
        GfParmSetNum(hparmHuman, sstring, "credit", (char*)NULL, (int)rmCredit);
        GfParmSetNum(hparmHuman, sstring, Fame, (char*)NULL, (double)rmFame);
        GfParmSetNum(hparmHuman, sstring, "licence", (char*)NULL, (int)rmLicence);

        carName = LegacyMenu::self().getCarPlayer();
        int dammages = LegacyMenu::self().getDammages();

        GfParmSetNum(hparmHuman, sstring, "dammages", (char*)NULL, (int)dammages);

        if(rmEndChampionship == true && rmCareer == true)
        {
            GfParmSetStr(hparmHuman, sstring, "championship", "none");
            GfParmSetNum(hparmHuman, sstring, "dammages", (char*)NULL, 0);
        }

        GfLogInfo("End Championship = %i - Career = %i - Level[%i] = %u - Credit = %lu - Licence = %i\n",
                   rmEndChampionship, rmCareer, rmIndex, rmLevel[rmIndex], rmCredit, rmLicence);

        if (( rmRanking[rmIndex] >= 7 && rmRanking[rmIndex +2] == -1 ))
        {
            strRank = rmrank[rmIndex2];
            GfParmSetNum(hparmHuman, sstring, strRank, (char*)NULL, rmRanking[rmIndex2]);
            strRank = rmrank[rmIndex3];
            GfParmSetNum(hparmHuman, sstring, strRank, (char*)NULL, rmRanking[rmIndex3]);
            GfLogDebug("# activate ranking level superior !!\n");
        }

        rmUpdate2 = false;

        GfParmWriteFile(NULL, hparmHuman, "human");
        GfParmReleaseHandle(hparmHuman);
    }
}

static void
rmUpdateAllOfFame(tSkillLevel level, const char *championship,
                  const char *car, int position, int points, int dammages)
{
    GfLogDebug("# Sent data championship on masterserver !!!\n");
    webServer().sendLogin(1);

    //send race data
    webServer().sendChampionship(
                level,                          //user_skill,
                championship,                   //championship name,
                car,                            //car_id,
                position,                       //final position,
                points,                         //final points,
                dammages,                       //dammages car,
                VERSION_LONG 					//cars sports racing version
                );

    GfLogDebug(" # Send %s info on masterserver\n", championship);
    LegacyMenu::self().setChampionshipMode(false);
}

/*static void
onRaceSelectMenuActivate(void *)
{
    LmRaceEngine().shutdown();
    GfuiScreenActivate(RmRaceSelectInit(MenuHandle));
}

static void
onRaceCareerSelectMenuActivate(void *)
{
    LmRaceEngine().shutdown();
    GfuiScreenActivate(RmCareerSelectInit(MenuHandle));
}*/

static void
rmReplayRace(void * /* dummy */)
{
    GfLogDebug("# Replay Race\n");
    const char *simuVersionName;
    int i;
    void *paramHandle = GfParmReadFileLocal(RACE_ENG_CFG, GFPARM_RMODE_REREAD | GFPARM_RMODE_CREAT);

    // Temporarily overwrite Simulation Type
    CurSimuVersion = DefaultSimuVersion;
    simuVersionName = GfParmGetStr(paramHandle, RM_SECT_MODULES, RM_ATTR_MOD_SIMU, SimuVersionList[DefaultSimuVersion]);

    for (i = 0; i < NbSimuVersions; i++)
    {
        if (strcmp(simuVersionName, SimuVersionList[i]) == 0)
        {
            CurSimuVersion = i;
            break;
        }
    }

    GfParmSetStr(paramHandle, RM_SECT_MODULES, RM_ATTR_MOD_SIMU, SimuVersionList[1]);
    GfParmWriteFile(NULL, paramHandle, "raceengine");

    LmRaceEngine().startNewRace();

    // Restore original Simulation type
    GfParmSetStr(paramHandle, RM_SECT_MODULES, RM_ATTR_MOD_SIMU, SimuVersionList[CurSimuVersion]);
    GfParmWriteFile(NULL, paramHandle, "raceengine");
    GfParmReleaseHandle(paramHandle);
}

static void
rmSaveRes(void *vInfo)
{
    GfLogDebug("# Save Resultats !!!\n");
    tRmInfo *info = (tRmInfo *)vInfo;

    GfParmWriteFile(0, info->results, "Results");

    GfuiVisibilitySet(rmScrHdle, rmSaveButtonId, GFUI_INVISIBLE);
}

static void
rmChgPracticeScreen(void *vprc)
{
    void		*prevScr = rmScrHdle;
    tRaceCall 	*prc = (tRaceCall*)vprc;

    rmPracticeResults(prc->prevHdle, prc->info, prc->start);
    GfuiScreenRelease(prevScr);
}

static void
rmPracticeResults(void *prevHdle, tRmInfo *info, int start)
{
    GfLogDebug("# Race Results Practice Results !\n");
    // Used across rmPracticeResults calls when multiple pages.
    static int NLastLapDamages = 0;

    void		*results = info->results;
    const char	*race = info->_reRaceName;
    int			i;
    int			y;
    static char	buf[256];
    static char	path[1024];
    char		*str;
    int 		damages = 0;
    int         totalPenalty = 0;
    float       rmtime = 0.0f;
    void		*paramHandle;
    const char	*replayRateSchemeName;

    // Create screen, load menu XML descriptor and create static controls.
    rmScrHdle = GfuiScreenCreate();

    GfLogTrace("Entering Practice Results menu\n");

    void *hmenu = GfuiMenuLoad("practiceresultsmenu.xml");
    GfuiMenuCreateStaticControls(rmScrHdle, hmenu);

    GfuiMenuDefaultKeysAdd(rmScrHdle);

    // Create variable title labels.
    snprintf(buf, sizeof(buf), "%s at %s", race, info->track->name);
    const int titleId = GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Title");
    GfuiLabelSetText(rmScrHdle, titleId, buf);

    snprintf(path, sizeof(path), "%s/%s/%s", info->track->name, RE_SECT_RESULTS, race);
    snprintf(buf, sizeof(buf), "%s (%s)", GfParmGetStr(results, path, RM_ATTR_DRVNAME, NULL),
             GfParmGetStr(results, path, RM_ATTR_CAR, NULL));

    const int subTitleId = GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "SubTitle");
    GfuiLabelSetText(rmScrHdle, subTitleId, buf);

    // Get layout properties.
    const int nMaxLines = (int)GfuiMenuGetNumProperty(hmenu, "nMaxResultLines", 15);
    const int yTopLine = (int)GfuiMenuGetNumProperty(hmenu, "yTopLine", 400);
    const int yLineShift = (int)GfuiMenuGetNumProperty(hmenu, "yLineShift", 20);

    // Display the result table.
    y = yTopLine;

    snprintf(path, sizeof(path), "%s/%s/%s", info->track->name, RE_SECT_RESULTS, race);
    const int totLaps = (int)GfParmGetEltNb(results, path);

    // Reset last damage value if top of the table.
    if (start == 0)
        NLastLapDamages = 0;
    else
    {
        snprintf(path, sizeof(path), "%s/%s/%s/%d", info->track->name, RE_SECT_RESULTS, race, start - 1);
        NLastLapDamages =  (int)(GfParmGetNum(results, path, RE_ATTR_DAMMAGES, NULL, 0));
    }

    for (i = 0 + start; i < MIN(start + nMaxLines, totLaps); i++)
    {
        snprintf(path, sizeof(path), "%s/%s/%s/%d", info->track->name, RE_SECT_RESULTS, race, i + 1);

        /* Lap */
        snprintf(buf, sizeof(buf), "%d", i+1);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "LapNumber", true, // From template.
                                   buf, GFUI_TPL_X, y);

        /* Time */
        str = GfTime2Str(GfParmGetNum(results, path, RE_ATTR_TIME, NULL, 0), "  ", false, 3);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "LapTime", true, // From template.
                                   str, GFUI_TPL_X, y);

        free(str);

        /* Best Lap Time */
        str = GfTime2Str(GfParmGetNum(results, path, RE_ATTR_BEST_LAP_TIME, NULL, 0), "  ", false, 3);
        rmtime = (float)(GfParmGetNum(results, path, RE_ATTR_BEST_LAP_TIME, NULL, 0.0));
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "BestTime", true, // From template.
                                   str, GFUI_TPL_X, y);

        GfLogDebug("Time = %s Time in float %.4f\n", str, rmtime);
        free(str);

        /* Top Spd */
        snprintf(buf, sizeof(buf), "%3.1f", (GfParmGetNum(results, path, RE_ATTR_TOP_SPEED, NULL, 0) * 3.6));
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "TopSpeed", true, // From template.
                                   buf, GFUI_TPL_X, y);

        /* Min Spd */
        snprintf(buf, sizeof(buf), "%3.1f", (GfParmGetNum(results, path, RE_ATTR_BOT_SPEED, NULL, 0) * 3.6));
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "MinSpeed", true, // From template.
                                   buf, GFUI_TPL_X, y);

        /* Damages in current lap + (total so far) */
        damages =  (int)(GfParmGetNum(results, path, RE_ATTR_DAMMAGES, NULL, 0));
        snprintf(buf, sizeof(buf), "%d (%d)", damages ? damages - NLastLapDamages : 0, damages);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Damages", true, // From template.
                                   buf, GFUI_TPL_X, y);
        NLastLapDamages = damages;
        totalPenalty = (int)(GfParmGetNum(results, path, RE_ATTR_TOTALPENALTY, NULL, 0));

        y -= yLineShift;
    }

    GfLogDebug("# Practice Results bestlap = %.3f - Ranking = %u - Index = %i - Totallaps = %i - Career = %i\n",
               rmtime, rmRanking[rmIndex], rmIndex, totLaps, rmCareer);

    if((rmtime < info->track->local.bestlap) && (rmRanking[rmIndex] < 1) && (i = totLaps) && !rmCareer && !rmUpdate2)
    {
        rmSkillMult = LegacyMenu::self().getSkillMult();
        int resultcredit = (int)((500 * rmSkillMult ) - (damages * 0.1));
        GfLogInfo("Practice : rmSkillMult = %.2f - result credit = %i\n", rmSkillMult, resultcredit);

        if (resultcredit > 0.0)
            rmCredit += resultcredit;

        double fame = 1 - ((totalPenalty * 0.1) + (damages * 0.0001));
        rmFame += fame;

        rmRanking[rmIndex] += 1;
        rmUpdate = true;
    }

    if((rmtime < info->track->local.bestlap) && (i = totLaps) && rmCareer && (rmLevel[rmIndex] < 2))
    {
        rmSkillMult = LegacyMenu::self().getSkillMult();
        int resultcredit = (500 * rmSkillMult ) - (damages / 10);
        GfLogInfo("rmSkillMult = %.2f - result credit = %i\n", rmSkillMult, resultcredit);

        if (resultcredit > 0.0)
            rmCredit += resultcredit;

        rmLevel[rmIndex] +=1;        
        totalPenalty = (totalPenalty * 0.1);
        double fame = 1 - (totalPenalty + (damages * 0.0001));
        rmFame += fame;
        GfLogDebug("Practice passed !! Level = %i - incremente level = %i\n", rmIndex, rmLevel[rmIndex]);
    }

    rmUpdateHumanPlayer(info);
    rmUpdate = false;

    if (start > 0)
    {
        RmPrevRace.prevHdle = prevHdle;
        RmPrevRace.info     = info;
        RmPrevRace.start    = start - nMaxLines;
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "PreviousPageArrow",
                                    (void*)&RmPrevRace, rmChgPracticeScreen);
        GfuiAddKey(rmScrHdle, GFUIK_PAGEUP,   "Previous Results", (void*)&RmPrevRace, rmChgPracticeScreen, NULL);
    }

    // Add "Continue" button
    GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "ContinueButton", prevHdle, GfuiScreenReplace);

    // Add "Replay" button (if available)
    paramHandle = GfParmReadFileLocal(RACE_ENG_CFG, GFPARM_RMODE_REREAD | GFPARM_RMODE_CREAT);
    replayRateSchemeName = GfParmGetStr(paramHandle, RM_SECT_RACE_ENGINE, RM_ATTR_REPLAY_RATE, RM_VAL_REPLAY_OFF);

    rmReplayButtonId = GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "ReplayButton", prevHdle, rmReplayRace);
    if (strcmp(replayRateSchemeName, RM_VAL_REPLAY_OFF) == 0)
        GfuiEnable(rmScrHdle, rmReplayButtonId, GFUI_DISABLE);

    GfParmReleaseHandle(paramHandle);

    //Create 'save' button in the bottom right
    //rmSaveButtonId = GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "SaveButton", info, rmSaveRes);

    if (i < totLaps)
    {
        RmNextRace.prevHdle = prevHdle;
        RmNextRace.info     = info;
        RmNextRace.start    = start + nMaxLines;
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "NextPageArrow",
                                    (void*)&RmNextRace, rmChgPracticeScreen);
        GfuiAddKey(rmScrHdle, GFUIK_PAGEDOWN, "Next Results", (void*)&RmNextRace, rmChgPracticeScreen, NULL);
    }

    GfuiAddKey(rmScrHdle, GFUIK_ESCAPE, "Continue", prevHdle, GfuiScreenReplace, NULL);
    GfuiAddKey(rmScrHdle, GFUIK_RETURN, "Continue", prevHdle, GfuiScreenReplace, NULL);

    GfuiScreenActivate(rmScrHdle);
}

static void
rmChgRaceScreen(void *vprc)
{
    GfLogDebug("# Change Screen RaceResults\n");
    void		*prevScr = rmScrHdle;
    tRaceCall 	*prc = (tRaceCall*)vprc;

    rmRaceResults(prc->prevHdle, prc->info, prc->start);
    GfuiScreenRelease(prevScr);
}

static void
rmRaceResults(void *prevHdle, tRmInfo *info, int start)
{
    GfLogDebug("# Race Results !!!\n");
    void		*results = info->results;
    const char	*race = info->_reRaceName;
    static char	buf[256];
    static char	path[512];
    char		*str;
    void		*paramHandle;
    const char	*replayRateSchemeName;

    rmChallenge = LegacyMenu::self().getChallengeMode();
    rmEndurance = LegacyMenu::self().getEnduranceMode();
    rmChampionship = LegacyMenu::self().getChampionshipMode();
    rmSkillMult = LegacyMenu::self().getSkillMult();

    GfLogTrace("Entering Race Results menu\n");

    // Create screen, load menu XML descriptor and create static controls.
    rmScrHdle = GfuiScreenCreate();
    void *hmenu = GfuiMenuLoad("raceresultsmenu.xml");
    GfuiMenuCreateStaticControls(rmScrHdle, hmenu);

    // Create variable title label.
    snprintf(buf, sizeof(buf), "%s at %s", race, info->track->name);
    const int titleId = GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Title");
    GfuiLabelSetText(rmScrHdle, titleId, buf);

    // Get layout properties.
    const int nMaxLines = (int)GfuiMenuGetNumProperty(hmenu, "nMaxResultLines", 15);
    const int yTopLine = (int)GfuiMenuGetNumProperty(hmenu, "yTopLine", 400);
    const int yLineShift = (int)GfuiMenuGetNumProperty(hmenu, "yLineShift", 20);
    const GfuiColor cPlaceGain =
            GfuiColor::build(GfuiMenuGetStrProperty(hmenu, "colorGainedPlaces", "0x32CD32"));
    const float* acPlaceGain = cPlaceGain.toFloatRGBA();
    const GfuiColor cPlaceLoss =
            GfuiColor::build(GfuiMenuGetStrProperty(hmenu, "colorLostPlaces", "0xF28500"));
    const float* acPlaceLoss = cPlaceLoss.toFloatRGBA();

    // Never used : remove ?
    //Get total laps, winner time
    //snprintf(path, sizeof(path), "%s/%s/%s", info->track->name, RE_SECT_RESULTS, race);
    //int totLaps = (int)GfParmGetNum(results, path, RE_ATTR_LAPS, NULL, 0);
    //snprintf(path, sizeof(path), "%s/%s/%s/%s/%d", info->track->name, RE_SECT_RESULTS, race, RE_SECT_RANK, 1);
    //tdble refTime = GfParmGetNum(results, path, RE_ATTR_TIME, NULL, 0);

    //Get number of cars
    snprintf(path, sizeof(path), "%s/%s/%s/%s", info->track->name, RE_SECT_RESULTS, race, RE_SECT_RANK);
    int nbCars = (int)GfParmGetEltNb(results, path);

    // Display the result table.
    int y = yTopLine;
    int i;

    for (i = start; i < MIN(start + nMaxLines, nbCars); i++)
    {
        snprintf(path, sizeof(path), "%s/%s/%s/%s/%d", info->track->name, RE_SECT_RESULTS, race, RE_SECT_RANK, i + 1);
        int laps = (int)GfParmGetNum(results, path, RE_ATTR_LAPS, NULL, 0);//Laps covered

        //Rank
        snprintf(buf, sizeof(buf), "%d", i+1);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Rank", true, // From template.
                                   buf, GFUI_TPL_X, y);

        int rank = i + 1;
        int damage = (int)GfParmGetNum(results, path, RE_ATTR_DAMMAGES, NULL, 0);
        double totalpenalty = (double)GfParmGetNum(results, path, RE_ATTR_TOTALPENALTY, NULL, 0);
        //totalpenalty = 1 - (totalpenalty / 10);
        int gains = (int)GfParmGetNum(results, path, RE_ATTR_GAINS, NULL, 0);
        GfLogDebug(" Rank Result = %i Dammages = %i - Total penalty = %.1f\n", rank, damage, totalpenalty);

        //Advance (The num.attrib 'index' holds the starting position)
        int advance = (int)(GfParmGetNum(results, path, RE_ATTR_INDEX, NULL, 0)) - i;
        snprintf(buf, sizeof(buf), "%d", advance);
        const float *aColor =
                advance > 0 ? acPlaceGain : (advance < 0 ? acPlaceLoss : GFUI_TPL_COLOR);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Advance", true, // From template.
                                   buf, GFUI_TPL_X, y, GFUI_TPL_FONTID, GFUI_TPL_WIDTH,
                                   GFUI_TPL_ALIGN, GFUI_TPL_MAXLEN, aColor);

        //Driver short name
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "DriverName", true, // From template.
                                   GfParmGetStr(results, path, RE_ATTR_SNAME, ""), GFUI_TPL_X, y);

        //Driver type
        const std::string strModName = GfParmGetStr(results, path, RE_ATTR_MODULE, "");
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "DriverType", true, // From template.
                                   GfDriver::getType(strModName).c_str(), GFUI_TPL_X, y);

        /*GfRaceManager* pRaceMan = LmRaceEngine().race()->getManager();
        int count = pRaceMan->getSessionCount() - 1;
        GfLogDebug("Race Count challenge or Endurance = %i\n\n", count);*/

        if(strModName == ROB_VAL_HUMAN)
        {
            GfLogDebug("# Race Results rank = %i - rmRanking = %u - Index = %i - Career = %i\n",
                       rank, rmRanking[rmIndex], rmIndex, rmCareer);

            if((rmRanking[rmIndex] < 9) && !rmCareer)
            {
                rmUpdate = true;
                std::string strEnd;
                std::string str;
                str = info->_reName;
                strEnd = str.substr(str.size() - 11);

                if ((strcmp(strEnd.c_str(), "Challenge 1") == 0) && rmRanking[rmIndex] < 2)
                    rmRanking[rmIndex] += 1;
                else if((strcmp(strEnd.c_str(), "Challenge 2") == 0) && rmRanking[rmIndex] < 3)
                    rmRanking[rmIndex] += 1;
                else if((strcmp(strEnd.c_str(), "Endurance 1") == 0) && rmRanking[rmIndex] < 4)
                    rmRanking[rmIndex] += 1;
                else if((strcmp(strEnd.c_str(), "Endurance 2") == 0) && rmRanking[rmIndex] < 6)
                    rmRanking[rmIndex] += 1;
                else if((strcmp(strEnd.c_str(), "Endurance 3") == 0) && rmRanking[rmIndex] < 8)
                    rmRanking[rmIndex] += 1;
                else if((strcmp(strEnd.c_str(), "Endurance 4") == 0) && rmRanking[rmIndex] < 9)
                    rmRanking[rmIndex] += 1;

                gains = (int)((gains * rmSkillMult) - ((damage * 0.1) + totalpenalty));
                GfLogInfo("Race : rmSkillMult = %.2f - result credit = %i\n", rmSkillMult, gains);
                rmCredit += gains;
                double fame = (rank * rmSkillMult) - ((damage * 0.0001) + (totalpenalty));
                rmFame += fame;

                GfLogInfo(" #Race result : rmIndex Challenge or Endurance = %i - Credit = %lu\n",
                           rmRanking[rmIndex], rmCredit);
            }

            GfLogDebug("# Race Results Career = %i\n", rmCareer);

            if(rmCareer)
            {
                if(rmUpdate2)
                {
                    if(rank < 10)
                    {
                        float mult3;

                        GfLogInfo("# Race Results Licence = %i\n", rmLicence);

                        if(rmLicence == true)
                            mult3 = 1;
                        else
                            mult3 = 0.5;

                        double mult2 = LegacyMenu::self().getSkillMult();

                        int val = (int)(((gains * mult3) * mult2) - (damage * 0.1));
                        GfLogInfo("Race Career : rmSkillMult = %.2f - result credit = %i - rmCredit = %i\n",
                                  mult2, val, (int)(rmCredit + val));

                        if(val < 0)
                            val = 0;

                        rmCredit += val;

                        if (rmChallenge == true)
                        {
                            rmFame += (11 - rank) * rmSkillMult;
                        }
                        else if (rmEndurance == true)
                        {
                            rmFame += ((((11 - rank) * 1.5)  * rmSkillMult) - ((damage * 0.0001) + totalpenalty)) ;
                        }
                        else if (rmChampionship == true)
                            rmFame += ((((11 - rank) * 2) * rmSkillMult)- ((damage * 0.0001) + totalpenalty));
                        else
                            rmFame += 0.0;

                        if ((rank < 5) && (rmLevel[rmIndex] > 3))
                            rmLicence = true;
                    }
                    else
                    {
                        rmFame += (11 - (rank * 0.5)) - ((damage * 0.0001) + totalpenalty);
                    }

                    if(!rmChampionship && rmUpdate2)
                    {
                        int index = rmLevel[rmIndex] % 10;

                        if (index == 9)
                            rmLevel[rmIndex] += 2;
                        else
                            rmLevel[rmIndex] += 1;

                        GfLogInfo("Race Result Career mode : Credit + %i - Rank = %i - Damage = %i - Level[%i] = %i - Fame = %.2f - Credit = %lu - Championship = %i\n",
                                   gains, rank, damage, rmIndex, rmLevel[rmIndex], rmFame, rmCredit, rmChampionship);
                    }
                }

            }

            GfLogInfo("# Race Results Career = %i - Championship = %i\n", rmCareer, (int)rmChampionship);

            if(rmChampionship == true)
            {
                int d = LegacyMenu::self().getDammages();
                LegacyMenu::self().setDammages( d + damage);
                GfLogDebug(" # Race dammages = %i\n", LegacyMenu::self().getDammages());
            }
            else if(rmChampionship == false)
            {
                LegacyMenu::self().setDammages(0);
            }

            rmUpdateHumanPlayer(info);
            rmUpdate = false;
            rmUpdate2 = false;
            GfLogDebug("Race Results Update Human !!!\n");
        } // end ROB_VAL_HUMAN

        //Car
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "CarModel", true, // From template.
                                   GfParmGetStr(results, path, RE_ATTR_CAR, ""), GFUI_TPL_X, y);

        //Nation
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Nation", true, // From template.
                                   GfParmGetStr(results, path, RE_ATTR_NATION, ""), GFUI_TPL_X, y);

        //Total Time
        str = GfTime2Str(GfParmGetNum(results, path, RE_ATTR_TIME, NULL, 0), 0, false, 3);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "TotalTime", true, // From template.
                                   str, GFUI_TPL_X, y);
        free(str);

        //Best lap
        str = GfTime2Str(GfParmGetNum(results, path, RE_ATTR_BEST_LAP_TIME, NULL, 0), 0, false, 3);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "BestLapTime", true, // From template.
                                   str, GFUI_TPL_X, y);
        free(str);

        //Laps covered
        snprintf(buf, sizeof(buf), "%d", laps);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Laps", true, // From template.
                                   buf, GFUI_TPL_X, y);

        //Top speed
        snprintf(buf, sizeof(buf), "%3.1f", (GfParmGetNum(results, path, RE_ATTR_TOP_SPEED, NULL, 0) * 3.6));
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "TopSpeed", true, // From template.
                                   buf, GFUI_TPL_X, y);

        //Damage
        snprintf(buf, sizeof(buf), "%d", (int)(GfParmGetNum(results, path, RE_ATTR_DAMMAGES, NULL, 0)));
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Damages", true, // From template.
                                   buf, GFUI_TPL_X, y);

        //Pitstops
        snprintf(buf, sizeof(buf), "%d", (int)(GfParmGetNum(results, path, RE_ATTR_NB_PIT_STOPS, NULL, 0)));
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Pits", true, // From template.
                                   buf, GFUI_TPL_X, y);

        y -= yLineShift;  //Line feed
    }//for i

    //If it is not the first screen of the results, show a 'Prev' button
    if (start > 0)
    {
        RmPrevRace.prevHdle = prevHdle;
        RmPrevRace.info     = info;
        RmPrevRace.start    = start - nMaxLines;
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "PreviousPageArrow",
                                    (void*)&RmPrevRace, rmChgRaceScreen);
        GfuiAddKey(rmScrHdle, GFUIK_PAGEUP,   "Previous Results", (void*)&RmPrevRace, rmChgRaceScreen, NULL);
    }//if start

    // Add "Continue" button
    //if(rmChampionship)
    {
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "ContinueButton", prevHdle, GfuiScreenReplace);
    }
    /*else if (rmCareer)
    {
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "ContinueButton", NULL, onRaceCareerSelectMenuActivate);
    }
    else
    {
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "ContinueButton", NULL, onRaceSelectMenuActivate);
    }*/

    // Add "Replay" button (if available)
    paramHandle = GfParmReadFileLocal(RACE_ENG_CFG, GFPARM_RMODE_REREAD | GFPARM_RMODE_CREAT);
    replayRateSchemeName = GfParmGetStr(paramHandle, RM_SECT_RACE_ENGINE, RM_ATTR_REPLAY_RATE, RM_VAL_REPLAY_OFF);

    rmReplayButtonId = GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "ReplayButton", prevHdle, rmReplayRace);

    if (strcmp(replayRateSchemeName, RM_VAL_REPLAY_OFF) == 0)
        GfuiEnable(rmScrHdle, rmReplayButtonId, GFUI_DISABLE);

    GfParmReleaseHandle(paramHandle);

    //Create 'save' button in the bottom right
    //rmSaveButtonId = GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "SaveButton", info, rmSaveRes);

    //If we did not display all the results yet, let's show a 'Next' button
    if (i < nbCars)
    {
        RmNextRace.prevHdle = prevHdle;
        RmNextRace.info     = info;
        RmNextRace.start    = start + nMaxLines;
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "NextPageArrow", (void*)&RmNextRace, rmChgRaceScreen);
        GfuiAddKey(rmScrHdle, GFUIK_PAGEDOWN, "Next Results", (void*)&RmNextRace, rmChgRaceScreen, NULL);
    }//if i

    //Link key handlers
    GfuiMenuDefaultKeysAdd(rmScrHdle);
    GfuiAddKey(rmScrHdle, GFUIK_ESCAPE, "Continue", prevHdle, GfuiScreenReplace, NULL);
    GfuiAddKey(rmScrHdle, GFUIK_RETURN, "Continue", prevHdle, GfuiScreenReplace, NULL);

    //Show!
    GfuiScreenActivate(rmScrHdle);
}//rmRaceResults

static void
rmChgQualifScreen(void *vprc)
{
    void		*prevScr = rmScrHdle;
    tRaceCall 	*prc = (tRaceCall*)vprc;

    rmQualifResults(prc->prevHdle, prc->info, prc->title, prc->start);
    GfuiScreenRelease(prevScr);
}

static void
rmQualifResults(void *prevHdle, tRmInfo *info, const char* pszTitle, int start)
{
    GfLogDebug("# Race Results Qualification Results !!\n");
    void            *results = info->results;
    const char		*race = info->_reRaceName;
    int             i;
    static char		buf[256];
    static char		path[512];
    char            *str;

    GfLogTrace("Entering %s Results menu\n", pszTitle);

    // Create screen, load menu XML descriptor and create static controls.
    rmScrHdle = GfuiScreenCreate();
    void *hmenu = GfuiMenuLoad("qualifsresultsmenu.xml");
    GfuiMenuCreateStaticControls(rmScrHdle, hmenu);

    // Create variable title label.
    const int titleId = GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Title");
    snprintf(buf, sizeof(buf), "%s at %s", race, info->track->name);
    GfuiLabelSetText(rmScrHdle, titleId, buf);

    // Get layout properties.
    const int nMaxLines = (int)GfuiMenuGetNumProperty(hmenu, "nMaxResultLines", 15);
    const int yTopLine = (int)GfuiMenuGetNumProperty(hmenu, "yTopLine", 400);
    const int yLineShift = (int)GfuiMenuGetNumProperty(hmenu, "yLineShift", 20);

    // Never used : remove ?
    //snprintf(path, sizeof(path), "%s/%s/%s/%s/%d", info->track->name, RE_SECT_RESULTS, race, RE_SECT_RANK, 1);
    //tdble refTime = GfParmGetNum(results, path, RE_ATTR_TIME, NULL, 0);
    //snprintf(path, sizeof(path), "%s/%s/%s", info->track->name, RE_SECT_RESULTS, race);
    //const int totLaps = (int)GfParmGetNum(results, path, RE_ATTR_LAPS, NULL, 0);
    snprintf(path, sizeof(path), "%s/%s/%s/%s", info->track->name, RE_SECT_RESULTS, race, RE_SECT_RANK);
    const int nbCars = (int)GfParmGetEltNb(results, path);
    GfLogDebug("rmQualifResults: path=%s, file=%s\n", path, GfParmGetFileName(results));
    GfLogDebug("rmQualifResults: start=%d, nbCars=%d, nMaxLines=%d\n", start, nbCars, nMaxLines);

    int y = yTopLine;

    for (i = start; i < MIN(start + nMaxLines, nbCars); i++)
    {
        snprintf(path, sizeof(path), "%s/%s/%s/%s/%d", info->track->name, RE_SECT_RESULTS, race, RE_SECT_RANK, i + 1);
        // Never used : remove ?
        //const int laps = (int)GfParmGetNum(results, path, RE_ATTR_LAPS, NULL, 0);
        int rank = i + 1;
        int gains = (int)GfParmGetNum(results, path, RE_ATTR_GAINS, NULL, 0);
        int damage = (int)GfParmGetNum(results, path, RE_ATTR_DAMMAGES, NULL, 0);
        double totalpenalty = GfParmGetNum(results, path, RE_ATTR_TOTALPENALTY, NULL, 0);

        //Rank
        snprintf(buf, sizeof(buf), "%d", i+1);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Rank", true, // From template.
                                   buf, GFUI_TPL_X, y);

        //Driver short name
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "DriverName", true, // From template.
                                   GfParmGetStr(results, path, RE_ATTR_SNAME, ""), GFUI_TPL_X, y);

        //Nation
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Nation", true, // From template.
                                   GfParmGetStr(results, path, RE_ATTR_NATION, ""), GFUI_TPL_X, y);

        //Driver type
        const std::string strModName = GfParmGetStr(results, path, RE_ATTR_MODULE, "");
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "DriverType", true, // From template.
                                   GfDriver::getType(strModName).c_str(), GFUI_TPL_X, y);

        //Car
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "CarModel", true, // From template.
                                   GfParmGetStr(results, path, RE_ATTR_CAR, ""), GFUI_TPL_X, y);

        //Best lap
        str = GfTime2Str(GfParmGetNum(results, path, RE_ATTR_BEST_LAP_TIME, NULL, 0), 0, false, 3);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "BestLapTime", true, // From template.
                                   str, GFUI_TPL_X, y);

        if(strModName == ROB_VAL_HUMAN)
        {
            GfLogInfo("# Race Results qualification rank = %i - Career = %i\n", rank, rmCareer);

            if((rank < 7) && rmUpdate2)
            {
                float mult;
                double mult2 = LegacyMenu::self().getSkillMult();

                GfLogDebug(" # Race Results Qualification Licence = %i\n", rmLicence);

                if (rmLicence == true)
                    mult = 1;
                else
                    mult = 0.5f;

                int skillmult = (int)(gains * mult * mult2);

                if (skillmult > 0)
                    rmCredit += (int)(skillmult);

                GfLogInfo("Qualif : rmSkillMult = %.2f - result credit = %i - rmCredit = %lu\n",
                          mult2, skillmult, rmCredit);

                rmFame += ((7 - rank) * rmSkillMult) - ((damage * 0.0001) + totalpenalty);

                GfLogInfo("Qualif : Credit + %i - Rank = %i - Credit = %lu\n", gains, rank, rmCredit);
            }
            else
            {
                rmFame += ((7 - (rank * 0.5)) - ((damage * 0.0001) + totalpenalty));
            }

            rmUpdateHumanPlayer(info);
            rmUpdate = false;
            rmUpdate2 = false;
        }

        free(str);

        // Next line.
        y -= yLineShift;
    }//for i

    if (start > 0)
    {
        RmPrevRace.prevHdle = prevHdle;
        RmPrevRace.info     = info;
        RmPrevRace.start    = start - nMaxLines;
        RmPrevRace.title    = pszTitle;
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "PreviousPageArrow",
                                    (void*)&RmPrevRace, rmChgQualifScreen);
        GfuiAddKey(rmScrHdle, GFUIK_PAGEUP,   "Previous Results", (void*)&RmPrevRace, rmChgQualifScreen, NULL);
    }

    // Add "Continue" button
    GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "ContinueButton", prevHdle, GfuiScreenReplace);

    //Create 'save' button in the bottom right
    //rmSaveButtonId = GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "savebutton", info, rmSaveRes);

    if (i < nbCars)
    {
        RmNextRace.prevHdle = prevHdle;
        RmNextRace.info     = info;
        RmNextRace.start    = start + nMaxLines;
        RmNextRace.title    = pszTitle;
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "NextPageArrow",
                                    (void*)&RmNextRace, rmChgQualifScreen);
        GfuiAddKey(rmScrHdle, GFUIK_PAGEDOWN, "Next Results", (void*)&RmNextRace, rmChgQualifScreen, NULL);
    }

    GfuiMenuDefaultKeysAdd(rmScrHdle);
    GfuiAddKey(rmScrHdle, GFUIK_ESCAPE, "Continue", prevHdle, GfuiScreenReplace, NULL);
    GfuiAddKey(rmScrHdle, GFUIK_RETURN, "Continue", prevHdle, GfuiScreenReplace, NULL);

    GfuiScreenActivate(rmScrHdle);
}

static void
rmChgStandingScreen(void *vprc)
{
    void		*prevScr = rmScrHdle;
    tRaceCall 	*prc = (tRaceCall*)vprc;

    RmShowStandings(prc->prevHdle, prc->info, prc->start);
    GfuiScreenRelease(prevScr);
}

/**
 * RmShowStandings
 *
 * Shows a results page, with optional prev/next results page buttons
 *
 * @param prevHdle	handle for previous results page
 * @param info	race results information
 * @param start	page number
*/
void
RmShowStandings(void *prevHdle, tRmInfo *info, int start)
{
    int			i;
    static char	buf[256];
    static char	path[512];
    void		*results = info->results;
    int         curTrackIdx = 0;
    unsigned    nCurrEventIndex = 0;

    rmCareer = LegacyMenu::self().getCareerMode();
    rmChallenge = LegacyMenu::self().getChallengeMode();
    rmEndurance = LegacyMenu::self().getEnduranceMode();
    rmChampionship = LegacyMenu::self().getChampionshipMode();
    rmSkillMult = LegacyMenu::self().getSkillMult();
    int d = LegacyMenu::self().getDammages();

    GfLogDebug("# race results Entering Standings menu\n");

    // Create screen, load menu XML descriptor and create static controls.
    rmScrHdle = GfuiScreenCreate();
    void *hmenu = GfuiMenuLoad("standingsmenu.xml");
    GfuiMenuCreateStaticControls(rmScrHdle, hmenu);

    // Create variable title label (with group info for the Career mode).
    const int titleId = GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Title");
    GfRaceManager* pRaceMan = LmRaceEngine().race()->getManager();

    if (pRaceMan->hasSubFiles())
    {
        const char* pszGroup = GfParmGetStr(info->params, RM_SECT_HEADER, RM_ATTR_NAME, "<no group>");
        snprintf(buf, sizeof(buf), "%s - %s", info->_reName, pszGroup);
    }
    else
        snprintf(buf, sizeof(buf), "%s", info->_reName);

    GfuiLabelSetText(rmScrHdle, titleId, buf);

    // Create variable subtitle label.
    const char* pszSessionName;
    const char* pszTrackName;

    if (pRaceMan->hasSubFiles())
    {
        // Career mode : Can't rely on GfRaceManager/GfRace, they don't support Career mode yet.
        pszSessionName = info->_reRaceName;
        curTrackIdx = (int)GfParmGetNum(results, RE_SECT_CURRENT, RE_ATTR_CUR_TRACK, NULL, 1) - 1;
        snprintf(path, sizeof(path), "%s/%d", RM_SECT_TRACKS, curTrackIdx);
        pszTrackName = GfParmGetStr(info->params, path, RM_ATTR_NAME, "<unkown track>");
    }
    else
    {
        // Non-Career mode : The session is the _last_ one ; the track is the _previous_ one.
        nCurrEventIndex = (unsigned)GfParmGetNum(results, RE_SECT_CURRENT, RE_ATTR_CUR_TRACK, NULL, 1);
        pszSessionName = pRaceMan->getSessionName(pRaceMan->getSessionCount() - 1).c_str();
        pszTrackName =
                pRaceMan->getPreviousEventTrack(nCurrEventIndex - 1)->getName().c_str();
        GfLogDebug(" Race Count = %i - Championship count race = %i\n\n", nCurrEventIndex,
                   pRaceMan->getEventCount());

        if(nCurrEventIndex == pRaceMan->getEventCount())
        {
            rmEndChampionship = true;
            GfLogDebug("# Championship name = %s - Current Race = %i\n", info->_reName, nCurrEventIndex);
        }
    }

    snprintf(buf, sizeof(buf), "%s at %s", pszSessionName, pszTrackName);
    const int subTitleId = GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "SubTitle");
    GfuiLabelSetText(rmScrHdle, subTitleId, buf);

    // Get layout properties.
    const int nMaxLines = (int)GfuiMenuGetNumProperty(hmenu, "nMaxResultLines", 15);
    const int yTopLine = (int)GfuiMenuGetNumProperty(hmenu, "yTopLine", 400);
    const int yLineShift = (int)GfuiMenuGetNumProperty(hmenu, "yLineShift", 20);

    // List results line by line, paginated
    int y = yTopLine;
    const int nbCars = (int)GfParmGetEltNb(results, RE_SECT_STANDINGS);

    for (i = start; i < MIN(start + nMaxLines, nbCars); i++)
    {
        snprintf(path, sizeof(path), "%s/%d", RE_SECT_STANDINGS, i + 1);

        //Rank
        snprintf(buf, sizeof(buf), "%d", i+1);
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Rank", true, // From template.
                                   buf, GFUI_TPL_X, y);
        int rank = i+1;

        //Driver short name
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "DriverName", true, // From template.
                                   GfParmGetStr(results, path, RE_ATTR_SNAME, ""), GFUI_TPL_X, y);

        //Driver type
        const std::string strModName = GfParmGetStr(results, path, RE_ATTR_MODULE, "");
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "DriverType", true, // From template.
                                   GfDriver::getType(strModName).c_str(), GFUI_TPL_X, y);

        //Car
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "CarModel", true, // From template.
                                   GfParmGetStr(results, path, RE_ATTR_CAR, ""), GFUI_TPL_X, y);

        //Points
        snprintf(buf, sizeof(buf), "%d", (int)GfParmGetNum(results, path, RE_ATTR_POINTS, NULL, 0));
        GfuiMenuCreateLabelControl(rmScrHdle, hmenu, "Points", true, // From template.
                                   buf, GFUI_TPL_X, y);

        int points = atoi(buf);

        // Next line.
        y -= yLineShift;	//Next line

        if(strModName == ROB_VAL_HUMAN)
        {
            GfLogDebug("# Race Results rank = %i - Current Event = %i - Championship = %i - Career = %i\n",
                       rank, nCurrEventIndex, rmChampionship, rmCareer);

            if((rank < 7) && nCurrEventIndex == 1 && rmEndChampionship && rmCareer == false)
            {
                std::string strEnd;
                std::string str;
                str = info->_reName;
                strEnd = str.substr(str.size() - 14);

                if ((strcmp(strEnd.c_str(), "Championship 1") == 0) && rmRanking[rmIndex] < 5)
                    rmRanking[rmIndex] += 1;
                else if((strcmp(strEnd.c_str(), "Championship 2") == 0) && rmRanking[rmIndex] < 6)
                    rmRanking[rmIndex] += 1;
                else if((strcmp(strEnd.c_str(), "Championship 3") == 0) && rmRanking[rmIndex] < 7)
                    rmRanking[rmIndex] += 1;

                rmUpdate = true;

                GfLogDebug(" #rmIndex = %i\n", rmRanking[rmIndex]);
            }
            else if((rank < 7) && nCurrEventIndex == 1 && rmEndChampionship && rmCareer == true)
            {
                switch (rank)
                {
                case 1:
                    rmCredit += 10000 * rmSkillMult;
                    rmFame += 20.0;
                    GfLogDebug("Credit + 1000 - Rank = %i\n", rank);
                    break;
                case 2:
                    rmCredit += 9000 * rmSkillMult;
                    rmFame += 18.0;
                    GfLogDebug("Credit + 900 - Rank = %i\n", rank);
                    break;
                case 3:
                    rmCredit += 8000 * rmSkillMult;
                    rmFame += 16.0;
                    GfLogDebug("Credit + 800 - Rank = %i\n", rank);
                    break;
                case 4:
                    rmCredit += 5000 * rmSkillMult;
                    rmFame += 14.0;
                    GfLogDebug("Credit + 500 - Rank = %i\n", rank);
                    break;
                case 5:
                    rmCredit += 2500 * rmSkillMult;
                    rmFame += 10.0;
                    GfLogDebug("Credit + 250 - Rank = %i\n", rank);
                    break;
                case 6:
                    rmCredit += 1500 * rmSkillMult;
                    rmFame += 6.0;
                    GfLogDebug("Credit + 150 - Rank = %i\n", rank);
                    break;
                default:
                    rmCredit += 0;
                    rmFame += 2.0;
                    break;
                }
            }

            if(nCurrEventIndex == 1 && rmChampionship == true && rmEndChampionship == true
                    && rmCareer == true)
            {
                int index = rmLevel[rmIndex] % 10;

                if (index == 9)
                    rmLevel[rmIndex] += 2;
                else
                    rmLevel[rmIndex] += 1;

                GfLogDebug("Standing : Level = %i - Fame = %.2f - Credit = %lu\n",
                           rmLevel[rmIndex], rmFame, rmCredit);
            }

            if(nCurrEventIndex == 1 && rmEndChampionship == true)
            {
                GfLogDebug("# Race Results Show Standing Update Human Player !\n");
                rmUpdateHumanPlayer(info);
                rmUpdate2 = false;
                int skill = LegacyMenu::self().getPlayerSkill();
                double aiskill = LegacyMenu::self().getAiSkill();
                tSkillLevel level = static_cast<tSkillLevel>(skill);

                if (skill > 3 && aiskill < 1.5)
                {
                    rmUpdateAllOfFame(level, info->_reName, carName.c_str(), rank, points, d);
                }

                rmEndChampionship = false;
                rmUpdate = false;
            }
        } // ROB_VAL_HUMAN
    }//for i

    // If not on first page, show 'previous results' button on the bottom left
    if (start > 0)
    {
        RmPrevRace.prevHdle = prevHdle;
        RmPrevRace.info     = info;
        RmPrevRace.start    = start - nMaxLines;
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "PreviousPageArrow",
                                    (void*)&RmPrevRace, rmChgStandingScreen);
        GfuiAddKey(rmScrHdle, GFUIK_PAGEUP, "Previous Results", (void*)&RmPrevRace, rmChgStandingScreen, NULL);
    }//if start

    // Add "Continue" button in the bottom left
    GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "ContinueButton", prevHdle, GfuiScreenReplace);

    // Add "save" button in the bottom right, but disable it when Career mode.
    rmSaveButtonId = GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "SaveButton", info, rmSaveRes);
    if (LmRaceEngine().race()->getManager()->hasSubFiles())
        GfuiEnable(rmScrHdle, rmSaveButtonId, GFUI_DISABLE);

    // If there is a next page, show 'next results' button on the bottom extreme right
    if (i < nbCars)
    {
        RmNextRace.prevHdle = prevHdle;
        RmNextRace.info     = info;
        RmNextRace.start    = start + nMaxLines;
        GfuiMenuCreateButtonControl(rmScrHdle, hmenu, "NextPageArrow",
                                    (void*)&RmNextRace, rmChgStandingScreen);
        GfuiAddKey(rmScrHdle, GFUIK_PAGEDOWN, "Next Results", (void*)&RmNextRace, rmChgStandingScreen, NULL);
    }//if i

    GfuiMenuDefaultKeysAdd(rmScrHdle);
    GfuiAddKey(rmScrHdle, GFUIK_ESCAPE, "Continue", prevHdle, GfuiScreenReplace, NULL);
    GfuiAddKey(rmScrHdle, GFUIK_RETURN, "Continue", prevHdle, GfuiScreenReplace, NULL);

    GfuiScreenActivate(rmScrHdle);
}//RmShowStandings

void
RmShowResults(void *prevHdle, tRmInfo *info)
{

    GfLogDebug("# Race Result start Show Results\n");
    rmCareer = LegacyMenu::self().getCareerMode();
    rmUpdate2 = true;
    std::string championship = info->_reName;
    championship.find("Championship") != std::string::npos
            ? LegacyMenu::self().setChampionshipMode(true)
            : LegacyMenu::self().setChampionshipMode(false);

    championship.find("Challenge") != std::string::npos
            ? LegacyMenu::self().setChallengeMode(true)
            : LegacyMenu::self().setChallengeMode(false);

    championship.find("Endurance") != std::string::npos
            ? LegacyMenu::self().setEnduranceMode(true)
            : LegacyMenu::self().setEnduranceMode(false);

    GfLogDebug("# RmshowResults Career = %i\n", rmCareer);
    GfLogDebug("# RmShowResults Championship = %i\n", LegacyMenu::self().getChampionshipMode());
    GfLogDebug("# RmShowResults Challenge    = %i\n", LegacyMenu::self().getChallengeMode());
    GfLogDebug("# RmShowResults Endurance    = %i\n", LegacyMenu::self().getEnduranceMode());

    switch (info->s->_raceType)
    {
    case RM_TYPE_PRACTICE:
    {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "%s/%s", info->track->name, RE_SECT_DRIVERS);
        int nCars = GfParmGetEltNb(info->results, buffer);
        bool bQualif = (nCars != 1);

        // Career special case : Practice results show multiple cars,
        // but only 1 driver, so no 'rank' section.
        // TODO: Rather fix the Career code ?
        if (bQualif)
        {
            snprintf(buffer, sizeof(buffer), "%s/%s/%s/%s", info->track->name, RE_SECT_RESULTS, info->_reRaceName, RE_SECT_RANK);
            nCars = (int)GfParmGetEltNb(info->results, buffer);
            GfLogDebug("RmShowResults: %d elements in %s\n", nCars, buffer);
            bQualif = bQualif && (nCars != 0);
        }

        if (bQualif)
            rmQualifResults(prevHdle, info, "Practice", 0);
        else
            rmPracticeResults(prevHdle, info, 0);
        break;
    }

    case RM_TYPE_RACE:
        rmRaceResults(prevHdle, info, 0);
        break;

    case RM_TYPE_QUALIF:
        rmQualifResults(prevHdle, info, "Qualification", 0);
        break;
    }//switch raceType
}//RmShowResults
