#include "LifeForm.h"
#include "Functions.h"
//#include "Base.h"
//#include "Family.h"
#include "Stat.h"
#include "Food.h"
#include "Egg.h"



void Lifeform::spawn(Stat & stat)
{
	mStat = stat;

	mRotation = GetRandFloat() * (2.0f*D3DX_PI);

	mLifeTime = 0.0f;
	mEggCycle = 0.0f;
	mFoodCycle = 0.0f;
	mPursuitTime = 0.0f;
	mDropFood = true;
	mHunting = false;
	mTired = false;
	mThinkTime = 0.0f;
	mTimeToThink = false;
	mStomach = 1;
	++BellyFoods; 
	mDead = false;
	updateDNA();
}

void Lifeform::updateLifeform(float dt, float trueTime)
{
	if (!mDead)
	{
	updatePosition(dt);

	mThinkTime += trueTime;
	if (mThinkTime > Intellect)
		mTimeToThink = true;

	//if (mStat.sightDistance > 10.0f)
	//	search();

	float Dist = mStat.sightDistance;

	if (!mStat.carnivore && mTimeToThink)
	{
		for (std::list<Food>::iterator i = foodList.begin(); i != foodList.end(); ++i)
		{
			D3DXVECTOR3 fPos = i->getPosition();
			D3DXVECTOR3 mPos = mStat.pos;
			//search for food
			if (mStat.sightDistance >= 10.0f)
			{
				float NewDist = 9999.0f;

				if ( mPos.x - fPos.x < Dist && mPos.x - fPos.x > -Dist ) 
					if ( mPos.y - fPos.y < Dist && mPos.y - fPos.y > -Dist )
						NewDist = Distance(mPos, fPos);

				if (NewDist < Dist)
				{
					Dist = NewDist;
			
					if (mPos.x > fPos.x && mPos.y < fPos.y)
						mRotation = atan( (mPos.x - fPos.x) /
										   (fPos.y - mPos.y) );

					else if (mPos.x > fPos.x && mPos.y > fPos.y)
						mRotation = (D3DX_PI/2.0f) + atan( (mPos.y - fPos.y) /
															(mPos.x - fPos.x) );
				
					else if (mPos.x < fPos.x && mPos.y > fPos.y)
						mRotation = (D3DX_PI) + atan( (mPos.x - fPos.x) /
													   (fPos.y - mPos.y) );

					else if (mPos.x < fPos.x && mPos.y < fPos.y)
						mRotation = (2.0f*D3DX_PI) - atan( (fPos.x - mPos.x) /
															(fPos.y - mPos.y) );

					else if (mPos.x < fPos.x && mPos.y == fPos.y)
						mRotation = (3.0f*D3DX_PI)/2.0f;

					else if (mPos.x > fPos.x && mPos.y == fPos.y)
						mRotation = (D3DX_PI/2.0f);

					else if (mPos.x == fPos.x && mPos.y < fPos.y)
						mRotation = (D3DX_PI);

					else if (mPos.x == fPos.x && mPos.y > fPos.y)
						mRotation = 0.0f;
				}
			}

			if ( mPos.x - fPos.x < 10.0f && mPos.x - fPos.x > -10.0f ) 
				if ( mPos.y - fPos.y < 10.0f && mPos.y - fPos.y > -10.0f )
					if (Distance(mPos, fPos) <= 10.0f)
						if (!i->isEaten())
							eatFood((*i));
		}
		//Search for lifeforms that want to eat me and run
		// To make efficient- keep a count of all carnivores. After counting that many in this loop stop.
		// Or separate lists somehow and go only through the carnivores
		if (Dist > mStat.fearDistance)
			for (std::list<Lifeform>::iterator i = lifeformList.begin(); i != lifeformList.end(); ++i)
			{
				if (i->isCarnivore())
				{
					float NewDist = 9999.0f;
					
					D3DXVECTOR3 lPos = i->getPosition();
					D3DXVECTOR3 mPos = mStat.pos;

					if ( mPos.x - lPos.x < Dist && mPos.x - lPos.x > -Dist ) 
						if ( mPos.y - lPos.y < Dist && mPos.y - lPos.y > -Dist )
							NewDist = Distance(mPos, lPos);

					if ( NewDist < Dist && NewDist < mStat.fearDistance) 
					{
						Dist = NewDist;
	
						if (mPos.x > lPos.x && mPos.y < lPos.y)
							mRotation = (D3DX_PI) + atan( (mPos.x - lPos.x) /
															(lPos.y - mPos.y) );

						else if (mPos.x > lPos.x && mPos.y > lPos.y)
							mRotation = (3.0f*D3DX_PI)/2.0f + atan( (mPos.y - lPos.y) /
																	(mPos.x - lPos.x) );
					
						else if (mPos.x < lPos.x && mPos.y > lPos.y)
							mRotation = (2.0f*D3DX_PI) + atan( (mPos.x - lPos.x) /
																(lPos.y - mPos.y) );

						else if (mPos.x < lPos.x && mPos.y < lPos.y)
							mRotation = (D3DX_PI) - atan( (lPos.x - mPos.x) /
	   														(lPos.y - mPos.y) );
	
						else if (mPos.x < lPos.x && mPos.y == lPos.y)
							mRotation = (D3DX_PI)/2.0f;

						else if (mPos.x > lPos.x && mPos.y == lPos.y)
							mRotation = (3.0f*D3DX_PI)/2.0f;
	
						else if (mPos.x == lPos.x && mPos.y < lPos.y)
							mRotation = (2.0f*D3DX_PI);

						else if (mPos.x == lPos.x && mPos.y > lPos.y)
							mRotation = D3DX_PI;
					}
				}
			}
	}
	/*if (mStat.eggEater)
	for (std::list<Egg>::iterator i = eggList.begin(); i != eggList.end(); ++i)
	{
		if ( Distance(mStat.pos, i->getPosition()) < 10.0f )// MAKE NOT EAT OWN BABIES
			eatEgg((*i));
	}
	*/
	
	
	else if (mStat.carnivore)
	{
		if (mTimeToThink)
		{
			mHunting = false;
			for (std::list<Lifeform>::iterator i = lifeformList.begin(); i != lifeformList.end(); ++i)
			{			
				if (!(i->isCarnivore())) 
				{
					D3DXVECTOR3 lPos = i->getPosition();
					D3DXVECTOR3 mPos = mStat.pos;

					//search for lifeforms to eat
					if (mStat.sightDistance >= 16.0f && !mTired)
					{
						float NewDist = 9999.0f;

						if ( mPos.x - lPos.x < Dist && mPos.x - lPos.x > -Dist ) 
							if ( mPos.y - lPos.y < Dist && mPos.y - lPos.y > -Dist )
								NewDist = Distance(mPos, lPos);
	
						if ( NewDist < Dist && NewDist > 0.0f )
						{
							Dist = NewDist;

							mHunting = true;

							if (mPos.x > lPos.x && mPos.y < lPos.y)
								mRotation = atan( (mPos.x - lPos.x) /
													(lPos.y - mPos.y) );
	
							else if (mPos.x > lPos.x && mPos.y > lPos.y)
								mRotation = (D3DX_PI/2.0f) + atan( (mPos.y - lPos.y) /
																	(mPos.x - lPos.x) );
					
							else if (mPos.x < lPos.x && mPos.y > lPos.y)
								mRotation = (D3DX_PI) + atan( (mPos.x - lPos.x) /
																(lPos.y - mPos.y) );
					
							else if (mPos.x < lPos.x && mPos.y < lPos.y)
								mRotation = (2.0f*D3DX_PI) - atan( (lPos.x - mPos.x) /
																	(lPos.y - mPos.y) );

							else if (mPos.x < lPos.x && mPos.y == lPos.y)
								mRotation = (3.0f*D3DX_PI)/2.0f;
	
							else if (mPos.x > lPos.x && mPos.y == lPos.y)
								mRotation = (D3DX_PI/2.0f);

							else if (mPos.x == lPos.x && mPos.y < lPos.y)
								mRotation = (D3DX_PI);

							else if (mPos.x == lPos.x && mPos.y > lPos.y)
								mRotation = 0.0f;
						}
					}
					//eat lifeforms
					if ( mPos.x - lPos.x < 16.0f && mPos.x - lPos.x > -16.0f )
						if ( mPos.y - lPos.y < 16.0f && mPos.y - lPos.y > -16.0f )
							if (Distance(mPos, lPos) <= 16.0f)
								if (!i->isDead())
									eatLifeform((*i)); 
				}
			}
		}
		if (mHunting || mTired)
			mPursuitTime += dt;
		if (!mHunting && !mTired)
			mPursuitTime = 0.0f;
		if (mPursuitTime > (mStat.pursuitLength + 5.0f) && mTired)
		{
			mTired = false;
			mPursuitTime = 0.0f;
		}
		if (mPursuitTime > mStat.pursuitLength && !mTired)
		{
			mTired = true;
			mHunting = true;
		}
		if (mHunting && mTired)
			mRotation = GetRandFloat()*2.0f*D3DX_PI;
	}

	mFoodCycle += dt;
	mEggCycle += dt;
	mLifeTime += dt;

	if (mFoodCycle > mStat.foodCycleLength)
	{
		--mStomach;
		--BellyFoods;
		mFoodCycle = 0.0f;
	}
	if (mEggCycle > mStat.eggCycleLength)
	{
		layEgg(mStat);
		mEggCycle = 0.0f;
	}
	if (mLifeTime > mStat.lifeTimeLength || mStomach <= 0)
	{
		die();
		mLifeTime = 0.0f;
	}

	if (mTimeToThink)
		mThinkTime = 0.0f;
	mTimeToThink = false;
	}
}

