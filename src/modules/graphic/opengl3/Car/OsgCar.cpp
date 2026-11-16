/***************************************************************************

    file                 : OsgCar.cpp
    created              : Mon Aug 21 18:24:02 CEST 2012
    copyright            : (C)2012 by Gaétan André, (C)2014 Xavier Bertaux
    email                : gaetan.andre@gmail.com

***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <osg/MatrixTransform>
#include <osg/Switch>
#include <osg/Group>
#include <osgUtil/Simplifier>
#include <osgViewer/Viewer>
#include <osg/Program>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Texture2D>
#include <osg/AlphaFunc>
#include <osg/BlendFunc>
#include <osg/Material>
#include <osg/Depth>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <portability.h>
#include <robottools.h>

#include "OsgLoader.h"
#include "OsgCar.h"
#include "OsgMath.h"
#include "OsgScreens.h"
#include "OsgRender.h"
#include "OsgMain.h"
#include "OsgSky.h"
#include "OsgShader.h"
#include "OsgReflectionMapping.h"
#include "OsgNodeMask.h"

namespace osggraph {

class SDRender;

SDCar::SDCar(void) :
    carTransform(NULL),
    lights_branch(NULL),
    instruments_branch(NULL),
    SteerRot(NULL),
    SteerRot2(NULL),
    rpm(NULL),
    speed(NULL),
    fuel(NULL),
    water(NULL),
    rpmRot(NULL),
    speedRot(NULL),
    fuelRot(NULL),
    waterRot(NULL),
    LODSelector(NULL),
    DRMSelector(NULL),
    DRMSelector2(NULL),
    DriverSelector(NULL),
    SteerSelector(NULL),
    RearWingSelector(NULL),
    gearSelector(NULL),
    brakeLightSelector(NULL),
    engineLightSelector(NULL),
    fuelLightSelector(NULL),
    absLightSelector(NULL),
    tclLightSelector(NULL),
    speedLightSelector(NULL),
    car_shaded_body(NULL),
    carEntity(NULL),
    _speedmodel(false),
    _rpmmodel(false),
    _fuelmodel(false),
    _watermodel(false),
    _gearmodel(false),
    _absmodel(false),
    _tclmodel(false),
    _brakelightmodel(false),
    _fuellightmodel(false),
    _waterlightmodel(false),
    _abslightmodel(false),
    _tcllightmodel(false),
    _speedlightmodel(false),
    _rpmlightsSelectorinsg(false),
    _gearSelectorinsg(false),
    nSteer(0),
    nDRM(0),
    nDRM2(0),
    steerWheelRot(0.0),
    rpmMin(0.0),
    rpmMax(0.0),
    speedMin(0.0),
    speedMax(0.0),
    car(NULL),
    shader(NULL),
    reflectionMapping(NULL),
    reflectionMappingMethod(REFLECTIONMAPPING_OFF)
{
    _cockpit =      false;
    _driver =       false;
    _instruments =  false;
    _wing1 =        false;
    _wing3 =        false;
    _steer =        false;
    _carLOD =       false;

    _carShader = 0;

    for (size_t i = 0; i < 32; ++i)
    {
        DRMSelectMask[i] = 0;
        DRMThreshold[i] = 0.0f;
    }
}

SDCar::~SDCar(void)
{
    if(instruments_branch)
    {
        instruments_branch->removeChildren(0, instruments_branch->getNumChildren());
        instruments_branch = NULL;
    }

    if (lights_branch)
        getCarLights()->getLightsRoot()->removeChild(lights_branch.get());

    if(carEntity != NULL)
    {
        carEntity->removeChildren(0, carEntity->getNumChildren());
        carEntity = NULL;
    }

    delete shader;
    delete reflectionMapping;
}

void SDCar::loadCarLights()
{
    SDCarLights *carLights = getCarLights();

    if (lights_branch)
        carLights->getLightsRoot()->removeChild(lights_branch);
    lights_branch = new osg::MatrixTransform;

    char path[1024] = {};
    void *handle = car->_carHandle;
    snprintf(path, 1023, "%s/%s", SECT_GROBJECTS, SECT_LIGHT);
    int lightNum = GfParmGetEltNb(handle, path);

    for (int i = 0; i < lightNum; i++)
    {
        snprintf(path, 1023, "%s/%s/%d", SECT_GROBJECTS, SECT_LIGHT, i + 1);

        osg::Vec3d position;
        position[0] = GfParmGetNum(handle, path, PRM_XPOS, NULL, 0);
        position[1] = GfParmGetNum(handle, path, PRM_YPOS, NULL, 0);
        position[2] = GfParmGetNum(handle, path, PRM_ZPOS, NULL, 0);

        osg::Vec3d normal;
        normal[0] = position[0] > 0 ? 1 : -1;

        const char *typeName = GfParmGetStr(handle, path, PRM_TYPE, "");
        CarLightType type = CAR_LIGHT_TYPE_NONE;
        if (!strcmp(typeName, VAL_LIGHT_HEAD1))
        {
            type = CAR_LIGHT_TYPE_FRONT;
        }
        else if (!strcmp(typeName, VAL_LIGHT_HEAD2))
        {
            type = CAR_LIGHT_TYPE_FRONT2;
        }
        else if (!strcmp(typeName, VAL_LIGHT_BRAKE))
        {
            type = CAR_LIGHT_TYPE_BRAKE;
        }
        else if (!strcmp(typeName, VAL_LIGHT_BRAKE2))
        {
            type = CAR_LIGHT_TYPE_BRAKE2;
        }
        else if (!strcmp(typeName, VAL_LIGHT_REAR))
        {
            type = CAR_LIGHT_TYPE_REAR;
        }
        else if (!strcmp(typeName, VAL_LIGHT_REVERSE))
        {
            type = CAR_LIGHT_TYPE_REVERSE;
        }
        else
        {
            GfLogError("Invalid light type '%s' (%s: %s)\n", typeName, car->_carName, path);
            continue;
        }

        double size = GfParmGetNum(handle, path, PRM_SIZE, NULL, 0.2);

        osg::ref_ptr<osg::StateSet> state_set = carLights->getStateSet(type);

        SDCarLight light;
        lights_branch->addChild( light.init(type, state_set, position, normal, size, 4) );
        lights.push_back(light);
    }

    lights_branch->setMatrix( carTransform->getMatrix() );
    carLights->getLightsRoot()->addChild(lights_branch);
}

osg::ref_ptr<osg::Node> SDCar::loadCar(tCarElt *Car, bool tracktype, bool subcat, int carshader)
{
    this->carTransform = new osg::MatrixTransform;
    this->car_shaded_body = new osg::Group;
    this->carEntity = new osg::Group;
    this->car = Car;
    int wings = this->car->_options.option_wings;

    /* Schedule texture mapping if we are using a custom skin and/or a master 3D model */
    const bool bMasterModel = strlen(this->car->_masterModel) != 0;
    const bool bCustomSkin = strlen(this->car->_skinName) != 0;

    std::string bSkinName;
    std::string bCarName;

    static const int nMaxTexPathSize = 512;
    char buf[nMaxTexPathSize];
    char path[nMaxTexPathSize];

    int index;
    void *handle;
    const char *param;

    int nranges = 0;

    _carShader = carshader;
    GfLogDebug("Graphic options car shader = %d\n", _carShader);

    osgLoader loader;

    std::string TmpPath = GfDataDir();
    std::string strTPath;

    index = car->index;	/* current car's index */
    handle = car->_carHandle;

    GfLogDebug("[gr] Init(%d) car %s for driver %s index %d\n", index, car->_carName, car->_modName, car->_driverIndex);
    GfLogDebug("[gr] Init(%d) car %s MasterModel name\n", index, car->_masterModel);
    GfLogDebug("[gr] Init(%d) car %s CustomSkin name\n", index, car->_skinName);

    snprintf(buf, nMaxTexPathSize, "%sdrivers/%s/%d/",
             GfDataDir(), car->_modName, car->_driverIndex);
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "%sdrivers/%s/%s/",
             GfDataDir(), car->_modName, car->_carName);
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "%sdrivers/%s/",
             GfDataDir(), car->_modName);
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "drivers/%s/%d/%s/",
             car->_modName, car->_driverIndex, car->_carName);
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "drivers/%s/%d/",
             car->_modName, car->_driverIndex);
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "drivers/%s/%s/",
             car->_modName, car->_carName);
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "drivers/%s/", car->_modName);
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "data/objects/");
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    snprintf(buf, nMaxTexPathSize, "data/textures/");
    strTPath = TmpPath+buf;
    loader.AddSearchPath(strTPath);

    /* loading raw car level 0*/
    //  selIndex = 0; 	/* current selector index */
    snprintf(buf, nMaxTexPathSize, "%s.acc",
             bMasterModel ? car->_masterModel : car->_carName); /* default car 3D model file */
    snprintf(path, nMaxTexPathSize, "%s/%s/1", SECT_GROBJECTS, LST_RANGES);
    param = GfParmGetStr(handle, path, PRM_CAR, buf);

    std::string strPath = GfDataDir();

    snprintf(buf, nMaxTexPathSize, "cars/models/%s/%s", bMasterModel ? car->_masterModel : car->_carName, param);

    strPath+=buf;

    std::string name = car->_carName;

    osg::ref_ptr<osg::Group> gCar = new osg::Group;
    gCar->setName("CAR");
    osg::ref_ptr<osg::Switch> pBody =new osg::Switch;
    pBody->setName("COCK");
    osg::ref_ptr<osg::Node> pCockpit = new osg::Node;
    osg::ref_ptr<osg::Switch> pWing = new osg::Switch;
    pWing->setName("WING");

    //strPath+=buf;
    GfLogDebug("Chemin Textures : %s\n", strTPath.c_str());

    //osg::ref_ptr<osg::Node> Car = new osg::Node;
    if (bMasterModel)
        bSkinName = name;

    if (bCustomSkin)
    {
        snprintf(buf, nMaxTexPathSize, "%s-%s", car->_carName, car->_skinName);
        bSkinName = buf;
        snprintf(buf, nMaxTexPathSize, "%s.png", car->_carName);
        bCarName = buf;
        GfLogDebug("Car Texture = %s - Car Name = %s\n", bSkinName.c_str(), bCarName.c_str());
    }

    osg::ref_ptr<osg::Node> pCar = loader.Load3dFile(strPath, true, bCarName, bSkinName);
    if (pCar)
        GfLogDebug("Load Car ACC !\n");
    else
    {
        GfLogError("Failed to load %s\n", strPath.c_str());
        pCar = new osg::Node;
    }

    /* Set a selector on the wing type MPA*/
    snprintf(path, nMaxTexPathSize, "%s/%s", SECT_GROBJECTS, SECT_WING_MODEL);
    param = GfParmGetStr(handle, path, PRM_WING_1, NULL);

    if (param)
    {
        _wing1 = true;

        std::string tmp = GfDataDir();
        snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
        tmp = tmp+buf;

        param = GfParmGetStr(handle, path, PRM_WING_1, NULL);
        strPath=tmp+param;
        osg::ref_ptr<osg::Node> pWin1 = loader.Load3dFile(strPath, true, bCarName, bSkinName);
        if (pWin1)
            GfLogDebug("Load Wing1 ACC ! %s\n", strPath.c_str() );
        else
        {
            GfLogError("Failed to load %s\n", strPath.c_str());
            pWin1 = new osg::Node;
        }
        pWin1->setName("WING1");

        param = GfParmGetStr(handle, path, PRM_WING_2, NULL);
        strPath=tmp+param;
        osg::ref_ptr<osg::Node> pWin2 = loader.Load3dFile(strPath, true, bCarName, bSkinName);
        if (pWin2)
            GfLogDebug("Load Wing2 ACC ! %s\n", strPath.c_str());
        else
        {
            GfLogError("Failed to load %s\n", strPath.c_str());
            pWin2 = new osg::Node;
        }
        pWin2->setName("WING2");
        GfLogDebug("Load Wing2 ACC ! %s\n", strPath.c_str());

        param = GfParmGetStr(handle, path, PRM_WING_3, NULL);
        strPath=tmp+param;
        osg::ref_ptr<osg::Node> pWin3 = loader.Load3dFile(strPath, true, bCarName, bSkinName);
        if (pWin3)
            GfLogDebug("Load Wing3 ACC ! %s\n", strPath.c_str());
        else
        {
            GfLogError("Failed to load %s\n", strPath.c_str());
            pWin3 = new osg::Node;
        }
        pWin3->setName("WING3");
        GfLogDebug("Load Wing3 ACC ! %s\n", strPath.c_str());

        pWing->addChild(pWin1.get(), false);
        pWing->addChild(pWin2.get(), false);
        pWing->addChild(pWin3.get(), true);

        GfLogDebug("tracktype = %d - subcat = %d\n", tracktype, subcat);

        if (tracktype == false)
        {
            pWing->setSingleChildOn(2);
            GfLogDebug("Activate Wing Road !\n");
        }
        else
        {
            if(subcat == false)
            {
                pWing->setSingleChildOn(0);
                GfLogDebug("Activate Wing Short !\n");
            }
            else
            {
                pWing->setSingleChildOn(1);
                GfLogDebug("Activate Wing Long !\n");
            }
        }
    }

    // Separate rear wing models for animation according to rear wing angle ...
    snprintf(path, nMaxTexPathSize, "%s/%s", SECT_GROBJECTS, LST_REARWING);
    nranges = GfParmGetEltNb(handle, path) + 1;

    if (nranges > 1)
    {
        _wing3 = true;
        this->DRMSelector2 = new osg::Switch;
        this->RearWingSelector = new osg::Switch;
        this->RearWingSelector->setName("WINGREAR");
        std::string tmp = GfDataDir();
        snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
        tmp = tmp+buf;

        // Add the rearwings
        for (int i = 1; i < nranges; i++)
        {
            snprintf(path, nMaxTexPathSize, "%s/%s/%d", SECT_GROBJECTS, LST_REARWING, i);
            param = GfParmGetStr(handle, path, PRM_REARWINGMODEL, "");

            strPath = tmp+param;
            osg::ref_ptr<osg::Node> pWing1_branch = loader.Load3dFile(strPath, true, bCarName, bSkinName);
            if (pWing1_branch)
                GfLogDebug("Loading Wing animate %i - %s !\n", i, strPath.c_str());
            else
            {
                GfLogError("failed to load Wing animate %i - %s !\n", i, strPath.c_str());
                pWing1_branch = new osg::Node;
            }

            this->DRMSelector2->addChild(pWing1_branch.get());
            strPath.clear();
        }

        this->DRMSelector2->setSingleChildOn(0);
        osg::ref_ptr<osg::Group> vide = new osg::Group;
        this->RearWingSelector->addChild(vide.get());
        this->RearWingSelector->addChild(this->DRMSelector2.get());
        this->RearWingSelector->setSingleChildOn(1);
        GfLogDebug("Rear Wing angle Loaded\n");
    }

    if(wings > 0)
    {
        std::string tmp = GfDataDir();
        snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
        tmp = tmp+buf;

        if(wings == 1)
        {
            snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_WING_MODEL);
            param = GfParmGetStr(handle, path, PRM_WING1, NULL);
        }
        else
        {
            snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_WING_MODEL);
            param = GfParmGetStr(handle, path, PRM_WING2, NULL);
        }

        if(param)
        {
            strPath = tmp+param;
            osg::ref_ptr<osg::Node> pWing1_branch = loader.Load3dFile(strPath, true, bCarName, bSkinName);

            if (pWing1_branch)
                GfLogDebug("Loading Wing Option - %s !\n", strPath.c_str());
            else
            {
                GfLogError("failed to load Wing Option - %s !\n", strPath.c_str());
                pWing1_branch = new osg::Node;
            }

            gCar->addChild(pWing1_branch.get());
        }
    }

    // Cockpit separate object loaded  ...
    snprintf(path, nMaxTexPathSize, "%s/%s", SECT_GROBJECTS, SECT_COCKPIT);
    param = GfParmGetStr(handle, path, PRM_MODELCOCKPIT, NULL);

    if (param)
    {
        _cockpit = true;
        std::string tmp = GfDataDir();
        snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
        tmp = tmp+buf;

        strPath= tmp+param;

        pCockpit = loader.Load3dFile(strPath, true, bCarName, bSkinName);
        if (pCockpit)
            GfLogDebug("Cockpit loaded = %s !\n", strPath.c_str());
        else
        {
            GfLogError("Failed to load %s\n", strPath.c_str());
            pCockpit = new osg::Node;
        }
    }

    pBody->addChild(pCockpit.get(), false);

    /* add Steering Wheel 0 (if one exists) */
    snprintf(path, nMaxTexPathSize, "%s/%s", SECT_GROBJECTS, SECT_STEERWHEEL);
    param = GfParmGetStr(handle, path, PRM_SW_MODEL, NULL);

    if (param)
    {
        _steer = true;
        std::string tmpPath = GfDataDir();
        snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
        tmpPath = tmpPath+buf;

        strPath = tmpPath + param;

        osg::ref_ptr<osg::Node> steerEntityLo = loader.Load3dFile(strPath, true, "", "");
        if (steerEntityLo == nullptr)
            GfLogError("Failed to load %s\n", strPath.c_str());
        else
        {
            this->nSteer = 1;
            this->steerWheelRot = GfParmGetNum(handle, SECT_STEER, PRM_STEERROT, NULL, 2.0) / 2.0;

            this->SteerSelector = new osg::Switch;
            this->SteerSelector->setName("STEER");
            osg::ref_ptr<osg::Group> steerbranch = new osg::Group;
            osg::ref_ptr<osg::MatrixTransform> steerLoc = new osg::MatrixTransform;

            tdble xpos = GfParmGetNum(handle, path, PRM_XPOS, NULL, 0.0);
            tdble ypos = GfParmGetNum(handle, path, PRM_YPOS, NULL, 0.0);
            tdble zpos = GfParmGetNum(handle, path, PRM_ZPOS, NULL, 0.0);
            float angl = GfParmGetNum(handle, path, PRM_SW_ANGLE, NULL, 0.0);

            this->SteerRot = new osg::MatrixTransform;
            osg::Matrix pos = osg::Matrix::translate(0.0, 0.0, 0.0);
            this->SteerRot->setMatrix( pos);
            this->SteerRot->setName("STEERBRANCHE");
            this->SteerRot->addChild(steerEntityLo.get());

            angl = SD_DEGREES_TO_RADIANS * angl;
            pos = osg::Matrix::translate(xpos, ypos, zpos);
            osg::Matrix rot = osg::Matrix::rotate(angl, osg::Y_AXIS);
            pos = rot * pos;
            steerLoc->setMatrix(pos);
            steerLoc->addChild(this->SteerRot.get());
            steerbranch->addChild( steerLoc );
            this->SteerSelector->addChild(steerbranch);
            this->SteerSelector->setSingleChildOn(0);

            param = GfParmGetStr(handle, path, PRM_SW_MODELHR, NULL);

            if (param)
            {
                tmpPath = GfDataDir();
                snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
                tmpPath = tmpPath+buf;

                strPath = tmpPath + param;
                osg::ref_ptr<osg::Node> steerEntityHi = loader.Load3dFile(strPath, true, "", "");
                if (steerEntityHi == nullptr)
                {
                    GfLogError("Failed to load: %s\n", strPath.c_str());
                    steerEntityHi = new osg::Node;
                }
                this->nSteer = 2;

                steerbranch = new osg::Group;
                steerLoc = new osg::MatrixTransform;

                this->SteerRot2 = new osg::MatrixTransform;
                pos = osg::Matrix::translate(0.0, 0.0, 0.0);
                this->SteerRot2->setMatrix(pos);
                this->SteerRot2->addChild(steerEntityHi.get());

                pos = osg::Matrix::translate(xpos, ypos, zpos);
                rot = osg::Matrix::rotate(angl, osg::Y_AXIS);
                pos = rot * pos;
                steerLoc->setMatrix(pos);
                steerLoc->addChild(this->SteerRot2.get());
                steerbranch->addChild( steerLoc );
                this->SteerSelector->addChild(steerbranch);
                this->SteerSelector->setSingleChildOn(1);
            }
        }

        GfLogDebug("Low Steer Loading \n");
    }

    // separate driver models for animation according to steering wheel angle ...
    snprintf(path, nMaxTexPathSize, "%s/%s", SECT_GROBJECTS, LST_DRIVER);
    nranges = GfParmGetEltNb(handle, path) + 1;
    this->nDRM = nranges - 1;
    this->DRMSelector = NULL;

    if (nranges > 1)
    {
        _driver = true;
        this->DriverSelector = new osg::Switch;
        this->DriverSelector->setName("DRIVER");
        int selIndex = 0;
        // We have at least one separate driver model to add...
        osg::ref_ptr<osg::Node> driverEntity = new osg::Node;
        osg::ref_ptr<osg::Switch> DRMSel = new osg::Switch;
        this->DRMSelector = new osg::Switch;
        this->carTransform->addChild(DriverSelector.get());

        std::string tmp = GfLocalDir();
        std::string driver_path;

        // add the drivers
        for (int i = 1; i < nranges; i++)
        {
            osg::ref_ptr<osg::Group> driverBody = new osg::Group;
            osg::ref_ptr<osg::MatrixTransform> driverLoc = new osg::MatrixTransform;

            snprintf(buf, nMaxTexPathSize, "%s/%s/%d", SECT_GROBJECTS, LST_DRIVER, i);
            param = GfParmGetStr(handle, buf, PRM_DRIVERMODEL, "");

            tdble xpos = GfParmGetNum(handle, buf, PRM_XPOS, NULL, 0.0);
            tdble ypos = GfParmGetNum(handle, buf, PRM_YPOS, NULL, 0.0);
            tdble zpos = GfParmGetNum(handle, buf, PRM_ZPOS, NULL, 0.0);
            osg::Matrix pos = osg::Matrix::translate(xpos, ypos, zpos);

            driverLoc->setMatrix(pos);

            driver_path = tmp+param;
            driverEntity = loader.Load3dFile(driver_path, true, bCarName, bSkinName);
            GfLogDebug("Loading Animated Driver %i - %s \n", i, driver_path.c_str());

            driverLoc->addChild(driverEntity.get());
            driverBody->addChild(driverLoc.get());
            this->DRMSelector->addChild(driverBody.get());

            this->DRMSelectMask[i-1] = 1 << selIndex;
            GfLogDebug(" # DRMSelectMask = %i - nDRM = %i\n", DRMSelectMask[i-1], nDRM);
            driver_path.clear();

            selIndex++;
        }

        int i;

        // select a default driver - steer value of 0.0 is desired...
        for (i = 1; i < nranges; i++)
        {
            if (this->DRMThreshold[i-1] == 0.0f)
            {
                this->DRMSelector->setSingleChildOn( DRMSelectMask[i-1] );
                break;
            }
        }

        if (i == nranges)
            this->DRMSelector->setSingleChildOn( DRMSelectMask[0] );

        osg::ref_ptr<osg::Group> vide = new osg::Group;

        this->DriverSelector->addChild(vide.get());
        this->DriverSelector->addChild(this->DRMSelector.get());
        this->DriverSelector->setSingleChildOn(0);
    }

    this->car_shaded_body->addChild(pCar.get());

    if(_wing1)
        this->car_shaded_body->addChild(pWing.get());

    if(_wing3)
        this->car_shaded_body->addChild(RearWingSelector.get());

    gCar->addChild(car_shaded_body.get());

    if(_driver)
        gCar->addChild(DriverSelector.get());

    if(_steer && SteerSelector)
        gCar->addChild(SteerSelector.get());

    pBody->addChild(gCar.get(), true);
    pBody->setSingleChildOn(1);

    osg::ref_ptr<osg::MatrixTransform> transform1 = new osg::MatrixTransform;

    transform1->addChild(pBody.get());

    // GfOut("loaded car %d",pCar.get());
    this->carTransform = transform1.get();

    //wheels = new SDWheels;
    this->carTransform->addChild(wheels.initWheels(car, handle));

    this->carEntity = new osg::Group;
    this->carEntity->addChild(carTransform);

    /* Instruments */
    if (gr3DInstruments == true)
    {
        snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_INSTRUMENTS);
        param = GfParmGetStr(handle, path, PRM_SPEED_A_MODEL, NULL);
        instruments_branch = new osg::MatrixTransform;

        if (param)
        {
            osg::ref_ptr<osg::Node> speedEntity = new osg::Node;
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;
            speedEntity = loader.Load3dFile(strPath, true, "", "");
            this->instruments_branch->addChild(speedEntity.get());
            GfLogDebug("# Load Speed Instrument !!!\n");
        }

        param = GfParmGetStr(handle, path, PRM_RPM_A_MODEL, NULL);

        if (param)
        {
            osg::ref_ptr<osg::Node> rpmEntity = new osg::Node;
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;
            rpmEntity = loader.Load3dFile(strPath, true, "", "");
            instruments_branch->addChild(rpmEntity.get());
            GfLogDebug("# Load RPM Instrument !!!\n");
        }

        param = GfParmGetStr(handle, path, PRM_WATER_A_MODEL, NULL);

        if (param)
        {
            osg::ref_ptr<osg::Node> waterEntity = new osg::Node;
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;
            waterEntity = loader.Load3dFile(strPath, true, "", "");
            instruments_branch->addChild(waterEntity.get());
            GfLogDebug("# Load Water Instrument !!!\n");
        }

        param = GfParmGetStr(handle, path, PRM_FUEL_A_MODEL, NULL);

        if (param)
        {
            osg::ref_ptr<osg::Node> fuelEntity = new osg::Node;
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;
            fuelEntity = loader.Load3dFile(strPath, true, "", "");
            instruments_branch->addChild(fuelEntity.get());
            GfLogDebug("# Load Fuel Instrument !!!\n");
        }

        param = GfParmGetStr(handle, path, PRM_RPM_NEEDLE_MODEL, NULL);

        if (param)
        {
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;

            osg::ref_ptr<osg::Node> rpmEntity = loader.Load3dFile(strPath, true, "", "");

            if (rpmEntity == nullptr)
                GfLogError("Failed to load %s\n", strPath.c_str());
            else
            {
                osg::ref_ptr<osg::Group> rpmBranch = new osg::Group;
                osg::ref_ptr<osg::MatrixTransform> rpmLoc = new osg::MatrixTransform;

                tdble xpos = GfParmGetNum(handle, path, PRM_RPM_XPOS, NULL, 0.0);
                tdble ypos = GfParmGetNum(handle, path, PRM_RPM_YPOS, NULL, 0.0);
                tdble zpos = GfParmGetNum(handle, path, PRM_RPM_ZPOS, NULL, 0.0);
                float angl = GfParmGetNum(handle, path, PRM_RPM_ANGLE, NULL, 0.0);

                this->rpmRot = new osg::MatrixTransform;
                osg::Matrix pos = osg::Matrix::translate(0.0, 0.0, 0.0);
                this->rpmRot->setMatrix( pos);
                this->rpmRot->setName("RPMBRANCH");
                this->rpmRot->addChild(rpmEntity.get());

                angl = SD_DEGREES_TO_RADIANS * angl;
                pos = osg::Matrix::translate(xpos, ypos, zpos);
                osg::Matrix rot = osg::Matrix::rotate(angl, osg::Y_AXIS);
                pos = rot * pos;
                rpmLoc->setMatrix(pos);
                rpmLoc->addChild(rpmRot.get());
                rpmBranch->addChild( rpmLoc );
                instruments_branch->addChild(rpmBranch.get());
                this->_rpmmodel = true;
                this->rpmMax = GfParmGetNum(handle, SECT_GROBJECTS, PRM_TACHO_MAXVAL, (char*)NULL, RPM2RADS(10000));
                this->rpmMax = rpmMax / 0.104719755f;
                GfLogDebug("# RPM Max = %.1f\n", this->rpmMax);
            }

            GfLogDebug("rpm needle Loading \n");
        }
        else
        {
            this->_rpmmodel = false;
        }

        param = GfParmGetStr(handle, path, PRM_SPEED_NEEDLE_MODEL, NULL);
        GfLogDebug("# Loading speed needle !\n");

        if (param)
        {
            this->_speedmodel = true;
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;
            GfLogDebug("# Path speed needle = %s\n", strPath.c_str());

            osg::ref_ptr<osg::Node> speedEntity = loader.Load3dFile(strPath, true, "", "");

            if (speedEntity == nullptr)
                GfLogError("Failed to load %s\n", strPath.c_str());
            else
            {
                //osg::ref_ptr<osg::Node> speedEntity = new osg::Node;
                osg::ref_ptr<osg::Group> speedBranch = new osg::Group;
                osg::ref_ptr<osg::MatrixTransform> speedLoc = new osg::MatrixTransform;

                tdble xpos = GfParmGetNum(handle, path, PRM_SPEED_XPOS, NULL, 0.0);
                tdble ypos = GfParmGetNum(handle, path, PRM_SPEED_YPOS, NULL, 0.0);
                tdble zpos = GfParmGetNum(handle, path, PRM_SPEED_ZPOS, NULL, 0.0);
                float angl = GfParmGetNum(handle, path, PRM_SPEED_ANGLE, NULL, 0.0);

                this->speedRot = new osg::MatrixTransform;
                osg::Matrix pos = osg::Matrix::translate(0.0, 0.0, 0.0);
                this->speedRot->setMatrix( pos);
                this->speedRot->setName("SPEEDBRANCH");
                this->speedRot->addChild(speedEntity.get());

                angl = SD_DEGREES_TO_RADIANS * angl;
                pos = osg::Matrix::translate(xpos, ypos, zpos);
                osg::Matrix rot = osg::Matrix::rotate(angl, osg::Y_AXIS);
                pos = rot * pos;
                speedLoc->setMatrix(pos);
                speedLoc->addChild(speedRot.get());
                speedBranch->addChild( speedLoc );
                instruments_branch->addChild(speedBranch.get());

                this->speedMax = GfParmGetNum(handle, SECT_GROBJECTS, PRM_SPEEDO_MAXVAL, (char*)NULL, 100);
                GfLogDebug("# SpeedMax = %.1f\n", this->speedMax);

            }

            GfLogDebug("speed needle Loading \n");
        }
        else
        {
            this->_speedmodel = false;
        }

        param = GfParmGetStr(handle, path, PRM_WATER_NEEDLE_MODEL, NULL);

        if (param)
        {
            this->_watermodel = 1;
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;

            osg::ref_ptr<osg::Node> waterEntity = loader.Load3dFile(strPath, true, "", "");

            if (waterEntity == nullptr)
                GfLogError("Failed to load %s\n", strPath.c_str());
            else
            {
                this->nSteer = 1;
                osg::ref_ptr<osg::Group> waterbranch = new osg::Group;
                osg::ref_ptr<osg::MatrixTransform> waterLoc = new osg::MatrixTransform;

                tdble xpos = GfParmGetNum(handle, path, PRM_WATER_XPOS, NULL, 0.0);
                tdble ypos = GfParmGetNum(handle, path, PRM_WATER_YPOS, NULL, 0.0);
                tdble zpos = GfParmGetNum(handle, path, PRM_WATER_ZPOS, NULL, 0.0);
                float angl = GfParmGetNum(handle, path, PRM_WATER_ANGLE, NULL, 0.0);

                this->waterRot = new osg::MatrixTransform;
                osg::Matrix pos = osg::Matrix::translate(0.0, 0.0, 0.0);
                this->waterRot->setMatrix( pos);
                this->waterRot->setName("WATERBRANCH");
                this->waterRot->addChild(waterEntity.get());

                angl = SD_DEGREES_TO_RADIANS * angl;
                pos = osg::Matrix::translate(xpos, ypos, zpos);
                osg::Matrix rot = osg::Matrix::rotate(angl, osg::Y_AXIS);
                pos = rot * pos;
                waterLoc->setMatrix(pos);
                waterLoc->addChild(waterRot.get());
                waterbranch->addChild( waterLoc );
                instruments_branch->addChild(waterbranch.get());
            }

            GfLogDebug("water needle Loading \n");

        }
        else
        {
            this->_watermodel = false;
        }

        param = GfParmGetStr(handle, path, PRM_FUEL_NEEDLE_MODEL, NULL);

        if (param)
        {
            this->_fuelmodel = 1;
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;

            osg::ref_ptr<osg::Node> fuelEntity = loader.Load3dFile(strPath, true, "", "");

            if (fuelEntity == nullptr)
                GfLogError("Failed to load %s\n", strPath.c_str());
            else
            {
                //this->nSteer = 1;
                osg::ref_ptr<osg::Group> fuelbranch = new osg::Group;
                osg::ref_ptr<osg::MatrixTransform> fuelLoc = new osg::MatrixTransform;

                tdble xpos = GfParmGetNum(handle, path, PRM_FUEL_XPOS, NULL, 0.0);
                tdble ypos = GfParmGetNum(handle, path, PRM_FUEL_YPOS, NULL, 0.0);
                tdble zpos = GfParmGetNum(handle, path, PRM_FUEL_ZPOS, NULL, 0.0);
                float angl = GfParmGetNum(handle, path, PRM_FUEL_ANGLE, NULL, 0.0);

                this->fuelRot = new osg::MatrixTransform;
                osg::Matrix pos = osg::Matrix::translate(0.0, 0.0, 0.0);
                this->fuelRot->setMatrix( pos);
                this->fuelRot->setName("WATERBRANCH");
                this->fuelRot->addChild(fuelEntity.get());

                angl = SD_DEGREES_TO_RADIANS * angl;
                pos = osg::Matrix::translate(xpos, ypos, zpos);
                osg::Matrix rot = osg::Matrix::rotate(angl, osg::Y_AXIS);
                pos = rot * pos;
                fuelLoc->setMatrix(pos);
                fuelLoc->addChild(fuelRot.get());
                fuelbranch->addChild( fuelLoc );
                instruments_branch->addChild(fuelbranch.get());
            }

            GfLogDebug("fuel needle Loading \n");
        }
        else
        {
            this->_fuelmodel = false;
        }

        snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_FUELLIGHT);
        param = GfParmGetStr(handle, path, PRM_FUEL_LIGHT_OFF, NULL);

        if (param)
        {
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;
            osg::ref_ptr<osg::Node> fuelEntity = new osg::Node;
            fuelEntity = loader.Load3dFile(strPath, true, "", "");

            if (fuelEntity)
            {
                this->_fuellightmodel = true;

                this->fuelLightSelector = new osg::Switch;
                this->fuelLightSelector->addChild(fuelEntity.get());
                this->fuelLightSelector->setSingleChildOn(0);

                param = GfParmGetStr(handle, path, PRM_FUEL_LIGHT_ON, NULL);

                if (param)
                {
                    strPath = tmpPath + param;
                    fuelEntity = loader.Load3dFile(strPath, true, "", "");
                    if (fuelEntity)
                    {
                        this->fuelLightSelector->addChild(fuelEntity.get());
                    }
                }

                instruments_branch->addChild(fuelLightSelector);
                GfLogDebug("fuel light switch Loading \n");
            }
        }
        else
        {
            this->_fuellightmodel = false;
        }

        /* add engine light (if one exists) */
        snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_WATERLIGHT);
        param = GfParmGetStr(handle, path, PRM_WATER_LIGHT_OFF, NULL);

        if (param)
        {
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;
            osg::ref_ptr<osg::Node> waterEntity = new osg::Node;
            waterEntity = loader.Load3dFile(strPath, true, "", "");

            if (waterEntity)
            {
                this->_waterlightmodel = true;

                this->engineLightSelector = new osg::Switch;
                this->engineLightSelector->addChild(waterEntity.get());
                this->engineLightSelector->setSingleChildOn(0);

                param = GfParmGetStr(handle, path, PRM_WATER_LIGHT_WARN, NULL);

                if (param)
                {
                    strPath = tmpPath + param;
                    waterEntity = loader.Load3dFile(strPath, true, "", "");
                    if (waterEntity)
                    {
                        this->engineLightSelector->addChild(waterEntity.get());
                    }
                }

                param = GfParmGetStr(handle, path, PRM_WATER_LIGHT_BROKEN, NULL);

                if (param)
                {
                    strPath = tmpPath + param;
                    waterEntity = loader.Load3dFile(strPath, true, "", "");
                    if (waterEntity)
                    {
                        this->engineLightSelector->addChild(waterEntity.get());
                    }
                }

                instruments_branch->addChild(this->engineLightSelector);
                GfLogDebug("water light switch Loading \n");
            }
        }
        else
        {
            this->_waterlightmodel = false;
        }

        snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_BRAKELIGHT);
        param = GfParmGetStr(handle, path, PRM_BRAKE_LIGHT_OFF, NULL);

        if (param)
        {
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;

            strPath = tmpPath + param;
            osg::ref_ptr<osg::Node> brakeEntity = new osg::Node;

            brakeEntity = loader.Load3dFile(strPath, true, "", "");

            if (brakeEntity)
            {
                this->_brakelightmodel = true;

                this->brakeLightSelector = new osg::Switch;
                this->brakeLightSelector->addChild(brakeEntity.get());
                this->brakeLightSelector->setSingleChildOn(0);

                param = GfParmGetStr(handle, path, PRM_BRAKE_LIGHT_WARN, NULL);

                if (param)
                {
                    strPath = tmpPath + param;
                    brakeEntity = loader.Load3dFile(strPath, true, "", "");
                    if (brakeEntity)
                    {
                        this->brakeLightSelector->addChild(brakeEntity.get());
                    }
                }

                param = GfParmGetStr(handle, path, PRM_WATER_LIGHT_BROKEN, NULL);

                if (param)
                {

                    brakeEntity = loader.Load3dFile(strPath, true, "", "");
                    if (brakeEntity)
                    {
                        this->brakeLightSelector->addChild(brakeEntity.get());
                    }
                }

                instruments_branch->addChild(this->brakeLightSelector);
                GfLogDebug("brake light switch Loading \n");
            }
        }
        else
        {
            this->_brakelightmodel = false;
        }

        // separate gears models ...
        snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, LST_GEARS);
        int ngears = GfParmGetEltNb(handle, path) + 1;

        if (ngears > 1)
        {
            this->gearSelector = new osg::Switch;
            this->gearSelector->setName("GEAR");

            // We have at least one separate driver model to add...
            osg::ref_ptr<osg::Node> gearEntity = new osg::Node;

            this->_gearmodel = true;

            // add the gears
            for (int i = 1; i < ngears; i++)
            {
                snprintf(buf, nMaxTexPathSize, "%s/%s/%d", SECT_GROBJECTS, LST_GEARS, i);
                param = GfParmGetStr(handle, buf, PRM_GEAR_MODEL, "");
                std::string tmpPath = GfDataDir();
                snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
                tmpPath = tmpPath+buf;

                strPath = tmpPath + param;
                GfLogDebug("# PAth gear %i = %s\n", i, strPath.c_str());
                gearEntity = loader.Load3dFile(strPath, true, "", "");

                if (gearEntity)
                {
                    gearEntity->setName("GEAR");
                    this->gearSelector->addChild(gearEntity);
                    GfLogDebug("Gear (%i) loading %s\n", i, param);
                }
            }

            this->gearSelector->setSingleChildOn(0);
            instruments_branch->addChild(this->gearSelector.get());

        }
        else
        {
            this->_gearmodel = false;
        }

        /* add ABS light (if one exists) */
        snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_ABS_LIGHT);
        param = GfParmGetStr(handle, path, PRM_ABS_LIGHT_OFF, NULL);
        osg::ref_ptr<osg::Node> absEntity = new osg::Node;

        if (param)
        {
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;
            strPath = tmpPath + param;
            GfLogDebug("#Path = %s\n", strPath.c_str());
            absEntity = loader.Load3dFile(strPath, true, "", "");
            this->absLightSelector = new osg::Switch;

            if (absEntity)
            {
                this->_abslightmodel = true;
                this->absLightSelector->addChild(absEntity.get());
                this->absLightSelector->setSingleChildOn(0);
                GfLogDebug("# Loaded ABS off !\n");

                param = GfParmGetStr(handle, path, PRM_ABS_LIGHT_ON, NULL);

                if (param)
                {
                    strPath = TmpPath+param;
                    absEntity = loader.Load3dFile(strPath, true, "", "");
                    if (absEntity)
                    {
                        this->absLightSelector->addChild(absEntity.get());
                    }
                }

                instruments_branch->addChild(this->absLightSelector.get());
                GfLogDebug("ABS light switch Loading \n");
            }
        }
        else
        {
            this->_abslightmodel = false;
        }

        // ABS Switch
        snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_TCL_LIGHT);
        param = GfParmGetStr(handle, path, PRM_TCL_LIGHT_OFF, NULL);

        osg::ref_ptr<osg::Node> tclEntity = new osg::Node;

        if (param)
        {
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;
            strPath = tmpPath + param;
            GfLogDebug("#Path = %s\n", strPath.c_str());
            absEntity = loader.Load3dFile(strPath, true, "", "");
            this->tclLightSelector = new osg::Switch;

            if (absEntity)
            {
                this->_tcllightmodel = true;
                this->tclLightSelector->addChild(absEntity.get());
                this->tclLightSelector->setSingleChildOn(0);
                GfLogDebug("# Loaded TCL off !\n");

                param = GfParmGetStr(handle, path, PRM_TCL_LIGHT_ON, NULL);

                if (param)
                {
                    strPath = TmpPath+param;
                    tclEntity = loader.Load3dFile(strPath, true, "", "");
                    if (tclEntity)
                    {
                        this->tclLightSelector->addChild(tclEntity.get());
                    }
                }

                instruments_branch->addChild(this->tclLightSelector.get());
                GfLogDebug("TCL light switch Loading \n");
            }
        }
        else
        {
            this->_tcllightmodel = false;
        }
        snprintf(path, sizeof(path), "%s/%s", SECT_GROBJECTS, SECT_SPEED_LIGHT);
        param = GfParmGetStr(handle, path, PRM_SPEED_LIGHT_OFF, NULL);

        osg::ref_ptr<osg::Node> speedEntity = new osg::Node;

        if (param)
        {
            std::string tmpPath = GfDataDir();
            snprintf(buf, nMaxTexPathSize, "cars/models/%s/", car->_carName);
            tmpPath = tmpPath+buf;
            strPath = tmpPath + param;
            GfLogDebug("#Path = %s\n", strPath.c_str());
            speedEntity = loader.Load3dFile(strPath, true, "", "");
            this->speedLightSelector = new osg::Switch;

            if (absEntity)
            {
                this->_speedlightmodel = true;
                this->speedLightSelector->addChild(speedEntity.get());
                this->speedLightSelector->setSingleChildOn(0);
                GfLogDebug("# Loaded SPEED off !\n");

                param = GfParmGetStr(handle, path, PRM_SPEED_LIGHT_ON, NULL);

                if (param)
                {
                    strPath = TmpPath+param;
                    speedEntity = loader.Load3dFile(strPath, true, "", "");
                    if (speedEntity)
                    {
                        this->speedLightSelector->addChild(speedEntity.get());
                    }
                }

                instruments_branch->addChild(this->speedLightSelector.get());
                GfLogDebug("SPEED light switch Loading \n");
            }
        }
        else
        {
            this->_speedlightmodel = false;
        }

        if(instruments_branch)
        {
            osg::StateSet* state = instruments_branch->getOrCreateStateSet();
            state->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
            instruments_branch->setMatrix(carTransform->getMatrix());
            GfLogDebug("#Instruments branch matrix set\n");
        }
    }

    if (_carShader > 1)
        this->reflectionMappingMethod = REFLECTIONMAPPING_DYNAMIC;
    else
        this->reflectionMappingMethod = REFLECTIONMAPPING_OFF;

    this->shader = new SDCarShader(car_shaded_body.get(), this);

    this->reflectionMapping = new SDReflectionMapping(this);
    this->setReflectionMap(this->reflectionMapping->getReflectionMap());

    /* Load visual attributes */
    car->_exhaustNb = GfParmGetEltNb(handle, SECT_EXHAUST);
    car->_exhaustNb = MIN(car->_exhaustNb, 2);
    car->_exhaustPower = GfParmGetNum(handle, SECT_EXHAUST, PRM_POWER, NULL, 1.0);

    /*if (car->_exhaustNb > 0)
    {
        this->backfire = new SDBackFire();
        backfire->init(handle, path, car, transform1.get());
    }*/

    loadCarLights();

    return this->carEntity;
}

