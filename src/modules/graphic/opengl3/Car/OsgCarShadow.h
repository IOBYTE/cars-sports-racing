/***************************************************************************

    file                 : OsgCarLight.h
    created              : Tue Mar 31 15:32:14 CEST 2020
    .........            : ...2020 Ivan Mahonin
    email                : bh@icystar.com
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

#ifndef _OSGCARSHADOW_H_
#define _OSGCARSHADOW_H_

#include <osg/Group>
#include <osg/StateSet>

#include <car.h>

class SDCar;

class SDCarShadow
{
private:
    osg::ref_ptr<osg::MatrixTransform> carShadow;
    osg::ref_ptr<osg::Geometry> shadowAnchor;
    osg::ref_ptr<osg::Vec3Array> shadow_vtx;

public:
    SDCarShadow() {}

    osg::ref_ptr<osg::MatrixTransform> init(tCarElt *car);
    void update(tCarElt *car, int visible);
};

#endif
