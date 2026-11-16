/***************************************************************************

    file                 : grwavesystem.cpp
    created              : Fri Jan 05 21:09:40 CEST 2024
    copyright            : (C) 2024 Xavier BERTAUX
    email                : bertauxx@yahoo.fr
    version              : $Id: grwavesystem.cpp 7786 2024-01-05 22:23:59Z shadow72 $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include <string.h>

#include "grLocal.h"
#include "grwavesystem.h"

#define G 9.8f

void cGrWaveSystem::updateAnimation ( float tim )
{
  if ( ntriangles <= 0 ||
       normals    == NULL || colours    == NULL ||
       texcoords  == NULL || vertices   == NULL ||
       orig_vertices == NULL )
    return ;

  int i ;

  float adjSpeed   [ GR_MAX_WAVETRAIN ] ;
  float sinHeading [ GR_MAX_WAVETRAIN ] ;
  float cosHeading [ GR_MAX_WAVETRAIN ] ;
  float length     [ GR_MAX_WAVETRAIN ] ;
  float lambda     [ GR_MAX_WAVETRAIN ] ;
  float height     [ GR_MAX_WAVETRAIN ] ;

  /* Pre-adjust speed's to allow for wind speed. */

  int num_trains = 0 ;

  for ( i = 0 ; i < GR_MAX_WAVETRAIN ; i++ )
    if ( train [ i ] != NULL )
    {
      adjSpeed   [num_trains] = train [ i ] -> getSpeed () * G *
                                                      tim / windSpeed ;
      sinHeading [num_trains] = (float) -sin ( train[i]->getHeading () *
                                                      SG_DEGREES_TO_RADIANS ) ;
      cosHeading [num_trains] = (float) cos ( train[i]->getHeading () *
                                                      SG_DEGREES_TO_RADIANS ) ;
      length     [num_trains] = train [ i ] -> getLength     () ;
      lambda     [num_trains] = train [ i ] -> getLambda     () ;
      height     [num_trains] = train [ i ] -> getWaveHeight () ;
      num_trains++ ;
    }

  for ( i = 0 ; i <= nstrips ; i++ )
  {
    float fade_i = (i<2) ? 0.0f : (i<7) ? (float)(i-2)/5.0f :
		   (i>nstrips-2) ? 0.0f :
		   (i>nstrips-7) ? (float)(nstrips-i-2)/5.0f : 1.0f ;

    for ( int j = 0 ; j <= nstacks ; j++ )
    {
      float fade_j = (j<2) ? 0.0f : (j<7) ? (float)(j-2)/5.0f :
		     (j>nstacks-2) ? 0.0f :
		     (j>nstacks-7) ? (float)(nstacks-j-2)/5.0f : 1.0f ;

      float edge_fade = fade_i * fade_j ;

      int  idx = i * (nstrips+1) + j ;

      float xx = orig_vertices [idx][0] + center[0] ;
      float yy = orig_vertices [idx][1] + center[1] ; 
      float zz = center[2] ;

      float x0 = xx + offset[0] ;
      float y0 = yy + offset[1] ;
      float z0 = vertices [idx][2] ;
      
      float depth = (gridGetter==NULL) ? 1000000.0f : gridGetter ( x0, y0 ) ;

      for ( int t = 0 ; t < num_trains ; t++ )
      {
	float adjHeight = height [ t ] * edge_fade ;
	float adjLength = ( depth < 0.2f ) ? 0.2f :
	                    ( depth > length[t] ) ? length[t] : depth ;

	float phase = ( x0 * sinHeading[t] + y0 * cosHeading[t] ) / adjLength -
                                             adjSpeed[t] - lambda[t] * z0 ;

        float delta = adjHeight * (float) sin ( phase ) ;

  	xx += delta * sinHeading [ t ] ;
  	yy += delta * cosHeading [ t ] ;
        zz += adjHeight * (float) -cos ( phase ) ;
      }

      sgSetVec3 ( vertices  [idx], xx, yy, zz ) ; 
      sgSetVec2 ( texcoords [idx], tu * x0 / size[0], tv * y0 /size[1] ) ;
    }
  }

  for ( i = 0 ; i < nstrips ; i++ )
  {
    int i1 =   i   * (nstrips+1) ;
    int i2 = (i+1) * (nstrips+1) ;

    for ( int j = 0 ; j < nstacks ; j++ )
    {
      int idx1 = i1 +   j   ;
      int idx2 = i2 +   j   ;
      int idx3 = i1 + (j+1) ;

      sgVec3 ab ; sgSubVec3 ( ab, vertices[idx3], vertices[idx1] ) ;
      sgVec3 ac ; sgSubVec3 ( ac, vertices[idx2], vertices[idx1] ) ;

      float nx = ab[1] * ac[2] - ab[2] * ac[1] ;
      float ny = ab[2] * ac[0] - ab[0] * ac[2] ;
      float nz = ab[0] * ac[1] - ab[1] * ac[0] ;

      /* About 10% of execution time is in this instruction!  */
      float rlen = 1.0f / (float) sqrt ( nx * nx + ny * ny + nz * nz ) ;

      normals[idx1][0] = nx * rlen ;
      normals[idx1][1] = ny * rlen ;
      normals[idx1][2] = nz * rlen ;
    }
  }

  for ( i = 0 ; i < nstrips ; i++ )
  {
    ssgVtxTable      *vt = (ssgVtxTable *) getKid ( i ) ;
    ssgVertexArray   *vv = vt -> getVertices  () ;
    ssgNormalArray   *nn = vt -> getNormals   () ;
    ssgColourArray   *cc = vt -> getColours   () ;
    ssgTexCoordArray *tt = vt -> getTexCoords () ;

    int i1 = (i+1) * (nstrips+1) ;
    int i2 =   i   * (nstrips+1) ;

    for ( int j = 0, jj = 0 ; j < nstacks + 1 ; j++, jj += 2, i1++, i2++ )
    {
      vv -> set ( vertices [i1], jj   ) ; vv -> set ( vertices [i2], jj+1 ) ;
      nn -> set ( normals  [i1], jj   ) ; nn -> set ( normals  [i2], jj+1 ) ;
      cc -> set ( colours  [i1], jj   ) ; cc -> set ( colours  [i2], jj+1 ) ;
      tt -> set ( texcoords[i1], jj   ) ; tt -> set ( texcoords[i2], jj+1 ) ;
    }
  }
}

