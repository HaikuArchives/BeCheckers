/******************************************************************************
/
/	File:			Checker.cpp, Version 1.0.0
/
/	Copyright 1999, Jeremy J. Gibbons
/
*******************************************************************************/

#include "Checker.h"

bool Checker::turn = DARK;
bool Checker::jumpAvailable = false;

bool Checker::drawsq = false;

int Checker::dCount = 0;
int Checker::lCount = 0;
int Checker::prevDist = 0;

Checker *Checker::checkerJumping = NULL;

Checker::Checker(bool color, int col, int row, bool rank)
			: BView(BRect(CheckerSize()),
					"CheckerView",
					B_NOT_RESIZABLE,
					B_WILL_DRAW) {
	if(color == DARK) ++dCount;
	else			  ++lCount;

	this->rank = rank;
	this->color = color;
	this->square = BPoint(col, row);

	SetViewColor(50, 100, 0);
	BPoint pos = ((CheckerBoard *)Parent())->SquareToPixal(square);
	MoveTo(pos.x - 21, pos.y - 21);
}

Checker::~Checker() {
	if(color == DARK) --dCount;
	else			  --lCount;
}

void Checker::Restart() {
	dCount = lCount = prevDist = 0;
	jumpAvailable = false;
	turn = DARK;
	drawsq=false;
}

void Checker::Draw(BRect updateRect) {

if(!drawsq){

	if(color == DARK) SetHighColor(255,   0,   0);
	else			  SetHighColor(250, 250, 250);

	FillEllipse(Bounds());
	SetHighColor(0, 0, 0);
	StrokeEllipse(Bounds());

	if(rank == KING) {
		BPoint crown[] = {BPoint(17, 26), BPoint(25, 26), BPoint(29, 18),
						  BPoint(24, 21), BPoint(21, 16), BPoint(18, 21), BPoint(13, 18)};

		SetHighColor(255, 200, 0);
		FillPolygon(crown, 7);
		FillEllipse(BPoint(31, 16), 1, 1);
		FillEllipse(BPoint(11, 16), 1, 1);
		FillEllipse(BPoint(21, 14), 1, 1);

		SetHighColor(0, 0, 0);
		StrokePolygon(crown, 7);
		StrokeEllipse(BPoint(31, 16), 1, 1);
		StrokeEllipse(BPoint(11, 16), 1, 1);
		StrokeEllipse(BPoint(21, 14), 1, 1);
	}

}else {
SetHighColor(0,100,50);
FillRect(Bounds());
}
}

bool Checker::CanMove(const Checker *c, const int spaces) {
	bool movable = false;

	if(c->color == DARK || c->rank == KING)
		movable = IsMovementPlayable(c, c->square + BPoint(-spaces, -spaces)) |
				  IsMovementPlayable(c, c->square + BPoint( spaces, -spaces));
	if(movable == false && (c->color == LIGHT || c->rank == KING))
		movable = IsMovementPlayable(c, c->square + BPoint(-spaces, spaces)) |
				  IsMovementPlayable(c, c->square + BPoint( spaces, spaces));

	return movable;
}

Checker * Checker::CheckerAt(const BPoint square) {
	Checker *c = (Checker *)((CheckerBoard *)Parent())->ChildAt(0);

	while(c && c->square != square)
		c = (Checker *)c->NextSibling();
	return c;
}

bool Checker::Contains(const BPoint point) {
	BPoint hcs = BPoint((CheckerSize()).right / 2, (CheckerSize()).bottom / 2);
	return ((hcs.x - point.x) * (hcs.x - point.x) + (hcs.y - point.y) * (hcs.y - point.y)) <= hcs.x * hcs.x;
}

bool Checker::IsMovementPlayable(const BPoint square) {
	return IsMovementPlayable(this, square);
}

bool Checker::IsMovementPlayable(const Checker *c, const BPoint newSquare) {
	BPoint deltaXY = newSquare - c->square,
		   midSquare = BPoint(c->square.x + deltaXY.x / 2, c->square.y + deltaXY.y / 2);
	int dist = (int)sqrt(pow(deltaXY.y, 2) + pow(deltaXY.x, 2));

	if(!IsSquarePlayable(newSquare) || CheckerAt(newSquare) != NULL || c->square == newSquare ||
	   deltaXY.x == 0 || deltaXY.y == 0 || (dist != 1 && dist != 2) || (dist == 1 && prevDist == 2) ||
	   (prevDist == 2 && checkerJumping != this) || (dist == 1 && jumpAvailable == true)) return false;

	if(dist == 2 && (CheckerAt(midSquare) == NULL ||
					 c->color == (CheckerAt(midSquare))->color)) return false;

	if(c->rank == KING) return true;
	else if(c->rank != KING && c->color == DARK  && deltaXY.y < 0) return true;
	else if(c->rank != KING && c->color == LIGHT && deltaXY.y > 0) return true;
	return false;
}

