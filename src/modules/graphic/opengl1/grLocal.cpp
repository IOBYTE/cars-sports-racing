/***************************************************************************

    file                 : grlocal.cpp
    created              : Thu Nov 25 21:09:40 CEST 2010
    copyright            : (C) 2010 by Jean-Philippe Meuret
    email                : bertauxx@yahoo.fr
    version              : $Id: grlocal.cpp 7786 2021-11-20 22:23:59Z torcs-ng $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#include "grLocal.h"
#include "grshapes.h"

static ssgBase *createCube() { return new cGrCube ( 0 ); }

void ssgaInit ()
{
  ssgRegisterType ( cGrTypeCube(), createCube );
}
