/***************************************************************************

    file                 : playerconfig.cpp
    created              : Wed Apr 26 20:05:12 CEST 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: playerconfig.cpp 8159 2022-05-14 11:10:46Z beaglejoe $

***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <deque>
#include <string>

#include <webserver.h>
#include <portability.h>
#include <tgfclient.h>
#include <tgfdata.h>
#include <cars.h>
#include <robot.h>
#include <playerpref.h>
#include <graphic.h>
#include <gui.h>
#include <drivers.h>

#include "controlconfig.h"
#include "playerconfig.h"

class GfCars;

static const char *PlayerNamePrompt	= "-- Enter name --";
static const char *NoPlayer = "-- No one --";
static const char *HumanDriverModuleName  = "human";
static const char *DefaultCarName  = "none";
static const char *DefaultCarRace  = "09gp5-fmc";
static const char *DefaultCarCareer  = "none";
static const char *DefaultCategoryName = "2009 Grand Prix 5";
static const char *DefaultCodeName = "PLA";
static const char *DefaultNationName = "FR";
static const char *DefaultChampionship = "none";
static const char *DefaultWebserverusername  = "username";
static const char *DefaultWebserverpassword  = "password";

static const char *strCategory[] = { "Supercars Class C", "2009 Grand Prix 5", "Supercars Class B", "2010 Grand Prix 4",
                                     "Supercars Class A", "1991 Grand Prix", "Long Day Series GT2", "2001 Grand Prix",
                                     "Long Day Series GT1", "2001 GP American", "Long Day Series P2", "2011 GP American",
                                     "Long Day Series P1", "2001 GP American", "1936 Grand Prix", "1967 Grand Prix" };

static const char *strNation[] = { "AC", "AD", "AE", "AF", "AG", "AI", "AL", "AM", "AO", "AQ", "AR", "AS", "AT", "AU", "AW",
                                 "AX", "AZ", "BA", "BB", "BD", "BE", "BF", "BG", "BH", "BI", "BJ", "BL", "BM", "BN", "BO",
                                 "BQ", "BR", "BS", "BT", "BV", "BW", "BY", "BZ", "CA", "CC", "CD", "CEFTA", "CF", "CG", "CH",
                                 "CI", "CK", "CL", "CM", "CN", "CO", "CP", "CR", "CU", "CV", "CW", "CX", "CY", "CZ", "DE",
                                 "DG", "DJ", "DK", "DM", "DO", "DZ", "EA", "EC", "EE", "EG", "EH", "ER", "ES", "ES-CT", "ES-GA",
                                 "ES-PV", "ET", "EU", "FI", "FJ", "FK", "FM", "FO", "FR", "GA", "GB", "GB-ENG", "GB-NIR", "GB-SCT",
                                 "GB-WLS", "GD", "GE", "GF", "GG", "GH", "GI", "GL", "GM", "GN", "GP", "GQ", "GR", "GS", "GT",
                                 "GU", "GW", "GY", "HK", "HM", "HN", "HR", "HT", "HU", "IC", "ID", "IE", "IL", "IM", "IN",
                                 "IO", "IQ", "IR", "IS", "IT", "JE", "JM", "JO", "JP", "KE", "KG", "KH", "KI", "KM", "KN",
                                 "KP", "KR", "KW", "KY", "KZ", "LA", "LB", "LC", "LI", "LK", "LR", "LS", "LT", "LU", "LV",
                                 "LY", "MA", "MC", "MD", "ME", "MF", "MG", "MH", "MK", "ML", "MM", "MN", "MO", "MP", "MQ",
                                 "MR", "MS", "MT", "MU", "MV","MW", "MX", "MY", "MZ", "NA", "NC", "NE", "NF", "NG", "NI",
                                 "NL", "NO", "NP", "NR", "NU", "NZ", "OM", "PA", "PE", "PF", "PG", "PH", "PK", "PL", "PM",
                                 "PN", "PR", "PS", "PT", "PW", "PY", "QA", "RE", "RO", "RS", "RU", "RW", "SA", "SB", "SC",
                                 "SD", "SE", "SG", "SH", "SI", "SJ", "SK", "SL", "SM", "SN", "SO", "SR", "SS", "ST", "SV",
                                 "SX", "SY", "SZ", "TA", "TC", "TD", "TF", "TG", "TH", "TJ", "TK", "TL", "TM", "TM", "TO",
                                 "TR", "TT", "TV", "TW", "TZ", "UA", "UG", "UM", "UN", "US", "UY", "UZ", "VA", "VC", "VE",
                                 "VG", "VI", "VN", "VU", "WF", "WS", "WW", "XK", "XX", "YE", "YT", "ZA", "ZM", "ZW" };


static const char *SkillLevelString[] = ROB_VALS_LEVEL;
static const int NbSkillLevels = sizeof(SkillLevelString) / sizeof(SkillLevelString[0]);

static char buf[1024];

static const float LabelColor[] = {1.0, 0.0, 1.0, 1.0};

static int	ScrollList;
static void	*ScrHandle = NULL;
static void	*PrevScrHandle = NULL;

// The car selection menu.
//static RmHumanGarageMenu GarageMenu;

static void	*PrefHdle = NULL;
static void	*PlayerHdle = NULL;
static void	*GraphHdle = NULL;
static void *GarageHdle = NULL;
static void *CurCarName = NULL;

static int CategoryEditId;
static int NameEditId;
static int NationEditId;
static int RaceNumEditId;
static int GearChangeEditId;
static int SkillEditId;
static int AutoReverseEditId;
static int AutoReverseLeftId;
static int AutoReverseRightId;
static int AutoReverseLabelId;
static int CopyButtonId;
static int RemoveButtonId;
static int ProfileButtonId;
static int WebUsernameEditId;
static int WebPasswordEditId;
static int WebServerCheckboxId;
static int WebServerTestLoginId;

static int olderIndex;

/* Struct to define a generic ("internal name / id", "displayable name") pair */
typedef struct tInfo
{
    char	*name;
    char	*dispname;
} tInfo;

/* Player info struct */
struct tPlayerInfo
{
public:

    tPlayerInfo(const char *name = HumanDriverModuleName, const char *dispname = 0,
                const char *sname = 0, const char *codename = 0, const char *nation = 0,
                const char *defcarname = 0, const char *defcarrace = 0, const char *defcarcareer = 0,
                const char *defcategoryname = 0, const char *defchamp = 0, int racenumber = 0,
                tSkillLevel skilllevel = ARCADE,
                int rank0 = 0, int rank1 = 0, int rank2 = -1, int rank3 = -1, int rank4 = -1,
                int rank5 = -1, int rank6 = -1, int rank7 = -1, int rank8 = -1, int rank9 = -1,
                int rank10 = -1, int rank11 = -1, int rank12 = -1, int rank13 = -1, int rank14 = -1,
                int rank15 = -1, int rank16 = -1, int rank17 = -1, int rank18 = -1, int rank19 = -1,
                int rank20 = -1, int rank21 = -1, int rank22 = -1, int rank23 = -1, int rank24 = -1,
                int rank25 = -1, int rank26 = -1, int rank27 = -1, int rank28 = -1, int rank29 = -1,
                int level0 = 0, int level1 = 0, int level2 = 0, int level3 = 0, int level4 = 0,
                int level5 = 0, int level6 = 0, int level7 = 0, int level8 = 0, int level9 = 0,
                int level10 = 0, int level11 = 0, int level12 = 0, int level13 = 0, int level14 = 0,
                int level15 = 0, int level16 = 0, int level17 = 0,
                int special0 = 0, int special1 = 0, int special2 = 0, int special3 = 0,
                int special4 = 0, int special5 = 0, int special6 = 0, int special7 = 0,
                int special8 = 0, int special9 = 0, int special10 = 0, int special11 = 0,
                int special12 = 0, int special13 = 0, int special14 = 0, int special15 = 0,
                int special16 = 0, int special17 = 0, double fame0 = 0.0, double fame1 = 0.0,
                double fame2 = 0.0, double fame3 = 0.0, double fame4 = 0.0, double fame5 = 0.0,
                double fame6 = 0.0, double fame7 = 0.0, double fame8 = 0.0, double fame9 = 0.0,
                double fame10 = 0.0, double fame11 = 0.0, double fame12 = 0.0, double fame13 = 0.0,
                double fame14 = 0.0, double fame15 = 0.0, double fame16 = 0.0, double fame17 = 0.0,
                bool licence = false, unsigned long int credit = 10000, float *color = 0,
                int maxranking = 2, int selectedcategory = 0, int selectednation = 0,
                tGearChangeMode gearchangemode = GEAR_MODE_AUTO, int autoreverse = 0,
                const char *webserverusername = 0,
                const char *webserverpassword = 0,
                int webserverenabled = 0)
    {
        _info.name = 0;
        setName(name);
        _info.dispname = 0;
        setDispName(dispname);
        _defcode = 0;
        setDefaultCodeName(codename);
        _defcarname = 0;
        setDefaultCarName(defcarname);
        _defcategory = 0;
        setDefaultCategoryName(defcategoryname);
        _defcarrace = 0;
        setDefaultCarRace(defcarrace);
        _defcarcareer = 0;
        setDefaultCarCareer(defcarcareer);
        _defchamp = 0;
        setDefaultChamp(defchamp);
        _defnation = 0;
        setDefaultNationName(nation);
        _racenumber = racenumber;
        _credit = credit;
        _gearchangemode = gearchangemode;
        _skilllevel = skilllevel;
        _numranking[0] = rank0;
        _numranking[1] = rank1;
        _numranking[2] = rank2;
        _numranking[3] = rank3;
        _numranking[4] = rank4;
        _numranking[5] = rank5;
        _numranking[6] = rank6;
        _numranking[7] = rank7;
        _numranking[8] = rank8;
        _numranking[9] = rank9;
        _numranking[10] = rank10;
        _numranking[11] = rank11;
        _numranking[12] = rank12;
        _numranking[13] = rank13;
        _numranking[14] = rank14;
        _numranking[15] = rank15;
        _numranking[16] = rank16;
        _numranking[17] = rank17;
        _numranking[18] = rank18;
        _numranking[19] = rank19;
        _numranking[20] = rank20;
        _numranking[21] = rank21;
        _numranking[22] = rank22;
        _numranking[23] = rank23;
        _numranking[24] = rank24;
        _numranking[25] = rank25;
        _numranking[26] = rank26;
        _numranking[27] = rank27;
        _numranking[28] = rank28;
        _numranking[29] = rank29;
        _level[0] = level0;
        _level[1] = level1;
        _level[2] = level2;
        _level[3] = level3;
        _level[4] = level4;
        _level[5] = level5;
        _level[6] = level6;
        _level[7] = level7;
        _level[8] = level8;
        _level[9] = level9;
        _level[10] = level10;
        _level[11] = level11;
        _level[12] = level12;
        _level[13] = level13;
        _level[14] = level14;
        _level[15] = level15;
        _level[16] = level16;
        _level[17] = level17;
        _special[0] = special0;
        _special[1] = special1;
        _special[2] = special2;
        _special[3] = special3;
        _special[4] = special4;
        _special[5] = special5;
        _special[6] = special6;
        _special[7] = special7;
        _special[8] = special8;
        _special[9] = special9;
        _special[10] = special10;
        _special[11] = special11;
        _special[12] = special12;
        _special[13] = special13;
        _special[14] = special14;
        _special[15] = special15;
        _special[16] = special16;
        _special[17] = special17;
        _fame[0] = fame0;
        _fame[1] = fame1;
        _fame[2] = fame2;
        _fame[3] = fame3;
        _fame[4] = fame4;
        _fame[5] = fame5;
        _fame[6] = fame6;
        _fame[7] = fame7;
        _fame[8] = fame8;
        _fame[9] = fame9;
        _fame[10] = fame10;
        _fame[11] = fame11;
        _fame[12] = fame12;
        _fame[13] = fame13;
        _fame[14] = fame14;
        _fame[15] = fame15;
        _fame[16] = fame16;
        _fame[17] = fame17;
        _licence = licence;
        _maxRanking = maxranking;
        _selectedCategory = selectedcategory;
        _selectedNation = selectednation;
        _autoreverse = autoreverse;
        _webserverusername = 0;
        setWebserverusername(webserverusername);
        _webserverpassword = 0;
        setWebserverpassword(webserverpassword);
        _webserverenabled = webserverenabled;
        _color[0] = color ? color[0] : 1.0;
        _color[1] = color ? color[1] : 1.0;
        _color[2] = color ? color[2] : 0.5;
        _color[3] = color ? color[3] : 1.0;
    }