void cGrWaveSystem::copy_from ( cGrWaveSystem *src, int clone_flags )
{
  cGrShape::copy_from ( src, clone_flags ) ;
 
  setDepthCallback ( src -> getDepthCallback () ) ;
  setWindSpeed     ( src -> getWindSpeed     () ) ;
  setWindDirn      ( src -> getWindDirn      () ) ;
  setEdgeFlatten   ( src -> getEdgeFlatten   () ) ;
} 

ssgBase *cGrWaveSystem::clone ( int clone_flags )
{
  cGrWaveSystem *b = new cGrWaveSystem ( getNumTris() ) ;
  b -> copy_from ( this, clone_flags ) ;
  return b ;
}

cGrWaveSystem::cGrWaveSystem ( int np ) : cGrShape ( np )
{
  type = cGrTypeWaveSystem ();

  setDepthCallback ( NULL ) ;
  setWindSpeed     ( 1.0f ) ;
  setWindDirn      ( 0.0f ) ;
  setEdgeFlatten   ( 0.0f ) ;

  nstrips = nstacks = 0 ;

  offset [ 0 ] = offset [ 1 ] = 0.0f ;

  normals   = NULL ;
  colours   = NULL ;
  texcoords = NULL ;
  vertices  = NULL ;
  orig_vertices  = NULL ;

  tu = tv = 1.0f ;

  for ( int i = 0 ; i < GR_MAX_WAVETRAIN ; i++ )
    train [ i ] = NULL ;

  regenerate();
}

cGrWaveSystem::~cGrWaveSystem (void) {}

const char *cGrWaveSystem::getTypeName(void) { return "cGrWaveSystem" ; }

