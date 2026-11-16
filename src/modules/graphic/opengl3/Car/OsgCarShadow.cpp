/***************************************************************************

    file                 : OsgCarLight.cpp
    created              : Tue Mar 31 15:34:18 CEST 2020
    .........            : 2020 Ivan Mahonin
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

#include <sstream>

#include <osg/PolygonOffset>
#include <osg/TexEnv>
#include <osg/Depth>
#include <osg/MatrixTransform>
#include <osg/Geometry>
#include <osg/Geode>
#include <osg/Texture2D>
#include <osgDB/ReadFile>

#include "OsgCarShadow.h"
//#include "OsgCar.h"

#include <robottools.h>

#define GR_SHADOW_POINTS	6

osg::ref_ptr<osg::MatrixTransform> SDCarShadow::init(tCarElt *car)
{
    char		buf[1024];
    const char	*shdTexName;
    osg::StateSet *stateSet = new osg::StateSet;
    stateSet->setMode(GL_CULL_FACE, osg::StateAttribute::ON);
    stateSet->setMode(GL_LIGHTING, osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    osg::Vec3	vtx;
    osg::Vec4	clr;
    osg::Vec3	nrm;
    osg::Vec2	tex;

    shadow_vtx = new osg::Vec3Array(GR_SHADOW_POINTS+1);
    osg::Vec4Array *shadow_clr = new osg::Vec4Array(1);
    osg::Vec3Array *shadow_nrm = new osg::Vec3Array(1);
    osg::Vec2Array *shadow_tex = new osg::Vec2Array(GR_SHADOW_POINTS+1);

    snprintf(buf, sizeof(buf), "cars/models/%s;", car->_carName);
    if (strlen(car->_masterModel) > 0)
        snprintf(buf + strlen(buf), sizeof(buf) - strlen(buf), "cars/models/%s;", car->_masterModel);

    std::string path = buf;

    shdTexName = GfParmGetStr(car->_carHandle, SECT_GROBJECTS, PRM_SHADOW_TEXTURE, "");
    shadowAnchor = new osg::Geometry;
    osg::Geode *sh = new osg::Geode;
    //grCarInfo[car->index].shadowAnchor = new ssgBranch();
    path = path + shdTexName;

    clr[0] = clr[1] = clr[2] = 1.0;
    clr[3] = 1.0;
    shadow_clr->push_back(clr);
    nrm[0] = nrm[1] = 0.0;
    nrm[2] = 1.0;
    shadow_nrm->push_back(nrm);

    /* vertices */
#define MULT	1.1
    vtx[2] = 0.0;
    for (int i = 0, x = car->_dimension_x * MULT / 2.0; i < GR_SHADOW_POINTS / 2;
         i++, x -= car->_dimension_x * MULT / (float)(GR_SHADOW_POINTS - 2) * 2.0)
    {

        vtx[0] = x;
        tex[0] = 1.0 - (float)i / (float)((GR_SHADOW_POINTS - 2) / 2.0);

        vtx[1] = -car->_dimension_y * MULT / 2.0;
        shadow_vtx->push_back(vtx);
        tex[1] = 0.0;
        shadow_tex->push_back(tex);

        vtx[1] = car->_dimension_y * MULT / 2.0;
        shadow_vtx->push_back(vtx);
        tex[1] = 1.0;
        shadow_tex->push_back(tex);
    }

    shadowAnchor->setName("shadow");
    shadowAnchor->setVertexArray(shadow_vtx);
    shadowAnchor->setNormalArray(shadow_nrm);
    shadowAnchor->setColorArray(shadow_clr, osg::Array::BIND_OVERALL);
    shadowAnchor->setTexCoordArray(0, shadow_tex, osg::Array::BIND_PER_VERTEX);
    shadowAnchor->addPrimitiveSet( new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN, 0, shadow_vtx->size()) );


    osg::ref_ptr<osg::Image> image = osgDB::readImageFile(path.c_str());
    osg::ref_ptr<osg::Texture2D> texture = new osg::Texture2D(image.get());
    texture->setWrap(osg::Texture::WRAP_S, osg::Texture::REPEAT);
    texture->setWrap(osg::Texture::WRAP_T, osg::Texture::REPEAT);
    stateSet->setTextureAttributeAndModes(0, texture.get());
    stateSet->setTextureMode(0, GL_TEXTURE_2D, osg::StateAttribute::ON);

    shadowAnchor->setStateSet(stateSet);

    /*grCarInfo[car->index].shadowBase = new ssgVtxTableShadow(GL_TRIANGLE_STRIP, shd_vtx, shd_nrm, shd_tex, shd_clr);
    grMipMap = 0;
    grCarInfo[car->index].shadowBase->setState(grSsgLoadTexState((char *)shdTexName));
    grCarInfo[car->index].shadowCurr = (ssgVtxTableShadow *)grCarInfo[car->index].shadowBase->clone(SSG_CLONE_GEOMETRY);
    grCarInfo[car->index].shadowAnchor->addKid(grCarInfo[car->index].shadowCurr);
    ShadowAnchor->addKid(grCarInfo[car->index].shadowAnchor);
    grCarInfo[car->index].shadowBase->ref();*/

    osg::MatrixTransform *shAnchor = new osg::MatrixTransform;
    sh->addDrawable(shadowAnchor);
    shAnchor->addChild(sh);

    return shAnchor;
}

void SDCarShadow::update(tCarElt *car, int visible)
{
    if (visible)
    {
        /*osg::Vec3 *p;
        tdble height1, height2, height3;

        for (int i = 0; i < GR_SHADOW_POINTS; i++)
        {
            height1 = shadow_vtx[i][0];
            height2 = shadow_vtx[i][1];
            height3 = RtTrackHeightG(car->_trkPos.seg, height1, height2) + 0.00;
            shadowAnchor->setVertexArray(osg::Vec3(height1, height2, height3));
        }*/
    }
}
