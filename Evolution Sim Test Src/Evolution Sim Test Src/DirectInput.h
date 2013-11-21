// Line up cache
#ifndef DIRECT_INPUT_H
#define DIRECT_INPUT_H

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class DirectInput
{
public:
	DirectInput(DWORD keyboardCoopFlags, DWORD mouseCoopFlags);
	~DirectInput();

	void init();
	void poll();
	bool keyDown(unsigned char key);
	bool mouseButtonDown(int button);
	float mouseDX();
	float mouseDY();
	float mouseDZ();

private:
	// Make private to prevent copying of members of this class.
	DirectInput(const DirectInput& rhs);
	DirectInput& operator=(const DirectInput& rhs);
		
private:
	IDirectInput8*       mDInput;//4 + 4

	IDirectInputDevice8* mKeyboard; //4 + 4
	unsigned char        mKeyboardState[256]; //1 * 256 = 256

	IDirectInputDevice8* mMouse; //4 + 4
	DIMOUSESTATE2        mMouseState;//20
	
	//Assuming pointer is 4bytes
	//int fourBytes;

	DWORD mKeyboardCoopFlags;//4
	DWORD mMouseCoopFlags;//4
};
extern DirectInput* gDInput;

#endif // DIRECT_INPUT_H