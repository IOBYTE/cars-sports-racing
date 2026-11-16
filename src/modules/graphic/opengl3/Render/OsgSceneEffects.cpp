/***************************************************************************

    file        : OsgSceneEffects.cpp
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

#include "OsgRender.h"

#include <osg/Program>
#include <osg/Node>
#include <osg/Uniform>
#include <osgDB/ReadFile>
#include <osgDB/FileUtils>

#include <tgf.h>

#include "OsgMain.h"
#include "OsgSceneEffects.h"

SDSceneEffects::SDSceneEffects(osg::Node *nod)
{
    std::string TmpPath = GfDataDir();

    program = new osg::Program;
    program->addShader(osgDB::readShaderFile(osg::Shader::VERTEX,
        osgDB::findDataFile(TmpPath + "data/shaders/car.vert")));
    program->addShader(osgDB::readShaderFile(osg::Shader::FRAGMENT,
        osgDB::findDataFile(TmpPath + "data/shaders/car.frag")));

    stateset = nod->getOrCreateStateSet();
    stateset->setAttributeAndModes(program);

    diffuseMap = new osg::Uniform("diffusemap", 0 );
    stateset->addUniform(diffuseMap);

    specularColor = new osg::Uniform("specularColor", osg::Vec4(0.8f, 0.8f, 0.8f, 1.0f));
    stateset->addUniform(specularColor);

    lightVector = stateset->getOrCreateUniform("lightvector",osg::Uniform::FLOAT_VEC3);
    lightPower = stateset->getOrCreateUniform("lightpower",osg::Uniform::FLOAT_VEC4);
    ambientColor = stateset->getOrCreateUniform("ambientColor",osg::Uniform::FLOAT_VEC4);
    shininess = new osg::Uniform("smoothness", 300.0f);
    stateset->addUniform(shininess);
}

void SDSceneEffects::update(const osg::Matrixf &view)
{
}
