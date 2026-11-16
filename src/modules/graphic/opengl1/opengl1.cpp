/***************************************************************************

    file                 : opengl1.cpp
    created              : Thu Aug 17 23:19:19 CEST 2000
    copyright            : (C) 2000 by Eric Espie
    email                : torcs@free.fr
    version              : $Id: opengl1.cpp 6097 2015-08-30 23:12:09Z shadow72 $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/



#include "opengl1.h"

#include "grmain.h"
#include "grtexture.h"


// The SsgGraph: singleton.
OpenGL1* OpenGL1::_pSelf = 0;

int openGfModule(const char* pszShLibName, void* hShLibHandle)
{
    // Instanciate the (only) module instance.
    OpenGL1::_pSelf = new OpenGL1(pszShLibName, hShLibHandle);

    // Register it to the GfModule module manager if OK.
    if (OpenGL1::_pSelf)
        GfModule::register_(OpenGL1::_pSelf);

    // Report about success or error.
    return OpenGL1::_pSelf ? 0 : 1;
}

int closeGfModule()
{
    // Unregister it from the GfModule module manager.
    if (OpenGL1::_pSelf)
        OpenGL1::unregister(OpenGL1::_pSelf);

    // Delete the (only) module instance.
    delete OpenGL1::_pSelf;
    OpenGL1::_pSelf = 0;

    // Report about success or error.
    return 0;
}

OpenGL1& OpenGL1::self()
{
    // Pre-condition : 1 successfull openGfModule call.
    return *_pSelf;
}

OpenGL1::OpenGL1(const std::string& strShLibName, void* hShLibHandle)
: GfModule(strShLibName, hShLibHandle)
{
    // Override the default SSG loader options object with our's
    // (workaround try for ssggraph crash at re-load time).
    _pDefaultSSGLoaderOptions = new ssgLoaderOptions;
    ssgSetCurrentOptions(_pDefaultSSGLoaderOptions);

    // Initialize the PLib SSG layer.
    ssgInit();

    //Setup image loaders
    grRegisterCustomSGILoader();
}

OpenGL1::~OpenGL1()
{
    // Terminate the PLib SSG layer.
    delete _pDefaultSSGLoaderOptions;
}

// Implementation of IGraphicsEngine ****************************************

bool OpenGL1::loadTrack(tTrack* pTrack)
{
    //GfLogDebug("SsgGraph::loadTrack\n");
    return ::initTrack(pTrack) == 0;
}

bool OpenGL1::loadCars(tSituation* pSituation)
{
    //GfLogDebug("SsgGraph::loadCars\n");
    return ::initCars(pSituation) == 0;
}

bool OpenGL1::setupView(int x, int y, int width, int height, void* pMenuScreen)
{
    //GfLogDebug("SsgGraph::setupView\n");
    return ::initView(x, y, width, height, GR_VIEW_STD, pMenuScreen) == 0;
}

void OpenGL1::redrawView(tSituation* pSituation)
{
    ::refresh(pSituation);
}

// void SsgGraph::bendCar(int index, sgVec3 poc, sgVec3 force, int count)
// {
// 	::bendCar(index, poc, force, count);
// }

void OpenGL1::unloadCars()
{
    //GfLogDebug("SsgGraph::unloadCars\n");
    ::shutdownCars();
}

void OpenGL1::unloadTrack()
{
    //GfLogDebug("SsgGraph::unloadTrack\n");
    ::shutdownTrack();
}

void OpenGL1::shutdownView()
{
    //GfLogDebug("SsgGraph::shutdownView\n");
    ::shutdownView();
}


Camera* OpenGL1::getCurCam()
{
    Camera *cam = new Camera;
    cGrCamera *gcam = grGetCurCamera();

    cam->Centerv = gcam->getCenterv();
    cam->Upv = gcam->getUpv();
    cam->Speedv = gcam->getSpeedv();
    cam->Posv = gcam->getPosv();
  
    return cam;
}
