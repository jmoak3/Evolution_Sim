// Line up cache
#ifndef D3DAPP_H
#define D3DAPP_H

#include "Functions.h"

class D3DApp
{
public:
	D3DApp(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD reqVertPrcs);
	virtual ~D3DApp();

	HINSTANCE getAppInst();
	HWND      getMainWnd();

	// Framework methods.  Derived client class overrides these methods to 
	// implement specific application requirements.
	virtual bool checkDeviceCaps()     { return true; }
	virtual void onLostDevice()        {}
	virtual void onResetDevice()       {}
	virtual void updateScene(float dt) {}
	virtual void drawScene()           {}

	// Override these methods only if you do not like the default window creation,
	// direct3D device creation, window procedure, or message loop.  In general,
	// for the sample programs of this book, we will not need to modify these.
	virtual void initMainWindow();
	virtual void initDirect3D();
	virtual int run();
	virtual LRESULT msgProc(UINT msg, WPARAM wParam, LPARAM lParam);

	void enableFullScreenMode(bool enable);
	bool isDeviceLost();

protected:
	// Derived client class can modify these data members in the constructor to 
	// customize the application.  
	std::string mMainWndCaption; //32
	D3DDEVTYPE  mDevType; //4
	DWORD       mReqVertPrcs; //4
	
	// Application, Windows, and Direct3D data members.
	HINSTANCE             mhInstance;//4
	HWND                  mhMainWnd;//4
	IDirect3D9*           md3dObject; //12
	bool				  mbTimeReset; //1

	//Assuming pointer is 4bytes
	//bool oneByte;
	//short twoBytes;
	//int fourBytes;

	//bool				  mbMinimized;
	D3DPRESENT_PARAMETERS md3dPP;//56
};

// Globals for convenient access.
extern D3DApp* gd3dApp;
extern IDirect3DDevice9* gd3dDevice;

#endif // D3DAPP_H