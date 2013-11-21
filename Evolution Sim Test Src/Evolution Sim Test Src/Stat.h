#ifndef STAT_H
#define STAT_H

#include "Functions.h"
//#include "Family.h"

class Stat // Size: 49 + 7 = 56/8 = 7
{
public:
	D3DXVECTOR3 pos; //12
	//bool canSee;
	//bool eggEater;
	bool carnivore; //1
	float lifeTimeLength; //4
	float eggTimeLength;//4
	float eggCycleLength; //4
	float foodCycleLength; //4
	float speed; //4
	float sightDistance; //4
	float fearDistance;  //4
	float pursuitLength; //4
	//int foodBeforeEgg; 
	//int eyeParts;
	//int eggEatParts;
	int carnivoreParts;  //4
	//Family family;
	//int species;
	float nameber;
	float fourBytes;
	//short sixBytes[3];
	//bool oneByte;
};

#endif

//Keeping the float fourBytes in gives 1.55ms
//W/O it is 1.56-7