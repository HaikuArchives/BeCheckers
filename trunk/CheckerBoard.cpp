/******************************************************************************
/
/	File:			CheckerBoard.cpp, Version 1.0.0
/
/	Copyright 1999, Jeremy J. Gibbons
/
*******************************************************************************/

#include "CheckerBoard.h"

CheckerBoard::CheckerBoard(BPoint point, char *label)
				: BView(BRect(point, point + BPoint(450, 450)),
						"CheckerBoardView",
						B_NOT_RESIZABLE,
						B_WILL_DRAW) {
	this->label = label;
	SetViewColor(255, 255, 230);
}

void CheckerBoard::Draw(BRect updateRect) {
	// s = shift pixals
	// x = from the right..left
	// y = from the top..down
	
	// Draw the green squares
	for(int s = 25; s <= 75; s += 50)
		for(int x = 400 - s; x >= 25; x -= 100)
			for(int y = s; y <= 375; y += 100) {
				SetHighColor(0, 100, 50);
				FillRect(BRect(x, y, x + 50, y + 50));
				SetHighColor(0, 0, 0);
				StrokeRect(BRect(x, y, x + 50, y + 50));
			}

	StrokeRect(BRect(0, 0, 450, 450));
    SetPenSize(2);
    StrokeRect(BRect(24, 24, 425, 425));

	if(label != NULL) DrawString(label, BPoint(450 / 2 - StringWidth(label) / 2, 442));
}

void CheckerBoard::MessageReceived(BMessage *message) {
	switch(message->what) {
		case CHECKER:
			if(message->IsSourceRemote()) {
				(new BAlert("Warning",
							"Checkers must belong to the local checkerboard.",
							"Okay", NULL, NULL,
							B_WIDTH_AS_USUAL, B_WARNING_ALERT))->Go();
			}
			else if(message->WasDropped()) {
				BPoint p = PixalToSquare(ConvertFromScreen(message->DropPoint()));
				Checker *c;
				message->FindPointer("CheckerPtr", (void **)(&c));
				// if it's a valid move, then update the checker
				if(c->IsMovementPlayable(p)) c->UpdateChecker(p);
				else c->DontUpdate();
			}
			break;
		default:
			BView::MessageReceived(message); break;
	}
}

BPoint CheckerBoard::PixalToSquare(const BPoint point) {
	return BPoint((int)(point.x + 25) / 50, (int)(point.y + 25) / 50);
}

BPoint CheckerBoard::SquareToPixal(const BPoint square) {
	return BPoint(square.x * 50, square.y * 50);
}