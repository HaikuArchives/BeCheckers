/******************************************************************************
/
/	File:			BeCheckers.h, Version 1.0.0
/
/	Copyright 2000, Jeremy J. Gibbons
/
*******************************************************************************/

#ifndef _BECHECKERS_H
#define _BECHECKERS_H

#include <Application.h>

#include "BeCheckersWindow.h"

#define APP_NME "BeCheckers"								// Name
#define APP_SIG "application/x-vnd.JGibbons-BeCheckers"		// Signature

#define APP_XTN ".bcsg"										// File extension for saved games
#define APP_SGP "./BeCheckers/SavedGames"					// Subdirectory for saved games

/*----------------------------------------------------------------*/
/*----- BeCheckers class -----------------------------------------*/

class BeCheckers : public BApplication {

public:
					BeCheckers(BRect frame, const char *title);
};

#endif
