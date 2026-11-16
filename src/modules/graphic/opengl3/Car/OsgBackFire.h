/***************************************************************************

    file                 : OsgBackFire.h
    created              : Tue Feb 01 15:32:14 CEST 2020
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

#ifndef _OSGBACKFIRE_H_
#define _OSGBACKFIRE_H_

#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/ModularEmitter>
#include <osgParticle/ModularProgram>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>
#include <osgParticle/RadialShooter>
#include <osgParticle/AccelOperator>
#include <osgParticle/FluidFrictionOperator>
#include <osgParticle/SmokeEffect>
#include <osgParticle/SmokeTrailEffect>
#include <osgParticle/FireEffect>

#include "OsgCar.h"

class SDCar;

class SDBackFire
{
private:
    osg::ref_ptr<osgParticle::ParticleSystem> _backFire;
    int Nb;

public:
    SDBackFire() {};
    ~SDBackFire() {};

    void init(void *handle, char *path, tCarElt *car, osg::MatrixTransform *Entity);
    void update(tCarElt *car);
    bool _enabled;
};

#endif
