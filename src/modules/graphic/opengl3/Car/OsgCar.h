/***************************************************************************

    file                 : OsgCar.h
    created              : Mon Aug 21 18:24:02 CEST 2012
    copyright            : (C)2012 by Gaétan André, (C)2014 Xavier Bertaux
    email                : gaetan.andré@gmail.com

***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _OSGCAR_H_
#define _OSGCAR_H_

#include <osg/TextureCubeMap>
#include <osg/LOD>

#include <car.h>
#include <raceman.h>
#include <vector>

#include "OsgWheel.h"
//#include "OsgBackFire.h"
#include "OsgCarLight.h"

namespace osggraph {

class SDCarShader;
class SDReflectionMapping;
class SDWheels;
//class SDBackFire;
class SDCarLight;

class InstrumentsCar
{
public:
    osg::ref_ptr<osg::Texture> TachoSpeed;
    osg::ref_ptr<osg::Texture> TachoRpm;
    tdble                      MinTachoSpeed;
    tdble                      MaxTachoSpeed;
    tdble                      MinTachoRpm;
    tdble                      MaxTachoRpm;
    tdble                      prevVal;
    tdble                      rawPrev;
};

class SDCar
{
private :
    osg::ref_ptr<osg::MatrixTransform> carTransform;
    osg::ref_ptr<osg::MatrixTransform> lights_branch;
    osg::ref_ptr<osg::MatrixTransform> instruments_branch;
    osg::ref_ptr<osg::MatrixTransform> SteerRot;
    osg::ref_ptr<osg::MatrixTransform> SteerRot2;
    osg::ref_ptr<osg::MatrixTransform> rpm;
    osg::ref_ptr<osg::MatrixTransform> speed;
    osg::ref_ptr<osg::MatrixTransform> fuel;
    osg::ref_ptr<osg::MatrixTransform> water;
    osg::ref_ptr<osg::MatrixTransform> rpmRot;
    osg::ref_ptr<osg::MatrixTransform> speedRot;
    osg::ref_ptr<osg::MatrixTransform> fuelRot;
    osg::ref_ptr<osg::MatrixTransform> waterRot;
    osg::ref_ptr<osg::Switch>          LODSelector;
    osg::ref_ptr<osg::Switch>          DRMSelector;
    osg::ref_ptr<osg::Switch>          DRMSelector2;
    osg::ref_ptr<osg::Switch>          DriverSelector;
    osg::ref_ptr<osg::Switch>          SteerSelector;
    osg::ref_ptr<osg::Switch>          RearWingSelector;
    osg::ref_ptr<osg::Switch>          gearSelector;
    osg::ref_ptr<osg::Switch>          brakeLightSelector;
    osg::ref_ptr<osg::Switch>          engineLightSelector;
    osg::ref_ptr<osg::Switch>          fuelLightSelector;
    osg::ref_ptr<osg::Switch>          absLightSelector;
    osg::ref_ptr<osg::Switch>          tclLightSelector;
    osg::ref_ptr<osg::Switch>          speedLightSelector;
    osg::ref_ptr<osg::Group>           car_shaded_body;
    osg::ref_ptr<osg::Group>           carEntity;

    bool                                _speedmodel;
    bool    							_rpmmodel;
    bool    							_fuelmodel;
    bool    							_watermodel;
    bool    							_gearmodel;
    bool    							_absmodel;
    bool    							_tclmodel;
    bool    							_brakelightmodel;
    bool    							_fuellightmodel;
    bool    							_waterlightmodel;
    bool    							_abslightmodel;
    bool    							_tcllightmodel;
    bool    							_speedlightmodel;
    bool    							_rpmlightsSelectorinsg;
    bool    							_gearSelectorinsg;
    int                                nSteer;
    int                                DRMSelectMask[32];
    int                                nDRM;
    int                                nDRM2;
    float                              DRMThreshold[32];
    tdble                              steerWheelRot;
    tdble                              rpmMin;
    tdble                              rpmMax;
    tdble                              speedMin;
    tdble                              speedMax;

    //osg::ref_ptr<osg::Vec3Array> shadowVertices;
    //osg::ref_ptr<osg::Geometry> quad;
    //osg::ref_ptr<osg::Node> initOcclusionQuad(tCarElt *car);
    //InstrumentsCar          InstCar;

    std::vector<SDCarLight> lights;

    tCarElt     *car;

    //SDWheels    wheels;
    SDCarShader *shader;
    SDReflectionMapping *reflectionMapping;
    //SDBackFire *backfire;

    int reflectionMappingMethod;

    void setReflectionMap(osg::ref_ptr<osg::Texture> map);
    void loadCarLights();

public :
    SDCar(void);
    ~SDCar(void);

    osg::ref_ptr<osg::Node> loadCar(tCarElt *Car, bool tracktype, bool subcat, int carshader);
    bool isCar(const tCarElt*c) const;

    bool    _cockpit;
    bool    _driver;
    bool    _instruments;
    bool    _wing1;
    bool    _wing3;
    bool    _steer;
    bool    _carLOD;

    int     _carShader;
    InstrumentsCar InstCar;

    SDReflectionMapping *getReflectionMap();

    int getReflectionMappingMethod() const;
    osg::ref_ptr<osg::Group> getCarOsgGroup() { return this->carTransform.get(); }
    osg::ref_ptr<osg::Group> getInstruments() { return this->instruments_branch.get(); }

    tCarElt *getCar() { return car; }
    const tCarElt *getCar() const { return car; }

    void markCarCurrent(const tCarElt *Car);
    void updateCar(tSituation *s, tCarElt *CurCar, int current, int driver);
    void updateShadingParameters(const osg::Matrixf &modelview);

    SDWheels    wheels;
};

class SDCars
{
private :
    std::vector<SDCar *> the_cars;
    osg::ref_ptr<osg::Group> cars_branch;
    osg::ref_ptr<osg::Group> shadow_branch;
    osg::ref_ptr<osg::Group> instruments_group;
    tSituation *situation;

    void addSDCar(SDCar * car);

public :
    SDCars(void);
    ~SDCars(void);

    void  loadCars(tSituation * pSituation, bool trackType, bool subCat);
    void  updateCars(tSituation *s, tCarElt *CurCar, int current, int driver);
    void  markCarCurrent(const tCarElt*car);
    SDCar *getCar(tCarElt*car);
    void  unLoad();
    void  updateShadingParameters(const osg::Matrixf &modelview);
    osg::ref_ptr<osg::Node> getShadowNode() { return shadow_branch.get(); }
    osg::ref_ptr<osg::Node> getCarsNode() { return cars_branch.get(); }
    osg::ref_ptr<osg::Group> getInstruments() { return instruments_group.get(); }
};

} // namespace osggraph

#endif /* _OSGCAR_H_ */
