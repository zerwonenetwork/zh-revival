/*
**	Command & Conquer Generals Zero Hour(tm)
**	Copyright 2025 Electronic Arts Inc.
**
**	This program is free software: you can redistribute it and/or modify
**	it under the terms of the GNU General Public License as published by
**	the Free Software Foundation, either version 3 of the License, or
**	(at your option) any later version.
**
**	This program is distributed in the hope that it will be useful,
**	but WITHOUT ANY WARRANTY; without even the implied warranty of
**	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**	GNU General Public License for more details.
**
**	You should have received a copy of the GNU General Public License
**	along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

////////////////////////////////////////////////////////////////////////////////
//																																						//
//  (c) 2001-2003 Electronic Arts Inc.																				//
//																																						//
////////////////////////////////////////////////////////////////////////////////

// GameMain.cpp
// The main entry point for the game
// Author: Michael S. Booth, April 2001

#include "PreRTS.h"	// This must go first in EVERY cpp file in the GameEngine

#include "Common/GameEngine.h"
#include "Common/GlobalData.h"

// P3-01: defined in WinMain.cpp — applies -width/-height overrides to TheGlobalData
// after engine init, so command-line resolution takes priority over Options.ini
extern void ApplyResolutionOverride( void );

/**
 * This is the entry point for the game system.
 */
void GameMain( int argc, char *argv[] )
{
	// initialize the game engine using factory function
	TheGameEngine = CreateGameEngine();
	TheGameEngine->init(argc, argv);

	// P3-01: apply command-line resolution overrides now that TheGlobalData is valid
	ApplyResolutionOverride();

	// run it
	TheGameEngine->execute();

	// since execute() returned, we are exiting the game
	delete TheGameEngine;
	TheGameEngine = NULL;

}