bool Lifeform::isDead()
{
	return mDead;
}


bool Lifeform::isCarnivore()
{
	return mStat.carnivore;
}


void Lifeform::eaten()
{
	mDropFood = false;
	die();
}

D3DXVECTOR3 Lifeform::getPosition()
{
	return mStat.pos;
}

float Lifeform::getRotation()
{
	return mRotation;
}

Stat Lifeform::getStat()
{
	return mStat;
}


int Lifeform::getManliness()
{
	return mStat.carnivoreParts;
}

int Lifeform::getStomach()
{
	return mStomach;
}

float Lifeform::getNameber()
{
	return mStat.nameber;
}

/*
Family Lifeform::getFamily()
{
	return mStat.family;
}
*/

void Lifeform::updateDNA()
{							
	while (GetRandFloat() < 0.3333f) --mStat.speed;
	while (GetRandFloat() < 0.3333f) --mStat.lifeTimeLength;
	while (GetRandFloat() < 0.3333f) --mStat.eggTimeLength;
	while (GetRandFloat() < 0.3333f) --mStat.eggCycleLength;
	while (GetRandFloat() < 0.3333f) --mStat.foodCycleLength;
	while (GetRandFloat() < 0.3333f) --mStat.sightDistance;
	//while (GetRandFloat() < 0.3333f) --mStat.foodBeforeEgg;


	//while (GetRandFloat() < 0.3333f) --mStat.eggEatParts;
	while (GetRandFloat() < 0.3333f) --mStat.carnivoreParts;


	while (GetRandFloat() > 0.6666f) ++mStat.speed;
	while (GetRandFloat() > 0.6666f) ++mStat.lifeTimeLength;
	while (GetRandFloat() > 0.6666f) ++mStat.eggTimeLength;
	while (GetRandFloat() > 0.6666f) ++mStat.eggCycleLength;
	while (GetRandFloat() >	0.6666f) ++mStat.foodCycleLength;
	while (GetRandFloat() > 0.6666f) ++mStat.sightDistance;
	//while (GetRandFloat() < 0.6666f) ++mStat.foodBeforeEgg;


	//while (GetRandFloat() < 0.3333f) --mStat.eyeParts;
	//while (GetRandFloat() > 0.6666f) ++mStat.eyeParts;
	//while (GetRandFloat() > 0.6666f) ++mStat.eggEatParts;
	while (GetRandFloat() > 0.6666f) ++mStat.carnivoreParts;
	

	//if (mStat.eyeParts >= 3) mStat.canSee = true;
	//else if (mStat.eyeParts < 3) mStat.canSee = false;

//	if (mStat.canSee) while (GetRandFloat() < 0.3333f) --mStat.sightDistance;
//	if (mStat.canSee) while (GetRandFloat() > 0.6666f) ++mStat.sightDistance;

	if (mStat.sightDistance > mStat.fearDistance) while (GetRandFloat() < 0.3333f) --mStat.fearDistance;
	if (mStat.sightDistance > mStat.fearDistance) while (GetRandFloat() > 0.6666f) ++mStat.fearDistance;

	//if (mStat.eggEatParts >= 1000 && !mStat.eggEater) mStat.eggEater = false; // NO EATING BABIES YET, MUST FIND WAY TO NOT EAT OWN
	//else if (mStat.eggEatParts < 1000 && mStat.eggEater) mStat.eggEater = false;
	
	//if (mStat.carnivoreParts >= 10 && !mStat.carnivore && mStat.sightDistance < 10.0f) mStat.carnivore = true;
	if (mStat.carnivoreParts >= 10 && !mStat.carnivore) mStat.carnivore = true;
	else if (mStat.carnivoreParts < 10 && mStat.carnivore) mStat.carnivore = false;

	if (mStat.carnivore) while (GetRandFloat() > 0.6666f) ++mStat.pursuitLength;
	if (mStat.carnivore) while (GetRandFloat() > 0.3333f) --mStat.pursuitLength;
	
	if (mStat.speed < 0.0f)
		mStat.speed = 0.0f;
	
	if (mStat.lifeTimeLength < 0.0f)
		mStat.lifeTimeLength = 0.0f;

	if (mStat.eggTimeLength < 0.0f)
		mStat.eggTimeLength = 0.0f;

	if (mStat.eggCycleLength < 0.0f)
		mStat.eggCycleLength = 0.0f;

	if (mStat.foodCycleLength < 0.0f)
		mStat.foodCycleLength = 0.0f;

	if (mStat.sightDistance < 0.0f)
		mStat.sightDistance = 0.0f;

	if (mStat.fearDistance < 16.0f)
		mStat.fearDistance = 16.0f;
	
	if (mStat.pursuitLength < 5.0f)
		mStat.pursuitLength = 5.0f;

	//if (mStat.foodBeforeEgg < 1)
	//	mStat.foodBeforeEgg = 1;

//	if (mStat.eyeParts < 0)
//		mStat.eyeParts = 0;

	//if (mStat.eggEatParts < 0)
	//	mStat.eggEatParts = 0;
	
	if (mStat.carnivoreParts < 0)
		mStat.carnivoreParts = 0;

	mStat.nameber = GetRandFloat()*1000.0f;
}

