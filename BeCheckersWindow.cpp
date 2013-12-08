/******************************************************************************
/
/	File:			BeCheckersWindow.cpp, Version 1.0.0
/
/	Copyright 1999, Jeremy J. Gibbons
/
*******************************************************************************/

#include "BeCheckersWindow.h"

char *BeCheckersWindow::openedFile = NULL;	// Keep a link to the last file opened, used for saving

BeCheckersWindow::BeCheckersWindow(BRect frame, const char *title)
					: BWindow(frame,
							  title,
							  B_TITLED_WINDOW,
							  B_NOT_RESIZABLE | B_NOT_ZOOMABLE) {
	BMenuBar *mb = new BMenuBar(BRect(0, 0, frame.right, 15), "MenuBar");
	BMenu *menu = new BMenu("File");
	menu->AddItem(new BMenuItem("New", new BMessage(BECHECKERS_NEW), 'N'));
	menu->AddItem(new BMenuItem("Open...", new BMessage(BECHECKERS_OPEN), 'O'));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Save", new BMessage(BECHECKERS_SAVE), 'S'));
	menu->AddItem(new BMenuItem("Save As", new BMessage(BECHECKERS_SAVEAS)));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("About BeCheckers...", new BMessage(B_ABOUT_REQUESTED)));
	menu->AddSeparatorItem();
	menu->AddItem(new BMenuItem("Quit", new BMessage(B_QUIT_REQUESTED), 'Q'));
	mb->AddItem(menu);
	AddChild(mb);

	mvIndicationLabel = new BStringView(BRect(0, 472, 450, 485), "Status", "Dark to move");
	mvIndicationLabel->SetViewColor(232, 232, 232);
	mvIndicationLabel->SetAlignment(B_ALIGN_CENTER);
	AddChild(mvIndicationLabel);

	StartGame();
}

void BeCheckersWindow::AboutRequested() {
	(new BAlert("About",
				"BeCheckers 1.0.1\n\nReleased on:\t19 September 2000\nAuthored by:\tJeremy J. Gibbons\n\nSpecial thanks to Seth Flaxman for providing code that allows the checkers to be drawn during a movement.",
				"Neato"))->Go();
}

char * BeCheckersWindow::File(const char *fileName) {
	char *f;

	app_info appInfo;
	be_app->GetAppInfo(&appInfo);

	BEntry entry(&(appInfo.ref));
	BDirectory dir;
	entry.GetParent(&dir);
	BPath p(&dir, APP_SGP);

    if(p.Path() != NULL) {
		f = new char[strlen(p.Path()) + strlen(fileName) + 6] = {'\0'};
		sprintf(f, "%s%s%s%s", p.Path(), "/", fileName, APP_XTN);	// Thanks, Charlie.
	}

	return(p.Path() == NULL ? NULL : f);
}

char * BeCheckersWindow::CreateFileName() {
	time_t now;
	time(&now);

	char *time = asctime(localtime(&now));
	char *fn = new char[strlen(time)] = {'\0'};

	strcat(fn, strtok(time, "\n"));
	return fn;
}

