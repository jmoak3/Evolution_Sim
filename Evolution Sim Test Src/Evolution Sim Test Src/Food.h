#ifndef FOOD_H
#define FOOD_H

#include "Functions.h"

class Food
{
public:
	Food() {}
	~Food() {}

	void spawn(D3DXVECTOR3 pos);
	void eaten();
	bool isEaten();
	D3DXVECTOR3 getPosition();
	
private:

	//short lolFillCache;
	//bool lolFillCache2;

	D3DXVECTOR3 mPosition;
	bool mEaten;

	//bool oneByte;
	//short twoBytes;
};

extern std::list<Food> foodList;

#endif