void Lifeform::eatFood(Food & food)
{
	++mStomach;
	++BellyFoods;
	mRotation = GetRandFloat()*(2.0f*D3DX_PI);
	food.eaten();
}

/*
void Lifeform::eatEgg(Egg & egg)
{
	++mStomach;
	mRotation = GetRandFloat()*(2.0f*D3DX_PI);
	egg.eaten();
}
*/



void Lifeform::eatLifeform(Lifeform & lifeform)
{
	BellyFoods += lifeform.getStomach();
	mStomach += lifeform.getStomach();
	lifeform.eaten();
}

/*
void Lifeform::search()
{
	float Dist = mStat.sightDistance;

	if (!mStat.carnivore)
	for (std::list<Food>::iterator i = foodList.begin(); i != foodList.end(); ++i)
	{
		float NewDist = Distance(mStat.pos, i->getPosition());

		if (NewDist < Dist)
		{
			Dist = NewDist;
			
			if (mStat.pos.x > i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = atanf( (mStat.pos.x - i->getPosition().x) /
								   (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x > i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = (D3DX_PI/2.0f) + atanf( (mStat.pos.y - i->getPosition().y) /
												    (mStat.pos.x - i->getPosition().x) );
				
			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = (D3DX_PI) + atanf( (mStat.pos.x - i->getPosition().x) /
											   (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (2.0f*D3DX_PI) - atanf( (i->getPosition().x - mStat.pos.x) /
												    (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y == i->getPosition().y)
				mRotation = (3.0f*D3DX_PI)/2.0f;

			else if (mStat.pos.x > i->getPosition().x && mStat.pos.y == i->getPosition().y)
				mRotation = (D3DX_PI/2.0f);

			else if (mStat.pos.x == i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (D3DX_PI);

			else if (mStat.pos.x == i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = 0.0f;
		}
	}

	/*
	if (mStat.eggEater)
	for (std::list<Egg>::iterator i = eggList.begin(); i != eggList.end(); ++i)
	{
		float NewDist = Distance(mStat.pos, i->getPosition());

		if ( NewDist < Dist && i->getSpecies() != mStat.species)
		{
			Dist = NewDist;

			if (mStat.pos.x > i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = atan( (mStat.pos.x - i->getPosition().x) /
								  (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x > i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = (D3DX_PI/2.0f) + atan( (mStat.pos.y - i->getPosition().y) /
												   (mStat.pos.x - i->getPosition().x) );
				
			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = (D3DX_PI) + atan( (mStat.pos.x - i->getPosition().x) /
											  (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (2.0f*D3DX_PI) - atan( (i->getPosition().x - mStat.pos.x) /
												   (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y == i->getPosition().y)
				mRotation = (3.0f*D3DX_PI)/2.0f;

			else if (mStat.pos.x > i->getPosition().x && mStat.pos.y == i->getPosition().y)
				mRotation = (D3DX_PI/2.0f);

			else if (mStat.pos.x == i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (D3DX_PI);

			else if (mStat.pos.x == i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = 0.0f;
		}
	}
	*/ // EGG STUFF. CAN FIX NOW GET TIME
		
	/*
	for (std::list<Lifeform>::iterator i = lifeformList.begin(); i != lifeformList.end(); ++i)
	{
		float NewDist = Distance(mStat.pos, i->getPosition());
		
		if (NewDist < Dist && NewDist > 0.0f && mStat.carnivore && !(i->isCarnivore()))
		{
			Dist = NewDist;
	
			if (mStat.pos.x > i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = atan( (mStat.pos.x - i->getPosition().x) /
								  (i->getPosition().y - mStat.pos.y) );
	
			else if (mStat.pos.x > i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = (D3DX_PI/2.0f) + atan( (mStat.pos.y - i->getPosition().y) /
												   (mStat.pos.x - i->getPosition().x) );
					
			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = (D3DX_PI) + atan( (mStat.pos.x - i->getPosition().x) /
											  (i->getPosition().y - mStat.pos.y) );
					
			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (2.0f*D3DX_PI) - atan( (i->getPosition().x - mStat.pos.x) /
												   (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y == i->getPosition().y)
				mRotation = (3.0f*D3DX_PI)/2.0f;
	
			else if (mStat.pos.x > i->getPosition().x && mStat.pos.y == i->getPosition().y)
				mRotation = (D3DX_PI/2.0f);

			else if (mStat.pos.x == i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (D3DX_PI);

			else if (mStat.pos.x == i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = 0.0f;
		}
		// else if less manly... RUN AWAY
		else if ( NewDist < Dist && NewDist > 0.0f && i->isCarnivore() && !mStat.carnivore && NewDist < mStat.fearDistance) 
		{
			Dist = NewDist;
	
			if (mStat.pos.x > i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (D3DX_PI) + atan( (mStat.pos.x - i->getPosition().x) /
											  (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x > i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = (3.0f*D3DX_PI)/2.0f + atan( (mStat.pos.y - i->getPosition().y) /
														(mStat.pos.x - i->getPosition().x) );
					
			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = (2.0f*D3DX_PI) + atan( (mStat.pos.x - i->getPosition().x) /
											       (i->getPosition().y - mStat.pos.y) );

			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (D3DX_PI) - atan( (i->getPosition().x - mStat.pos.x) /
	   									      (i->getPosition().y - mStat.pos.y) );
	
			else if (mStat.pos.x < i->getPosition().x && mStat.pos.y == i->getPosition().y)
				mRotation = (D3DX_PI)/2.0f;

			else if (mStat.pos.x > i->getPosition().x && mStat.pos.y == i->getPosition().y)
				mRotation = (3.0f*D3DX_PI)/2.0f;
	
			else if (mStat.pos.x == i->getPosition().x && mStat.pos.y < i->getPosition().y)
				mRotation = (2.0f*D3DX_PI);

			else if (mStat.pos.x == i->getPosition().x && mStat.pos.y > i->getPosition().y)
				mRotation = D3DX_PI;
		}
	}
}*/

