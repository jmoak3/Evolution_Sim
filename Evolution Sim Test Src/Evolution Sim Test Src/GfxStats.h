// Line up cache
#ifndef GFX_STATS_H
#define GFX_STATS_H

#include <d3dx9.h>

class GfxStats
{
public:
	GfxStats();
	~GfxStats();
	
	void onLostDevice();
	void onResetDevice();

	void addVerticies(DWORD n);
	void subVerticies(DWORD n);
	void addTriangles(DWORD n);
	void subTriangles(DWORD n);

	void setTriCount(DWORD n);
	void setVertexCount(DWORD n);

	void update(float dt);
	void display();

private:
	GfxStats(const GfxStats & rhs);
	GfxStats& operator = (const GfxStats & rhs);

private:
	ID3DXFont * mFont;// 8

	//Assuming pointer is 4bytes
//	int fourBytes;

	float mFPS; //4
	float mMilliSecPerFrame; //4
	DWORD mNumTris; //4
	DWORD mNumVerticies; //4
};

#endif // GFX_STATS_H