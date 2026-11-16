/***************************************************************************

    file                 : OsgSkidmarks.h
    created              : Sat Dec 10 22:24:02 CEST 2022
    copyright            : (C) 2022 by Xavier BERTAUX
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

#ifndef _OSGSKIDMARKS_H_
#define _OSGSKIDMARKS_H_

#include <osg/Geometry>
#include <car.h>

#define DELTATSTRIP 0.3f
#define MAXPOINT_BY_STRIP 600
#define MAXSTRIP_BYWHEEL  40
#define DIST_INTERVAL     0.2f

#define SKID_UNUSED  1
#define SKID_BEGIN   2
#define SKID_RUNNING 3
#define SKID_STOPPED 4

class OsgSkidStrip
{
public:
    OsgSkidStrip();
    virtual ~OsgSkidStrip();

    virtual void Begin(osg::Vec3d *tvtx, osg::Vec2d *tclist, osg::Vec4d *cur_clr, const double t);
    virtual void End();

    osg::Vec3dArray		*vtx;
    osg::Vec3dArray   	*vta;
    osg::Vec2dArray	    *tex;
    osg::Vec4dArray		*clr;
    osg::Vec4			smooth_colour;
    int					*state;
    int					*size;
    double				timeStrip;
    int					running_skid;
    int					next_skid;
    bool				last_state_of_skid;
    int					skid_full;
    float				tex_state;
};

class SDSkidmarks
{
private :
    tCarElt *car;
    osg::ref_ptr<osg::Vec4Array> brake_colors[4];

public :
    SDSkidmarks() : car(nullptr) { }
    void setCar(tCarElt * car_elt);
    osg::Node *initSkidmarks(int wheelIndex);
    void updateSkidmarks(const tCarElt *car, const double t);
};

#endif /* _OSGSKIDMARKS_H_ */
