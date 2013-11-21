#include "Food.h"
#include "Functions.h"


void Food::spawn(D3DXVECTOR3 pos)
{
	mPosition = pos;
	mEaten = false;
}

void Food::eaten()
{
	mEaten = true;
}

bool Food::isEaten()
{
	return mEaten;
}

D3DXVECTOR3 Food::getPosition()
{
	return mPosition;
}
