#ifndef LIFEFORM_H
#define LIFEFORM_H

#include "Functions.h"
#include "Stat.h"
//#include "Base.h"

class Lifeform //: public Base
{
public:
	Lifeform() {}
	~Lifeform() {}

	void spawn(Stat & stat);
	void updateLifeform(float dt, float trueTime);
	void eaten();
	bool isDead();
	bool isCarnivore();
	//bool isEggEater();
	D3DXVECTOR3 getPosition();
	float getRotation();
	//int getSpecies();
	Stat getStat();
	int getManliness();
	int getStomach();
	float getNameber();
	//Family getFamily();
	
private:
	void updateDNA();
	void updatePosition(float dt);
	void search();
	void layEgg(Stat & stat);
	void eatFood(Food & food);
	//void eatEgg(Egg & egg);
	void eatLifeform(Lifeform & lifeform);
	void die();

	Stat mStat; 

	//short lolFillCache;
//	float fourBytes; // 4
	float mThinkTime;  //4
	bool mTimeToThink; //4
	float mRotation; //4
	bool mDead; //1
	bool mDropFood; //1
	bool mTired; //1
	bool mHunting; //1
	float mLifeTime; //4
	float mPursuitTime; //4
	float mEggCycle; //4
	float mFoodCycle; //4
	int mStomach; //4
};

extern std::list<Lifeform> lifeformList;

#endif