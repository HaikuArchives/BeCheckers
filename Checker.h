/******************************************************************************
/
/	File:			Checker.h, Version 1.0.0
/
/	Copyright 2000, Jeremy J. Gibbons
/
*******************************************************************************/

#ifndef _CHECKER_H
#define _CHECKER_H

#include <math.h>

#include <Application.h>
#include <View.h>

#include "CheckerBoard.h"

/* Color constants */

#define DARK	1
#define LIGHT	0

/* Rank constants */

#define KING	1
#define MAN		0

/* Checker message constants */

const uint32 CHECKER      = 'Chkr';
const uint32 CHECKER_WIN  = 'Wnnr';
const uint32 CHECKER_TURN = 'CTrn';

/*----------------------------------------------------------------*/
/*----- Checker class --------------------------------------------*/

class Checker : public BView {

public:
					Checker(bool color, int col, int row, bool rank = MAN);
					~Checker();

		void		Draw(BRect updateRect);

		BRect		CheckerSize() const { return BRect(0, 0, 42, 42); }

		bool		CanMove(const Checker *c, const int spaces);
		bool		CanJump();
		Checker *	CheckerAt(const BPoint square);
		bool		Color() { return color; }
		bool		Contains(const BPoint point);
		bool		IsMovementPlayable(const BPoint square);
		bool		IsSquarePlayable(const BPoint square);
		bool		Rank() { return rank; }
		bool		Stalemate();
		BPoint		Square() { return square; }
		void		UpdateChecker(const BPoint square);


void DontUpdate();

static	BPoint		CheckerJumping() { return (checkerJumping == NULL ? B_ORIGIN : checkerJumping->square); }
static	bool		JumpAvailable() { return jumpAvailable; }
static	int			PrevDist() { return prevDist; }
static	void		Restart();
static	void		SetCheckerJumping(Checker *c) { checkerJumping = c; }
static	void		SetJumpAvailable(bool jA) { jumpAvailable = jA; }
static	void		SetTurn(bool t) { turn = t; }
static	void		SetPrevDist(int pD) { prevDist = pD; }
static	bool		Turn() { return turn; }

private:
		bool		IsMovementPlayable(const Checker *c, const BPoint square);
		void		MouseDown(BPoint point);

		bool		color, rank;
		BPoint		square;

static	bool		turn, jumpAvailable;
static	int			dCount, lCount, prevDist;
static	Checker		*checkerJumping;

static bool drawsq;
};

#endif