bool SDCar::isCar(const tCarElt*c) const
{
    return c == car;
}
SDReflectionMapping *SDCar::getReflectionMap()
{
    return this->reflectionMapping;
}

int SDCar::getReflectionMappingMethod() const
{
    return this->reflectionMappingMethod;
}

/*#define GR_SHADOW_POINTS 6
#define MULT 1.1
osg::ref_ptr<osg::Node> SDCar::initOcclusionQuad(tCarElt *car)
{
    osg::Vec3f vtx;
    osg::Vec2f tex;
    float x;
    int i;

    char buf[512];
    std::string TmpPath = GfDataDir();

    //  GfOut("\n################## LOADING SHADOW ###############################\n");
    std::string shadowTextureName = GfParmGetStr(car->_carHandle, SECT_GROBJECTS, PRM_SHADOW_TEXTURE, "");

    snprintf(buf, sizeof(buf), "cars/models/%s/", car->_carName);
    if (strlen(car->_masterModel) > 0) // Add the master model path if we are using a template.
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "cars/models/%s/", car->_masterModel);

    std::string dir = buf;
    shadowTextureName = TmpPath +dir+shadowTextureName;

    // GfOut("\n lepath = %s\n",shadowTextureName.c_str());
    osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
    osg::ref_ptr<osg::Vec2Array> texcoords = new osg::Vec2Array;
    vtx._v[2] = 0.0;

    for (i = 0, x = car->_dimension_x * MULT / 2.0; i < GR_SHADOW_POINTS / 2;
         i++, x -= car->_dimension_x * MULT / (float)(GR_SHADOW_POINTS - 2) * 2.0)
    {
        vtx._v[0] = x;
        tex._v[0] = 1.0 - (float)i / (float)((GR_SHADOW_POINTS - 2) / 2.0);

        vtx._v[1] = -car->_dimension_y * MULT / 2.0;
        vertices->push_back(vtx);
        tex._v[1] = 0.0;
        texcoords->push_back(tex);

        vtx._v[1] = car->_dimension_y * MULT / 2.0;
        vertices->push_back(vtx);
        tex._v[1] = 1.0;
        texcoords->push_back(tex);
    }

    osg::ref_ptr<osg::Vec3Array> normals = new osg::Vec3Array;
    normals->push_back( osg::Vec3(0.0f,0.0f, 1.0f) );

    osg::ref_ptr<osg::Vec4Array> colors = new osg::Vec4Array;
    colors->push_back( osg::Vec4(1.0f, 1.0f, 1.0f, 1.0f) );

    quad = new osg::Geometry;
    quad->setVertexArray( vertices.get() );
    quad->setNormalArray( normals.get() );
    quad->setNormalBinding( osg::Geometry::BIND_OVERALL );
    quad->setColorArray( colors.get() );
    quad->setColorBinding( osg::Geometry::BIND_OVERALL );
    quad->setTexCoordArray( 0, texcoords.get() );
    quad->addPrimitiveSet( new osg::DrawArrays(GL_TRIANGLE_STRIP, 0, vertices->size()) );

    quad->setDataVariance(osg::Object::DYNAMIC);

    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D;
    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(shadowTextureName);
    texture->setImage( image.get() );

    osg::ref_ptr<osg::BlendFunc> blendFunc = new osg::BlendFunc;
    blendFunc->setFunction( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    osg::ref_ptr<osg::Geode> root = new osg::Geode;
    root->addDrawable( quad.get() );

    osg::StateSet* stateset = root->getOrCreateStateSet();
    stateset->setRenderBinDetails( 2, "DepthSortedBin");
    stateset->setTextureAttributeAndModes(0, texture.get() );
    stateset->setAttributeAndModes( blendFunc );
    stateset->setRenderingHint(osg::StateSet::TRANSPARENT_BIN );

    shadowVertices = vertices;

    //  GfOut("\n################## LOADED SHADOW ###############################\n");
    return root.get();
}*/