void Lifeform::layEgg(Stat & stat)
{
	if (mStomach >= 2)
	{
		--mStomach;	
		--BellyFoods;
		Egg egg;
		//Stat stat = mStat;
		//stat.family.pParent = this;

		//Base * pChild = &egg;
		//mStat.family.pChildList.push_back(pChild);

		egg.spawn(mStat);
		eggList.push_back(egg);
		mEggCycle = 0.0f;
	}
}

void Lifeform::updatePosition(float dt)
{
	static float TWOPI = 2.0f*D3DX_PI;
	if (mRotation > (2.0f)*D3DX_PI) mRotation -= (TWOPI);
	else if (mRotation < 0.0f) mRotation += (TWOPI);

	if (mStat.pos.x > HalfMapX) 
	{
		mStat.pos.x = HalfMapX;
		mRotation = (TWOPI) - mRotation;
	}

	if (mStat.pos.x < -HalfMapX)
	{
		mStat.pos.x = -HalfMapX;
		mRotation = (TWOPI) - mRotation;
	}

	if (mStat.pos.y > HalfMapY) 
	{
		mStat.pos.y = HalfMapY;
		if (mRotation <= D3DX_PI) mRotation = D3DX_PI - mRotation;
		else if (mRotation > D3DX_PI) mRotation = D3DX_PI + (TWOPI - mRotation);
		else if (mRotation == 0.0f) mRotation += D3DX_PI;
	}

	if (mStat.pos.y < -HalfMapY)
	{
		mStat.pos.y = -HalfMapY;
		if (mRotation <= D3DX_PI) mRotation = D3DX_PI - mRotation;
		else if (mRotation > D3DX_PI) mRotation = D3DX_PI + (TWOPI - mRotation);
		else if (mRotation == 0.0f) mRotation += D3DX_PI;
	}
	
	D3DXVECTOR3 lifeformDir(-sinf(mRotation), cosf(mRotation), 0.0f);
	mStat.pos  += lifeformDir * mStat.speed * dt;
}

void Lifeform::die()
{
	if (mDropFood)
	{
		for (int i=0; i<mStomach; ++i)
		{
			Food food;
			food.spawn( D3DXVECTOR3(
				mStat.pos.x + GetRandFloat()*16.0f - 8.0f, 
				mStat.pos.y + GetRandFloat()*16.0f - 8.0f,
				0.0f));
			foodList.push_back(food);
		}
	}
	BellyFoods -= mStomach;

	mDead = true;
}
/*
int Lifeform::getSpecies()
{
	return mStat.species;
}
*/