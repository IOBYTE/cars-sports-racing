/*
PLIB - A Suite of Portable Game Libraries
Copyright (C) 1998,2002  Steve Baker
Copyright (C) 2015 Xavier Bertaux

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA

For further information visit http://plib.sourceforge.net

$Id: dxtctool.h 2173 2012-08-24 13:18:22Z wolfram_kuss $
*/

#ifndef DXTCTOOL_H
#define DXTCTOOL_H 

#include <algorithm>    // std::swap

#include "ssg.h"

//#include <osg/Texture>

#if defined(_MSC_VER)

    typedef __int8  dxtc_int8;
    typedef __int16 dxtc_int16;
    typedef __int32 dxtc_int32;
    typedef __int64 dxtc_int64;

    #define HEX_0x000000000000FFFF 0x000000000000FFFF
    #define HEX_0x000000000FFF0000 0x000000000FFF0000
    #define HEX_0x000000FFF0000000 0x000000FFF0000000
    #define HEX_0x000FFF0000000000 0x000FFF0000000000
    #define HEX_0xFFF0000000000000 0xFFF0000000000000

#else

    typedef char        dxtc_int8;
    typedef short       dxtc_int16;
    typedef int         dxtc_int32;
    typedef long long   dxtc_int64;

    #define HEX_0x000000000000FFFF 0x000000000000FFFFll
    #define HEX_0x000000000FFF0000 0x000000000FFF0000ll
    #define HEX_0x000000FFF0000000 0x000000FFF0000000ll
    #define HEX_0x000FFF0000000000 0x000FFF0000000000ll
    #define HEX_0xFFF0000000000000 0xFFF0000000000000ll

#endif

namespace dxtc_tool {

// C-like function wrappers
bool isDXTC(GLenum pixelFormat);

bool VerticalFlip(size_t Width, size_t Height, GLenum Format, void * pPixels);

bool CompressedImageTranslucent(size_t Width, size_t Height, GLenum Format, void * pPixels);


// Class holding reference to DXTC image pixels 
class dxtc_pixels
{
public:
    inline dxtc_pixels(size_t Width, size_t Height, GLenum Format, void * pPixels);

    // Vertically flip the whole picture
    bool VFlip() const;

protected:

    dxtc_pixels& operator = (const dxtc_pixels&) { return *this; }

    // Limitation check functions
    inline bool DXT1() const;
    inline bool DXT3() const;
    inline bool DXT5() const;
    inline bool OpenGLSize() const;
    inline bool SupportedFormat() const;

    // Vertical flipping functions
    void VFlip_DXT1() const;    
    void VFlip_DXT3() const;
    void VFlip_DXT5() const;

    // Block vertical flipping functions
    inline void BVF_Color_H2(void * const pBlock) const;                            // V. flip one color block with its virtual height == 2
    inline void BVF_Color_H4(void * const pBlock) const;                            // V. flip one color block with its virtual height == 4
    inline void BVF_Color(void * const pBlock1, void * const pBlock2) const;        // V. flip and swap two color blocks, with their virtual height == 4
    inline void BVF_Alpha_DXT3_H2(void * const pBlock) const;                        // V. flip one alpha (DXT3) block with its virtual height == 2
    inline void BVF_Alpha_DXT3_H4(void * const pBlock) const;                        // V. flip one alpha (DXT3) block with its virtual height == 4
    inline void BVF_Alpha_DXT3(void * const pBlock1, void * const pBlock2) const;    // V. flip and swap two alpha (DXT3) blocks, with their virtual height == 4
    inline void BVF_Alpha_DXT5_H2(void * const pBlock) const;                        // V. flip one alpha (DXT5) block with its virtual height == 2
    inline void BVF_Alpha_DXT5_H4(void * const pBlock) const;                        // V. flip one alpha (DXT5) block with its virtual height == 4
    inline void BVF_Alpha_DXT5(void * const pBlock1, void * const pBlock2) const;    // V. flip and swap two alpha (DXT5) blocks, with their virtual height == 4
    