    tPlayerInfo(const tPlayerInfo &src)
    {
        _info.name = 0;
        setName(src._info.name);
        _info.dispname = 0;
        setDispName(src._info.dispname);
        _defnation = 0;
        setDefaultNationName(src._defnation);
        _defcarname = 0;
        setDefaultCarName(src._defcarname);
        _defcategory = 0;
        setDefaultCategoryName(src._defcategory);
        _defcarrace = 0;
        setDefaultCarRace(src._defcarname);
        _defcarcareer = 0;
        setDefaultCarCareer(src._defcarcareer);
        _defchamp = 0;
        setDefaultChamp(src._defchamp);
        _racenumber = src._racenumber;
        _credit = src._credit;
        _gearchangemode = src._gearchangemode;
        _skilllevel = src._skilllevel;
        _numranking[0] = src._numranking[0];
        _numranking[1] = src._numranking[1];
        _numranking[2] = src._numranking[2];
        _numranking[3] = src._numranking[3];
        _numranking[4] = src._numranking[5];
        _numranking[5] = src._numranking[5];
        _numranking[6] = src._numranking[6];
        _numranking[7] = src._numranking[7];
        _numranking[8] = src._numranking[8];
        _numranking[9] = src._numranking[9];
        _numranking[10] = src._numranking[10];
        _numranking[11] = src._numranking[11];
        _numranking[12] = src._numranking[12];
        _numranking[13] = src._numranking[13];
        _numranking[14] = src._numranking[14];
        _numranking[15] = src._numranking[15];
        _numranking[16] = src._numranking[16];
        _numranking[17] = src._numranking[17];
        _numranking[18] = src._numranking[18];
        _numranking[19] = src._numranking[19];
        _numranking[20] = src._numranking[20];
        _numranking[21] = src._numranking[21];
        _numranking[22] = src._numranking[22];
        _numranking[23] = src._numranking[23];
        _numranking[24] = src._numranking[24];
        _numranking[25] = src._numranking[25];
        _numranking[26] = src._numranking[26];
        _numranking[27] = src._numranking[27];
        _numranking[28] = src._numranking[28];
        _numranking[29] = src._numranking[29];
        _level[0]       = src._level[0];
        _level[1]       = src._level[1];
        _level[2]       = src._level[2];
        _level[3]       = src._level[3];
        _level[4]       = src._level[5];
        _level[5]       = src._level[5];
        _level[6]       = src._level[6];
        _level[7]       = src._level[7];
        _level[8]       = src._level[8];
        _level[9]       = src._level[9];
        _level[10]      = src._level[10];
        _level[11]      = src._level[11];
        _level[12]      = src._level[12];
        _level[13]      = src._level[13];
        _level[14]      = src._level[14];
        _level[15]      = src._level[15];
        _level[16]      = src._level[16];
        _level[17]      = src._level[17];
        _special[0]     = src._special[0];
        _special[1]     = src._special[1];
        _special[2]     = src._special[2];
        _special[3]     = src._special[3];
        _special[4]     = src._special[4];
        _special[5]     = src._special[5];
        _special[6]     = src._special[6];
        _special[7]     = src._special[7];
        _special[8]     = src._special[8];
        _special[9]     = src._special[9];
        _special[10]    = src._special[10];
        _special[11]    = src._special[11];
        _special[12]    = src._special[12];
        _special[13]    = src._special[13];
        _special[14]    = src._special[14];
        _special[15]    = src._special[15];
        _special[16]    = src._special[16];
        _special[17]    = src._special[17];
        _fame[0]       = src._fame[0];
        _fame[1]       = src._fame[1];
        _fame[2]       = src._fame[2];
        _fame[3]       = src._fame[3];
        _fame[4]       = src._fame[5];
        _fame[5]       = src._fame[5];
        _fame[6]       = src._fame[6];
        _fame[7]       = src._fame[7];
        _fame[8]       = src._fame[8];
        _fame[9]       = src._fame[9];
        _fame[10]      = src._fame[10];
        _fame[11]      = src._fame[11];
        _fame[12]      = src._fame[12];
        _fame[13]      = src._fame[13];
        _fame[14]      = src._fame[14];
        _fame[15]      = src._fame[15];
        _fame[16]      = src._fame[16];
        _fame[17]      = src._fame[17];
        _licence        = src._licence;
        _maxRanking = src._maxRanking;
        _selectedCategory = src._selectedCategory;
        _selectedNation = src._selectedNation;
        _autoreverse = src._autoreverse;
        _webserverusername = 0;
        setWebserverusername(src._webserverusername);
        _webserverpassword = 0;
        setWebserverpassword(src._webserverpassword);
        _webserverenabled = src._webserverenabled;
        _color[0] = src._color[0];
        _color[1] = src._color[1];
        _color[2] = src._color[2];
        _color[3] = src._color[3];
    }

    const char *name()  const { return _info.name; };
    const char *dispName()  const { return _info.dispname; }
    const char *defaultCodeName() const { return _defcode; }
    const char *defaultNationName() const { return _defnation; }
    const char *defaultCarName()  const { return _defcarname; }
    const char *defaultCarRace() const { return _defcarrace; }
    const char *defaultCarCareer() const { return _defcarcareer; }
    const char *defaultCategoryName() const { return _defcategory; }
    const char *defaultChamp() const { return _defchamp; }
    int raceNumber() const { return _racenumber; }
    unsigned long int credit() const { return _credit; }
    tGearChangeMode gearChangeMode() const { return _gearchangemode; }
    float color(int idx) const { return (idx >= 0 && idx < 4) ? _color[idx] : 0.0; }
    tSkillLevel skillLevel() const { return _skilllevel; }
    int Category() const { return _selectedCategory; }
    int Nation() const { return _selectedNation; }
    int ranking0() const { return _numranking[0]; }
    int ranking1() const { return _numranking[1]; }
    int ranking2() const { return _numranking[2]; }
    int ranking3() const { return _numranking[3]; }
    int ranking4() const { return _numranking[4]; }
    int ranking5() const { return _numranking[5]; }
    int ranking6() const { return _numranking[6]; }
    int ranking7() const { return _numranking[7]; }
    int ranking8() const { return _numranking[8]; }
    int ranking9() const { return _numranking[9]; }
    int ranking10() const { return _numranking[10]; }
    int ranking11() const { return _numranking[11]; }
    int ranking12() const { return _numranking[12]; }
    int ranking13() const { return _numranking[13]; }
    int ranking14() const { return _numranking[14]; }
    int ranking15() const { return _numranking[15]; }
    int ranking16() const { return _numranking[16]; }
    int ranking17() const { return _numranking[17]; }
    int ranking18() const { return _numranking[18]; }
    int ranking19() const { return _numranking[19]; }
    int ranking20() const { return _numranking[20]; }
    int ranking21() const { return _numranking[21]; }
    int ranking22() const { return _numranking[22]; }
    int ranking23() const { return _numranking[23]; }
    int ranking24() const { return _numranking[24]; }
    int ranking25() const { return _numranking[25]; }
    int ranking26() const { return _numranking[26]; }
    int ranking27() const { return _numranking[27]; }
    int ranking28() const { return _numranking[28]; }
    int ranking29() const { return _numranking[29]; }
    int level0() const { return _level[0]; }
    int level1() const { return _level[1]; }
    int level2() const { return _level[2]; }
    int level3() const { return _level[3]; }
    int level4() const { return _level[4]; }
    int level5() const { return _level[5]; }
    int level6() const { return _level[6]; }
    int level7() const { return _level[7]; }
    int level8() const { return _level[8]; }
    int level9() const { return _level[9]; }
    int level10() const { return _level[10]; }
    int level11() const { return _level[11]; }
    int level12() const { return _level[12]; }
    int level13() const { return _level[13]; }
    int level14() const { return _level[14]; }
    int level15() const { return _level[15]; }
    int level16() const { return _level[16]; }
    int level17() const { return _level[17]; }
    int special0() const { return _special[0]; }
    int special1() const { return _special[1]; }
    int special2() const { return _special[2]; }
    int special3() const { return _special[3]; }
    int special4() const { return _special[4]; }
    int special5() const { return _special[5]; }
    int special6() const { return _special[6]; }
    int special7() const { return _special[7]; }
    int special8() const { return _special[8]; }
    int special9() const { return _special[9]; }
    int special10() const { return _special[10]; }
    int special11() const { return _special[11]; }
    int special12() const { return _special[12]; }
    int special13() const { return _special[13]; }
    int special14() const { return _special[14]; }
    int special15() const { return _special[15]; }
    int special16() const { return _special[16]; }
    int special17() const { return _special[17]; }
    double fame0() const { return _fame[0]; }
    double fame1() const { return _fame[1]; }
    double fame2() const { return _fame[2]; }
    double fame3() const { return _fame[3]; }
    double fame4() const { return _fame[4]; }
    double fame5() const { return _fame[5]; }
    double fame6() const { return _fame[6]; }
    double fame7() const { return _fame[7]; }
    double fame8() const { return _fame[8]; }
    double fame9() const { return _fame[9]; }
    double fame10() const { return _fame[10]; }
    double fame11() const { return _fame[11]; }
    double fame12() const { return _fame[12]; }
    double fame13() const { return _fame[13]; }
    double fame14() const { return _fame[14]; }
    double fame15() const { return _fame[15]; }
    double fame16() const { return _fame[16]; }
    double fame17() const { return _fame[17]; }
    bool licence() const { return _licence; }
    int maxRanking() const { return _maxRanking; }
    int autoReverse() const { return _autoreverse; }
    const char *webserverusername()  const { return _webserverusername; }
    const char *webserverpassword()  const { return _webserverpassword; }
    int webserverenabled() const { return _webserverenabled; }
    void setCategory(int cat) { _selectedCategory = cat; }
    void setNation(int nat) { _selectedNation = nat; }

    void setName(const char *name)
    {
        if (_info.name)
            delete[] _info.name;

        if (!name || strlen(name) == 0)
            name = HumanDriverModuleName;

        _info.name = new char[strlen(name)+1];
        strcpy(_info.name, name); // Can't use strdup : free crashes in destructor !?
    }
    void setDispName(const char *dispname)
    {
        if (_info.dispname)
            delete[] _info.dispname;

        if (!dispname)
            dispname = NoPlayer;

        _info.dispname = new char[strlen(dispname)+1];
        strcpy(_info.dispname, dispname); // Can't use strdup : free crashes in destructor !?
    }

    void setDefaultCarName(const char *defcarname)
    {
        if (_defcarname)
            delete[] _defcarname;

        if (!defcarname || strlen(defcarname) == 0)
            defcarname = DefaultCarName;

        _defcarname = new char[strlen(defcarname)+1];
        strcpy(_defcarname, defcarname); // Can't use strdup : free crashes in destructor !?
    }

    void setDefaultCarRace(const char *defcarrace)
    {
        if (_defcarrace)
            delete[] _defcarrace;

        if (!defcarrace || strlen(defcarrace) == 0)
            defcarrace = DefaultCarRace;

        _defcarrace = new char[strlen(defcarrace)+1];
        strcpy(_defcarrace, defcarrace); // Can't use strdup : free crashes in destructor !?
    }

    void setDefaultCarCareer(const char *defcarcareer)
    {
        if (_defcarcareer)
            delete[] _defcarcareer;

        if (!defcarcareer || strlen(defcarcareer) == 0)
            defcarcareer = DefaultCarCareer;

        _defcarcareer = new char[strlen(defcarcareer)+1];
        strcpy(_defcarcareer, defcarcareer); // Can't use strdup : free crashes in destructor !?
    }

    void setDefaultChamp(const char *defchampname)
    {
        if (_defchamp)
            delete[] _defchamp;

        if (!defchampname || strlen(defchampname) == 0)
            defchampname = DefaultChampionship;

        _defchamp = new char[strlen(defchampname)+1];
        strcpy(_defchamp, defchampname); // Can't use strdup : free crashes in destructor !?
    }

    void setDefaultCategoryName(const char *defcategoryname)
    {
        if (_defcategory)
            delete[] _defcategory;

        if (!defcategoryname || strlen(defcategoryname) == 0)
            defcategoryname = DefaultCategoryName;

        _defcategory = new char[strlen(defcategoryname)+1];
        strcpy(_defcategory, defcategoryname); // Can't use strdup : free crashes in destructor !?
    }

    void setDefaultCodeName(const char *defcodename)
    {
        if (_defcode)
            delete[] _defcode;

        if (!defcodename || strlen(defcodename) == 0)
            defcodename = DefaultCodeName;

        _defcode = new char[strlen(defcodename)+1];
        strcpy(_defcode, defcodename); // Can't use strdup : free crashes in destructor !?
    }

