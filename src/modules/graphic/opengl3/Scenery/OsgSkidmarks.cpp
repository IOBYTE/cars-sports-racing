/***************************************************************************

    file                 : OsgSkidmarks.cpp
    created              : Mon Jun 01 20:13:56 CEST 2022
    copyright            : (C) 2022 Xavier Bertaux
    email                : bertauxx@yahoo.fr
    version              : $Id: OsgSkidmarks.cpp 2436 2022-06-01 23:22:43Z torcs-ng $

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
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>
#include <osgDB/Registry>

#include "OsgScenery.h"
#include "OsgNodeMask.h"

SDSkidmarks::SDSkidmarks(void) :
    _skidmarks(NULL)
{
}

SDSkidmarks::~SDSkidmarks(void)
{
    if(_skidmarks != NULL)
    {
        _skidmarks->removeChildren(0, _skidmarks->getNumChildren());
        _skidmarks = NULL;
    }
}

void SDSkidmarks::build(void)
{
    _skidmarks = new osg::Group;
    _skidmarks->setName("SKIDMARKS");
    _skidmarks->setCullingActive(true);
    _skidmarks->setNodeMask(NODE_MASK_SHADOW_RECV);
    osg::StateSet* stateSet = _skidmarks->getOrCreateStateSet();
    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::ON | osg::StateAttribute::PROTECTED);
    stateSet->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
    stateSet->setMode(GL_BLEND, osg::StateAttribute::ON);
    stateSet->setMode(GL_ALPHA_TEST, osg::StateAttribute::ON);
}
