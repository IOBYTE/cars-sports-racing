/***************************************************************************

    file        : OsgSceneEffects.h
    created     : Fri Mar 2022 15:52:19 CEST 2022
    copyright   : (C) 2022 Xavier Bertaux
    email       : bertauxx@gmail.com
    version     : $Id$
 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _OSGSCENEEFFECTS_H_
#define _OSGSCENEEFFECTS_H_

class SDSceneEffects
{
private :
    osg::ref_ptr<osg::Program> program ;
    osg::ref_ptr<osg::Node> pCar;

    osg::ref_ptr<osg::StateSet> stateset;

    osg::ref_ptr<osg::Uniform> diffuseMap;
    osg::ref_ptr<osg::Uniform> normalMap;
    //osg::ref_ptr<osg::Uniform> shadowMap;
    osg::ref_ptr<osg::Uniform> specularColor;
    osg::ref_ptr<osg::Uniform> lightVector;
    osg::ref_ptr<osg::Uniform> lightPower;
    osg::ref_ptr<osg::Uniform> ambientColor;
    osg::ref_ptr<osg::Uniform> shininess;

    osg::ref_ptr<osg::Uniform> reflectionMappingMethod;
    osg::ref_ptr<osg::Uniform> reflectionMapCube;
    osg::ref_ptr<osg::Uniform> reflectionMap2DSampler;
    osg::ref_ptr<osg::Uniform> reflectionMapStaticOffsetCoords;

public :
    SDSceneEffects(osg::Node *nod);
    void update(const osg::Matrixf &view);
};

#endif //_OSGSCENEEFFECTS_H_
