/***************************************************************************

    file        : OsgParticles.h
    created     : Sat May 2024 15:52:19 CEST 2024
    copyright   : (C) 2024 by MADBAD
    email       : madbad82@gmail.com
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _OSGPARTICLES_H_
#define _OSGPARTICLES_H_

#include <osgParticle/ModularEmitter>
#include <osgParticle/Particle>
#include <osgParticle/ParticleSystem>
#include <osgParticle/ParticleSystemUpdater>
#include <osgParticle/RadialShooter>
#include <osgParticle/RandomRateCounter>
#include <osgParticle/SectorPlacer>

namespace osggraph
{
class csrParticleSystem
{
public:
    csrParticleSystem(osg::Group *MySceneRoot, osg::Group *myAttachToObj);
    ~csrParticleSystem(void);

    osgParticle::Particle ptemplate;
    osgParticle::ParticleSystem *particleSystem = new osgParticle::ParticleSystem;
    osgParticle::ModularEmitter *emitter = new osgParticle::ModularEmitter;
    osgParticle::SectorPlacer *placer = new osgParticle::SectorPlacer;
    osgParticle::RadialShooter *shooter = new osgParticle::RadialShooter;
    osgParticle::RandomRateCounter *randomRateCounter = static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());
    osg::ref_ptr<osg::Geode> geode = new osg::Geode;
    osg::Group *sceneRoot;
    osg::Group *attachToObj;
    bool isActive=false;

    void initialize();
    void startEmitting();
    void stopEmitting();
    void setEmissionType(std::string groundMaterialType);
};

class csrParticleSystemManager
{
public:
    csrParticleSystemManager(void);
    virtual ~csrParticleSystemManager(void);
    std::vector <csrParticleSystem*> tiresSmokeParticleSystems;
    std::vector <csrParticleSystem*> sparksParticleSystems;

    void initialize(tSituation *s);
    void update(tSituation *s);
    void shutdown();
};

class csrBackFireSystem
{
public:
    csrBackFireSystem(osg::Group *MySceneRoot, osg::Group *myAttachToObj);
    ~csrBackFireSystem(void);

    void initialize();
    void startEmitting();
    void stopEmitting();
    void setEmissionType(std::string groundMaterialType);

    osgParticle::Particle           ptemplate;
    osgParticle::ParticleSystem     *backfireSystem = new osgParticle::ParticleSystem;
    osgParticle::ModularEmitter     *emitter = new osgParticle::ModularEmitter;
    osgParticle::SectorPlacer       *placer = new osgParticle::SectorPlacer;
    osgParticle::RadialShooter      *shooter = new osgParticle::RadialShooter;
    osgParticle::RandomRateCounter  *randomRateCounter = static_cast<osgParticle::RandomRateCounter *>(emitter->getCounter());
    osg::ref_ptr<osg::Geode>        geode = new osg::Geode;

    osg::Group *sceneRoot;
    osg::Group *attachToObj;
    bool isActive = false;
};

class csrBackFireSystemManager
{
public:
    csrBackFireSystemManager(void);
    virtual ~csrBackFireSystemManager(void);
    std::vector <csrBackFireSystem*> backfireParticleSystems;

    void initialize(tSituation *s);
    void update(tSituation *s);
    void shutdown();
};
}

#endif
