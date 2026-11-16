/***************************************************************************

    file                 : grwshapes.h
    created              : Fri Jan 05 21:09:40 CEST 2024
    copyright            : (C) 2024 Xavier BERTAUX
    email                : bertauxx@yahoo.fr
    version              : $Id: grshapes.h 7786 2024-01-05 22:23:59Z shadow72 $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


#ifndef _GRSHAPES_H_
#define _GRSHAPES_H_  1

#include <plib/ssg.h>

typedef float sgVec9 [ 9 ] ;  /* Needed for ssgaPatch */

class cGrShape : public ssgBranch
{
  int corrupted ;

protected:
  virtual void copy_from ( cGrShape *src, int clone_flags ) ;

  sgVec4 colour ;
  sgVec3 center ;
  sgVec3 size   ;

  int ntriangles ;

  ssgState   *kidState      ;
  ssgCallback kidPreDrawCB  ;
  ssgCallback kidPostDrawCB ;

  void init () ;

protected:
  ssgState    *getKidState      () { return kidState      ; }
  ssgCallback  getKidPreDrawCB  () { return kidPreDrawCB  ; }
  ssgCallback  getKidPostDrawCB () { return kidPostDrawCB ; }

public:
  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  cGrShape (void) ;
  cGrShape ( int numtris ) ;
  virtual ~cGrShape (void) ;
  virtual const char *getTypeName(void) ;

  void makeCorrupt  () { corrupted = TRUE  ; }
  int  isCorrupt    () { return corrupted  ; }

  float *getCenter  () { return center     ; }
  float *getSize    () { return size       ; }
  int    getNumTris () { return ntriangles ; }

  void setColour  ( sgVec4 c ) { sgCopyVec4 ( colour, c ) ; regenerate () ; }
  void setCenter  ( sgVec3 c ) { sgCopyVec3 ( center, c ) ; regenerate () ; }
  void setSize    ( sgVec3 s ) { sgCopyVec3 ( size  , s ) ; regenerate () ; }
  void setSize    ( float  s ) { sgSetVec3  ( size,s,s,s) ; regenerate () ; }
  void setNumTris ( int ntri ) { ntriangles = ntri ; regenerate () ; }

  void setKidState    ( ssgState *s )
  {
    kidState = s ;

    for ( int i = 0 ; i < getNumKids() ; i++ )
      ((ssgLeaf *)getKid(i)) -> setState ( s ) ;
  }

  void setKidCallback ( int cb_type, ssgCallback cb )
  {
    if ( cb_type == SSG_CALLBACK_PREDRAW )
      kidPreDrawCB = cb ;
    else
      kidPostDrawCB = cb ;

    for ( int i = 0 ; i < getNumKids() ; i++ )
      ((ssgLeaf *)getKid(i)) -> setCallback ( cb_type, cb ) ;
  }

  virtual void regenerate () = 0 ;

  virtual int load ( FILE * ) ;
  virtual int save ( FILE * ) ;
} ;

class cGrCube : public cGrShape
{
protected:
  virtual void copy_from ( cGrCube *src, int clone_flags ) ;
public:
  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  cGrCube (void) ;
  cGrCube ( int numtris ) ;
  virtual ~cGrCube (void) ;
  virtual const char *getTypeName(void) ;
  virtual void regenerate () ;
} ;

#define GR_HAVE_PATCH 1

class cGrPatch : public cGrShape
{
  int   levels ;
  sgVec9 control_points[4][4] ;
  void  makePatch  ( sgVec9 points[4][4], int levels ) ;
  void  writePatch ( sgVec9 points[4][4] ) ;
  void  makeHSpline  ( sgVec9 points[4]   , sgVec9 nv[7]    ) ;
  void  makeVSplines ( sgVec9 points[4][7], sgVec9 nv[7][7] ) ;


protected:
  virtual void copy_from ( cGrPatch *src, int clone_flags ) ;
public:
  virtual ssgBase *clone ( int clone_flags = 0 ) ;
  cGrPatch (void) ;
  cGrPatch ( int numtris ) ;

  void setControlPoint ( int s, int t, sgVec3 xyz, sgVec2 uv, sgVec4 rgba ) ;
  void setControlPoint ( int s, int t,
                         float x, float y, float z,
                         float u, float v,
                         float r, float g, float b, float a ) ;
  void getControlPoint ( int s, int t, sgVec3 xyz, sgVec2 uv, sgVec4 rgba ) ;

  virtual ~cGrPatch (void) ;
  virtual const char *getTypeName(void) ;
  virtual void regenerate () ;

  virtual int load ( FILE * ) ;
  virtual int save ( FILE * ) ;
} ;

#endif

