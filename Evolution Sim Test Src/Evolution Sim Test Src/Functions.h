#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#if defined(DEBUG) | defined(_DEBUG)
#ifndef D3D_DEBUG_INFO
#define D3D_DEBUG_INFO
#endif
#endif

#include <d3d9.h>
#include <d3dx9.h>
#include <dxerr.h>
#include <string>
#include <sstream>
#include <list>
#include <memory>
//===============================================================
// Globals for convenient access.
class D3DApp;
extern D3DApp* gd3dApp;
extern IDirect3DDevice9* gd3dDevice;

//class Base;
//class Family;
class Stat;
class Lifeform;
class Food;
class Egg;
//===============================================================
// Clean up

#define ReleaseCOM(x) { if(x){ x->Release();x = 0; } }

//===============================================================
// Debug

#if defined(DEBUG) | defined(_DEBUG)
	#ifndef HR
	#define HR(x)                                      \
	{                                                  \
		HRESULT hr = x;                                \
		if(FAILED(hr))                                 \
		{                                              \
			DXTrace(__FILE__, __LINE__, hr, #x, TRUE); \
		}                                              \
	}
	#endif

#else
	#ifndef HR
	#define HR(x) x;
	#endif
#endif 


extern const float MapX;
extern const float MapY;
extern const float HalfMapX;
extern const float HalfMapY;

extern int BellyFoods;

extern const float Intellect;

inline float GetRandFloat()
{
	float r;
	r = (rand() % 10001) * 0.0001f;
	return r;
}

inline D3DXVECTOR3 GetRandPos()
{
	float x;
	float y;
	x = (GetRandFloat()*MapX) - HalfMapX;
	y = (GetRandFloat()*MapY) - HalfMapY;
	D3DXVECTOR3 pos(x, y, 0.0f);
	return pos;
}

inline float Distance( const D3DXVECTOR3  & pos1,  const D3DXVECTOR3 & pos2)
{
	float dist;
	dist = powf((powf((pos2.x - pos1.x), 2.000f) + powf((pos2.y - pos1.y), 2.000f)), 1.000f/2.000f);
	return dist;
}

#endif