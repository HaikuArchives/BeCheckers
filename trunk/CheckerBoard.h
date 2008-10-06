/******************************************************************************
/
/	File:			CheckerBoard.h, Version 1.0.0
/
/	Copyright 2000, Jeremy J. Gibbons
/
*******************************************************************************/

#ifndef _CHECKERBOARD_H
#define _CHECKERBOARD_H

#include <Alert.h>
#include <Application.h>
#include <Point.h>
#include <Rect.h>
#include <View.h>

#include "Checker.h"	// Forward declaration for checker message constants

/*----------------------------------------------------------------*/
/*----- CheckerBoard class ---------------------------------------*/

class CheckerBoard : public BView {

public:
					CheckerBoard(BPoint point, char *label = NULL);

		void		Draw(BRect updateRect);

		BPoint		PixalToSquare(const BPoint point);
		BPoint		SquareToPixal(const BPoint square);

		BRect		BoardSize() const { return BRect(0, 0, 450, 450); }
		BRect		SquareSize() const { return BRect(0, 0, 50, 50); }

private:
		void		MessageReceived(BMessage *message);

		char		*label;
};

#endif