    void setDefaultNationName(const char *defnationname)
    {
        if (_defnation)
            delete[] _defnation;

        if (!defnationname || strlen(defnationname) == 0)
            defnationname = DefaultNationName;

        _defnation = new char[strlen(defnationname)+1];
        strcpy(_defnation, defnationname); // Can't use strdup : free crashes in destructor !?
    }

    void setWebserverusername(const char *webserverusername)
    {
        if (_webserverusername)
            delete[] _webserverusername;

        if (!webserverusername || strlen(webserverusername) == 0)
            webserverusername = DefaultWebserverusername;

        _webserverusername = new char[strlen(webserverusername)+1];
        strcpy(_webserverusername, webserverusername); // Can't use strdup : free crashes in destructor !?
    }

    void setWebserverpassword(const char *webserverpassword)
    {
        if (_webserverpassword)
            delete[] _webserverpassword;
        if (!webserverpassword || strlen(webserverpassword) == 0)
            webserverpassword = DefaultWebserverpassword;
        _webserverpassword = new char[strlen(webserverpassword)+1];
        strcpy(_webserverpassword, webserverpassword); // Can't use strdup : free crashes in destructor !?
    }

    void setWebserverEnabled(int webserverenabled) { _webserverenabled = webserverenabled; }
    void setRaceNumber(int raceNumber) { _racenumber = raceNumber; }
    void setCredit(unsigned long int credit) { _credit = credit; }
    void setGearChangeMode(tGearChangeMode gearChangeMode) { _gearchangemode = gearChangeMode; }
    void setSkillLevel(tSkillLevel skillLevel) { _skilllevel = skillLevel; }
    void setAutoReverse(int autoReverse) { _autoreverse = autoReverse; }

    ~tPlayerInfo()
    {
        if (_info.dispname)
            delete[] _info.dispname;

        if (_info.name)
            delete[] _info.name;

        if (_defcarname)
            delete[] _defcarname;

        if (_defcarrace)
            delete[] _defcarrace;

        if (_defcarcareer)
            delete[] _defcarcareer;

        if(_defcategory)
            delete[] _defcategory;

        if (_defchamp)
            delete[] _defchamp;

        if(_defnation)
            delete[] _defnation;

        if (_webserverusername)
            delete[] _webserverusername;

        if (_webserverpassword)
            delete[] _webserverpassword;
    }

    // Gear change mode enum to string conversion
    const char *gearChangeModeString() const
    {
        const char *gearChangeStr;

        if (_gearchangemode == GEAR_MODE_AUTO)
        {
            gearChangeStr = HM_VAL_AUTO;
        }
        else if (_gearchangemode == GEAR_MODE_GRID)
        {
            gearChangeStr = HM_VAL_GRID;
        }
        else if (_gearchangemode == GEAR_MODE_HBOX)
        {
            gearChangeStr = HM_VAL_HBOX;
        }
        else
        {
            gearChangeStr = HM_VAL_SEQ;
        }

        return gearChangeStr;
    }

private:
    tInfo			_info;
    char*			_defcarname;
    char*           _defcategory;
    char*           _defcarrace;
    char*           _defcarcareer;
    char*           _defchamp;
    char*           _defcode;
    char*           _defnation;
    int				_racenumber;
    unsigned long int  _credit;

    tGearChangeMode	_gearchangemode;
    float			_color[4];
    tSkillLevel		_skilllevel;
    int             _numranking[30];
    int             _level[18];
    int             _special[18];
    double          _fame[18];
    bool            _licence;
    int             _maxRanking;
    int             _selectedCategory;
    int             _selectedNation;
    int				_autoreverse;
    char*			_webserverusername;
    char*			_webserverpassword;
    int 			_webserverenabled;
};

/* The human driver (= player) info list */
typedef std::deque<tPlayerInfo*> tPlayerInfoList;
static tPlayerInfoList PlayersInfo;

/* The currently selected player (PlayersInfo.end() if none) */
static tPlayerInfoList::iterator CurrPlayer;

/* A bool to ("yes", "no") conversion table */
static const char *Yn[] = {HM_VAL_YES, HM_VAL_NO};

static int ReloadValues = 1;

static void
updateWebserverControls(int enabled)
{
    int flag = GFUI_DISABLE;

    if(enabled)
    {
        flag = GFUI_ENABLE;
    }

    GfuiEnable(ScrHandle, WebServerTestLoginId, flag);
    GfuiEnable(ScrHandle, WebPasswordEditId, flag);
    GfuiEnable(ScrHandle, WebUsernameEditId, flag);
}

/* Load screen editable fields with relevant values :
   - all empty if no player selected,
   - from selected player current settings otherwise
*/
static void
refreshEditVal(void)
{
    int autoRevVisible = GFUI_INVISIBLE;

    if (CurrPlayer == PlayersInfo.end())
    {
        GfuiLabelSetText(ScrHandle, CategoryEditId, "");
        GfuiEnable(ScrHandle, CategoryEditId, GFUI_DISABLE);

        GfuiEditboxSetString(ScrHandle, NameEditId, "");
        GfuiEnable(ScrHandle, NameEditId, GFUI_DISABLE);

        GfuiEditboxSetString(ScrHandle, NationEditId, "");
        GfuiEnable(ScrHandle, NationEditId, GFUI_DISABLE);

        GfuiEditboxSetString(ScrHandle, RaceNumEditId, "");
        GfuiEnable(ScrHandle, RaceNumEditId, GFUI_DISABLE);

        GfuiLabelSetText(ScrHandle, GearChangeEditId, "");
        GfuiEnable(ScrHandle, GearChangeEditId, GFUI_DISABLE);

        GfuiLabelSetText(ScrHandle, SkillEditId, "");
        GfuiEnable(ScrHandle, SkillEditId, GFUI_DISABLE);

        GfuiLabelSetText(ScrHandle, AutoReverseEditId, "");
        GfuiEnable(ScrHandle, AutoReverseEditId, GFUI_DISABLE);

        GfuiEditboxSetString(ScrHandle, WebUsernameEditId, "");
        GfuiEnable(ScrHandle, WebUsernameEditId, GFUI_DISABLE);

        GfuiEditboxSetString(ScrHandle, WebPasswordEditId, "");
        GfuiEnable(ScrHandle, WebPasswordEditId, GFUI_DISABLE);
        
        GfuiCheckboxSetChecked(ScrHandle, WebServerCheckboxId, 0);
        GfuiEnable(ScrHandle, WebPasswordEditId, GFUI_DISABLE);

        GfuiEnable(ScrHandle, WebServerTestLoginId, GFUI_DISABLE);
    }
    else
    {
        if (strcmp((*CurrPlayer)->dispName(), NoPlayer))
        {
            GfuiEditboxSetString(ScrHandle, NameEditId, (*CurrPlayer)->dispName());
        }
        else
        {
            GfuiEditboxSetString(ScrHandle, NameEditId, PlayerNamePrompt);
        }

        GfuiLabelSetText(ScrHandle, CategoryEditId, (*CurrPlayer)->defaultCategoryName());
        GfuiEnable(ScrHandle, CategoryEditId, GFUI_ENABLE);

        GfuiEnable(ScrHandle, NameEditId, GFUI_ENABLE);

        snprintf(buf, sizeof(buf), "%d", (*CurrPlayer)->raceNumber());
        GfuiEditboxSetString(ScrHandle, RaceNumEditId, buf);
        GfuiEnable(ScrHandle, RaceNumEditId, GFUI_ENABLE);

        GfuiLabelSetText(ScrHandle, GearChangeEditId, (*CurrPlayer)->gearChangeModeString());
        GfuiEnable(ScrHandle, GearChangeEditId, GFUI_ENABLE);

        GfuiLabelSetText(ScrHandle, NationEditId, (*CurrPlayer)->defaultNationName());
        GfuiEnable(ScrHandle, NationEditId, GFUI_ENABLE);

        GfuiLabelSetText(ScrHandle, SkillEditId, SkillLevelString[(*CurrPlayer)->skillLevel()]);
        GfuiEnable(ScrHandle, SkillEditId, GFUI_ENABLE);

        GfuiLabelSetText(ScrHandle, AutoReverseEditId, Yn[(*CurrPlayer)->autoReverse()]);
        GfuiEnable(ScrHandle, AutoReverseEditId, GFUI_ENABLE);

        snprintf(buf, sizeof(buf), "%s", (*CurrPlayer)->webserverusername());
        GfuiEditboxSetString(ScrHandle, WebUsernameEditId, buf);
        GfuiEnable(ScrHandle, WebUsernameEditId, GFUI_ENABLE);

        snprintf(buf, sizeof(buf), "%s", (*CurrPlayer)->webserverpassword());
        GfuiEditboxSetString(ScrHandle, WebPasswordEditId, buf);
        GfuiEnable(ScrHandle, WebPasswordEditId, GFUI_ENABLE);
        
        GfuiCheckboxSetChecked(ScrHandle, WebServerCheckboxId, (*CurrPlayer)->webserverenabled());
        GfuiEnable(ScrHandle, WebServerCheckboxId, GFUI_ENABLE);

        updateWebserverControls((*CurrPlayer)->webserverenabled());

        if ((*CurrPlayer)->gearChangeMode() == GEAR_MODE_AUTO)
            autoRevVisible = GFUI_VISIBLE;
    }

    GfuiVisibilitySet(ScrHandle, AutoReverseLabelId, autoRevVisible);
    GfuiVisibilitySet(ScrHandle, AutoReverseLeftId, autoRevVisible);
    GfuiVisibilitySet(ScrHandle, AutoReverseEditId, autoRevVisible);
    GfuiVisibilitySet(ScrHandle, AutoReverseRightId, autoRevVisible);
}

static void
onSelect(void * /* Dummy */)
{
    tPlayerInfoList::difference_type elemIdx;
    GfuiScrollListGetSelectedElement(ScrHandle, ScrollList, (void**)&elemIdx);

    CurrPlayer = PlayersInfo.begin() + elemIdx;

    //GfuiEnable(ScrHandle, CopyButtonId, GFUI_ENABLE);
    GfuiEnable(ScrHandle, RemoveButtonId, GFUI_ENABLE);
    GfuiEnable(ScrHandle, ProfileButtonId, GFUI_ENABLE);

    refreshEditVal();
}

/* Update players scroll-list from PlayersInfo array */
static void
UpdtScrollList(void)
{
    const char	*str;
    int		i;
    void	*tmp;

    /* free the previous scrollist elements */
    while((str = GfuiScrollListExtractElement(ScrHandle, ScrollList, 0, (void**)&tmp)) != NULL)
    {
    }

    for (i = 0; i < (int)PlayersInfo.size(); i++)
    {
        GfuiScrollListInsertElement(ScrHandle, ScrollList, PlayersInfo[i]->dispName(), i, (void*)(long)i);
    }

    if (CurrPlayer != PlayersInfo.end())
    {
        GfuiScrollListShowElement(ScrHandle, ScrollList, (int)(CurrPlayer - PlayersInfo.begin()));
    }
}

static void
PutGarageSettings(unsigned index)
{
    if(!GarageHdle)
    {
        GfLogDebug("! Not Garage Handle\n");
        return;
    }

    char GarageSectionPath[128];
    snprintf(GarageSectionPath, sizeof(GarageSectionPath), "%s/%s/%u", "cars", ROB_LIST_INDEX, index);
    GfLogDebug(" ! Index = %u\n", index);
    GfParmSetStr(GarageHdle, GarageSectionPath, ROB_ATTR_GARAGE1, "none");
    GfParmSetNum(GarageHdle, GarageSectionPath, "option1", (char*)NULL, 0);
    GfParmSetNum(GarageHdle, GarageSectionPath, "option2", (char*)NULL, 0);
    GfParmSetNum(GarageHdle, GarageSectionPath, "option3", (char*)NULL, 0);
    GfParmSetNum(GarageHdle, GarageSectionPath, "option4", (char*)NULL, 0);
    GfParmSetNum(GarageHdle, GarageSectionPath, "option5", (char*)NULL, 0);
    GfParmSetNum(GarageHdle, GarageSectionPath, "option6", (char*)NULL, 0);
    GfParmSetNum(GarageHdle, GarageSectionPath, "option7", (char*)NULL, 0);
    //GfParmWriteFile(0, GarageHdle, "garage");
    //GfParmReleaseHandle(GarageHdle);
}

