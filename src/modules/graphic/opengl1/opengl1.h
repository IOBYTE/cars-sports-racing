/***************************************************************************

    file        : opengl1.h
    copyright   : (C) 2011 by Jean-Philippe Meuret 
	copyright   : (C) 2023 by Xavier Bertaux                       
    email       : bertauxx@gmail.com 
    version     : $Id: opengl1.h 5095 2013-01-12 17:57:47Z shadow72 $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/
 
/** @file    
    		The "ssggraph" graphics engine module
    @version    $Id: ssggraph.h 5095 2013-01-12 17:57:47Z pouillot $
*/

#ifndef _OPENGL1_H_
#define _OPENGL1_H_

#include <igraphicsengine.h>

#include <tgf.hpp>

class ssgLoaderOptions;

// DLL exported symbols declarator for Windows.
#ifdef WIN32
# ifdef OPENGL1_DLL
#  define OPENGL1_API __declspec(dllexport)
# else
#  define OPENGL1_API __declspec(dllimport)
# endif
#else
# define OPENGL1_API
#endif

// The C interface of the module.
extern "C" int OPENGL1_API openGfModule(const char* pszShLibName, void* hShLibHandle);
extern "C" int OPENGL1_API closeGfModule();

// The module main class
// (Singleton, inherits GfModule, and implements IGraphicsEngine).
class OPENGL1_API OpenGL1 : public GfModule, public IGraphicsEngine
{
 public:
	// Implementation of IGraphicsEngine.
	virtual bool loadTrack(struct Track* pTrack);
	virtual bool loadCars(struct Situation *pSituation);
	virtual bool setupView(int x, int y, int width, int height, void* pMenuScreen);
	virtual void redrawView(struct Situation *pSituation);
	//virtual void bendCar(int index, sgVec3 poc, sgVec3 force, int count = 0);
	virtual void shutdownView();
	virtual void unloadCars();
	virtual void unloadTrack();
	virtual Camera* getCurCam();

	// Accessor to the singleton.
    static OpenGL1& self();

	// Destructor.
    virtual ~OpenGL1();

 protected:
	// Protected constructor to avoid instanciation outside (but friends).
    OpenGL1(const std::string& strShLibName, void* hShLibHandle);
	
	// Make the C interface functions nearly member functions.
	friend int openGfModule(const char* pszShLibName, void* hShLibHandle);
	friend int closeGfModule();

 protected:
	// The singleton.
	static OpenGL1* _pSelf;

	// The default SSGLoaderOptions instance.
	ssgLoaderOptions* _pDefaultSSGLoaderOptions;
};

#endif /* _OPENGL1_H_ */ 