void SDCar::markCarCurrent(const tCarElt *Car)
{
    if(this->car == Car)
    {
        this->carTransform->setNodeMask(NODE_MASK_CURCAR);
        this->lights_branch->setNodeMask(NODE_MASK_CURCAR);

        if(gr3DInstruments)
            instruments_branch->setNodeMask(NODE_MASK_CURCAR);

        if(_driver)
            this->DriverSelector->setNodeMask(NODE_MASK_CURDRV);
    }
    else
    {
        this->carTransform->setNodeMask(NODE_MASK_ALL);
        this->lights_branch->setNodeMask(NODE_MASK_ALL);

        if(gr3DInstruments)
            this->instruments_branch->setNodeMask(NODE_MASK_CURDRV);

        if(_driver)
            this->DriverSelector->setNodeMask(NODE_MASK_ALL);
    }
}

void SDCar::updateCar(tSituation *s, tCarElt *CurCar, int current, int driver)
{
    osg::Vec3 p;
    float wingangle = this->car->_wingRCmd * 180 / PI;
    float steerangle = this->car->_steerCmd;

    p[0] = this->car->_pos_X;//+ car->_drvPos_x;
    p[1] = this->car->_pos_Y;//+car->_drvPos_y;
    p[2] = this->car->_pos_Z;//+car->_drvPos_z;

    osg::Matrix mat(car->_posMat[0][0],car->_posMat[0][1],car->_posMat[0][2],car->_posMat[0][3],
            car->_posMat[1][0],car->_posMat[1][1],car->_posMat[1][2],car->_posMat[1][3],
            car->_posMat[2][0],car->_posMat[2][1],car->_posMat[2][2],car->_posMat[2][3],
            car->_posMat[3][0],car->_posMat[3][1],car->_posMat[3][2],car->_posMat[3][3]);

    if ((car == CurCar) && (current != 1))
    {
        if(_driver)
        {
            if (this->nDRM > 0)
                this->DRMSelector->setSingleChildOn(0);
        }
    }

    if (_wing3)
    {
        if ((wingangle > 0.0) && (wingangle < 10.0))
            this->DRMSelector2->setSingleChildOn(0);
        else if ((wingangle > 10.0) && (wingangle < 35.0))
            this->DRMSelector2->setSingleChildOn(1);
        else
            this->DRMSelector2->setSingleChildOn(2);
    }

    if (_driver)
    {
        if (driver || car != CurCar)
        {
            this->DriverSelector->setSingleChildOn(1);

            if (this->nDRM > 14)
            {
                if ((steerangle >= -0.03f) && (steerangle <= 0.03f))
                    this->DRMSelector->setSingleChildOn(0);
                else if ((steerangle > 0.03f) && (steerangle <= 0.07f))
                    this->DRMSelector->setSingleChildOn(1);
                else if ((steerangle > 0.07f) && (steerangle <= 0.13f))
                    this->DRMSelector->setSingleChildOn(2);
                else if ((steerangle > 0.13f) && (steerangle <= 0.21f))
                    this->DRMSelector->setSingleChildOn(3);
                else if ((steerangle > 0.21f) && (steerangle <= 0.30f))
                    this->DRMSelector->setSingleChildOn(4);
                else if ((steerangle > 0.30f) && (steerangle <= 0.45f))
                    this->DRMSelector->setSingleChildOn(5);
                else if ((steerangle > 0.45f) && (steerangle <= 0.70f))
                    this->DRMSelector->setSingleChildOn(6);
                else if (steerangle > 0.70f)
                    this->DRMSelector->setSingleChildOn(7);
                else if ((steerangle < -0.03f) && (steerangle >= -0.07f))
                    this->DRMSelector->setSingleChildOn(8);
                else if ((steerangle < -0.07f) && (steerangle >= -0.13f))
                    this->DRMSelector->setSingleChildOn(9);
                else if ((steerangle < -0.13f) && (steerangle >= -0.21f))
                    this->DRMSelector->setSingleChildOn(10);
                else if ((steerangle < -0.21f) && (steerangle >= -0.30f))
                    this->DRMSelector->setSingleChildOn(11);
                else if ((steerangle < -0.30f) && (steerangle >= -0.45f))
                    this->DRMSelector->setSingleChildOn(12);
                else if ((steerangle < -0.45f) && (steerangle >= -0.70f))
                    this->DRMSelector->setSingleChildOn(13);
                else if (steerangle < -0.70f)
                    this->DRMSelector->setSingleChildOn(14);
            }
            else
            {
                if ((steerangle >= -0.03f) && (steerangle <= 0.03f))
                    this->DRMSelector->setSingleChildOn(0);
                else if ((steerangle > 0.03f) && (steerangle <= 0.07f))
                    this->DRMSelector->setSingleChildOn(1);
                else if ((steerangle > 0.07f) && (steerangle <= 0.13f))
                    this->DRMSelector->setSingleChildOn(2);
                else if ((steerangle > 0.13f) && (steerangle <= 0.21f))
                    this->DRMSelector->setSingleChildOn(3);
                else if ((steerangle > 0.21f) && (steerangle <= 0.30f))
                    this->DRMSelector->setSingleChildOn(4);
                else if ((steerangle > 0.30f) && (steerangle <= 0.45f))
                    this->DRMSelector->setSingleChildOn(5);
                else if ((steerangle > 0.45f))
                    this->DRMSelector->setSingleChildOn(6);
                else if ((steerangle < -0.03f) && (steerangle >= -0.07f))
                    this->DRMSelector->setSingleChildOn(7);
                else if ((steerangle < -0.07f) && (steerangle >= -0.13f))
                    this->DRMSelector->setSingleChildOn(8);
                else if ((steerangle < -0.13f) && (steerangle >= -0.21f))
                    this->DRMSelector->setSingleChildOn(9);
                else if ((steerangle < -0.21f) && (steerangle >= -0.30f))
                    this->DRMSelector->setSingleChildOn(10);
                else if ((steerangle < -0.30f) && (steerangle >= -0.45f))
                    this->DRMSelector->setSingleChildOn(11);
                else if ((steerangle < -0.45f))
                    this->DRMSelector->setSingleChildOn(12);
            }
        }
    }

    if(_steer && SteerSelector)
    {
        if (driver || car != CurCar)
        {
            this->SteerSelector->setSingleChildOn(0);
            steerangle = (-steerangle * steerWheelRot);
            osg::Matrix rotation = osg::Matrix::rotate(steerangle, osg::X_AXIS);
            this->SteerRot->setMatrix(rotation);
        }
        else
        {
            //if (nSteer > 1)
            {
                this->SteerSelector->setSingleChildOn(1);
                steerangle = (-steerangle * steerWheelRot);
                osg::Matrix rotation = osg::Matrix::rotate(steerangle, osg::X_AXIS);
                this->SteerRot2->setMatrix(rotation);
                GfLogDebug(" # update steer branch\n");
            }
        }
    }

    if(gr3DInstruments && !driver)
    {
        if(this->_gearmodel)
        {
            if(car->_gear < 0)
                this->gearSelector->setSingleChildOn(0);
            else
                this->gearSelector->setSingleChildOn(car->_gear + 1);
        }

        if(this->_fuellightmodel)
        {
            if (car->_fuel > 5)
                this->fuelLightSelector->setSingleChildOn(0);
            else
                this->fuelLightSelector->setSingleChildOn(1);
        }

        if(this->_waterlightmodel)
        {
            if ((car->priv.engineTempWater) < car->priv.engineMaxTempWater)
                this->engineLightSelector->setSingleChildOn(0);
            else if ((car->priv.engineTempWater) >= car->priv.engineMaxTempWater)
                this->engineLightSelector->setSingleChildOn(1);
            else if((car->priv.engineTempWater) > (car->priv.engineMaxTempWater) + 5.0)
                this->engineLightSelector->setSingleChildOn(2);
        }

        if(this->_brakelightmodel)
        {
            bool disk = (car->_brakeDiskWidth(0) < 19.5f) || (car->_brakeDiskWidth(1) < 19.5f) ||
                    (car->_brakeDiskWidth(2) < 19.5f) || (car->_brakeDiskWidth(3) < 19.5f) ||
                    (car->_brakeDiskTemp(0) > car->_brakeDiskMaxTemp(0)) ||
                    (car->_brakeDiskTemp(1) > car->_brakeDiskMaxTemp(1)) ||
                    (car->_brakeDiskTemp(2) > car->_brakeDiskMaxTemp(2)) ||
                    (car->_brakeDiskTemp(3) > car->_brakeDiskMaxTemp(3));

            if (disk == true)
            {
                if ((car->_brakeDiskWidth(0) < 19.0f) || (car->_brakeDiskWidth(1) < 19.0f) ||
                        (car->_brakeDiskWidth(2) < 19.0f) || (car->_brakeDiskWidth(3) < 19.0f))
                    this->engineLightSelector->setSingleChildOn(2);
                else
                    this->engineLightSelector->setSingleChildOn(1);
            }
            else
                this->engineLightSelector->setSingleChildOn(0);
        }

        if(this->_speedmodel)
        {
            tdble val = (car->_speed_x) / this->speedMax;
            val = MIN(1.0, MAX(0.0, val));    // val between 0.0 and 1.0
            val = -40.0 + (val * 4.50);
            osg::Matrix rotation = osg::Matrix::rotate(val, osg::X_AXIS);
            this->speedRot->setMatrix(rotation);
        }

        if(this->_rpmmodel)
        {
            tdble val = (car->_enginerpm) / this->rpmMax;
            val = MIN(1.0, MAX(0.0, val));    // val between 0.0 and 1.0
            val = 230.0 + val * 46.0;
            osg::Matrix rotation = osg::Matrix::rotate(val, osg::X_AXIS);
            this->rpmRot->setMatrix(rotation);
        }

        if(this->_fuelmodel)
        {
            GfLogDebug("# Update needle fuel ...\n");
            tdble val = ((car->priv.fuel) / car->info.tank);
            val = MIN(1.0, MAX(0.0, val));    // val between 0.0 and 1.0
            val = 320 - (val * (45 - 130));
            osg::Matrix rotation = osg::Matrix::rotate(DEG2RAD(val), osg::X_AXIS);
            this->fuelRot->setMatrix(rotation);
        }

        if(this->_watermodel)
        {
            GfLogDebug("# Update watermodel \n");
            tdble val = (car->priv.engineTempWater) / car->priv.engineMaxTempWater;
            val = MIN(1.0, MAX(0.0, val));    // val between 0.0 and 1.0
            val = 175 - (val * (40 - 188));
            osg::Matrix rotation = osg::Matrix::rotate(DEG2RAD(val), osg::X_AXIS);
            this->waterRot->setMatrix(rotation);
        }

        if (this->_abslightmodel || this->_tcllightmodel || this->_speedlightmodel)
        {
            GfLogDebug(" # update ABS Light Model\n");
            bool abs = false;   // Show ABS indicator?
            bool tcl = false;   // Show TCS indicator?
            bool spd = false;   // Show speed limiter indicator?

            for (int i = 0; i < 4; i++)
            {
                if(this->_abslightmodel)
                {
                    abs = abs || strstr(car->ctrl.msg[i], "ABS");

                    if (!abs)
                        this->absLightSelector->setSingleChildOn(0);
                    else
                        this->absLightSelector->setSingleChildOn(1);
                }

                if (this->_tcllightmodel)
                {
                    GfLogDebug(" # update TCL Light Model\n");
                    tcl = tcl || strstr(car->ctrl.msg[i], "TCS");

                    if (!tcl)
                        this->tclLightSelector->setSingleChildOn(0);
                    else
                        this->tclLightSelector->setSingleChildOn(1);
                }

                if (this->_speedlightmodel)
                {
                    GfLogDebug(" # update SPEED Light Model\n");
                    spd = spd || strstr(car->ctrl.msg[i], "Speed Limiter On");

                    if (!spd)
                        this->speedLightSelector->setSingleChildOn(0);
                    else
                        this->speedLightSelector->setSingleChildOn(1);
                }

                if(_rpmmodel)
                {
                    tdble val = (car->_enginerpm) / this->rpmMax;
                    val = MIN(1.0, MAX(0.0, val));    // val between 0.0 and 1.0
                    val = 230.0 + val * 46.0;
                    osg::Matrix rotation = osg::Matrix::rotate(val, osg::X_AXIS);
                    this->rpmRot->setMatrix(rotation);
                }
            }
        }
    }

    for(std::vector<SDCarLight>::iterator i = lights.begin(); i != lights.end(); ++i)
        i->update(*this);

    wheels.updateWheels();

    this->carTransform->setMatrix(mat);
    this->lights_branch->setMatrix(mat);

    if (gr3DInstruments)
    {
        this->instruments_branch->setMatrix(mat);
        GfLogDebug("# Car Update instruments_branch matrix\n");
    }

    if(_carShader > 3)
    {
        this->reflectionMapping->update();
        this->setReflectionMap(this->reflectionMapping->getReflectionMap());
    }
    else if(_carShader > 2 && car == CurCar)
    {
        this->reflectionMapping->update();
        this->setReflectionMap(this->reflectionMapping->getReflectionMap());
    }


    //ugly computation,
    /*if (SHADOW_TECHNIQUE == 0)
    {
        osg::ref_ptr<osg::Vec3Array> vertices = new osg::Vec3Array;
        for(unsigned int i=0;i<shadowVertices->size();i++)
        {
            osg::Vec3 vtx = (*shadowVertices.get())[i];
            osg::Vec4 vtx_world = osg::Vec4(vtx,1.0f)*mat;
            vtx_world._v[2] = RtTrackHeightG(car->_trkPos.seg, vtx_world.x(), vtx_world.y()); //0.01 needed, we have to sort out why
            vertices->push_back(osg::Vec3(vtx_world.x(), vtx_world.y(), vtx_world.z()));
        }
        quad->setVertexArray(vertices);
    }*/
}

