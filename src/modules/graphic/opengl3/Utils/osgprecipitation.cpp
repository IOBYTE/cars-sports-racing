/***************************************************************************

    file                 : OsgPrecipitation.cpp
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

#include "OsgPrecipitation.h"
#include "OsgNodeMask.h"
#include "OsgMath.h"

#include <osg/ClipNode>

SDPrecipitation::SDPrecipitation() :
    _freeze(false),
    _enabled(true),
    _droplet_external(false),
    _snow_intensity(0.0),
    _rain_intensity(0.0),
    _clip_distance(5.0),
    _rain_droplet_size(0.0),
    _snow_flake_size(0.0),
    _illumination(1.0)
{
    _precipitationEffect = new osgParticle::PrecipitationEffect;
}

void SDPrecipitation::setEnabled( bool value )
{
    _enabled = value;

    if (!_enabled)
    {
        _precipitationEffect->snow(0);
        _precipitationEffect->rain(0);
    }
}

void SDPrecipitation::setDropletExternal( bool value )
{
    _droplet_external = value;
}

bool SDPrecipitation::getEnabled() const
{
    return _enabled;
}

osg::Group* SDPrecipitation::build(void)
{
    if (!_enabled)
        return nullptr;

    osg::ref_ptr<osg::Group> group = new osg::Group;

    _precipitationEffect->snow(0);
    _precipitationEffect->rain(0);

    if (_clip_distance!=0.0)
    {
        osg::ref_ptr<osg::ClipNode> clipNode = new osg::ClipNode;
        osg::ref_ptr<osg::ClipPlane> clipPlane = new osg::ClipPlane(0);
        clipNode->addClipPlane(clipPlane.get());
        clipNode->getClipPlane(0)->setClipPlane(0.0, 0.0, -1.0, -_clip_distance);
        clipNode->setReferenceFrame(osg::ClipNode::ABSOLUTE_RF);
        clipNode->addChild(_precipitationEffect.get());

        group->addChild(clipNode.get());
    }
    else
    {
        group->addChild(_precipitationEffect.get());
    }

    group->setNodeMask( ~(NODE_MASK_SHADOW_CAST | NODE_MASK_SKY_MODEL) );

    return group.release();
}

void SDPrecipitation::setSnowIntensity(float intensity)
{
    if (this->_snow_intensity < intensity-0.001)
        this->_snow_intensity += 0.001;
    else if (this->_snow_intensity > intensity+0.001)
        this->_snow_intensity -= 0.001;
    else
        this->_snow_intensity = intensity;
}

void SDPrecipitation::setRainIntensity(float intensity)
{
    if (this->_rain_intensity < intensity-0.001)
        this->_rain_intensity += 0.001;
    else if (this->_rain_intensity > intensity+0.001)
        this->_rain_intensity -= 0.001;
    else
        this->_rain_intensity = intensity;
}

void SDPrecipitation::setRainDropletSize(float size)
{
    _rain_droplet_size = size;
}

void SDPrecipitation::setIllumination(float illumination)
{
    _illumination = illumination;
}

void SDPrecipitation::setSnowFlakeSize(float size)
{
    _snow_flake_size = size;
}

void SDPrecipitation::setClipDistance(float distance)
{
    _clip_distance = distance;
}

void SDPrecipitation::setFreezing(bool freeze)
{
    if ((this->_freeze)&&(!freeze))
    {
        this->_rain_intensity = this->_snow_intensity;
        this->_snow_intensity = 0.0;
    }

    this->_freeze = freeze;
}

void SDPrecipitation::setWindProperty(double heading, double speed)
{
    double x, y, z;

    heading = (heading + 180) * SD_DEGREES_TO_RADIANS;
    speed = speed * SD_FEET_TO_METER;

    x = -cos(heading) * speed;
    y = sin(heading) * speed;
    z = 0;

    this->_wind_vec = osg::Vec3(x, y, z);
}

bool SDPrecipitation::update(void)
{
    if (!_enabled)
        return false;

    if (this->_freeze)
    {
        if (this->_rain_intensity > 0)
        {
            this->_snow_intensity = this->_rain_intensity;
        }
    }

    if (_enabled && this->_snow_intensity > 0)
    {
        _precipitationEffect->setWind(_wind_vec);

        if(_droplet_external)
        {
            if ((_freeze) && (_rain_droplet_size > 0.03))
            {
                _precipitationEffect->setParticleSize(_rain_droplet_size*1.5f);
                _precipitationEffect->setParticleSpeed( -1.0f - 22.36f*sqrtf(_rain_droplet_size));
                _precipitationEffect->setMaximumParticleDensity(_snow_intensity * 4.8f);
            }
            else if (_freeze)
            {
                _precipitationEffect->setParticleSize(_rain_droplet_size*1.3f);
                _precipitationEffect->setParticleSpeed( -0.75f - 0.25f*_snow_intensity);
                _precipitationEffect->setMaximumParticleDensity(_snow_intensity * 10.0f);
            }
            else
            {
                _precipitationEffect->setParticleSize(_snow_flake_size);
                _precipitationEffect->setParticleSpeed( -0.75f - 0.25f*_snow_intensity);
                _precipitationEffect->setMaximumParticleDensity(_snow_intensity * 7.2f);
            }
        }
        else
        {
            _precipitationEffect->setMaximumParticleDensity(_snow_intensity * 7.2f);
            _precipitationEffect->setParticleSize(0.02f + 0.03f*_snow_intensity);
            _precipitationEffect->setParticleSpeed( -0.75f - 0.25f*_snow_intensity);
        }


        _precipitationEffect->setCellSize(osg::Vec3(5.0f / (0.25f+_snow_intensity), 5.0f / (0.25f+_snow_intensity), 5.0f));

        _precipitationEffect->setNearTransition(25.f);
        _precipitationEffect->setFarTransition(100.0f - 60.0f*sqrtf(_snow_intensity));

        _precipitationEffect->setParticleColor(osg::Vec4(0.85 * _illumination, 0.85 * _illumination, 0.85 * _illumination, 1.0) - osg::Vec4(0.1, 0.1, 0.1, 1.0) * _snow_intensity);
    }
    else if (_enabled && this->_rain_intensity > 0)
    {
        _precipitationEffect->setWind(_wind_vec);


        if(_droplet_external)
        {
            _precipitationEffect->setParticleSize(_rain_droplet_size);
            _precipitationEffect->setParticleSpeed( -1.0f - 22.36f*sqrtf(_rain_droplet_size));
        }
        else
        {
            _precipitationEffect->setParticleSize(0.01 + 0.02*_rain_intensity);
            _precipitationEffect->setParticleSpeed( -2.0f + -5.0f*_rain_intensity);
        }

        _precipitationEffect->setMaximumParticleDensity(_rain_intensity * 7.5f);
        _precipitationEffect->setCellSize(osg::Vec3(5.0f / (0.25f+_rain_intensity), 5.0f / (0.25f+_rain_intensity), 5.0f));

        _precipitationEffect->setNearTransition(25.f);
        _precipitationEffect->setFarTransition(100.0f - 60.0f*sqrtf(_rain_intensity));

        _precipitationEffect->setParticleColor( osg::Vec4(0.64 * _illumination, 0.64 * _illumination, 0.64 * _illumination, 0.5));
    } else
    {
        _precipitationEffect->snow(0);
        _precipitationEffect->rain(0);
    }

    return true;
}
