/***************************************************************************

    file                 : grlensflare.h
    created              : Fri Jan 05 21:09:40 CEST 2024
    copyright            : (C) 2024 Xavier BERTAUX
    email                : bertauxx@yahoo.fr
    version              : $Id: grlensflare.h 7786 2024-01-05 22:23:59Z shadow72 $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "grshapes.h"

class cGrLensFlare : public cGrShape
{
  ssgVtxTable      *vt;
  ssgVertexArray   *v0;
  ssgNormalArray   *n0;
  ssgColourArray   *c0;
  ssgTexCoordArray *t0;

  void update( sgMat4 m );

protected:
  virtual void copy_from( cGrLensFlare *src, int clone_flags );
public:

  cGrLensFlare() ;
  cGrLensFlare( int nt );

  virtual ~cGrLensFlare();

  virtual ssgBase    *clone( int clone_flags = 0 );
  virtual void        regenerate ();
  virtual const char *getTypeName( void );
  virtual void        cull  ( sgFrustum *f, sgMat4 m, int test_needed );
} ;

unsigned char *_cGrGetLensFlareTexture ();