/* Put given player settings (from PlayersInfo array) to the human drivers and preferences params (index is the identification number in params, beginning at 1)*/
static void
PutPlayerSettings(unsigned index)
{
    if (!PlayerHdle || !PrefHdle)
    {
        return;
    }

    tPlayerInfo *player = PlayersInfo[index-1];

    // Graphics params (take driver name changes into account for camera settings).
    char drvSectionPath[128];
    snprintf(drvSectionPath, sizeof(drvSectionPath), "%s/%s/%u", ROB_SECT_ROBOTS, ROB_LIST_INDEX, index);
    const char* pszOldDispName = GfParmGetStr(PlayerHdle, drvSectionPath, ROB_ATTR_NAME, "");
    GfLogDebug( "! Index player = %u\n", index);

    if (strcmp(pszOldDispName, player->dispName()))
    { // Only if the display name changed.
        char drvDispSecPath[128];
        snprintf(drvDispSecPath, sizeof(drvDispSecPath), "%s/%s", GR_SCT_DISPMODE, pszOldDispName);

        if (!GraphHdle) // Load graphic params file if not already done.
        {
            GraphHdle = GfParmReadFileLocal(GR_PARAM_FILE, GFPARM_RMODE_REREAD);
        }

        if (GfParmExistsSection(GraphHdle, drvDispSecPath))
        { // Change section name.
            GfParmListRenameElt(GraphHdle, GR_SCT_DISPMODE, pszOldDispName, player->dispName());
        }

        if (!GfParmListSeekFirst(GraphHdle, GR_SCT_DISPMODE))
        {
            do
            {
                const char* pszSecName = GfParmListGetCurEltName(GraphHdle, GR_SCT_DISPMODE);
                if (!pszSecName || !isdigit(*pszSecName))
                    continue; // Ignore sections whose name is not (likely) a screen id.
                snprintf(drvDispSecPath, sizeof(drvDispSecPath), "%s/%s", GR_SCT_DISPMODE, pszSecName);
                const char* pszCurDrvName =
                    GfParmGetStr(GraphHdle, drvDispSecPath, GR_ATT_CUR_DRV, "");
                if (!strcmp(pszOldDispName, pszCurDrvName)) // Change current driver name.
                    GfParmSetStr(GraphHdle, drvDispSecPath, GR_ATT_CUR_DRV, player->dispName());
            } while (!GfParmListSeekNext(GraphHdle, GR_SCT_DISPMODE));
        }
    }

    // Human driver params
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_NAME, player->dispName());
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_SNAME, player->dispName());
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_CODE, player->defaultCodeName());
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_NATION, player->defaultNationName());
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_CAR, player->defaultCarName());
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_CAR_RACE, player->defaultCarRace());
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_CAR_CAREER, player->defaultCarCareer());
    GfParmSetStr(PlayerHdle, drvSectionPath, "championship", player->defaultChamp());
    GfParmSetNum(PlayerHdle, drvSectionPath, ROB_ATTR_RACENUM, (char*)NULL, player->raceNumber());
    GfParmSetNum(PlayerHdle, drvSectionPath, "credit", (char*)NULL, player->credit());
    GfParmSetNum(PlayerHdle, drvSectionPath, ROB_ATTR_RED, (char*)NULL, player->color(0));
    GfParmSetNum(PlayerHdle, drvSectionPath, ROB_ATTR_GREEN, (char*)NULL, player->color(1));
    GfParmSetNum(PlayerHdle, drvSectionPath, ROB_ATTR_BLUE, (char*)NULL, player->color(2));
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_TYPE, ROB_VAL_HUMAN);
    GfParmSetStr(PlayerHdle, drvSectionPath, ROB_ATTR_LEVEL, SkillLevelString[player->skillLevel()]);
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking0", (char*)NULL, player->ranking0());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking1", (char*)NULL, player->ranking1());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking2", (char*)NULL, player->ranking2());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking3", (char*)NULL, player->ranking3());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking4", (char*)NULL, player->ranking4());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking5", (char*)NULL, player->ranking5());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking6", (char*)NULL, player->ranking6());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking7", (char*)NULL, player->ranking7());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking8", (char*)NULL, player->ranking8());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking9", (char*)NULL, player->ranking9());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking10", (char*)NULL, player->ranking10());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking11", (char*)NULL, player->ranking11());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking12", (char*)NULL, player->ranking12());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking13", (char*)NULL, player->ranking13());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking14", (char*)NULL, player->ranking14());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking15", (char*)NULL, player->ranking15());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking16", (char*)NULL, player->ranking16());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking17", (char*)NULL, player->ranking17());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking18", (char*)NULL, player->ranking18());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking19", (char*)NULL, player->ranking19());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking20", (char*)NULL, player->ranking20());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking21", (char*)NULL, player->ranking21());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking22", (char*)NULL, player->ranking22());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking23", (char*)NULL, player->ranking23());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking24", (char*)NULL, player->ranking24());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking25", (char*)NULL, player->ranking25());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking26", (char*)NULL, player->ranking26());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking27", (char*)NULL, player->ranking27());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking28", (char*)NULL, player->ranking28());
    GfParmSetNum(PlayerHdle, drvSectionPath, "ranking29", (char*)NULL, player->ranking29());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level0", (char*)NULL, player->level0());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level1", (char*)NULL, player->level1());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level2", (char*)NULL, player->level2());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level3", (char*)NULL, player->level3());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level4", (char*)NULL, player->level4());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level5", (char*)NULL, player->level5());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level6", (char*)NULL, player->level6());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level7", (char*)NULL, player->level7());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level8", (char*)NULL, player->level8());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level9", (char*)NULL, player->level9());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level10", (char*)NULL, player->level10());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level11", (char*)NULL, player->level11());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level12", (char*)NULL, player->level12());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level13", (char*)NULL, player->level13());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level14", (char*)NULL, player->level14());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level15", (char*)NULL, player->level15());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level16", (char*)NULL, player->level16());
    GfParmSetNum(PlayerHdle, drvSectionPath, "level17", (char*)NULL, player->level17());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special0", (char*)NULL, player->special0());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special1", (char*)NULL, player->special1());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special2", (char*)NULL, player->special2());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special3", (char*)NULL, player->special3());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special4", (char*)NULL, player->special4());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special5", (char*)NULL, player->special5());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special6", (char*)NULL, player->special6());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special7", (char*)NULL, player->special7());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special8", (char*)NULL, player->special8());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special9", (char*)NULL, player->special9());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special10", (char*)NULL, player->special10());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special11", (char*)NULL, player->special11());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special12", (char*)NULL, player->special12());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special13", (char*)NULL, player->special13());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special14", (char*)NULL, player->special14());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special15", (char*)NULL, player->special15());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special16", (char*)NULL, player->special16());
    GfParmSetNum(PlayerHdle, drvSectionPath, "special17", (char*)NULL, player->special17());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame0", (char*)NULL, player->fame0());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame1", (char*)NULL, player->fame1());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame2", (char*)NULL, player->fame2());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame3", (char*)NULL, player->fame3());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame4", (char*)NULL, player->fame4());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame5", (char*)NULL, player->fame5());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame6", (char*)NULL, player->fame6());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame7", (char*)NULL, player->fame7());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame8", (char*)NULL, player->fame8());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame9", (char*)NULL, player->fame9());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame10", (char*)NULL, player->fame10());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame11", (char*)NULL, player->fame11());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame12", (char*)NULL, player->fame12());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame13", (char*)NULL, player->fame13());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame14", (char*)NULL, player->fame14());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame15", (char*)NULL, player->fame15());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame16", (char*)NULL, player->fame16());
    GfParmSetNum(PlayerHdle, drvSectionPath, "fame17", (char*)NULL, player->fame17());
    GfParmSetNum(PlayerHdle, drvSectionPath, "licence", (char*)NULL, player->licence());
    // Driver preferences params
    snprintf(drvSectionPath, sizeof(drvSectionPath), "%s/%s/%u", HM_SECT_PREF, HM_LIST_DRV, index);
    GfParmSetStr(PrefHdle, drvSectionPath, HM_ATT_TRANS, player->gearChangeModeString());
    GfParmSetStr(PrefHdle, drvSectionPath, HM_ATT_AUTOREVERSE, Yn[player->autoReverse()]);
    GfParmSetStr(PrefHdle, drvSectionPath, "WebServerUsername", player->webserverusername());
    GfParmSetStr(PrefHdle, drvSectionPath, "WebServerPassword", player->webserverpassword());

    if (player->skillLevel() > 3)
        GfParmSetNum(PrefHdle, drvSectionPath, "WebServerEnabled", (char*)NULL, player->webserverenabled());
    else
        GfParmSetNum(PrefHdle, drvSectionPath, "WebServerEnabled", (char*)NULL, 0);

    /* Allow neutral gear in sequential mode if neutral gear command not defined */
    if (player->gearChangeMode() == GEAR_MODE_SEQ
        && !strcmp(GfParmGetStr(PrefHdle, drvSectionPath, HM_ATT_GEAR_N, "-"), "-"))
        GfParmSetStr(PrefHdle, drvSectionPath, HM_ATT_SEQSHFT_ALLOW_NEUTRAL, HM_VAL_YES);
    else
        GfParmSetStr(PrefHdle, drvSectionPath, HM_ATT_SEQSHFT_ALLOW_NEUTRAL, HM_VAL_NO);

    /* Allow reverse gear in sequential mode if reverse gear command not defined */
    if (player->gearChangeMode() == GEAR_MODE_SEQ
        && !strcmp(GfParmGetStr(PrefHdle, drvSectionPath, HM_ATT_GEAR_R, "-"), "-"))
        GfParmSetStr(PrefHdle, drvSectionPath, HM_ATT_SEQSHFT_ALLOW_REVERSE, HM_VAL_YES);
    else
        GfParmSetStr(PrefHdle, drvSectionPath, HM_ATT_SEQSHFT_ALLOW_REVERSE, HM_VAL_NO);

    /* Release gear lever goes neutral in grid mode if no neutral gear command defined */
    if (player->gearChangeMode() == GEAR_MODE_GRID
        && !strcmp(GfParmGetStr(PrefHdle, drvSectionPath, HM_ATT_GEAR_N, "-"), "-"))
        GfParmSetStr(PrefHdle, drvSectionPath, HM_ATT_REL_BUT_NEUTRAL, HM_VAL_YES);
    else
        GfParmSetStr(PrefHdle, drvSectionPath, HM_ATT_REL_BUT_NEUTRAL, HM_VAL_NO);

    char CurCarSectionPath[128];
    CurCarName = GfParmReadFileLocal("drivers/curcarnames.xml", GFPARM_RMODE_CREAT);

    if (CurCarName)
    {
        snprintf(CurCarSectionPath, sizeof(CurCarSectionPath), "drivers/%s/%d", "human", index);
        GfParmSetStr (CurCarName, CurCarSectionPath, RM_ATTR_CARNAME, player->defaultCarName());
        GfParmWriteFile (0, CurCarName, "Car names");
        GfParmReleaseHandle (CurCarName);
        CurCarName = NULL;
    }
}

/* Create a brand new player into the list, and update players scroll-list and all editable fields*/
static void
onNewPlayer(void * /* dummy */)
{
    unsigned newPlayerIdx, playerIdx;
    char sectionPath[128];
    char driverId[8];
    char newDriverId[8];

    // Insert new player after current (or after last if no current)
    CurrPlayer = PlayersInfo.insert(CurrPlayer + (CurrPlayer == PlayersInfo.end() ? 0 : 1),
                                    new tPlayerInfo(HumanDriverModuleName));

    // Get new current player index (= identification number in params).
    newPlayerIdx = (unsigned)(CurrPlayer - PlayersInfo.begin()) + 1;

    // Update preferences and drivers params (rename those after, add new).
    snprintf(sectionPath, sizeof(sectionPath), "%s/%s", HM_SECT_PREF, HM_LIST_DRV);

    for (playerIdx = PlayersInfo.size() - 1; playerIdx >= newPlayerIdx; playerIdx--)
    {
        snprintf(driverId, sizeof(driverId), "%u", playerIdx);
        snprintf(newDriverId, sizeof(newDriverId), "%u", playerIdx+1);
        GfParmListRenameElt(PrefHdle, sectionPath, driverId, newDriverId);
    }

    snprintf(sectionPath, sizeof(sectionPath), "%s/%s", ROB_SECT_ROBOTS, ROB_LIST_INDEX);

    for (playerIdx = PlayersInfo.size() - 1; playerIdx >= newPlayerIdx; playerIdx--)
    {
        snprintf(driverId, sizeof(driverId), "%u", playerIdx);
        snprintf(newDriverId, sizeof(newDriverId), "%u", playerIdx+1);
        GfParmListRenameElt(PlayerHdle, sectionPath, driverId, newDriverId);
    }

    snprintf(sectionPath, sizeof(sectionPath), "%s/%s", "cars", ROB_LIST_INDEX);

    for (playerIdx = PlayersInfo.size() - 1; playerIdx >= newPlayerIdx; playerIdx--)
    {
        snprintf(driverId, sizeof(driverId), "%u", playerIdx);
        snprintf(newDriverId, sizeof(newDriverId), "%u", playerIdx+1);
        GfParmListRenameElt(GarageHdle, sectionPath, driverId, newDriverId);
    }

    PutPlayerSettings(newPlayerIdx);
    PutGarageSettings(newPlayerIdx);

    // Update GUI.
    refreshEditVal();
    UpdtScrollList();
}