void SDCar::updateShadingParameters(const osg::Matrixf &modelview)
{
    if (shader) shader->update(modelview);
}

void SDCar::setReflectionMap(osg::ref_ptr<osg::Texture> map)
{
    this->car_shaded_body->getOrCreateStateSet()->setTextureAttributeAndModes(2, map,
                                                                              osg::StateAttribute::ON | osg::StateAttribute::OVERRIDE);
}

SDCars::SDCars(void) :
    cars_branch(nullptr),
    shadow_branch(nullptr),
    instruments_group(nullptr),
    situation(nullptr)
{
}

SDCars::~SDCars(void)
{
    for(unsigned i=0; i<the_cars.size(); i++)
    {
        delete the_cars[i];
    }
}

void SDCars::addSDCar(SDCar *car)
{
    the_cars.insert(the_cars.end(), car);
}

void SDCars::loadCars(tSituation *pSituation, bool trackType, bool subCat)
{
    this->cars_branch = new osg::Group;

    if (gr3DInstruments)
        this->instruments_group = new osg::Group;

    int carShader = getRender()->getShader();
    tSituation *s = pSituation;
    this->situation = pSituation;

    for (int i = 0; i < s->_ncars; i++)
    {
        tCarElt* elt = s->cars[i];
        strncpy(elt->_masterModel, GfParmGetStr(elt->_carHandle, SECT_GROBJECTS, PRM_TEMPLATE, ""), MAX_NAME_LEN - 1);
        elt->_masterModel[MAX_NAME_LEN - 1] = 0;
        SDCar * car = new SDCar;
        this->addSDCar(car);
        this->cars_branch->addChild(car->loadCar(elt, trackType, subCat, carShader));

        if(gr3DInstruments)
        {
            GfLogDebug("# SDCars added instruments\n");
            this->instruments_group->addChild(car->getInstruments());
            GfLogDebug("# Passed !!!\n");
        }
    }

    return;
}

