/***************************************************************************

    file                 : OsgSkidmarks.cpp
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

#include <osg/Geometry>
#include <osg/Geode>

#include <portability.h>

#include "OsgSkidmarks.h"
#include "OsgMath.h"

static int grSkidMaxStripByWheel;
static int grSkidMaxPointByStrip;
static double grSkidDeltaT;

OsgSkidStrip::OsgSkidStrip()
{
    vtx = new osg::Vec3dArray(grSkidMaxStripByWheel);
    tex = new osg::Vec2dArray(grSkidMaxStripByWheel);
    vta = new osg::Vec3dArray(grSkidMaxStripByWheel);
    clr = new osg::Vec4dArray(grSkidMaxStripByWheel);

    smooth_colour[0]=0.0;
    smooth_colour[1]=0.0;
    smooth_colour[2]=0.0;
    smooth_colour[3]=0.0;

    state = new int(grSkidMaxStripByWheel);
    size  = new int(grSkidMaxStripByWheel);

    for(int k = 0; k < grSkidMaxStripByWheel; k++)
    {
        state[k] = SKID_UNUSED;
        vtx[k] = new osg::Vec3dArray(grSkidMaxPointByStrip + 1);
        tex[k] = new osg::Vec2dArray(grSkidMaxPointByStrip + 1);
        clr[k] = new osg::Vec4dArray(grSkidMaxPointByStrip + 1);
        vta[k] = new osg::Vec3dArray(GL_TRIANGLE_STRIP, vtx[k], shd_nrm, tex[k], clr[k]);
        vta[k]->setCullFace(0);
        vta[k]->setState(skidState);
        timeStrip = 0;
        SkidAnchor->addKid(vta[k]);
    }//for k

    running_skid = 0;	// no skid is in use
    next_skid = 0;	// the next skid to used is the first one
    last_state_of_skid = false;	// there was no skid for this wheel during the next shot
    tex_state = 0.0;
}//cGrSkidStrip::cGrSkidStrip


OsgSkidStrip::~OsgSkidStrip()
{
    delete [] vtx;
    delete [] vta;
    delete [] tex;
    delete [] state;
    delete [] size;
    delete [] clr;
}//~cGrSkidStrip

void OsgSkidStrip::End()
{
    if(last_state_of_skid == true)
    {
        /* end the running skid */
        state[running_skid] = SKID_STOPPED;
        vta[running_skid]->recalcBSphere();
        last_state_of_skid = false;

        next_skid += 1;
        if (next_skid >= grSkidMaxStripByWheel) {
            next_skid = 0;
            /* reset the next skid vertexArray */
            vtx[next_skid]->removeAll();
            tex[running_skid]->removeAll();
            clr[next_skid]->removeAll();
        }//if next_skid >= grSkidMaxStripByWheel
    }//if last_state_of_skid == true
}//cGrSkidStrip::End

void OsgSkidStrip::Begin(osg::Vec3d *tvtx, osg::Vec2d *tclist, osg::Vec4d *cur_clr, const double t)
{
    if(last_state_of_skid == false)
    {
        /* begin case */
        last_state_of_skid = true;
        /* start a new one */
        running_skid = next_skid;
        if(state[running_skid] != SKID_UNUSED)
        {
            vtx[running_skid]->removeAll();
            tex[running_skid]->removeAll();
            clr[running_skid]->removeAll();
            tex_state = 0.0;
        }//if state != SKID_UNUSED

        tex_state = 0.0;
        state[running_skid] = SKID_BEGIN;

        vtx[running_skid]->add(tvtx[0]);
        vtx[running_skid]->add(tvtx[1]);
        tex[running_skid]->add(tclist[0]);
        tex[running_skid]->add(tclist[1]);
        clr[running_skid]->add(*cur_clr);
        clr[running_skid]->add(*cur_clr);

        vta[running_skid]->recalcBSphere();
        size[running_skid] = 2;
        timeStrip = t;
        vta[running_skid]->setCullFace(TRUE);
    }
    else
    {
        /* continue case */
        vtx[running_skid]->add(tvtx[0]);
        vtx[running_skid]->add(tvtx[1]);
        tex[running_skid]->add(tclist[0]);
        tex[running_skid]->add(tclist[1]);
        clr[running_skid]->add(*cur_clr);
        clr[running_skid]->add(*cur_clr);

        size[running_skid] += 2;
        if(state[running_skid] == SKID_BEGIN)
        {
            state[running_skid] = SKID_RUNNING;
            /*SkidAnchor->addKid(vta[running_skid]);*/
        }//if state == SKID_BEGIN
        vta[running_skid]->recalcBSphere();
        timeStrip=t;
    }
}