bool Checker::CanJump() {
	bool movable = false;
	Checker *c = (Checker *)((CheckerBoard *)Parent())->ChildAt(0);

	while(c && !movable) {
		if(c->color == turn) movable = CanMove(c, 2);
		c = (Checker *)c->NextSibling();
	}
	return movable;
}

bool Checker::IsSquarePlayable(const BPoint square) {
	return !((	   square.x    < 1 ||	   square.y    < 1) ||
			 (	   square.x    > 8 ||	   square.y    > 8) ||
			 ((int)square.x%2 == 0 && (int)square.y%2 == 0) ||
			 ((int)square.x%2 != 0 && (int)square.y%2 != 0));
}

bool Checker::Stalemate() {
	bool movable = false;
	Checker *c = (Checker *)((CheckerBoard *)Parent())->ChildAt(0);

	while(c && !movable) {
		if(c->color == turn) movable = CanMove(c, 1) | CanMove(c, 2);
		c = (Checker *)c->NextSibling();
	}
	return !movable;
}

void Checker::UpdateChecker(const BPoint newSquare) {
	bool changeTurn = false;
	BPoint deltaXY = newSquare - square,
		   midSquare = BPoint(square.x + deltaXY.x / 2, square.y + deltaXY.y / 2);
	int dist = (int)sqrt(pow(deltaXY.y, 2) + pow(deltaXY.x, 2));

	MoveTo(((CheckerBoard *)Parent())->SquareToPixal(newSquare) - BPoint(21, 21));
	square = newSquare;

	if(dist == 2) {
		Checker *c = CheckerAt(midSquare);
		c->RemoveSelf();
		delete c;

		prevDist = dist;
		checkerJumping = this;
	}

	if(rank == MAN && ((newSquare.y == 1 && color == DARK) ||
					   (newSquare.y == 8 && color == LIGHT))) {
			rank = KING;
			changeTurn = true;
	}

	if(dist == 1 || (dist == 2 && !CanMove(this, 2)) || changeTurn) {
		prevDist = 0;
		jumpAvailable = false;
		checkerJumping = NULL;
		turn = !turn;

		if(dCount == 0 || lCount == 0 || Stalemate()) {
			BMessage message(CHECKER_WIN);
			message.AddBool("Color", !turn);
			Window()->PostMessage(&message);
		}
		else {
			BMessage message(CHECKER_TURN);
			message.AddBool("Turn", turn == DARK ? DARK : LIGHT);
			Window()->PostMessage(&message);
		}

		jumpAvailable = CanJump();
	}
	drawsq=false;
}

void Checker::DontUpdate() {
  drawsq=false;
  Draw(Bounds());
}

#include <Bitmap.h>

void Checker::MouseDown(BPoint point) {
	if(turn == color && Contains(point)) {
	
	drawsq = true;
	Draw(Bounds());
	
	BBitmap *bitmap = new BBitmap(BRect(0,0,42,42), B_CMAP8, true);
	BView *view = new BView(BRect(0,0,42,42), "view", B_FOLLOW_NONE, B_WILL_DRAW);
	bitmap->AddChild(view);
	bitmap->Lock();

	view->SetHighColor(B_TRANSPARENT_COLOR);
	view->FillRect(view->Bounds());

	if(color == DARK) view->SetHighColor(255,   0,   0);
	else			  view->SetHighColor(250, 250, 250);

	view->FillEllipse(Bounds());	
	view->SetHighColor(0, 0, 0);
	view->StrokeEllipse(Bounds());

	if(rank == KING) {
		BPoint crown[] = {BPoint(17, 26), BPoint(25, 26), BPoint(29, 18),
						  BPoint(24, 21), BPoint(21, 16), BPoint(18, 21), BPoint(13, 18)};

		view->SetHighColor(255, 200, 0);
		view->FillPolygon(crown, 7);
		view->FillEllipse(BPoint(31, 16), 1, 1);
		view->FillEllipse(BPoint(11, 16), 1, 1);
		view->FillEllipse(BPoint(21, 14), 1, 1);

		view->SetHighColor(0, 0, 0);
		view->StrokePolygon(crown, 7);
		view->StrokeEllipse(BPoint(31, 16), 1, 1);
		view->StrokeEllipse(BPoint(11, 16), 1, 1);
		view->StrokeEllipse(BPoint(21, 14), 1, 1);
	}
	view->Sync();
	bitmap->RemoveChild(view);
	bitmap->Unlock();
	delete view;


		BMessage message(CHECKER);
		message.AddPointer("CheckerPtr", this);

		DragMessage(&message, bitmap, B_OP_ALPHA, point);
	}
}