    // Block localization functions
    inline void * GetBlock(size_t i, size_t j, size_t BlockSize) const;

    // mighty const and var
    static const size_t BSIZE_DXT1;
    static const size_t BSIZE_DXT3;
    static const size_t BSIZE_DXT5;
    static const size_t BSIZE_ALPHA_DXT3;
    static const size_t BSIZE_ALPHA_DXT5;

    const size_t    m_Width;
    const size_t    m_Height;
    const GLenum    m_Format;
    void * const    m_pPixels;
};




//////////////////////////////////////////////////////////////////////
// C-Like Function Wrappers
//////////////////////////////////////////////////////////////////////

inline bool isDXTC(GLenum pixelFormat)
{
    switch(pixelFormat)
    {
        case(GL_COMPRESSED_RGB_S3TC_DXT1_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT1_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT3_EXT):
        case(GL_COMPRESSED_RGBA_S3TC_DXT5_EXT):
            return true;
        default:
            return false;
    }
}

inline bool VerticalFlip(size_t Width, size_t Height, GLenum Format, void * pPixels) {
    return (dxtc_pixels(Width, Height, Format, pPixels)).VFlip(); 
}



//////////////////////////////////////////////////////////////////////
// dxtc_pixels Inline Functions
//////////////////////////////////////////////////////////////////////

inline dxtc_pixels::dxtc_pixels(size_t Width, size_t Height, GLenum Format, void * pPixels) :
    m_Width(Width), m_Height(Height), m_Format(Format), m_pPixels(pPixels) { }


inline bool dxtc_pixels::DXT1() const {
    return ((m_Format == GL_COMPRESSED_RGBA_S3TC_DXT1_EXT) || (m_Format == GL_COMPRESSED_RGB_S3TC_DXT1_EXT));
}


inline bool dxtc_pixels::DXT3() const 
{
    return (m_Format == GL_COMPRESSED_RGBA_S3TC_DXT3_EXT);
}


inline bool dxtc_pixels::DXT5() const 
{
    return (m_Format == GL_COMPRESSED_RGBA_S3TC_DXT5_EXT);
}


inline bool dxtc_pixels::SupportedFormat() const 
{
    return (DXT1() || DXT3() || DXT5());
}


inline void dxtc_pixels::BVF_Color_H2(void * const pBlock) const
{
    // Swap the two first row of pixels
    dxtc_int8 * pP = ((dxtc_int8 *) pBlock) + 4;

    std::swap(pP[0], pP[1]);
}


inline void dxtc_pixels::BVF_Color_H4(void * const pBlock) const
{
    // Swap the first row of pixels with the last one, then the 2 middle row of pixels
    dxtc_int8 * pP = ((dxtc_int8 *) pBlock) + 4;

    std::swap(pP[0], pP[3]);
    std::swap(pP[1], pP[2]);
}


inline void dxtc_pixels::BVF_Color(void * const pBlock1, void * const pBlock2) const
{
    // Swap the "2 colors" header (32bits each header)
    dxtc_int32 * pHdr1 = (dxtc_int32 * ) pBlock1;
    dxtc_int32 * pHdr2 = (dxtc_int32 * ) pBlock2;

    std::swap(* pHdr1, * pHdr2);

    // Now swap the pixel values
    dxtc_int8 * pP1 = ((dxtc_int8 * ) pBlock1) + 4;
    dxtc_int8 * pP2 = ((dxtc_int8 * ) pBlock2) + 4;

    std::swap(pP1[0], pP2[3]);
    std::swap(pP1[1], pP2[2]);
    std::swap(pP1[2], pP2[1]);
    std::swap(pP1[3], pP2[0]);
}


inline void dxtc_pixels::BVF_Alpha_DXT3_H2(void * const pBlock) const 
{
    // Swap the two first row of pixels
    dxtc_int16 * pP = (dxtc_int16 * ) pBlock;

    std::swap(pP[0], pP[1]);
}


inline void dxtc_pixels::BVF_Alpha_DXT3_H4(void * const pBlock) const
{
    // Swap the first row of pixels with the last one, then the 2 middle row of pixels
    dxtc_int16 * pP = (dxtc_int16 * ) pBlock;

    std::swap(pP[0], pP[3]);
    std::swap(pP[1], pP[2]);
}


inline void dxtc_pixels::BVF_Alpha_DXT3(void * const pBlock1, void * const pBlock2) const
{
    // Swap all the pixel values
    dxtc_int16 * pP1 = (dxtc_int16 * ) pBlock1;
    dxtc_int16 * pP2 = (dxtc_int16 * ) pBlock2;

    std::swap(pP1[0], pP2[3]);
    std::swap(pP1[1], pP2[2]);
    std::swap(pP1[2], pP2[1]);
    std::swap(pP1[3], pP2[0]);
}


inline void dxtc_pixels::BVF_Alpha_DXT5_H2(void * const pBlock) const
{
    // Swap the two first row of pixels (kinda tricky with DXT5 unaligned encoding)
    dxtc_int32 * pP = (dxtc_int32 * ) (((dxtc_int8 * ) pBlock) + 2);

    dxtc_int32 TmpDWord = (pP[0] & 0xFF000000);
    TmpDWord |=    (pP[0] & 0x00000FFF) << 12;
    TmpDWord |= (pP[0] & 0x00FFF000) >> 12;
    pP[0] = TmpDWord;
}

inline void dxtc_pixels::BVF_Alpha_DXT5_H4(void * const pBlock) const
{
    // Swap the first row of pixels with the last one, then the 2 middle row of pixels (tricky again)
    dxtc_int64 * pB = (dxtc_int64 * ) pBlock;

    dxtc_int64 TmpQWord = (pB[0] & HEX_0x000000000000FFFF);
    TmpQWord |= (pB[0] & HEX_0x000000000FFF0000) << 36;
    TmpQWord |= (pB[0] & HEX_0x000000FFF0000000) << 12;
    TmpQWord |= (pB[0] & HEX_0x000FFF0000000000) >> 12;
    TmpQWord |= (pB[0] & HEX_0xFFF0000000000000) >> 36;
    pB[0] = TmpQWord;
}


inline void dxtc_pixels::BVF_Alpha_DXT5(void * const pBlock1, void * const pBlock2) const 
{
    // Swap all the pixel values (same trick for DXT5)
    dxtc_int64 * pB1 = (dxtc_int64 * ) pBlock1;
    dxtc_int64 * pB2 = (dxtc_int64 * ) pBlock2;

    dxtc_int64 TmpQWord1 = (pB1[0] & HEX_0x000000000000FFFF);
    TmpQWord1 |= (pB1[0] & HEX_0x000000000FFF0000) << 36;
    TmpQWord1 |= (pB1[0] & HEX_0x000000FFF0000000) << 12;
    TmpQWord1 |= (pB1[0] & HEX_0x000FFF0000000000) >> 12;
    TmpQWord1 |= (pB1[0] & HEX_0xFFF0000000000000) >> 36;

    dxtc_int64 TmpQWord2 = (pB2[0] & HEX_0x000000000000FFFF);
    TmpQWord2 |= (pB2[0] & HEX_0x000000000FFF0000) << 36;
    TmpQWord2 |= (pB2[0] & HEX_0x000000FFF0000000) << 12;
    TmpQWord2 |= (pB2[0] & HEX_0x000FFF0000000000) >> 12;
    TmpQWord2 |= (pB2[0] & HEX_0xFFF0000000000000) >> 36;

    pB1[0] = TmpQWord2;
    pB2[0] = TmpQWord1;
}


inline void * dxtc_pixels::GetBlock(size_t i, size_t j, size_t BlockSize) const
{
    const dxtc_int8 * pPixels = (const dxtc_int8 *) m_pPixels;

    return (void *) (pPixels + i * ((m_Width + 3) / 4) * BlockSize + j * BlockSize);
}




} // namespace dxtc_tool

#endif // DXTCTOOL_H
