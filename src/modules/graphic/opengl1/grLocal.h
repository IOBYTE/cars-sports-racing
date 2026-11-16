/***************************************************************************

    file                 : grlocal.h
    created              : Thu Nov 25 21:09:40 CEST 2010
    copyright            : (C) 2010 by Jean-Philippe Meuret
    email                : bertauxx@yahoo.fr
    version              : $Id: grlocal.h 7786 2021-11-20 22:23:59Z torcs-ng $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#ifndef _GRLOCAL_H_
#define _GRLOCAL_H_ 1

#include <plib/ssg.h>

//#include "ssgaParticleSystem.h"
//#include "ssgaFire.h"

#define _GR_TYPE_SHAPE          0x00008000
#define _GR_TYPE_CUBE           0x00004000
#define _GR_TYPE_PATCH          0x00008000
#define _GR_TYPE_PARTICLESYSTEM 0x00010000
#define _GR_TYPE_WAVESYSTEM     0x00040000
#define _GR_TYPE_LENSFLARE      0x00080000

inline int cGrTypeShape() { return _GR_TYPE_SHAPE | ssgTypeBranch (); }
inline int cGrTypeCube() { return _GR_TYPE_CUBE | cGrTypeShape (); }
inline int cGrTypePatch() { return _GR_TYPE_PATCH | cGrTypeShape (); }

inline int cGrTypeParticleSystem() { return _GR_TYPE_PARTICLESYSTEM | cGrTypeShape (); }
inline int cGrTypeWaveSystem() { return _GR_TYPE_WAVESYSTEM | cGrTypeShape (); }
inline int cGrTypeLensFlare() { return _GR_TYPE_LENSFLARE | cGrTypeShape (); }

void _ssgPushMatrix( sgMat4 m );
void _ssgPopMatrix();

void ssgaInit ();

#endif

