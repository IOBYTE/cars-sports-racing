/****************************************************************************

    file                 : sgisect.cpp
    created              : Mon Jan 15 22:54:56 CET 2024
    copyright            : (C) 1998,2002  Steve Baker - PLIB
    copyright			 : (C) 2024 Xavier BERTAUX
    email                : bertauxx@yahoo.fr
    version              : $Id: sgisect.h 4811 2012-07-29 17:16:37Z shadow72 $

 ****************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "sg.h"

int sgIsectPlanePlane ( sgVec3 point, sgVec3 dir,
                        sgVec4 plane1, sgVec4 plane2 )
{
    /* Determine intersection line direction. */

    sgVectorProductVec3 ( dir, plane1, plane2 ) ;

    SGfloat dnorm = sgLengthVec3 ( dir ) ;

    /* If planes are parallel then fail */

    if ( dnorm < FLT_EPSILON )
    {
        sgZeroVec3 ( point ) ;
        sgZeroVec3 ( dir   ) ;
        return FALSE ;
    }

    /* Determine intersection point with the best suited coordinate plane. */

    SGfloat abs ;
    SGfloat maxabs = sgAbs(dir[0]);
    int index = 0;

    if ((abs = sgAbs(dir[1])) > maxabs) { maxabs = abs ; index = 1; }
    if ((abs = sgAbs(dir[2])) > maxabs) { maxabs = abs ; index = 2; }

    switch ( index )
    {
    case 0:
        sgSetVec3( point,
                   SG_ZERO,
                   (plane1[2] * plane2[3] - plane2[2] * plane1[3]) / dir[0],
                (plane2[1] * plane1[3] - plane1[1] * plane2[3]) / dir[0] );
        break;
    case 1:
        sgSetVec3( point,
                   (plane2[2] * plane1[3] - plane1[2] * plane2[3]) / dir[1],
                SG_ZERO,
                (plane1[0] * plane2[3] - plane2[0] * plane1[3]) / dir[1] );
        break;
    case 2:
        sgSetVec3( point,
                   (plane1[1] * plane2[3] - plane2[1] * plane1[3]) / dir[2],
                (plane2[0] * plane1[3] - plane1[0] * plane2[3]) / dir[2],
                SG_ZERO );
        break;

    default: return FALSE ;  /* Impossible */
    }

    /* Normalize the direction */
    sgScaleVec3( dir, SG_ONE / dnorm );
    return TRUE;
}

int sgIsectInfLinePlane( sgVec3 dst, sgVec3 l_org, sgVec3 l_vec, sgVec4 plane )
{
    SGfloat tmp = sgScalarProductVec3 ( l_vec, plane ) ;

    /* Is line parallel to plane? */

    if ( sgAbs ( tmp ) < FLT_EPSILON )
        return FALSE ;

    sgScaleVec3 ( dst, l_vec, -( sgScalarProductVec3 ( l_org, plane )
                                 + plane[3] ) / tmp ) ;
    sgAddVec3  ( dst, l_org ) ;

    return TRUE ;
}

int sgIsectInfLineInfLine( sgVec3 dst,
                           sgVec3 l1_org, sgVec3 l1_vec,
                           sgVec3 l2_org, sgVec3 l2_vec )
{
    sgVec3 vec_l1, vec_l2 ;

    sgNormalizeVec3 ( vec_l1, l1_vec ) ;
    sgNormalizeVec3 ( vec_l2, l2_vec ) ;

    sgVec3 perp ;
    sgVectorProductVec3 ( perp, vec_l1, vec_l2 ) ;
    SGfloat dist = sgScalarProductVec3 ( perp, perp ) ;

    if ( dist < FLT_EPSILON )
    {
        sgCopyVec3 ( dst, l2_org );

        return TRUE ;
    }

    sgScaleVec3 ( perp, SG_ONE / sgSqrt( dist ) ) ;

    sgVec4 pa ;
    sgVec4 pb ;
    sgVec3 tmp ;

    sgVectorProductVec3 ( tmp, perp, vec_l1 ) ;
    sgNormalizeVec3     ( tmp );
    sgMakePlane         ( pa, tmp, l1_org ) ;

    sgVectorProductVec3 ( tmp, perp, vec_l2 ) ;
    sgNormalizeVec3     ( tmp );
    sgMakePlane         ( pb, tmp, l2_org ) ;

    sgVec3 tmp_org, tmp_vec ;

    if ( ! sgIsectPlanePlane( tmp_org, tmp_vec, pa, pb ) )
    {
        sgCopyVec3 ( dst, l2_org );

        return FALSE ;
    }

    if ( ! sgIsectInfLinePlane ( dst, l2_org, vec_l2, pa ) )
    {
        sgCopyVec3 ( dst, l2_org );

        return FALSE ;
    }

    return TRUE ;
}

SGfloat sgIsectLinesegPlane ( sgVec3 dst, sgVec3 v1, sgVec3 v2, sgVec4 plane )
{
    sgVec3 delta ;

    sgSubVec3 ( delta, v2, v1 ) ;

    SGfloat p = sgScalarProductVec3 ( plane, delta ) ;

    if ( p == SG_ZERO )
    {
        dst [ 0 ] = dst [ 1 ] = dst [ 2 ] = FLT_MAX ;
        return FLT_MAX ;

    }

    float s = - ( sgScalarProductVec3 ( plane, v1 ) + plane[3] ) / p ;

    sgScaleVec3 ( dst, delta, s ) ;
    sgAddVec3   ( dst, dst, v1 ) ;

    return s ;
}

