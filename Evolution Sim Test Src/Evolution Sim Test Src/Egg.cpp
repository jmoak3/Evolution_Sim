#include "Egg.h"
#include "Functions.h"
#include "Base.h"
#include "Family.h"
#include "Stat.h"
#include "LifeForm.h"


void Egg::spawn(Stat & stat)
{
	mStat = stat;

	mEggTime = 0.0f;
	mDead = false;
}

void Egg::updateEgg(float dt)
{
	mEggTime += dt;
	if (mEggTime > mStat.eggTimeLength)
		hatch();
}

void Egg::hatch() // ITERATE THROUGH LIST OF LIFE FORM, WHEN THIS PARENT IS EQUAL TO THE CHILD OF LIFEFORM, MAKE THAT CHILD POINT TO NEW LIFEFORM
{
	Lifeform lifeform;
	/*
	Stat stat = mStat;
	if (mStat.family.pParent->getFamily().pParent != NULL)
	for (std::list<Lifeform>::iterator i = lifeformList.begin(); i != lifeformList.end(); ++i)
	{
		for (std::list<Base*>::iterator j = i->getFamily().pChildList.begin(); j != i->getFamily().pChildList.end(); ++i)
		{
			if (*j == mStat.family.pParent->getFamily().pParent)
				*j = stat.family.pParent->getFamily().pParent;
		}
	}
	*/
	lifeform.spawn(mStat);
	lifeformList.push_back(lifeform);
	mDead = true;
}

void Egg::eaten()
{
	mDead = true;
}
 
bool Egg::isHatched()
{
	return mDead;
}

D3DXVECTOR3 Egg::getPosition()
{
	return mStat.pos;
}

/*
Family Egg::getFamily()
{
	return mFamily;
}
*/
