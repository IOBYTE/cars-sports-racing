/***************************************************************************

    file                 : grwshapes.cpp
    created              : Fri Jan 05 21:09:40 CEST 2024
    copyright            : (C) 2024 Xavier BERTAUX
    email                : bertauxx@yahoo.fr
    version              : $Id: grshapes.cpp 7786 2024-01-05 22:23:59Z shadow72 $

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

#include "grshapes.h"
#include "grLocal.h"

#define SIN(x) ((float)(sin(x)))
#define COS(x) ((float)(cos(x)))
#define ATAN2(x, y) ((float)(atan2(x, y)))

void cGrShape::copy_from( cGrShape *src, int clone_flags )
{
    ssgBranch::copy_from ( src, clone_flags );

    if ( src -> isCorrupt () )
        makeCorrupt ();

    sgCopyVec4 ( colour, src->colour );
    sgCopyVec3 ( center, src->center );
    sgCopyVec3 ( size  , src->size   );

    ntriangles    = src -> ntriangles;
    kidState      = src -> getKidState();
    kidPreDrawCB  = src -> getKidPreDrawCB();
    kidPostDrawCB = src -> getKidPostDrawCB();
}

void cGrCube::copy_from( cGrCube *src, int clone_flags) { cGrShape::copy_from ( src, clone_flags ) ;}
void cGrPatch::copy_from( cGrPatch *src, int clone_flags) { cGrShape::copy_from ( src, clone_flags ) ;}

ssgBase *cGrShape::clone( int clone_flags )
{
    return NULL;
}

ssgBase *cGrCube::clone( int clone_flags )
{
    cGrCube *b = new cGrCube;
    b -> copy_from ( this, clone_flags );

    return b;
}

ssgBase *cGrPatch::clone( int clone_flags )
{
    cGrPatch *b = new cGrPatch;
    b -> copy_from ( this, clone_flags );

    return b ;
}

cGrShape::cGrShape (void)
{
    ntriangles = 50;
    init () ;
}

cGrShape::cGrShape ( int np )
{
    ntriangles = np;
    init () ;
}

void cGrShape::init ()
{
    type = cGrTypeShape ();
    corrupted = FALSE;
    sgZeroVec3( center );
    sgSetVec4( colour, 1.0f, 1.0f, 1.0f, 1.0f );
    sgSetVec3( size, 1.0f, 1.0f, 1.0f );
    kidState      = NULL;
    kidPreDrawCB  = NULL;
    kidPostDrawCB = NULL;
} 

cGrCube ::cGrCube ( void ):cGrShape()  {type=cGrTypeCube ();regenerate(); }
cGrCube ::cGrCube (int nt):cGrShape(nt){type=cGrTypeCube ();regenerate(); }
cGrPatch::cGrPatch( void ):cGrShape()  {type=cGrTypePatch();regenerate(); }
cGrPatch::cGrPatch(int nt):cGrShape(nt){type=cGrTypePatch();regenerate(); }

cGrShape   ::~cGrShape(void) {}
cGrCube    ::~cGrCube(void) {}
cGrPatch   ::~cGrPatch(void) {}

const char *cGrShape   ::getTypeName(void) { return "cGrShape"; }
const char *cGrCube    ::getTypeName(void) { return "cGrCube"; }
const char *cGrPatch   ::getTypeName(void) { return "cGrPatch"; }

void cGrCube::regenerate()
{
    if ( kidState != NULL)
        kidState -> ref();

    removeAllKids();

    if ( kidState != NULL)
        kidState -> deRef();

    if (ntriangles == 0)
        return;

    ssgVtxTable *vt0 = new ssgVtxTable();
    ssgVtxTable *vt1 = new ssgVtxTable();

    ssgVertexArray *v0 = new ssgVertexArray( 8 );
    ssgVertexArray *v1 = new ssgVertexArray( 8 );
    ssgNormalArray *n0 = new ssgNormalArray( 8 );
    ssgNormalArray *n1 = new ssgNormalArray( 8 );
    ssgColourArray *c0 = new ssgColourArray( 8 );
    ssgColourArray *c1 = new ssgColourArray( 8 );
    ssgTexCoordArray *t0 = new ssgTexCoordArray( 8 );
    ssgTexCoordArray *t1 = new ssgTexCoordArray( 8 );

    vt0 -> setPrimitiveType ( GL_TRIANGLE_STRIP );
    vt1 -> setPrimitiveType ( GL_TRIANGLE_STRIP );

    addKid ( vt0 );
    addKid ( vt1 );

    vt0 -> setState( getKidState());
    vt1 -> setState( getKidState());
    vt0 -> setCallback( SSG_CALLBACK_PREDRAW , getKidPreDrawCB());
    vt1 -> setCallback( SSG_CALLBACK_PREDRAW , getKidPreDrawCB());
    vt0 -> setCallback( SSG_CALLBACK_POSTDRAW, getKidPostDrawCB());
    vt1 -> setCallback( SSG_CALLBACK_POSTDRAW, getKidPostDrawCB());

    sgVec3 v ;
    sgVec3 n ;
    sgVec2 t ;

    sgSetVec3 ( v, center[0]+size[0]/2.0f, center[1]-size[1]/2.0f, center[2]+size[2]/2.0f ) ; v0->add ( v ) ;
    sgSetVec3 ( n,          1            ,          0            ,          0             ) ; n0->add ( n ) ;
    c0->add ( colour ) ;
    sgSetVec2 ( t,          0                       ,                1                    ) ; t0->add ( t ) ;

    sgSetVec3 ( v, center[0]+size[0]/2.0f, center[1]-size[1]/2.0f, center[2]-size[2]/2.0f ) ; v0->add ( v ) ;
    sgSetVec3 ( n,          1            ,          0            ,          0             ) ; n0->add ( n ) ;
    c0->add ( colour ) ;
    sgSetVec2 ( t,          0                       ,                0                    ) ; t0->add ( t ) ;

    sgSetVec3 ( v, center[0]+size[0]/2.0f, center[1]+size[1]/2.0f, center[2]+size[2]/2.0f ) ; v0->add ( v ) ;
    sgSetVec3 ( n,          1            ,          0            ,          0             ) ; n0->add ( n ) ;
    c0->add ( colour ) ;
    sgSetVec2 ( t,          1                       ,                1                    ) ; t0->add ( t ) ;

    sgSetVec3 ( v, center[0]+size[0]/2.0f, center[1]+size[1]/2.0f, center[2]-size[2]/2.0f ) ; v0->add ( v ) ;
    sgSetVec3 ( n,          1            ,          0            ,          0             ) ; n0->add ( n ) ;
    c0->add ( colour ) ;
    sgSetVec2 ( t,          1                       ,                0                    ) ; t0->add ( t ) ;

    sgSetVec3 ( v, center[0]-size[0]/2.0f, center[1]+size[1]/2.0f, center[2]+size[2]/2.0f ) ; v0->add ( v ) ;
    sgSetVec3 ( n,          0            ,          1            ,          0             ) ; n0->add ( n ) ;
    c0->add ( colour ) ;
    sgSetVec2 ( t,          2                       ,                1                    ) ; t0->add ( t ) ;

    sgSetVec3 ( v, center[0]-size[0]/2.0f, center[1]+size[1]/2.0f, center[2]-size[2]/2.0f ) ; v0->add ( v ) ;
    sgSetVec3 ( n,          0            ,          1            ,          0             ) ; n0->add ( n ) ;
    c0->add ( colour ) ;
    sgSetVec2 ( t,          2                       ,                0                    ) ; t0->add ( t ) ;

    sgSetVec3 ( v, center[0]-size[0]/2.0f, center[1]-size[1]/2.0f, center[2]+size[2]/2.0f ) ; v0->add ( v ) ;
    sgSetVec3 ( n,         -1            ,          0            ,          0             ) ; n0->add ( n ) ;
    c0->add ( colour ) ;
    sgSetVec2 ( t,          3                       ,                1                    ) ; t0->add ( t ) ;

    sgSetVec3(v, center[0]-size[0]/2.0f, center[1]-size[1]/2.0f, center[2]-size[2]/2.0f);
    v0->add ( v );
    sgSetVec3(n, -1, 0, 0);
    n0->add ( n );
    c0->add ( colour );
    sgSetVec2 ( t, 3, 0);
    t0->add ( t ) ;

    vt0 -> setVertices( v0 );
    vt0 -> setNormals( n0 );
    vt0 -> setColours( c0 );
    vt0 -> setTexCoords( t0 );

    vt0 -> recalcBSphere();

    sgSetVec3( v, center[0]-size[0]/2.0f, center[1]+size[1]/2.0f, center[2]-size[2]/2.0f );
    v1->add ( v );
    sgSetVec3( n, 0, 0, -1);
    n1->add( n );
    c1->add( colour );
    sgSetVec2( t, 0, 0);
    t1->add ( t ) ;

    sgSetVec3( v, center[0]+size[0]/2.0f, center[1]+size[1]/2.0f, center[2]-size[2]/2.0f ) ;
    v1->add ( v ) ;
    sgSetVec3( n,          0            ,          0            ,         -1             ) ;
    n1->add ( n ) ;
    c1->add( colour ) ;
    sgSetVec2( t,          1                       ,                0                    ) ;
    t1->add ( t ) ;

    sgSetVec3( v, center[0]-size[0]/2.0f, center[1]-size[1]/2.0f, center[2]-size[2]/2.0f ) ;
    v1->add ( v ) ;
    sgSetVec3( n,          0            ,          0            ,         -1             ) ;
    n1->add ( n ) ;
    c1->add( colour ) ;
    sgSetVec2 ( t,          0                       ,                1                    ) ;
    t1->add ( t ) ;

    sgSetVec3 ( v, center[0]+size[0]/2.0f, center[1]-size[1]/2.0f, center[2]-size[2]/2.0f ) ;
    v1->add ( v ) ;
    sgSetVec3 ( n,          0            ,          0            ,         -1             ) ;
    n1->add ( n ) ;
    c1->add ( colour ) ;
    sgSetVec2 ( t,          1                       ,                1                    ) ;
    t1->add ( t ) ;

    sgSetVec3 ( v, center[0]-size[0]/2.0f, center[1]-size[1]/2.0f, center[2]+size[2]/2.0f );
    v1->add ( v );
    sgSetVec3 ( n,          0            ,         -1            ,          0             );
    n1->add ( n );
    c1->add ( colour );
    sgSetVec2 ( t,          0                       ,                2                    );
    t1->add ( t );

    sgSetVec3 ( v, center[0]+size[0]/2.0f, center[1]-size[1]/2.0f, center[2]+size[2]/2.0f );
    v1->add ( v );
    sgSetVec3 ( n,          0            ,         -1            ,          0             );
    n1->add ( n );
    c1->add ( colour );
    sgSetVec2 ( t,          1                       ,                2                    );
    t1->add ( t );

    sgSetVec3 ( v, center[0]-size[0]/2.0f, center[1]+size[1]/2.0f, center[2]+size[2]/2.0f );
    v1->add ( v );
    sgSetVec3 ( n,          0            ,          0            ,          1             );
    n1->add ( n );
    c1->add ( colour );
    sgSetVec2 ( t,          0                       ,                3                    );
    t1->add ( t );

    sgSetVec3 ( v, center[0]+size[0]/2.0f, center[1]+size[1]/2.0f, center[2]+size[2]/2.0f );
    v1->add ( v ) ;
    sgSetVec3 ( n,          0            ,          0            ,          1             );
    n1->add ( n ) ;
    c1->add ( colour ) ;
    sgSetVec2 ( t, 1, 3);
    t1->add ( t );

    vt1 -> setVertices  ( v1 ) ;
    vt1 -> setNormals   ( n1 ) ;
    vt1 -> setColours   ( c1 ) ;
    vt1 -> setTexCoords ( t1 ) ;

    vt1 -> recalcBSphere () ;

    recalcBSphere () ;
}

#define CZ 0.89442719099991f   /* 2/sqrt(5) */
#define SZ 0.44721359549995f   /* 1/sqrt(5) */
#define C1 0.951056516f        /* cos(18) */
#define S1 0.309016994f        /* sin(18) */
#define C2 0.587785252f        /* cos(54) */
#define S2 0.809016994f        /* sin(54) */
#define X1 (C1*CZ)
#define Y1 (S1*CZ)
#define X2 (C2*CZ)
#define Y2 (S2*CZ)