// return the sign of a value
static inline int SG_SIGN( const SGfloat x )
{
    return x < 0 ? -1 : 1;
}


// return the minimum of two values

static inline SGfloat SG_MIN2( const SGfloat a, const SGfloat b )
{
    return a < b ? a : b ;
}


// return the minimum of three values

static inline SGfloat SG_MIN3 ( const SGfloat a,
                                const SGfloat b, const SGfloat c )
{
    return ( a < b ? SG_MIN2(a, c) : SG_MIN2(b, c) );
}

// return the minimum and maximum of three values
static void SG_MIN_MAX3 ( SGfloat &min, SGfloat &max, const SGfloat &a,
                          const SGfloat &b, const SGfloat &c )
{
    if( a > b )
    {
        if( a > c )
        {
            max = a;
            min = SG_MIN2( b, c );
        }
        else
        {
            max = c;
            min = SG_MIN2( a, b );
        }
    }
    else
    {
        if( b > c )
        {
            max = b;
            min = SG_MIN2( a, c );
        }
        else
        {
            max = c;
            min = SG_MIN2( a, b );
        }
    }
}

/*
 * Given a point and a triangle lying on the same plane
 * check to see if the point is inside the triangle
 */

bool sgPointInTriangle3 ( sgVec3 point, sgVec3 tri[3] )
{
    sgVec3 dif ;

    /* Early-out if point is not in bounding cube of triangle */

    for( int i = 0 ; i < 3 ; i++ )
    {
        SGfloat min, max ;

        SG_MIN_MAX3 ( min, max, tri[0][i], tri[1][i], tri[2][i] ) ;

        if( point[i] < min || point[i] > max )
            return false ;

        dif[i] = max - min ;  /* Calc size of bounding cube */
    }

    /* Drop the smallest dimension so we only have to work in 2d. */

    sgVec2 p, a, b, c ;

    if ( dif[0] <= dif[1] && dif[0] <= dif[2] )
    {
        p[0] = point[1]  ; p[1] = point[2]  ;
        a[0] = tri[0][1] ; a[1] = tri[0][2] ;
        b[0] = tri[1][1] ; b[1] = tri[1][2] ;
        c[0] = tri[2][1] ; c[1] = tri[2][2] ;
    }
    else
        if ( dif[1] <= dif[0] && dif[1] <= dif[2] )
        {
            p[0] = point[0]  ; p[1] = point[2]  ;
            a[0] = tri[0][0] ; a[1] = tri[0][2] ;
            b[0] = tri[1][0] ; b[1] = tri[1][2] ;
            c[0] = tri[2][0] ; c[1] = tri[2][2] ;
        }
        else
        {
            p[0] = point[0]  ; p[1] = point[1]  ;
            a[0] = tri[0][0] ; a[1] = tri[0][1] ;
            b[0] = tri[1][0] ; b[1] = tri[1][1] ;
            c[0] = tri[2][0] ; c[1] = tri[2][1] ;
        }

    /*
    Check if the intersection point is on the same side of
    each edge of the triangle as the remaining point.
  */

    sgVec3 le ;  /* Line Equation */

    sgMake2DLine ( le, a, b ) ;

    if ( SG_SIGN ( le[0]*c[0] + le[1]*c[1] + le[2] ) !=
         SG_SIGN ( le[0]*p[0] + le[1]*p[1] + le[2] ) ) return false ;

    sgMake2DLine ( le, b, c ) ;

    if ( SG_SIGN ( le[0]*a[0] + le[1]*a[1] + le[2] ) !=
         SG_SIGN ( le[0]*p[0] + le[1]*p[1] + le[2] ) ) return false ;

    sgMake2DLine ( le, c, a ) ;

    if ( SG_SIGN ( le[0]*b[0] + le[1]*b[1] + le[2] ) !=
         SG_SIGN ( le[0]*p[0] + le[1]*p[1] + le[2] ) ) return false ;

    return true ;
}



bool sgPointInTriangle2 ( sgVec2 point, sgVec2 tri[3] )
{
    SGfloat min, max ;

    /* Early-out if outside bounding box */

    SG_MIN_MAX3 ( min, max, tri[0][0], tri[1][0], tri[2][0] );
    if ( point[0] < min || point[0] > max ) return false ;

    SG_MIN_MAX3 ( min, max, tri[0][1], tri[1][1], tri[2][1] );
    if ( point[1] < min || point[1] > max ) return false ;

    sgVec2 a, b, c ;

    sgCopyVec2 ( a, tri[0] ) ;
    sgCopyVec2 ( b, tri[1] ) ;
    sgCopyVec2 ( c, tri[2] ) ;

    /*
    Check if the intersection point is on the same side of
    each edge of the triangle as the remaining point.
  */

    sgVec3 le ;  /* Line Equation */

    sgMake2DLine ( le, a, b ) ;

    if ( SG_SIGN ( le[0]*c[0]     + le[1]*c[1]     + le[2] ) !=
         SG_SIGN ( le[0]*point[0] + le[1]*point[1] + le[2] ) ) return false ;

    sgMake2DLine ( le, b, c ) ;

    if ( SG_SIGN ( le[0]*a[0]     + le[1]*a[1]     + le[2] ) !=
         SG_SIGN ( le[0]*point[0] + le[1]*point[1] + le[2] ) ) return false ;

    sgMake2DLine ( le, c, a ) ;

    if ( SG_SIGN ( le[0]*b[0]     + le[1]*b[1]     + le[2] ) !=
         SG_SIGN ( le[0]*point[0] + le[1]*point[1] + le[2] ) ) return false ;

    return true ;
}


