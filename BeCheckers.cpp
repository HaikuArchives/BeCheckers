/******************************************************************************
/
/	File:			BeCheckers.cpp, Version 1.0.0
/
/	Copyright 2000, Jeremy J. Gibbons
/
*******************************************************************************/

#include "BeCheckers.h"

int main(void) {
	BeCheckers *beCheckers = new BeCheckers(BRect(100, 100, 550, 585), APP_NME);
	beCheckers->Run();
	delete beCheckers;
	return 0;
}

BeCheckers::BeCheckers(BRect frame, const char *title) : BApplication(APP_SIG) {
	BeCheckersWindow *beCheckersWindow = new BeCheckersWindow(frame, title);
	beCheckersWindow->Show();
}