/* Create a new player into the list by copying the currently selected one, and update players scroll-list and all editable fields (copy also the control settings) */
static void
onCopyPlayer(void * /* dummy */)
{
    unsigned curPlayerIdx, newPlayerIdx;
    unsigned playerIdx;
    char sectionPath[128];
    char driverId[8];
    char newDriverId[8];

    tGearChangeMode gearChange;

    if (CurrPlayer != PlayersInfo.end())
    {
        // Get current (source) player index (= identification number in params).
        curPlayerIdx = (unsigned)(CurrPlayer - PlayersInfo.begin()) + 1;

        // Save current player gear change mode
        gearChange = (*CurrPlayer)->gearChangeMode();

        // Get current player control settings.
        ControlGetSettings(PrefHdle, curPlayerIdx);

        // Insert new player after current
        CurrPlayer = PlayersInfo.insert(CurrPlayer + 1, new tPlayerInfo(**CurrPlayer));

        // Get new (copy) player index (= identification number in params).
        newPlayerIdx = (unsigned)(CurrPlayer - PlayersInfo.begin()) + 1;
        
        //ovverryde the copied WebServer data (username password and enabled status) with the default settings

        const char *str;
        char sstring[128];
        int webserverenabledval;
        /* Load players settings from human/preferences.xml file*/
        PrefHdle = GfParmReadFileLocal(HM_PREF_FILE, GFPARM_RMODE_REREAD);
        if (!PrefHdle)
        {
            return;
        }

        str = GfParmGetStr(PrefHdle, sstring, "WebServerUsername", 0);
        (*CurrPlayer)->setWebserverusername(str);

        str = GfParmGetStr(PrefHdle, sstring, "WebServerPassword", 0);
        (*CurrPlayer)->setWebserverpassword(str);
        
        webserverenabledval = GfParmGetNum(PrefHdle, sstring, "WebServerEnabled", (char*)NULL, (int)0);

        (*CurrPlayer)->setWebserverEnabled(webserverenabledval);

        // Update preferences and drivers params (rename those after, add new).
        snprintf(sectionPath, sizeof(sectionPath), "%s/%s", HM_SECT_PREF, HM_LIST_DRV);

        for (playerIdx = PlayersInfo.size() - 1; playerIdx >= newPlayerIdx; playerIdx--)
        {
            snprintf(driverId, sizeof(driverId), "%u", playerIdx);
            snprintf(newDriverId, sizeof(newDriverId), "%u", playerIdx+1);
            GfParmListRenameElt(PrefHdle, sectionPath, driverId, newDriverId);
        }

        snprintf(sectionPath, sizeof(sectionPath), "%s/%s", ROB_SECT_ROBOTS, ROB_LIST_INDEX);

        for (playerIdx = PlayersInfo.size() - 1; playerIdx >= newPlayerIdx; playerIdx--)
        {
            snprintf(driverId, sizeof(driverId), "%u", playerIdx);
            snprintf(newDriverId, sizeof(newDriverId), "%u", playerIdx+1);
            GfParmListRenameElt(PlayerHdle, sectionPath, driverId, newDriverId);
        }

        PutPlayerSettings(newPlayerIdx);
        //PutGarageSettings(newPlayerIdx);

        // Set new player control settings (copy of previous current one's).
        ControlPutSettings(PrefHdle, newPlayerIdx, gearChange);

        // Update GUI.
        refreshEditVal();
        UpdtScrollList();
    }
}

/* Remove the selected player from the list, and update players scroll-list and all editable fields*/
static void
onDeletePlayer(void * /* dummy */)
{
    int delPlayerIdx;
    unsigned int playerIdx;
    char sectionPath[128];
    char driverId[8];
    char newDriverId[8];

    if (CurrPlayer != PlayersInfo.end())
    {
        // Get current player index (= identification number in params).
        delPlayerIdx = CurrPlayer - PlayersInfo.begin() + 1;

        // Remove current player from list. Select next if any.
        delete *CurrPlayer;
        CurrPlayer = PlayersInfo.erase(CurrPlayer);

        // Update preferences and drivers params.
        snprintf(sectionPath, sizeof(sectionPath), "%s/%s", HM_SECT_PREF, HM_LIST_DRV);
        snprintf(driverId, sizeof(driverId), "%d", delPlayerIdx);

        if (!GfParmListRemoveElt(PrefHdle, sectionPath, driverId))
        {
            for (playerIdx = delPlayerIdx; playerIdx <= PlayersInfo.size(); playerIdx++)
            {
                snprintf(driverId, sizeof(driverId), "%u", playerIdx+1);
                snprintf(newDriverId, sizeof(newDriverId), "%u", playerIdx);
                GfParmListRenameElt(PrefHdle, sectionPath, driverId, newDriverId);
            }
        }

        snprintf(sectionPath, sizeof(sectionPath), "%s/%s", ROB_SECT_ROBOTS, ROB_LIST_INDEX);
        snprintf(driverId, sizeof(driverId), "%d", delPlayerIdx);

        if (!GfParmListRemoveElt(PlayerHdle, sectionPath, driverId))
        {
            for (playerIdx = delPlayerIdx; playerIdx <= PlayersInfo.size(); playerIdx++)
            {
                snprintf(driverId, sizeof(driverId), "%u", playerIdx+1);
                snprintf(newDriverId, sizeof(newDriverId), "%u", playerIdx);
                GfParmListRenameElt(PlayerHdle, sectionPath, driverId, newDriverId);
            }
        }

        snprintf(sectionPath, sizeof(sectionPath), "%s/%s", "cars", ROB_LIST_INDEX);
        snprintf(driverId, sizeof(driverId), "%d", delPlayerIdx);

        if (!GfParmListRemoveElt(GarageHdle, sectionPath, driverId))
        {
            for (playerIdx = delPlayerIdx; playerIdx <= PlayersInfo.size(); playerIdx++)
            {
                snprintf(driverId, sizeof(driverId), "%u", playerIdx+1);
                snprintf(newDriverId, sizeof(newDriverId), "%u", playerIdx);
                GfParmListRenameElt(GarageHdle, sectionPath, driverId, newDriverId);
            }
        }

        // Update GUI.
        refreshEditVal();
        UpdtScrollList();
    }
}

/* Activate the control config screen if a player is selected */
static void
onConfControls(void * /* dummy */ )
{
    unsigned curPlayerIdx;

    if (CurrPlayer != PlayersInfo.end())
    {
        ReloadValues = 0;

        curPlayerIdx = (unsigned)(CurrPlayer - PlayersInfo.begin()) + 1;
        GfuiScreenActivate(ControlMenuInit(ScrHandle, PrefHdle, curPlayerIdx, (*CurrPlayer)->gearChangeMode(), 0));
    }
}

static int
GenerateLoadingCategory(const char *category)
{
    std::string strCat = "";
    //std::string strCar = category;
    strCat = GfCars::self()->getCar(category)->getCategoryName();

    for(int i = 0; i < 18; i++)
    {
        if(strcmp(strCat.c_str(), strCategory[i]) == 0)
        {
            int SelectedCategory = i;
            GfLogDebug("Selected Category = %i - %s\n", i, strCategory[i]);
            return SelectedCategory;
        }
    }

    return 0;
}

static int
GenerateLoadingNation(const char *nation)
{
    for(int i = 0; i < 258; i++)
    {
        if(strcmp(nation, strNation[i]) == 0)
        {
            int SelectedNation = i;
            GfLogDebug("Selected Nation = %i - %s\n", i, strNation[SelectedNation]);
            return SelectedNation;
        }
    }

    return 0;
}

/* Load human driver (= player) info list (PlayersInfo) from preferences and human drivers files ;
   load associated scroll list */
