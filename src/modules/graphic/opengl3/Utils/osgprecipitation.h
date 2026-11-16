/***************************************************************************

    file                 : OsgPrecipitation.h
    created              : Mon Dec 28 10:24:02 CEST 2020
    copyright            : (C) 2020 Xavier BERTAUX
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

#ifndef _OSGPRECIPITATION_H
#define _OSGPRECIPITATION_H

#include <osg/Group>
#include <osg/Referenced>
#include <osgParticle/PrecipitationEffect>


class SDPrecipitation : public osg::Referenced
{
protected:
    bool _freeze;
    bool _enabled;
    bool _droplet_external;

    float _snow_intensity;
    float _rain_intensity;
    float _clip_distance;
    float _rain_droplet_size;
    float _snow_flake_size;
    float _illumination;

    osg::Vec3 _wind_vec;

    osg::ref_ptr<osgParticle::PrecipitationEffect> _precipitationEffect;

public:
    SDPrecipitation();
    virtual ~SDPrecipitation() {}
    osg::Group* build(void);
    bool update(void);

    void setWindProperty(double, double);
    void setFreezing(bool);
    void setDropletExternal(bool);
    void setRainIntensity(float);
    void setSnowIntensity(float);
    void setRainDropletSize(float);
    void setSnowFlakeSize(float);
    void setIllumination(float);
    void setClipDistance(float);

    void setEnabled(bool);
    bool getEnabled() const;
};

#endif