void BeCheckersWindow::OpenGame(const char *file) {
	int x, y, cjx, cjy, prevDist;
	int color, rank, turn, jumpAvailable;

	ifstream inGame(file, ios::in);
	if(!inGame)
		(new BAlert("LoadError",
					"Error while loading BeCheckers game.",
					"Okay", NULL, NULL,
					B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
	else {
		cb->RemoveSelf();
		delete cb;
		cb = new CheckerBoard(BPoint(0, 20), APP_NME);
		AddChild(cb);

		inGame >> turn >> prevDist >> jumpAvailable >> cjx >> cjy;
		while(inGame >> x >> y >> color >> rank)
			cb->AddChild(new Checker((bool)color, x, y, (bool)rank));

		if(BPoint(cjx, cjy) != B_ORIGIN)
			Checker::SetCheckerJumping(((Checker *)cb->ChildAt(0))->CheckerAt(BPoint(cjx, cjy)));

		Checker::SetTurn((bool)turn);
		Checker::SetPrevDist(prevDist);
		Checker::SetJumpAvailable((bool)jumpAvailable);
		mvIndicationLabel->SetText(turn == DARK ? "Dark to move" : "Light to move");
	}
}

void BeCheckersWindow::SaveGame(const char *file) {
	ofstream outGame(file, ios::out);
	if(!outGame)
		(new BAlert("SaveError",
					"Error while saving BeCheckers game.",
					"Okay", NULL, NULL,
					B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
	else {
		Checker *c = (Checker *)cb->ChildAt(0);
		outGame << Checker::Turn()			<< ' '
				<< Checker::PrevDist()		<< ' '
				<< Checker::JumpAvailable() << ' '
				<< Checker::CheckerJumping().x << ' '
				<< Checker::CheckerJumping().y << endl;
		while(c) {
			outGame << c->Square().x << ' ' << c->Square().y << ' '
					<< c->Color()	 << ' ' << c->Rank()	 << endl;
			c = (Checker *)c->NextSibling();
		}
	}
}

void BeCheckersWindow::SavedGames(BListView *list) {
	char name[B_FILE_NAME_LENGTH];
	app_info appInfo;

	be_app->GetAppInfo(&appInfo);
	BEntry entry(&(appInfo.ref));
	BDirectory dir;
	entry.GetParent(&dir);
	BPath p(&dir, APP_SGP);

	dir.SetTo(p.Path());
	dir.Rewind();

	while (dir.GetNextEntry(&entry) == B_OK) {
		entry.GetName(name);
		strtok(name, ".");	// Strip the filename extension
		list->AddItem(new BStringItem(name));
	}
}

void BeCheckersWindow::StartGame() {
	if(CountChildren() > 2) {	// Remove the checkerboard if one already exists
		cb->RemoveSelf();
		delete cb;
		Checker::Restart();				// Reset checker class internal state to start
		mvIndicationLabel->SetText("Dark to move");	// Reset move indication label
	}

	cb = new CheckerBoard(BPoint(0, 20), APP_NME);
	AddChild(cb);

	for(int x = 1; x < 9; x++)
		if(x%2 == 0) {
			cb->AddChild(new Checker(LIGHT, x, 1));
			cb->AddChild(new Checker(LIGHT, x, 3));
			cb->AddChild(new Checker(DARK,  x, 7));
		}
		else {
			cb->AddChild(new Checker(LIGHT, x, 2));
			cb->AddChild(new Checker(DARK,  x, 6));
			cb->AddChild(new Checker(DARK,  x, 8));
		}
}

void BeCheckersWindow::MessageReceived(BMessage *message) {
	switch(message->what) {
		case B_ABOUT_REQUESTED:
			AboutRequested(); break;
		case BECHECKERS_NEW:
			if(openedFile != NULL) openedFile = NULL;	// I think there is a small mem leak here??
			StartGame();
			break;
		case BECHECKERS_OPEN: {
			// This is a cludge, because I was too lazy to create another class, but it works ;)
			// Create the open saved game window here

			BRect frame(Frame());
			// The bounds are (0,0,350,125), I hate magic numbers too but oh well.  BeCheckers is full of em!
			openWindow = new BWindow(BRect(frame.left+48, frame.top+40, frame.left+398, frame.top+165),
							 "Saved Games",
							 B_FLOATING_WINDOW_LOOK,B_MODAL_APP_WINDOW_FEEL,
							 B_NOT_RESIZABLE | B_NOT_ZOOMABLE);

			BView *v = new BView(BRect(0, 0, 350, 125), "v", B_NOT_RESIZABLE, B_WILL_DRAW);
			v->SetViewColor(216, 216, 216);

			BButton *b = new BButton(BRect(10, 16, 70, 41), "Open", "Open", new BMessage(BECHECKERS_OPEN_OPEN));
			b->MakeDefault(true);
			b->SetTarget(this);
			v->AddChild(b);

			BButton *b1 = new BButton(BRect(10, 51, 70, 76), "Delete", "Delete", new BMessage(BECHECKERS_OPEN_DELETE));
			b1->SetTarget(this);
			v->AddChild(b1);

			BButton *b2 = new BButton(BRect(10, 86, 70, 111), "Close", "Close", new BMessage(BECHECKERS_OPEN_CLOSE));
			b2->SetTarget(this);
			v->AddChild(b2);

			savedGameList = new BListView(BRect(95, 15, 320, 108), "City");
			SavedGames(savedGameList);	// Get the saved games from disk

			BScrollView *sv = new BScrollView("ScrollView", savedGameList,B_FOLLOW_LEFT|B_FOLLOW_TOP, 0, false, true, B_FANCY_BORDER);
			v->AddChild(sv);

			openWindow->AddChild(v);
			openWindow->Show();
		} break;
		case BECHECKERS_OPEN_OPEN: {
			int index = savedGameList->CurrentSelection();
			if(index >= 0) {
				// Set seleted file as openedFile and open it
				openedFile = File(((BStringItem *) savedGameList->ItemAt(index))->Text());
				OpenGame(openedFile);
				openWindow->Lock();
				openWindow->Quit();
			}
			else (new BAlert("NoFile",
							 "A saved game must be selected before opening.",
							 "Okay"))->Go();
		} break;
		case BECHECKERS_OPEN_CLOSE:
			openWindow->Lock();
			openWindow->Quit();
			break;
		case BECHECKERS_OPEN_DELETE: {
			int index = savedGameList->CurrentSelection();
			if(index >= 0) {
				// If the file being deleted was the last file opened then set openedFile to (null)
				if(openedFile != NULL && strcmp(File(((BStringItem *) savedGameList->ItemAt(index))->Text()), openedFile) == 0)
					openedFile = NULL;
				// Delete saved game from harddisk
				BEntry entry(File(((BStringItem *) savedGameList->ItemAt(index))->Text()));
				entry.Remove();
				// Delete the filename from the BListView
				openWindow->Lock();
				savedGameList->RemoveItem(index);
				openWindow->Unlock();
			}
			else (new BAlert("NoFile",
							 "A saved game must be selected before deleting.",
							 "Okay"))->Go();
		} break;
		case BECHECKERS_SAVE:
			if(openedFile != NULL) SaveGame(openedFile);
			else this->PostMessage(BECHECKERS_SAVEAS);
			break;
		case BECHECKERS_SAVEAS:
			// Set the new file as the last openedFile and save to harddisk
			openedFile = File(CreateFileName());
			SaveGame(openedFile);
			break;
		case CHECKER_WIN:
			bool color;
			message->FindBool("Color", &color);
			mvIndicationLabel->SetText(color == DARK ? "Dark won" : "Light won");
			if((new BAlert("Winner",
						   color == DARK ? "The dark colored checkers won the game!" :
						   				   "The light colored checkers won the game!",
						   "Quit", "Play again"))->Go() == 0)
				 be_app->PostMessage(B_QUIT_REQUESTED);
			else this->PostMessage(BECHECKERS_NEW);
			break;
		case CHECKER_TURN:
			bool turn;
			message->FindBool("Turn", &turn);
			mvIndicationLabel->SetText(turn == DARK ? "Dark to move" : "Light to move");
			break;
		default:
			BWindow::MessageReceived(message); break;
	}
}

bool BeCheckersWindow::QuitRequested() {
	be_app->PostMessage(B_QUIT_REQUESTED);
	return true;
}