static int
GenPlayerList(void)
{
    char sstring[128];
    int i;
    int j;
    const char *driver;
    const char *sdriver;
    const char *cdriver;
    const char *defaultNation;
    const char *defaultCar;
    const char *defaultCarRace;
    const char *defaultCarCareer;
    const char *defaultCategory;
    const char *defaultChamp;
    tSkillLevel skilllevel;
    const char *str;
    int racenumber;
    unsigned long int credit;
    int numranking[30];
    int level[18];
    int special[18];
    double fame[18];
    float color[4];
    int webserverenabledval;

    /* Reset players list */
    tPlayerInfoList::iterator playerIter;

    for (playerIter = PlayersInfo.begin(); playerIter != PlayersInfo.end(); ++playerIter)
        delete *playerIter;

    PlayersInfo.clear();

    /* Load players settings from human.xml file *//*was meant: preferences.xml file?*/
    PlayerHdle = GfParmReadFileLocal(HM_DRV_FILE, GFPARM_RMODE_REREAD);

    if (PlayerHdle == NULL)
    {
        return -1;
    }

    GarageHdle = GfParmReadFileLocal("drivers/human/garage.xml", GFPARM_RMODE_REREAD);

    for (i = 0; ; i++)
    {
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", ROB_SECT_ROBOTS, ROB_LIST_INDEX, i+1);
        driver = GfParmGetStr(PlayerHdle, sstring, ROB_ATTR_NAME, "");

        int maxranking = 0;
        int SelectedCategory = 0;
        int SelectedNation = 0;
        bool licence = false;

        if (strlen(driver) == 0)
        {
            break; // Exit at end of driver list.
        }
        else
        {
            str = GfParmGetStr(PlayerHdle, sstring, ROB_ATTR_LEVEL, SkillLevelString[0]);
            skilllevel = ARCADE;
            for(j = 0; j < NbSkillLevels; j++)
            {
                if (strcmp(SkillLevelString[j], str) == 0)
                {
                    skilllevel = static_cast<tSkillLevel>(j);
                    break;
                }
            }

            sdriver         = GfParmGetStr(PlayerHdle, sstring, ROB_ATTR_SNAME, 0);
            cdriver         = GfParmGetStr(PlayerHdle, sstring, ROB_ATTR_CODE, 0);
            defaultNation   = GfParmGetStr(PlayerHdle, sstring, ROB_ATTR_NATION, 0);
            SelectedNation  = GenerateLoadingNation(defaultNation);
            defaultCar      = GfParmGetStr(PlayerHdle, sstring, ROB_ATTR_CAR, 0);
            defaultCarRace  = GfParmGetStr(PlayerHdle, sstring, ROB_ATTR_CAR_RACE, 0);

            if (strcmp(defaultCar, defaultCarRace) != 0)
                defaultCar = defaultCarRace;

            SelectedCategory = GenerateLoadingCategory(defaultCar);
            defaultCategory = strCategory[SelectedCategory];

            defaultCarCareer  = GfParmGetStr(PlayerHdle, sstring, ROB_ATTR_CAR_CAREER, 0);
            defaultChamp      = GfParmGetStr(PlayerHdle, sstring, "championship", 0);
            racenumber      = (int)GfParmGetNum(PlayerHdle, sstring, ROB_ATTR_RACENUM, (char*)NULL, 0);
            numranking[0]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking0", (char*)NULL, 0);
            numranking[1]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking1", (char*)NULL, 0);
            numranking[2]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking2", (char*)NULL, -1);
            numranking[3]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking3", (char*)NULL, -1);
            numranking[4]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking4", (char*)NULL, -1);
            numranking[5]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking5", (char*)NULL, -1);
            numranking[6]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking6", (char*)NULL, -1);
            numranking[7]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking7", (char*)NULL, -1);
            numranking[8]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking8", (char*)NULL, -1);
            numranking[9]   = (int)GfParmGetNum(PlayerHdle, sstring, "ranking9", (char*)NULL, -1);
            numranking[10]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking10", (char*)NULL, -1);
            numranking[11]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking11", (char*)NULL, -1);
            numranking[12]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking12", (char*)NULL, -1);
            numranking[13]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking13", (char*)NULL, -1);
            numranking[14]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking14", (char*)NULL, -1);
            numranking[15]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking15", (char*)NULL, -1);
            numranking[16]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking16", (char*)NULL, -1);
            numranking[17]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking17", (char*)NULL, -1);
            numranking[18]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking18", (char*)NULL, -1);
            numranking[19]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking19", (char*)NULL, -1);
            numranking[20]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking20", (char*)NULL, -1);
            numranking[21]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking21", (char*)NULL, -1);
            numranking[22]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking22", (char*)NULL, -1);
            numranking[23]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking23", (char*)NULL, -1);
            numranking[24]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking24", (char*)NULL, -1);
            numranking[25]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking25", (char*)NULL, -1);
            numranking[26]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking26", (char*)NULL, -1);
            numranking[27]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking27", (char*)NULL, -1);
            numranking[28]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking28", (char*)NULL, -1);
            numranking[29]  = (int)GfParmGetNum(PlayerHdle, sstring, "ranking29", (char*)NULL, -1);
            level[0]   = (int)GfParmGetNum(PlayerHdle, sstring, "level0", (char*)NULL, 0);
            level[1]   = (int)GfParmGetNum(PlayerHdle, sstring, "level1", (char*)NULL, 0);
            level[2]   = (int)GfParmGetNum(PlayerHdle, sstring, "level2", (char*)NULL, 0);
            level[3]   = (int)GfParmGetNum(PlayerHdle, sstring, "level3", (char*)NULL, 0);
            level[4]   = (int)GfParmGetNum(PlayerHdle, sstring, "level4", (char*)NULL, 0);
            level[5]   = (int)GfParmGetNum(PlayerHdle, sstring, "level5", (char*)NULL, 0);
            level[6]   = (int)GfParmGetNum(PlayerHdle, sstring, "level6", (char*)NULL, 0);
            level[7]   = (int)GfParmGetNum(PlayerHdle, sstring, "level7", (char*)NULL, 0);
            level[8]   = (int)GfParmGetNum(PlayerHdle, sstring, "level8", (char*)NULL, 0);
            level[9]   = (int)GfParmGetNum(PlayerHdle, sstring, "level9", (char*)NULL, 0);
            level[10]  = (int)GfParmGetNum(PlayerHdle, sstring, "level10", (char*)NULL, 0);
            level[11]  = (int)GfParmGetNum(PlayerHdle, sstring, "level11", (char*)NULL, 0);
            level[12]  = (int)GfParmGetNum(PlayerHdle, sstring, "level12", (char*)NULL, 0);
            level[13]  = (int)GfParmGetNum(PlayerHdle, sstring, "level13", (char*)NULL, 0);
            level[14]  = (int)GfParmGetNum(PlayerHdle, sstring, "level14", (char*)NULL, 0);
            level[15]  = (int)GfParmGetNum(PlayerHdle, sstring, "level15", (char*)NULL, 0);
            level[16]  = (int)GfParmGetNum(PlayerHdle, sstring, "level16", (char*)NULL, 0);
            level[17]  = (int)GfParmGetNum(PlayerHdle, sstring, "level17", (char*)NULL, 0);
            special[0]   = (int)GfParmGetNum(PlayerHdle, sstring, "special0", (char*)NULL, 0);
            special[1]   = (int)GfParmGetNum(PlayerHdle, sstring, "special1", (char*)NULL, 0);
            special[2]   = (int)GfParmGetNum(PlayerHdle, sstring, "special2", (char*)NULL, 0);
            special[3]   = (int)GfParmGetNum(PlayerHdle, sstring, "special3", (char*)NULL, 0);
            special[4]   = (int)GfParmGetNum(PlayerHdle, sstring, "special4", (char*)NULL, 0);
            special[5]   = (int)GfParmGetNum(PlayerHdle, sstring, "special5", (char*)NULL, 0);
            special[6]   = (int)GfParmGetNum(PlayerHdle, sstring, "special6", (char*)NULL, 0);
            special[7]   = (int)GfParmGetNum(PlayerHdle, sstring, "special7", (char*)NULL, 0);
            special[8]   = (int)GfParmGetNum(PlayerHdle, sstring, "special8", (char*)NULL, 0);
            special[9]   = (int)GfParmGetNum(PlayerHdle, sstring, "special9", (char*)NULL, 0);
            special[10]  = (int)GfParmGetNum(PlayerHdle, sstring, "special10", (char*)NULL, 0);
            special[11]  = (int)GfParmGetNum(PlayerHdle, sstring, "special11", (char*)NULL, 0);
            special[12]  = (int)GfParmGetNum(PlayerHdle, sstring, "special12", (char*)NULL, 0);
            special[13]  = (int)GfParmGetNum(PlayerHdle, sstring, "special13", (char*)NULL, 0);
            special[14]  = (int)GfParmGetNum(PlayerHdle, sstring, "special14", (char*)NULL, 0);
            special[15]  = (int)GfParmGetNum(PlayerHdle, sstring, "special15", (char*)NULL, 0);
            special[16]  = (int)GfParmGetNum(PlayerHdle, sstring, "special16", (char*)NULL, 0);
            special[17]  = (int)GfParmGetNum(PlayerHdle, sstring, "special17", (char*)NULL, 0);
            fame[0]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame0", (char*)NULL, 0);
            fame[1]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame1", (char*)NULL, 0);
            fame[2]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame2", (char*)NULL, 0);
            fame[3]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame3", (char*)NULL, 0);
            fame[4]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame4", (char*)NULL, 0);
            fame[5]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame5", (char*)NULL, 0);
            fame[6]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame6", (char*)NULL, 0);
            fame[7]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame7", (char*)NULL, 0);
            fame[8]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame8", (char*)NULL, 0);
            fame[9]   = (double)GfParmGetNum(PlayerHdle, sstring, "fame9", (char*)NULL, 0);
            fame[10]  = (double)GfParmGetNum(PlayerHdle, sstring, "fame10", (char*)NULL, 0);
            fame[11]  = (double)GfParmGetNum(PlayerHdle, sstring, "fame11", (char*)NULL, 0);
            fame[12]  = (double)GfParmGetNum(PlayerHdle, sstring, "fame12", (char*)NULL, 0);
            fame[13]  = (double)GfParmGetNum(PlayerHdle, sstring, "fame13", (char*)NULL, 0);
            fame[14]  = (double)GfParmGetNum(PlayerHdle, sstring, "fame14", (char*)NULL, 0);
            fame[15]  = (double)GfParmGetNum(PlayerHdle, sstring, "fame15", (char*)NULL, 0);
            fame[16]  = (double)GfParmGetNum(PlayerHdle, sstring, "fame16", (char*)NULL, 0);
            fame[17]  = (double)GfParmGetNum(PlayerHdle, sstring, "fame17", (char*)NULL, 0);
            credit          = (unsigned long int)GfParmGetNum(PlayerHdle, sstring, "credit", (char*)NULL, 10000);
            licence         = (bool)GfParmSetNum(PlayerHdle, sstring, "licence", (char*)NULL, 0);
            color[0]        = (float)GfParmGetNum(PlayerHdle, sstring, ROB_ATTR_RED, (char*)NULL, 1.0);
            color[1]        = (float)GfParmGetNum(PlayerHdle, sstring, ROB_ATTR_GREEN, (char*)NULL, 1.0);;
            color[2]        = (float)GfParmGetNum(PlayerHdle, sstring, ROB_ATTR_BLUE, (char*)NULL, 0.5);;
            color[3]        = 1.0;

            maxranking = 0;
#if 0
            for (int j=0; j < 30; j++)
            {
                if (j < 2)
                {
                    if(numranking[j] > -1)
                        maxranking += 1;
                }
                else if (numranking[j] > -1 && ( numranking[j-1] > -1 || numranking[j-2] > -1))
                        maxranking += 1;
                else
                    numranking[j] = -1;
            }

            GfLogDebug(" # Max Ranking Release = %i\n", maxranking);
#else
            for (int j=0; j < 30; j++)
            {
                if (j < 2)
                {
                    if(numranking[j] > -1)
                        maxranking += 1;
                }
                else if (numranking[j] > -1 && ( numranking[j-1] > -1 || numranking[j-2] > -1))
                        maxranking += 1;
            }

            GfLogDebug(" # Max Ranking Developpement = %i\n", maxranking);
#endif

            PlayersInfo.push_back(new tPlayerInfo(HumanDriverModuleName, // Driver module name
                                                  driver,     // Player (display) name
                                                  sdriver,    // Short name player.
                                                  cdriver,    // Code name player.
                                                  defaultNation,     // Nation player.
                                                  defaultCar, // Default car name.
                                                  defaultCarRace,
                                                  defaultCarCareer,
                                                  defaultCategory,
                                                  defaultChamp,
                                                  racenumber, // Race number
                                                  skilllevel, // skill level
                                                  numranking[0],
                                                  numranking[1],
                                                  numranking[2],
                                                  numranking[3],
                                                  numranking[4],
                                                  numranking[5],
                                                  numranking[6],
                                                  numranking[7],
                                                  numranking[8],
                                                  numranking[9],
                                                  numranking[10],
                                                  numranking[11],
                                                  numranking[12],
                                                  numranking[13],
                                                  numranking[14],
                                                  numranking[15],
                                                  numranking[16],
                                                  numranking[17],
                                                  numranking[18],
                                                  numranking[19],
                                                  numranking[20],
                                                  numranking[21],
                                                  numranking[22],
                                                  numranking[23],
                                                  numranking[24],
                                                  numranking[25],
                                                  numranking[26],
                                                  numranking[27],
                                                  numranking[28],
                                                  numranking[29],
                                                  level[0],
                                                  level[1],
                                                  level[2],
                                                  level[3],
                                                  level[4],
                                                  level[5],
                                                  level[6],
                                                  level[7],
                                                  level[8],
                                                  level[9],
                                                  level[10],
                                                  level[11],
                                                  level[12],
                                                  level[13],
                                                  level[14],
                                                  level[15],
                                                  level[16],
                                                  level[17],
                                                  special[0],
                                                  special[1],
                                                  special[2],
                                                  special[3],
                                                  special[4],
                                                  special[5],
                                                  special[6],
                                                  special[7],
                                                  special[8],
                                                  special[9],
                                                  special[10],
                                                  special[11],
                                                  special[12],
                                                  special[13],
                                                  special[14],
                                                  special[15],
                                                  special[16],
                                                  special[17],
                                                  fame[0],
                                                  fame[1],
                                                  fame[2],
                                                  fame[3],
                                                  fame[4],
                                                  fame[5],
                                                  fame[6],
                                                  fame[7],
                                                  fame[8],
                                                  fame[9],
                                                  fame[10],
                                                  fame[11],
                                                  fame[12],
                                                  fame[13],
                                                  fame[14],
                                                  fame[15],
                                                  fame[16],
                                                  fame[17],
                                                  licence,
                                                  credit,
                                                  color,
                                                  maxranking - 1,
                                                  SelectedCategory,
                                                  SelectedNation));    // Colors
        }
    }

    /* No currently selected player */
    CurrPlayer = PlayersInfo.end();

    /* Update scroll-list from PlayersInfo */
    UpdtScrollList();

    /* Load players settings from human/preferences.xml file*/
    PrefHdle = GfParmReadFileLocal(HM_PREF_FILE, GFPARM_RMODE_REREAD);

    if (!PrefHdle)
    {
        return -1;
    }

    for (i = 0; i < (int)PlayersInfo.size(); i++)
    {
        snprintf(sstring, sizeof(sstring), "%s/%s/%d", HM_SECT_PREF, HM_LIST_DRV, i+1);
        str = GfParmGetStr(PrefHdle, sstring, HM_ATT_TRANS, HM_VAL_AUTO);

        if (!strcmp(str, HM_VAL_AUTO))
        {
            PlayersInfo[i]->setGearChangeMode(GEAR_MODE_AUTO);
        }
        else if (!strcmp(str, HM_VAL_GRID))
        {
            PlayersInfo[i]->setGearChangeMode(GEAR_MODE_GRID);
        }
        else if (!strcmp(str, HM_VAL_HBOX))
        {
            PlayersInfo[i]->setGearChangeMode(GEAR_MODE_HBOX);
        }
        else
        {
            PlayersInfo[i]->setGearChangeMode(GEAR_MODE_SEQ);

        } /* Note: Deprecated "manual" value (after R1.3.0) smoothly converted to "sequential" (backward compatibility) */

        if (!strcmp(GfParmGetStr(PrefHdle, sstring, HM_ATT_AUTOREVERSE, Yn[0]), Yn[0]))
        {
            PlayersInfo[i]->setAutoReverse(0);
        }
        else
        {
            PlayersInfo[i]->setAutoReverse(1);
        }

        str = GfParmGetStr(PrefHdle, sstring, "WebServerUsername", 0);
        PlayersInfo[i]->setWebserverusername(str);

        str = GfParmGetStr(PrefHdle, sstring, "WebServerPassword", 0);
        PlayersInfo[i]->setWebserverpassword(str);
        
        webserverenabledval = GfParmGetNum(PrefHdle, sstring, "WebServerEnabled", (char*)NULL, (int)0);
        PlayersInfo[i]->setWebserverEnabled(webserverenabledval);
    }

    return 0;
}

