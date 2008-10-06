/******************************************************************************
/
/	File:			BeCheckersWindow.h, Version 1.0.0
/
/	Copyright 2000, Jeremy J. Gibbons
/
*******************************************************************************/

#ifndef _BECHECKERSWINDOW_H
#define _BECHECKERSWINDOW_H

#include <fstream.h>
#include <string.h>
#include <stdio.h>
#include <time.h>

#include <Alert.h>
#include <Button.h>
#include <Directory.h>
#include <Entry.h>
#include <ListItem.h>
#include <ListView.h>
#include <MenuBar.h>
#include <MenuItem.h>
#include <Message.h>
#include <Path.h>
#include <Rect.h>
#include <Roster.h>
#include <ScrollView.h>
#include <StringView.h>

#include "BeCheckers.h"
#include "Checker.h"
#include "CheckerBoard.h"

/* Menuitem message constants */

const uint32 BECHECKERS_NEW    = 'NewG';
const uint32 BECHECKERS_OPEN   = 'Open';
const uint32 BECHECKERS_SAVE   = 'Save';
const uint32 BECHECKERS_SAVEAS = 'SvAs';

/* Openwindow button message constants */

const uint32 BECHECKERS_OPEN_OPEN   = 'OpnB';
const uint32 BECHECKERS_OPEN_CLOSE  = 'ClsB';
const uint32 BECHECKERS_OPEN_DELETE = 'DelB';

/*----------------------------------------------------------------*/
/*----- BeCheckersWindow class -----------------------------------*/

class BeCheckersWindow : public BWindow {

public:
					BeCheckersWindow(BRect frame, const char *title);

private:
		void			AboutRequested();
		char *			File(const char *fileName);
		char *			CreateFileName();
		void			OpenGame(const char *file);
		void			SaveGame(const char *file);
		void			SavedGames(BListView *list);
		void			StartGame();
		void			MessageReceived(BMessage *message);
		bool			QuitRequested();

		BListView		*savedGameList;
		BStringView		*mvIndicationLabel;
		BWindow			*openWindow;

		CheckerBoard	*cb;

static	char			*openedFile;
};

#endif