SDCar *SDCars::getCar(tCarElt *car)
{
    std::vector<SDCar *>::iterator it;
    SDCar *res = new SDCar;

    for(it = the_cars.begin(); it!= the_cars.end(); ++it)
    {
        if((*it)->isCar(car))
        {
            res = *it;
        }
    }

    return res;
}

void SDCars::updateCars(tSituation *s, tCarElt *CurCar, int current, int driver)
{
    std::vector<SDCar *>::iterator it;

    for(it = the_cars.begin(); it!= the_cars.end(); ++it)
    {
        (*it)->updateCar(s, CurCar, current, driver);
    }
}

void SDCars::markCarCurrent(const tCarElt *car)
{
    std::vector<SDCar *>::iterator it;

    for(it = the_cars.begin(); it!= the_cars.end(); ++it)
    {
        (*it)->markCarCurrent(car);
    }
}

void SDCars::updateShadingParameters(const osg::Matrixf &modelview)
{
    std::vector<SDCar *>::iterator it;

    for(it = the_cars.begin(); it!= the_cars.end(); ++it)
    {
        (*it)->updateShadingParameters(modelview);
    }
}

void SDCars::unLoad()
{
    this->cars_branch->removeChildren(0, cars_branch->getNumChildren());
    this->cars_branch = NULL;

    if (instruments_group)
    {
        this->instruments_group->removeChildren(0, instruments_group->getNumChildren());
        this->instruments_group = NULL;
    }
}

} // namespace osggraph