void cGrWaveSystem::regenerate ()
{
  delete[] normals   ;
  delete[] colours   ;
  delete[] texcoords ;
  delete[] vertices  ;
  delete[] orig_vertices  ;

  normals   = NULL ;
  colours   = NULL ;
  texcoords = NULL ;
  vertices  = NULL ;
  orig_vertices  = NULL ;

  nstrips = nstacks = 0 ;

  if ( kidState != NULL ) 
		kidState -> ref () ;
  removeAllKids () ;

  if ( kidState != NULL )
	 kidState -> deRef () ;

  if ( ntriangles <= 0 )
    return ;

  int gridSize = (int) sqrt ( (float) ntriangles / 2.0f ) ;
 
  nstacks = gridSize ;
  nstrips = gridSize ;

  if ( nstacks < 1 ) nstacks = 1 ;
  if ( nstrips < 1 ) nstrips = 1 ;

  normals   = new sgVec3 [ (nstacks+1) * (nstrips+1) ] ;
  colours   = new sgVec4 [ (nstacks+1) * (nstrips+1) ] ;
  texcoords = new sgVec2 [ (nstacks+1) * (nstrips+1) ] ;
  vertices  = new sgVec3 [ (nstacks+1) * (nstrips+1) ] ;
  orig_vertices = new sgVec3 [ (nstacks+1) * (nstrips+1) ] ;

  int i;
  for ( i = 0 ; i <= nstrips ; i++ )
    for ( int j = 0 ; j <= nstacks ; j++ )
    {
      int idx = i * (nstrips+1) + j ;

      float x = (float) j / (float) nstacks ;
      float y = (float) i / (float) nstrips ;

      sgSetVec3  ( vertices [idx], (x-0.5f) * size[0],
                                   (y-0.5f) * size[1], 0.0f ) ;
      sgSetVec3  ( normals  [idx], 0.0f, 0.0f, 1.0f ) ;
      sgSetVec2  ( texcoords[idx], x * tu, y * tv ) ;
      sgCopyVec4 ( colours  [idx], colour ) ;
      sgCopyVec3 ( orig_vertices [ idx ], vertices [idx] ) ;
    }

  for ( i = 0 ; i < nstrips ; i++ )
  {
    ssgVtxTable      *vt = new ssgVtxTable ;
    ssgVertexArray   *vv = new ssgVertexArray   ( nstacks * 2 + 2 ) ;
    ssgNormalArray   *nn = new ssgNormalArray   ( nstacks * 2 + 2 ) ;
    ssgColourArray   *cc = new ssgColourArray   ( nstacks * 2 + 2 ) ;
    ssgTexCoordArray *tt = new ssgTexCoordArray ( nstacks * 2 + 2 ) ;

    addKid ( vt ) ;

    vt -> setState    ( getKidState () ) ;
    vt -> setCallback ( SSG_CALLBACK_PREDRAW , getKidPreDrawCB  () ) ;
    vt -> setCallback ( SSG_CALLBACK_POSTDRAW, getKidPostDrawCB () ) ;

    vt -> setPrimitiveType ( GL_TRIANGLE_STRIP ) ;

    for ( int j = 0 ; j < nstacks + 1 ; j++ )
    {
      int idx = (i+1) * (nstrips+1) + j ;

      vv -> add ( vertices [ idx ] ) ; nn -> add ( normals  [ idx ] ) ;
      cc -> add ( colours  [ idx ] ) ; tt -> add ( texcoords[ idx ] ) ;

      idx = i * (nstrips+1) + j ;

      vv -> add ( vertices [ idx ] ) ; nn -> add ( normals  [ idx ] ) ;
      cc -> add ( colours  [ idx ] ) ; tt -> add ( texcoords[ idx ] ) ;
    }

    vt -> setVertices  ( vv ) ;
    vt -> setNormals   ( nn ) ;
    vt -> setColours   ( cc ) ;
    vt -> setTexCoords ( tt ) ;

    vt -> recalcBSphere () ;
  }

  recalcBSphere () ;
}

extern int _ssgLoadObject ( FILE *, ssgBase **, int ) ;
extern int _ssgSaveObject ( FILE *, ssgBase * ) ;

#define load_field(fp, name) (fread(&(name), 1, sizeof(name), fp) == sizeof(name))
#define save_field(fp, name) (fwrite(&(name), 1, sizeof(name), fp) == sizeof(name))

int cGrWaveSystem::load ( FILE *fp )
{
   return ( load_field ( fp, windSpeed ) &&
            load_field ( fp, windHeading ) &&
            load_field ( fp, edgeFlatten ) &&
            load_field ( fp, tu ) &&
            load_field ( fp, tv ) &&
	    cGrShape::load ( fp ) ) ;
}

int cGrWaveSystem::save ( FILE *fp )
{
   return ( save_field ( fp, windSpeed ) &&
            save_field ( fp, windHeading ) &&
            save_field ( fp, edgeFlatten ) &&
            save_field ( fp, tu ) &&
            save_field ( fp, tv ) &&
        cGrShape::save ( fp ) ) ;
}