#define P0 {   0,   0,   1 }
#define P1 { -X2, -Y2,  SZ }
#define P2 {  X2, -Y2,  SZ }
#define P3 {  X1,  Y1,  SZ }
#define P4 {   0,  CZ,  SZ }
#define P5 { -X1,  Y1,  SZ }
#define P6 { -X1, -Y1, -SZ }
#define P7 {   0, -CZ, -SZ }
#define P8 {  X1, -Y1, -SZ }
#define P9 {  X2,  Y2, -SZ }
#define PA { -X2,  Y2, -SZ }
#define PB {   0,   0,  -1 }

struct Triangle
{
    sgVec3 v0, v1, v2 ;
};

// XXX really need these (and ssgLocal.h is not accessible):
extern int _ssgLoadObject ( FILE *, ssgBase **, int ) ;
extern int _ssgSaveObject ( FILE *, ssgBase * ) ;

#define load_field(fp, name) (fread(&(name), 1, sizeof(name), fp) == sizeof(name))
#define save_field(fp, name) (fwrite(&(name), 1, sizeof(name), fp) == sizeof(name))

int cGrShape::load ( FILE *fp )
{
    return ( load_field ( fp, corrupted ) &&
             load_field ( fp, colour ) &&
             load_field ( fp, center ) &&
             load_field ( fp, size ) &&
             load_field ( fp, ntriangles ) &&
             _ssgLoadObject ( fp, (ssgBase **) &kidState, ssgTypeState () ) &&
             ssgBranch::load ( fp ) ) ;
}

int cGrShape::save ( FILE *fp )
{
    return ( save_field ( fp, corrupted ) &&
             save_field ( fp, colour ) &&
             save_field ( fp, center ) &&
             save_field ( fp, size ) &&
             save_field ( fp, ntriangles ) &&
             _ssgSaveObject ( fp, kidState ) &&
             ssgBranch::save ( fp ) ) ;
}

int cGrPatch::load ( FILE *fp )
{
    return ( load_field ( fp, levels ) &&
             fread(control_points, 1, 16*9*sizeof(float), fp) == 16*9*sizeof(float) &&
             cGrShape::load ( fp ) ) ;
}

int cGrPatch::save ( FILE *fp )
{
    return ( save_field ( fp, levels ) &&
             fwrite(control_points, 1, 16*9*sizeof(float), fp) == 16*9*sizeof(float) &&
             cGrShape::save ( fp ) ) ;
}
