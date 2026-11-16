/***************************************************************************

    file                 : OpenGL3.cpp
    created              : Thu Aug 17 23:19:19 CEST 2000
    copyright            : (C) 2012 by Xavier Bertaux
    email                : bertauxx@yahoo.fr
    version              : $Id: OpenGL3.cpp 3741 2011-07-21 22:29:34Z torcs-ng $

 ***************************************************************************/

/***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "OpenGL3.h"
#include "OsgMain.h"

// The OpenGL3: singleton.
OpenGL3* OpenGL3::_pSelf = 0;

int openGfModule(const char* pszShLibName, void* hShLibHandle)
{
    // Instanciate the (only) module instance.
    OpenGL3::_pSelf = new OpenGL3(pszShLibName, hShLibHandle);

    // Register it to the GfModule module manager if OK.
    if (OpenGL3::_pSelf)
        GfModule::register_(OpenGL3::_pSelf);

    // Report about success or error.
    return OpenGL3::_pSelf ? 0 : 1;
}

int closeGfModule()
{
    // Unregister it from the GfModule module manager.
    if (OpenGL3::_pSelf)
        OpenGL3::unregister(OpenGL3::_pSelf);

    // Delete the (only) module instance.
    delete OpenGL3::_pSelf;
    OpenGL3::_pSelf = 0;

    // Report about success or error.
    return 0;
}

OpenGL3& OpenGL3::self()
{
    // Pre-condition : 1 successfull openGfModule call.
    return *_pSelf;
}

OpenGL3::OpenGL3(const std::string& strShLibName, void* hShLibHandle)
    : GfModule(strShLibName, hShLibHandle)
{
    GfLogDebug("OpenGL3::Init\n");
}

OpenGL3::~OpenGL3()
{
}

// Implementation of IGraphicsEngine ****************************************
bool OpenGL3::loadTrack(tTrack* pTrack)
{
    GfLogDebug("OpenGL3::loadTrack\n");
    return osggraph::initTrack(pTrack) == 0;
}

bool OpenGL3::loadCars(tSituation* pSituation)
{
    GfLogDebug("OpenGL3::loadCars\n");
    return osggraph::initCars(pSituation) == 0;
}

bool OpenGL3::setupView(int x, int y, int width, int height, void* pMenuScreen)
{
    GfLogDebug("OpenGL3::setupView\n");
    return osggraph::initView(x, y, width, height, GR_VIEW_STD, pMenuScreen) == 0;
}

void OpenGL3::redrawView(tSituation* pSituation)
{
    osggraph::refresh(pSituation);
}

void OpenGL3::unloadCars()
{
    GfLogDebug("OpenGL3::unloadCars\n");
    osggraph::shutdownCars();
}

void OpenGL3::unloadTrack()
{
    GfLogDebug("OpenGL3::unloadTrack\n");
    osggraph::shutdownTrack();
}

void OpenGL3::shutdownView()
{
    GfLogDebug("OpenGL3::shutdownView\n");
    osggraph::shutdownView();
}

Camera* OpenGL3::getCurCam()
{
    return osggraph::getCamera();
}
