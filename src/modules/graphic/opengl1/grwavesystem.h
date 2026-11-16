/***************************************************************************

    file                 : grwavesystem.h
    created              : Fri Jan 05 21:09:40 CEST 2024
    copyright            : (C) 2024 Xavier BERTAUX
    email                : bertauxx@yahoo.fr
    version              : $Id: grwavesystem.h 7786 2024-01-05 22:23:59Z shadow72 $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <plib/sg.h>

#include "grshapes.h"

typedef float (* cGrWSDepthCallback ) ( float x, float y ) ;

#define GR_MAX_WAVETRAIN  16

class cGrWaveTrain
{
  float height ;
  float length ;
  float lambda ;
  float speed  ;
  float heading;

public:
  cGrWaveTrain ()
  {
    height  = 0.5f ;
    length  = 0.8f ;
    lambda  = 1.0f ;
    speed   = (float) sqrt ( 2.0f/3.0f ) ;
    heading = 0.0f ;
  }

  float getSpeed () { return speed  ; }
  void  setSpeed ( float h ) { speed  = h ; }

  float getLength () { return length  ; }
  void  setLength ( float h ) { length  = h ; }

  float getLambda () { return lambda  ; }
  void  setLambda ( float h ) { lambda  = h ; }

  float getHeading () { return heading  ; }
  void  setHeading ( float h ) { heading  = h ; }

  float getWaveHeight () { return height  ; }
  void  setWaveHeight ( float h ) { height  = h ; }
} ;

class cGrWaveSystem : public cGrShape
{
  cGrWSDepthCallback gridGetter ;

  sgVec2  offset    ;
  sgVec3 *normals   ;
  sgVec4 *colours   ;
  sgVec2 *texcoords ;
  sgVec3 *vertices  ;
  sgVec3 *orig_vertices  ;

  cGrWaveTrain *train [ GR_MAX_WAVETRAIN ] ;

  float windSpeed   ;
  float windHeading ;
  float edgeFlatten ;

  float tu, tv ;

  int nstrips ;
  int nstacks ;

protected:
  virtual void copy_from ( cGrWaveSystem *src, int clone_flags ) ;
public:

  cGrWaveSystem ( int ntri ) ;

  virtual ~cGrWaveSystem () ;

  virtual ssgBase    *clone       ( int clone_flags = 0 ) ;
  virtual void        regenerate  () ;
  virtual const char *getTypeName ( void ) ;
 
  virtual int load ( FILE * ) ;
  virtual int save ( FILE * ) ;

  cGrWSDepthCallback getDepthCallback () { return gridGetter ; } 

  cGrWaveTrain *getWaveTrain ( int i )
  {
    return ( i < 0 || i >= GR_MAX_WAVETRAIN ) ? NULL : train [ i ] ;
  }

  void setWaveTrain ( int i, cGrWaveTrain *t )
  {
    assert ( i >= 0 && i < GR_MAX_WAVETRAIN ) ;
    train [ i ] = t ;
  }

  float getWindSpeed     () { return windSpeed   ; }
  float getWindDirn      () { return windHeading ; }
  float getEdgeFlatten   () { return edgeFlatten ; }
  float getTexScaleU () { return tu ; }
  float getTexScaleV () { return tv ; }

  void  setDepthCallback ( cGrWSDepthCallback cb )  { gridGetter  = cb ; } 
  void  setWindSpeed     ( float speed            ) { windSpeed   = speed ; }
  void  setWindDirn      ( float dirn             ) { windHeading = dirn  ; }
  void  setEdgeFlatten   ( float dist             ) { edgeFlatten = dist ; }
  void  setTexScale      ( float u, float v       ) { tu = u ; tv = v ; }

  void  setOffset ( sgVec2 _offset ) { sgCopyVec2 ( offset, _offset ) ; }

  void updateAnimation ( float t ) ;
} ;
