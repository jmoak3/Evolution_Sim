#include "Functions.h"
#include "d3dApp.h"
#include "GfxStats.h"
#include <tchar.h>

GfxStats::GfxStats()
	: mFont(0), mFPS(0), mMilliSecPerFrame(0.0f), mNumTris(0), mNumVerticies(0)
{
	D3DXFONT_DESC fontDesc;
	fontDesc.Height = 18;
	fontDesc.Width = 0;
	fontDesc.Weight = 0;
	fontDesc.MipLevels = 1;
	fontDesc.Italic = false;
	fontDesc.CharSet = DEFAULT_CHARSET;
	fontDesc.OutputPrecision = OUT_DEFAULT_PRECIS;
	_tcscpy(fontDesc.FaceName, _T("Times New Roman"));

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));
}

GfxStats::~GfxStats()
{
	ReleaseCOM(mFont);
}

void GfxStats::onLostDevice()
{
	HR(mFont->OnLostDevice());
}

void GfxStats::onResetDevice()
{
	HR(mFont->OnResetDevice());
}

void GfxStats::addVerticies(DWORD n)
{
	mNumVerticies += n;
}

void GfxStats::subVerticies(DWORD n)
{
	mNumVerticies -= n;
}

void GfxStats::addTriangles(DWORD n)
{
	mNumTris += n;
}

void GfxStats::subTriangles(DWORD n)
{
	mNumTris -= n;
}

void GfxStats::setTriCount(DWORD n)
{
	mNumTris = n;
}

void GfxStats::setVertexCount(DWORD n)
{
	mNumVerticies = n;
}

void GfxStats::update(float dt)
{
	static float numFrames = 0.0f;
	static float timeElapsed = 0.0f;

	numFrames += 1.0f;

	timeElapsed += dt;

	if( timeElapsed >= 1.0f )
	{
		mFPS = numFrames;
		
		mMilliSecPerFrame = 1000.0f / mFPS;
		timeElapsed = 0.0f;
		numFrames = 0.0f;
	}
}

void GfxStats::display()
{
	static char buffer[256];

	sprintf(buffer, "Frames Per Second = %.2f\n"
					  "Milliseconds Per Frame = %.4f\n"
					  "Triangle Count = %d\n"
					  "Vertext Count = %d", mFPS, mMilliSecPerFrame, mNumTris, mNumVerticies);

	RECT R = {5, 5, 0, 0};
	HR(mFont->DrawTextA(0, buffer, -1, &R, DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0)));
}
