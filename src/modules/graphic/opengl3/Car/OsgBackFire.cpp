/***************************************************************************

    file                 : OsgBackFire.cpp
    created              : Tue Feb 01 15:34:18 CEST 2022
    .........            : 2022 Xavier BERTAUX
    email                : bertauxx@gmail.com
    version              : $Id$

***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <string>
#include <tgf.h>

#include "OsgBackFire.h"
#include "OsgCar.h"

void SDBackFire::init(void *handle, char *path, tCarElt *car, osg::MatrixTransform *carTransform)
{
    std::string tPath = GetDataDir();
    for (int i = 0; i < car->_exhaustNb; i++)
    {
        snprintf(path, 1024, "%s/%d", SECT_EXHAUST, i + 1);
        car->_exhaustPos[i].x = GfParmGetNum(handle, path, PRM_XPOS, NULL, -car->_dimension_x / 2.0);
        car->_exhaustPos[i].y = -GfParmGetNum(handle, path, PRM_YPOS, NULL, car->_dimension_y / 2.0);
        car->_exhaustPos[i].z = GfParmGetNum(handle, path, PRM_ZPOS, NULL, 0.1);

        osg::Vec3 position = osg::Vec3(car->_exhaustPos[i].x, car->_exhaustPos[i].z, car->_exhaustPos[i].y);
        osgParticle::SmokeTrailEffect* smoke = new osgParticle::SmokeTrailEffect(position, 10.0f);
        osgParticle::FireEffect* fire = new osgParticle::FireEffect(position, 10.0f);
        osgParticle::ParticleSystem *ps = new osgParticle::ParticleSystem;
        osgParticle::SectorPlacer *placer = new osgParticle::SectorPlacer;
        placer->setCenter(car->_exhaustPos[i].x, car->_exhaustPos[i].z, car->_exhaustPos[i].y);
        placer->setRadiusRange(2.5, 5);
        placer->setPhiRange(0, 2 * osg::PI);    // 360 angle to make a circle

        char buf[1024];
        snprintf(buf, 1024, "data/textures/smoke.png");
        tPath += buf;

        ps->setDefaultAttributes("", true, false);
        osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
        emitter->setPlacer(placer);
        emitter->setLifeTime(2000);
        emitter->setParticleSystem(ps);
        osgParticle::RandomRateCounter *rrc =
                static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());
        rrc->setRateRange(20, 30);
        carTransform->addChild(emitter);
        carTransform->addChild(ps);
        carTransform->addChild(smoke);
        carTransform->addChild(fire);
        smoke->setUseLocalParticleSystem(false);
        fire->setUseLocalParticleSystem(false);
    }
}

void SDBackFire::update(tCarElt *car)
{

}