/* Quit driver config menu (changes must have been saved before if needed) */
static void
onQuitPlayerConfig(void * /* dummy */)
{
    // Next time, we'll have to reload the player settings from the files.
    ReloadValues = 1;

    // Reset player list
    tPlayerInfoList::iterator playerIter;

    for (playerIter = PlayersInfo.begin(); playerIter != PlayersInfo.end(); ++playerIter)
        delete *playerIter;

    PlayersInfo.clear();

    // Close driver and preference params files.
    GfParmReleaseHandle(PlayerHdle);
    PlayerHdle = 0;

    GfParmReleaseHandle(PrefHdle);
    PrefHdle = 0;

    GfParmReleaseHandle(GarageHdle);
    GarageHdle = 0;

    //GfParmReleaseHandle(CurCarName);
    //CurCarName = 0;

    // Activate caller screen/menu
    GfuiScreenActivate(PrevScrHandle);
}

/* Save players info (from PlayersInfo array) to the human drivers and preferences XML files,
   as well as graphics settings for players if they changed */
static void
onSavePlayerList(void * /* dummy */)
{
    int		index;

    if (!PlayerHdle || !PrefHdle || !GarageHdle)
    {
        return;
    }

    // Force current edit to loose focus (if one has it) and update associated variable.
    GfuiUnSelectCurrent();

    for (index = 1; index <= (int)PlayersInfo.size(); index++)
    {
        PutPlayerSettings(index);
    }

    /*for (index = 2; index <= (int)PlayersInfo.size(); index++)
    {
        {
            PutGarageSettings(index);
        }
    }*/

    GfParmWriteFile(NULL, PlayerHdle, HumanDriverModuleName);
    GfParmWriteFile(NULL, PrefHdle, "preferences");
    GfParmWriteFile(NULL, GarageHdle, "garage");

    if (GraphHdle) // Write graphic params file if needed.
        GfParmWriteFile(NULL, GraphHdle, "Graph");

    // Temporary, as long as this menu has not been ported to using the tgfdata layer
    // in order to access drivers data (rather that directly read/writing the XML files).
    // Reload the drivers from disk.
    GfDrivers::self()->reload();

    onQuitPlayerConfig(0 /* dummy */);
}

static void
onChangeName(void * /* dummy */)
{
    char	*val;

    if (CurrPlayer != PlayersInfo.end())
    {
        val = GfuiEditboxGetString(ScrHandle, NameEditId);

        // Remove leading spaces (#587)
        std::string strIn(val);
        size_t startpos = strIn.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
        size_t endpos = strIn.find_last_not_of(" \t");  // Find last non-whitespace char position

        if (startpos != std::string::npos && endpos != std::string::npos)
        {
            strIn = strIn.substr(startpos, endpos - startpos + 1);
        }
        else
        {
            strIn.assign(NoPlayer); // If it was all whitespace, assign default
        }

        (*CurrPlayer)->setDispName((strIn == PlayerNamePrompt || strIn == NoPlayer) ? NoPlayer : strIn.c_str());
    }

    UpdtScrollList();
}

static void
onChangeWebserverusername(void * /* dummy */)
{
    char	*val;

    if (CurrPlayer != PlayersInfo.end())
    {
        val = GfuiEditboxGetString(ScrHandle, WebUsernameEditId);

        // Remove leading spaces (#587)
        std::string strIn(val);
        size_t startpos = strIn.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
        size_t endpos = strIn.find_last_not_of(" \t");  // Find last non-whitespace char position

        if (startpos != std::string::npos && endpos != std::string::npos)
        {
            strIn = strIn.substr(startpos, endpos - startpos + 1);
        }
        else
        {
            strIn.assign(DefaultWebserverusername); // If it was all whitespace, assign default
        }

        (*CurrPlayer)->setWebserverusername(strIn.c_str());
    }

    UpdtScrollList();
}

static void
onChangeWebserverpassword(void * /* dummy */)
{
    char	*val;

    if (CurrPlayer != PlayersInfo.end())
    {
        val = GfuiEditboxGetString(ScrHandle, WebPasswordEditId);

        // Remove leading spaces (#587)
        std::string strIn(val);
        size_t startpos = strIn.find_first_not_of(" \t"); // Find the first character position after excluding leading blank spaces
        size_t endpos = strIn.find_last_not_of(" \t");  // Find last non-whitespace char position

        if (startpos != std::string::npos && endpos != std::string::npos)
        {
            strIn = strIn.substr(startpos, endpos - startpos + 1);
        }
        else
        {
            strIn.assign(DefaultWebserverpassword); // If it was all whitespace, assign default
        }

        (*CurrPlayer)->setWebserverpassword(strIn.c_str());
    }

    UpdtScrollList();
}

static void
onChangeWebserverenabled(tCheckBoxInfo* pInfo)
{
    int isChecked;

    if (CurrPlayer != PlayersInfo.end())
    {
        isChecked = GfuiCheckboxIsChecked(ScrHandle, WebServerCheckboxId);
        (*CurrPlayer)->setWebserverEnabled(isChecked);
        updateWebserverControls(isChecked);
        //GfLogDebug("WebServer features: %i\n", isChecked);
        //GfLogDebug("WebServer features: %i\n", (*CurrPlayer)->webserverenabled());

    }

    UpdtScrollList();
}

static void onWebserverLoginTest(void * /* dummy */)
{
    if (CurrPlayer != PlayersInfo.end())
    {
        std::string username = (*CurrPlayer)->webserverusername();
        std::string password = (*CurrPlayer)->webserverpassword();
        //int skilling = (*CurrPlayer)->webserverskilling();

        webServer().sendLogin(username.c_str(),password.c_str());

        //request a redisplay
        GfuiApp().eventLoop().postRedisplay();
    }
}

//#641 New player name should get empty when clicking on it
// In the Player Configuration menu, when you create
// a new player, his name is set to "-- Enter name --",
// as a prompt for changing it.
// But when you want to change it, you have to first erase
// this prompt string all.
// Would be great if the prompt string disappear when clicked.
// Note(kilo): this way if one clicks the player name editbox
// and leaves without entering anything, the new player will be called
// "--No Player--" and stored with that name.
static void
onActivateName(void * /* dummy */)
{
    std::string strIn = GfuiEditboxGetString(ScrHandle, NameEditId);
    if (strIn == PlayerNamePrompt)
    {
        (*CurrPlayer)->setDispName("");
        GfuiEditboxSetString(ScrHandle, NameEditId, (*CurrPlayer)->dispName());
    }

    UpdtScrollList();
}

static void
onChangeNum(void * /* dummy */)
{
    char	*val;

    if (CurrPlayer != PlayersInfo.end())
    {
        val = GfuiEditboxGetString(ScrHandle, RaceNumEditId);
        (*CurrPlayer)->setRaceNumber((int)strtol(val, (char **)NULL, 0));
        snprintf(buf, sizeof(buf), "%d", (*CurrPlayer)->raceNumber());
        GfuiEditboxSetString(ScrHandle, RaceNumEditId, buf);
    }
}

static void
onChangeCategory(void *vp)
{
    if (CurrPlayer == PlayersInfo.end())
    {
        return;
    }

    int SelectedCategory = (*CurrPlayer)->Category();
    int maxranking = (*CurrPlayer)->maxRanking();
    GfLogDebug(" # Change category maxRanking = %i\n", maxranking);

    if (vp == 0)
    {
        if (SelectedCategory == 0)
            SelectedCategory = maxranking - 1;
        else
            SelectedCategory -= 1;
    }
    else
    {
       if (SelectedCategory == maxranking)
            SelectedCategory = 0;
       else
           SelectedCategory += 1;
    }

    GfLogDebug(" # Selected Category = %i\n", SelectedCategory);

    if(SelectedCategory == 0)
    {
        (*CurrPlayer)->setDefaultCarName("sc-deckard-conejo");
        (*CurrPlayer)->setDefaultCarRace("sc-deckard-conejo");

    }
    else if(SelectedCategory == 1)
    {
        (*CurrPlayer)->setDefaultCarName("09gp5-fmc");
        (*CurrPlayer)->setDefaultCarRace("09gp5-fmc");
    }
    else if(SelectedCategory == 2)
    {
        (*CurrPlayer)->setDefaultCarName("sc-boxer-96");
        (*CurrPlayer)->setDefaultCarRace("sc-boxer-96");

    }
    else if(SelectedCategory == 3)
    {
        (*CurrPlayer)->setDefaultCarName("10gp4-diamond");
        (*CurrPlayer)->setDefaultCarRace("10gp4-diamond");
    }
    else if(SelectedCategory == 4)
    {
        (*CurrPlayer)->setDefaultCarName("sc-spirit-300");
        (*CurrPlayer)->setDefaultCarRace("sc-spirit-300");

    }
    else if(SelectedCategory == 5)
    {
        (*CurrPlayer)->setDefaultCarName("91gp-boxer-amani");
        (*CurrPlayer)->setDefaultCarRace("91gp-boxer-amani");
    }
    else if(SelectedCategory == 6)
    {
        (*CurrPlayer)->setDefaultCarName("ls2-cavallo-360r");
        (*CurrPlayer)->setDefaultCarRace("ls2-cavallo-360r");

    }
    else if(SelectedCategory == 7)
    {
        (*CurrPlayer)->setDefaultCarName("01gp-midland-ps05");
        (*CurrPlayer)->setDefaultCarRace("01gp-midland-ps05");
    }
    else if(SelectedCategory == 8)
    {
        (*CurrPlayer)->setDefaultCarName("ls1-archer-r9");
        (*CurrPlayer)->setDefaultCarRace("ls1-archer-r9");

    }
    else if(SelectedCategory == 9)
    {
        (*CurrPlayer)->setDefaultCarName("01gpa-murasama");
        (*CurrPlayer)->setDefaultCarRace("01gpa-murasama");
    }
    else if(SelectedCategory == 10)
    {
        (*CurrPlayer)->setDefaultCarName("lp2-spire-s101");
        (*CurrPlayer)->setDefaultCarRace("lp2-spire-s101");

    }
    else if(SelectedCategory == 11)
    {
        (*CurrPlayer)->setDefaultCarName("11gpa-murasama");
        (*CurrPlayer)->setDefaultCarRace("11gpa-murasama");
    }
    else if(SelectedCategory == 12)
    {
        (*CurrPlayer)->setDefaultCarName("lp1-lolita-exr57");
        (*CurrPlayer)->setDefaultCarRace("lp1-lolita-exr57");
    }
    else if(SelectedCategory == 13)
    {
        (*CurrPlayer)->setDefaultCarName("12gpa-murasama");
        (*CurrPlayer)->setDefaultCarRace("12gpa-murasama");
    }
    else if(SelectedCategory == 14)
    {
        (*CurrPlayer)->setDefaultCarName("36gp-ettore-t59");
        (*CurrPlayer)->setDefaultCarRace("36gp-ettore-t59");
    }
    else if(SelectedCategory == 15)
    {
        (*CurrPlayer)->setDefaultCarName("67gp-mango-ms7");
        (*CurrPlayer)->setDefaultCarRace("67gp-mango-ms7");
    }
    /*else if(SelectedCategory == 16)
    {

    }
    else if(SelectedCategory == 17)
    {
        (*CurrPlayer)->setDefaultCarName("rs2-lion-2m");
        (*CurrPlayer)->setDefaultCarRace("rs2-lion-2m");
    }*/

    (*CurrPlayer)->setDefaultCategoryName(strCategory[SelectedCategory]);
    (*CurrPlayer)->setCategory(SelectedCategory);

    refreshEditVal();
}