void SDSkidmarks::updateSkidmarks(const tCarElt* car, const double t)
{
    osg::Vec4 cur_clr;
    cur_clr[0] = cur_clr[1] = cur_clr[2] = 1.0f;

    for(int i = 0; i < 4; i++) {
        tdble sling_mud = 1.0f;
        tdble skid_sensitivity = 0.75f;

        if(car->priv.wheel[i].seg) { // sanity check
            const char* s = car->priv.wheel[i].seg->surface->material;

            if(strstr(s, "sand"))
            {
                cur_clr[0] = 0.8f;
                cur_clr[1] = 0.6f;
                cur_clr[2] = 0.35f;
                skid_sensitivity = 0.9f;
            }
            else if (strstr(s, "dirt"))
            {
                cur_clr[0] = 0.7f;
                cur_clr[1] = 0.55f;
                cur_clr[2] = 0.45f;
                skid_sensitivity = 0.9f;
            }
            else if (strstr(s,"mud"))
            {
                cur_clr[0] = 0.5f;
                cur_clr[1] = 0.35f;
                cur_clr[2] = 0.15f;
                skid_sensitivity = 1.0f;
            }
            else if (strstr(s,"grass"))
            {
                cur_clr[0] = 0.75f;
                cur_clr[1] = 0.5f;
                cur_clr[2] = 0.3f;
                skid_sensitivity = 0.8f;
            }
            else if (strstr(s,"gravel"))
            {
                cur_clr[0] = 0.6f;
                cur_clr[1] = 0.6f;
                cur_clr[2] = 0.6f;
                skid_sensitivity = 0.7f;
            }
            else
            {
                sling_mud=0.0;
                cur_clr[0] = 0.0f;
                cur_clr[1] = 0.0f;
                cur_clr[2] = 0.0f;
                skid_sensitivity = 0.5f;
            }//if strstr(s, some_material)
        }//if car->priv.wheel

        if(car->_skid[i] > 0.1f)
        {
            cur_clr[3] = tanh(skid_sensitivity*car->_skid[i]);
        }
        else
        {
            cur_clr[3] = 0.0f;
        }//if car->skid

        for(int c = 0; c < 3; c++)
        {
            tdble tmp = strips[i].smooth_colour[c];
            strips[i].smooth_colour[c] = 0.9f*tmp + 0.1f*cur_clr[c];
            cur_clr[c] = tmp;
        }//for c

        if((t - strips[i].timeStrip) < grSkidDeltaT)
        {
            continue;
        }

        if((car->_speed_x * car->_speed_x + car->_speed_y * car->_speed_y) > 1.0f)
        {
            if (cur_clr[3] > 0.1f)
            {
                ssgVertexArray *basevtx = new ssgVertexArray(4 * 2 + 1);
                tdble sling_left = -sling_mud;
                tdble sling_right = sling_mud;
                tdble z_adjust = 0.95f;
                tdble contact_z = car->priv.wheel[i].relPos.z - car->_wheelRadius(i)*z_adjust;

                //One side
                osg::Vec3 vtx;
                vtx[0] = car->priv.wheel[i].relPos.x - car->_tireHeight(i);
                // Because of backface culling the winding of the triangles matters.
                if (car->_speed_x > 0.0f)
                {
                    vtx[1] = car->priv.wheel[i].relPos.y + (sling_right + 1.0f)*car->_tireWidth(i) / 2.0f;
                }
                else
                {
                    vtx[1] = car->priv.wheel[i].relPos.y + (sling_left - 1.0f)* car->_tireWidth(i) / 2.0f;
                }
                vtx[2] = contact_z;
                basevtx->add(vtx);

                if (car->_speed_x > 0.0f)
                {
                    vtx[1] = car->priv.wheel[i].relPos.y + (sling_left - 1.0f)* car->_tireWidth(i) / 2.0f;
                }
                else
                {
                    vtx[1] = car->priv.wheel[i].relPos.y + (sling_right + 1.0f)*car->_tireWidth(i) / 2.0f;
                }

                basevtx->add(vtx);

                ssgTexCoordArray *texcoords = new ssgTexCoordArray();
                osg::Vec2 TxVtx;
                TxVtx[0] = strips[i].tex_state;
                TxVtx[1] = 0.75f + sling_right * 0.25f;
                texcoords->add(TxVtx);

                TxVtx[1] = 0.25f + sling_left * 0.25f;
                texcoords->add(TxVtx);

                //should get it from somewhere else really.
                //I could also account for radius, but that's a bit of an overkill.
                float dt = 0.01f;
                strips[i].tex_state += dt*car->_wheelSpinVel(i);

                osg::Vec3 *tvtx;
                osg::Vec2 *tclist;
                base = new ssgVtxTable(GL_TRIANGLE_STRIP, basevtx, NULL, texcoords, NULL);
                base->transform(grCarInfo[car->index].carPos);
                base->getVertexList((void**)&tvtx);
                base->getTexCoordList((void**)&tclist);

                strips[i].Begin(tvtx, tclist, &cur_clr, t);

                basevtx->removeAll();
                delete base;
            }
            else
            {
                strips[i].End();
            }
        }
    }
}
