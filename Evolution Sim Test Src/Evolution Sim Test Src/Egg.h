#ifndef EGG_H
#define EGG_H

#include "Functions.h"
#include "Stat.h"
//#include "Base.h"

class Egg //: public Base
{
public:
	Egg() {}
	~Egg() {}

	void spawn(Stat & stat);
	void hatch();
	void updateEgg(float dt);
	void eaten();
	bool isHatched();
	D3DXVECTOR3 getPosition();
	//Family getFamily();

private:
	Stat mStat;

	//short lolFillCache;

	bool mDead;
	float mEggTime;

//	bool oneByte;
	//short twoBytes;
};

extern std::list<Egg> eggList;

#endif