static void
onChangeNation(void *vp)
{
    if (CurrPlayer == PlayersInfo.end())
    {
        return;
    }

    int SelectedNation = (*CurrPlayer)->Nation();

    if (vp == 0)
    {
        if (SelectedNation == 0)
            SelectedNation = 267;
        else
            SelectedNation -= 1;
    }
    else
    {
       if (SelectedNation == 267)
            SelectedNation = 0;
       else
           SelectedNation += 1;
    }

    (*CurrPlayer)->setDefaultNationName(strNation[SelectedNation]);
    (*CurrPlayer)->setNation(SelectedNation);

    refreshEditVal();
}

static void
onChangeLevel(void *vp)
{
    if (CurrPlayer == PlayersInfo.end())
    {
        return;
    }

    tSkillLevel skillLevel = (*CurrPlayer)->skillLevel();

    if (vp == 0)
    {
        if (skillLevel == ARCADE)
            skillLevel = PRO;
        else
            skillLevel = static_cast<tSkillLevel>(int(skillLevel) - 1);
    }
    else
    {
       if (skillLevel == PRO)
            skillLevel = ARCADE;
       else
           skillLevel = static_cast<tSkillLevel>(int(skillLevel) + 1);
    }

    (*CurrPlayer)->setSkillLevel(skillLevel);

    refreshEditVal();
}

static void
onChangeReverse(void *vdelta)
{
    const long delta = (long)vdelta;

    if (CurrPlayer == PlayersInfo.end())
    {
        return;
    }

    int autoReverse = (*CurrPlayer)->autoReverse();
    autoReverse += (int)delta;

    if (autoReverse < 0)
    {
        autoReverse = 1;
    }
    else if (autoReverse > 1)
    {
        autoReverse = 0;
    }

    (*CurrPlayer)->setAutoReverse(autoReverse);

    refreshEditVal();
}

/* Gear change mode change callback */
static void
onChangeGearChange(void *vp)
{
    if (CurrPlayer == PlayersInfo.end())
    {
        return;
    }

    tGearChangeMode gearChangeMode = (*CurrPlayer)->gearChangeMode();

    if (vp == 0)
    {
        if (gearChangeMode == GEAR_MODE_AUTO)
        {
            gearChangeMode = GEAR_MODE_HBOX;
        }
        else if (gearChangeMode == GEAR_MODE_SEQ)
        {
            gearChangeMode = GEAR_MODE_AUTO;
        }
        else if (gearChangeMode == GEAR_MODE_HBOX)
        {
            gearChangeMode = GEAR_MODE_GRID;
        }
        else
        {
            gearChangeMode = GEAR_MODE_SEQ;
        }
    }
    else
    {
        if (gearChangeMode == GEAR_MODE_AUTO)
        {
            gearChangeMode = GEAR_MODE_SEQ;
        }
        else if (gearChangeMode == GEAR_MODE_SEQ)
        {
            gearChangeMode = GEAR_MODE_GRID;
        }
        else if (gearChangeMode == GEAR_MODE_GRID)
        {
            gearChangeMode = GEAR_MODE_HBOX;
        }
        else
        {
            gearChangeMode = GEAR_MODE_AUTO;
        }
    }

    (*CurrPlayer)->setGearChangeMode(gearChangeMode);

    refreshEditVal();
}

static void
onActivate(void * /* dummy */)
{
    if (ReloadValues)
    {
        /* Load players settings */
        GenPlayerList();

        /* Initialize current player and select it */
        CurrPlayer = PlayersInfo.begin();
        GfuiScrollListSetSelectedElement(ScrHandle, ScrollList, 0);
        GfuiEnable(ScrHandle, ProfileButtonId, GFUI_DISABLE);
    }

    /* Display editable fields values */
    refreshEditVal();
}

static void
onActivateProfile(void * /* dummy */)
{
    char driverId[8];
    char newDriverId[8];
    char sectionPath[128];

    // Get new current player index (= identification number in params).
    olderIndex = (unsigned)(CurrPlayer - PlayersInfo.begin()) + 1;

    // Update preferences and drivers params (rename those after, add new).
    snprintf(sectionPath, sizeof(sectionPath), "%s/%s", HM_SECT_PREF, HM_LIST_DRV);

    snprintf(driverId, sizeof(driverId), "%u", 1);
    snprintf(newDriverId, sizeof(newDriverId), "%u", 0);
    GfParmListRenameElt(PrefHdle, sectionPath, driverId, newDriverId);
    GfLogDebug(" Ancien = %u\n", olderIndex);

    snprintf(driverId, sizeof(driverId), "%u", olderIndex);
    snprintf(newDriverId, sizeof(newDriverId), "%u", 1);
    GfParmListRenameElt(PrefHdle, sectionPath, driverId, newDriverId);

    snprintf(driverId, sizeof(driverId), "%u", 0);
    snprintf(newDriverId, sizeof(newDriverId), "%u", olderIndex);
    GfParmListRenameElt(PrefHdle, sectionPath, driverId, newDriverId);

    snprintf(sectionPath, sizeof(sectionPath), "%s/%s", ROB_SECT_ROBOTS, ROB_LIST_INDEX);

    snprintf(driverId, sizeof(driverId), "%u", 1);
    snprintf(newDriverId, sizeof(newDriverId), "%u", 0);
    GfParmListRenameElt(PlayerHdle, sectionPath, driverId, newDriverId);

    snprintf(driverId, sizeof(driverId), "%u", olderIndex);
    snprintf(newDriverId, sizeof(newDriverId), "%u", 1);
    GfParmListRenameElt(PlayerHdle, sectionPath, driverId, newDriverId);

    snprintf(driverId, sizeof(driverId), "%u", 0);
    snprintf(newDriverId, sizeof(newDriverId), "%u", olderIndex);
    GfParmListRenameElt(PlayerHdle, sectionPath, driverId, newDriverId);

    snprintf(sectionPath, sizeof(sectionPath), "%s/%s", "cars", ROB_LIST_INDEX);

    snprintf(driverId, sizeof(driverId), "%u", 1);
    snprintf(newDriverId, sizeof(newDriverId), "%u", 0);
    GfParmListRenameElt(GarageHdle, sectionPath, driverId, newDriverId);

    snprintf(driverId, sizeof(driverId), "%u", olderIndex);
    snprintf(newDriverId, sizeof(newDriverId), "%u", 1);
    GfParmListRenameElt(GarageHdle, sectionPath, driverId, newDriverId);

    snprintf(driverId, sizeof(driverId), "%u", 0);
    snprintf(newDriverId, sizeof(newDriverId), "%u", olderIndex);
    GfParmListRenameElt(GarageHdle, sectionPath, driverId, newDriverId);

    GfParmWriteFile(NULL, PlayerHdle, HumanDriverModuleName);
    GfParmWriteFile(NULL, PrefHdle, "preferences");
    GfParmWriteFile(NULL, GarageHdle, "cars");

    // Update GUI.
    refreshEditVal();
    GfDrivers::self()->reload();

    onQuitPlayerConfig(0);
}

void *
PlayerConfigMenuInit(void *prevMenu)
{
    /* Save previous screen handle for exit time */
    PrevScrHandle = prevMenu;
    //CarN = NULL;

    /* Screen already created : nothing more to do */
    if (ScrHandle)
    {
        return ScrHandle;
    }

    /* Create the screen, load menu XML descriptor and create static controls */
    ScrHandle = GfuiScreenCreate((float*)NULL, NULL, onActivate, NULL, (tfuiCallback)NULL, 1);
    void *param = GfuiMenuLoad("playerconfigmenu.xml");
    GfuiMenuCreateStaticControls(ScrHandle, param);

    /* Player scroll list */
    ScrollList = GfuiMenuCreateScrollListControl(ScrHandle, param, "playerscrolllist", NULL, onSelect);

    /* New, copy and delete player buttons */
    GfuiMenuCreateButtonControl(ScrHandle, param, "new", NULL, onNewPlayer);
    CopyButtonId = GfuiMenuCreateButtonControl(ScrHandle, param, "copy", NULL, onCopyPlayer);
    RemoveButtonId = GfuiMenuCreateButtonControl(ScrHandle, param, "delete", NULL, onDeletePlayer);
    ProfileButtonId = GfuiMenuCreateButtonControl(ScrHandle, param, "profile", NULL, onActivateProfile);

    GfuiEnable(ScrHandle, CopyButtonId, GFUI_DISABLE);
    GfuiEnable(ScrHandle, RemoveButtonId, GFUI_DISABLE);
    GfuiEnable(ScrHandle, ProfileButtonId, GFUI_DISABLE);

    /* Access to control screen button */
    GfuiMenuCreateButtonControl(ScrHandle, param, "controls", NULL, onConfControls);

    /* Category's car select */
    GfuiMenuCreateButtonControl(ScrHandle, param, "categoryleftarrow", (void*)0, onChangeCategory);
    GfuiMenuCreateButtonControl(ScrHandle, param, "categoryrightarrow", (void*)1, onChangeCategory);
    CategoryEditId = GfuiMenuCreateLabelControl(ScrHandle, param, "categorytext");

    /* Player name editbox */
    NameEditId = GfuiMenuCreateEditControl(ScrHandle, param, "nameedit", NULL, onActivateName, onChangeName);

    /* Player skill level "combobox" (left arrow, label, right arrow) */
    GfuiMenuCreateButtonControl(ScrHandle, param, "skillleftarrow", (void*)0, onChangeLevel);
    GfuiMenuCreateButtonControl(ScrHandle, param, "skillrightarrow", (void*)1, onChangeLevel);
    SkillEditId = GfuiMenuCreateLabelControl(ScrHandle, param, "skilltext");

    /* Player nationality "combobox" (left arrow, label, right arrow) */
    GfuiMenuCreateButtonControl(ScrHandle, param, "nationleftarrow", (void*)0, onChangeNation);
    GfuiMenuCreateButtonControl(ScrHandle, param, "nationrightarrow", (void*)1, onChangeNation);
    NationEditId = GfuiMenuCreateLabelControl(ScrHandle, param, "nationtext");

    /* Races and pits numbers editboxes (Must they really stay here ?) */
    /* TODO: definitely should be moved from here to pre-race menus. kilo */
    RaceNumEditId = GfuiMenuCreateEditControl(ScrHandle, param, "racenumedit", NULL, NULL, onChangeNum);

    /* Gear changing mode and associated "combobox" (left arrow, label, right arrow) */
    GfuiMenuCreateButtonControl(ScrHandle, param, "gearleftarrow", (void*)0, onChangeGearChange);
    GfuiMenuCreateButtonControl(ScrHandle, param, "gearrightarrow", (void*)1, onChangeGearChange);
    GearChangeEditId = GfuiMenuCreateLabelControl(ScrHandle, param, "geartext");

    /* Gear changing auto-reverse flag and associated "combobox" (left arrow, label, right arrow) */
    AutoReverseLabelId = GfuiMenuCreateLabelControl(ScrHandle, param, "autoreversetext");
    AutoReverseLeftId = GfuiMenuCreateButtonControl(ScrHandle, param, "autoleftarrow", (void*)-1, onChangeReverse);
    AutoReverseRightId = GfuiMenuCreateButtonControl(ScrHandle, param, "autorightarrow", (void*)1, onChangeReverse);
    AutoReverseEditId = GfuiMenuCreateLabelControl(ScrHandle, param, "autotext");

    // Accept and Cancel buttons.
    GfuiMenuCreateButtonControl(ScrHandle, param, "ApplyButton", NULL, onSavePlayerList);
    GfuiMenuCreateButtonControl(ScrHandle, param, "CancelButton", NULL, onQuitPlayerConfig);

    /* Web username and password editbox */
    WebUsernameEditId = GfuiMenuCreateEditControl(ScrHandle, param, "webusernameedit", NULL, NULL, onChangeWebserverusername);
    WebPasswordEditId = GfuiMenuCreateEditControl(ScrHandle, param, "webpasswordedit", NULL, NULL, onChangeWebserverpassword);

    /* Web test button */
    WebServerTestLoginId = GfuiMenuCreateButtonControl(ScrHandle, param, "weblogintest", NULL, onWebserverLoginTest);
    
   	WebServerCheckboxId =
    GfuiMenuCreateCheckboxControl(ScrHandle, param, "webservercheckbox", NULL, onChangeWebserverenabled);

    // Close menu XML descriptor.
    GfParmReleaseHandle(param);

    // Register keyboard shortcuts.
    GfuiMenuDefaultKeysAdd(ScrHandle);
    GfuiAddKey(ScrHandle, GFUIK_RETURN, "Accept and save changes", NULL, onSavePlayerList, NULL);
    GfuiAddKey(ScrHandle, GFUIK_ESCAPE, "Cancel changes", NULL, onQuitPlayerConfig, NULL);

    return ScrHandle;
}
