//James Moak
//Now efficient as hell
//It has a separate vertex structure for the lifeforms and the Egg/Foods, to save sending useless data
//It fills an Vertex/Index buffer for lifeforms and another for Egg/Food
//Draws them
//At the start of each separate draw function, it releases the memory inside the buffer already.
//Releasing at the end causes crash because destructor will try to release the empty buffer's data - which there is none.
// 900 frames per second!
// 


#include "Functions.h"
#include "D3DApp.h"
#include "Directinput.h"
#include "LifeForm.h"
#include "Food.h"
#include "Egg.h"
#include "Stat.h"
#include <tchar.h>
#include <ctime>
#include "Vertex.h"
#include "GfxStats.h"

std::list<Lifeform> lifeformList;
std::list<Food> foodList;
std::list<Egg> eggList;

const float MapX = 3200.0f;
const float MapY = 2400.0f;
const float HalfMapX = MapX*0.5f;
const float HalfMapY = MapY*0.5f;

int BellyFoods = 0;

const float Intellect = 0.003666f;

// consider making below cache lined...

class Evolution : public D3DApp
{
public:
	Evolution(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD reqVertPrcs);
	~Evolution();

	void init();

	bool checkDeviceCaps();
	void onLostDevice();
	void onResetDevice();
	void updateScene(float dt);
	void drawScene();
	void drawEggsFood();
	void drawLifeforms();

	void buildFX();
	void buildBuffers();
	void buildViewMtx();
	void buildProjMtx();
private:	
	// Line up cache
	GfxStats* mGfxStats;


	int mFoodAmount;
	float mFoodSpawnTime;
	float mFoodSpawnCycleLength;
	int mMaxBio;

	//int mBellyFoods;

	float mTrueTime; 
	float mTimeSpeed; 

	float mCoolTime;

	D3DXVECTOR3 mCameraPos; //12

	//3 out of line:
	//bool oneByte;
	//short twoBytes;

	bool mbDrawInfo; //1
	Stat mDrawInfo; 

	float mFollowNumber;
	D3DXVECTOR3 mFollowPos;
	bool mbFollow;

	IDirect3DTexture9 * mFoodTex; //56
	IDirect3DTexture9 * mLifeformTex; //56

	IDirect3DTexture9 * mCarnTex;//56
	IDirect3DTexture9 * mSightTex;//56
	
	IDirect3DTexture9 * mEggTex;//56
	ID3DXFont * mFont; //4+4

	D3DXMATRIX mView; 
	D3DXMATRIX mProj; 

	IDirect3DVertexBuffer9 * mEFVB; //40
	IDirect3DIndexBuffer9 *  mEFIB;//40
	IDirect3DVertexBuffer9 * mLVB; //40
	IDirect3DIndexBuffer9 *  mLIB;//40
	ID3DXEffect* mFX;//8
	D3DXHANDLE   mhEFTech;//4
	D3DXHANDLE   mhLTech;//4
	D3DXHANDLE   mhWVP;//4
	D3DXHANDLE   mhWorld;//4
	D3DXHANDLE	 mhTex;//4
	D3DXHANDLE	 mhTex2;//4
	D3DXHANDLE	 mhTex3;//4
	//D3DXHANDLE	 mhAlpha1;
	//D3DXHANDLE	 mhAlpha2;

	bool mbLStart;
	bool mbEFStart;
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
				   PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.
	#if defined(DEBUG) | defined(_DEBUG)
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	#endif

	srand((unsigned int)time(NULL));

	Evolution app(hInstance, "Evolution - James Moak III AKA jmoak3", D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING);
	app.init();
	gd3dApp = &app;

	DirectInput di(DISCL_NONEXCLUSIVE|DISCL_FOREGROUND, DISCL_NONEXCLUSIVE|DISCL_FOREGROUND);
	di.init();
	gDInput = &di;

	return gd3dApp->run();
}

Evolution::Evolution(HINSTANCE hInstance, std::string winCaption, D3DDEVTYPE devType, DWORD reqVertPrcs)
: D3DApp(hInstance, winCaption, devType, reqVertPrcs)
{
	if(!checkDeviceCaps())
	{
		MessageBox(0, "checkDeviceCaps() Failed", 0, 0);
		PostQuitMessage(0);
	}

	if ( D3DXCreateTextureFromFile(gd3dDevice, "Lifeform.bmp", &mLifeformTex) )
	{	
		MessageBox(0, "Lifeform.bmp not found! D:", 0, 0);
		PostQuitMessage(0);
	}
	if ( D3DXCreateTextureFromFile(gd3dDevice, "CarnParts.bmp", &mCarnTex) )
	{	
		MessageBox(0, "CarnParts.bmp not found! D:", 0, 0);
		PostQuitMessage(0);
	}
	if ( D3DXCreateTextureFromFile(gd3dDevice, "SightParts.bmp", &mSightTex) )
	{	
		MessageBox(0, "SightParts.bmp not found! D:", 0, 0);
		PostQuitMessage(0);
	}
	if ( D3DXCreateTextureFromFile(gd3dDevice, "Food.bmp", &mFoodTex) )
	{	
		MessageBox(0, "Food.bmp not found! D:", 0, 0);
		PostQuitMessage(0);
	}
	if ( D3DXCreateTextureFromFile(gd3dDevice, "Egg.bmp", &mEggTex) )
	{	
		MessageBox(0, "Egg.bmp not found! D:", 0, 0);
		PostQuitMessage(0);
	}


	D3DXFONT_DESC fontDesc;
	fontDesc.Height				= 18;
	fontDesc.Width				= 0;
	fontDesc.Weight				= FW_SEMIBOLD;
	fontDesc.MipLevels			= 1;
	fontDesc.Italic				= false;
	fontDesc.CharSet			= DEFAULT_CHARSET;
	fontDesc.OutputPrecision	= OUT_DEFAULT_PRECIS;
	fontDesc.Quality			= DEFAULT_QUALITY;
	fontDesc.PitchAndFamily		= DEFAULT_PITCH | FF_DONTCARE;
	_tcscpy(fontDesc.FaceName, _T("Times New Roman"));

	HR(D3DXCreateFontIndirect(gd3dDevice, &fontDesc, &mFont));

	mGfxStats = new GfxStats();

	mCameraPos = D3DXVECTOR3(0.0f, 0.0f, -800.0f);
	
	mTimeSpeed = 1.0f;
	mTrueTime = 0.0f;
	mCoolTime = 0.0f;

	mbFollow = false;
	mFollowNumber = 0.0f;
	mFollowPos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	mbLStart = true;
	mbEFStart = true;

	mbDrawInfo = false;

	buildBuffers();
	buildFX();

	onResetDevice();

	InitAllVertexDeclarations();
}

Evolution::~Evolution()
{
	delete mGfxStats;

	ReleaseCOM(mEFVB);
	ReleaseCOM(mEFIB);
	ReleaseCOM(mLVB);
	ReleaseCOM(mLIB);
	ReleaseCOM(mFX);
	ReleaseCOM(mFont);
	ReleaseCOM(mFoodTex);
	ReleaseCOM(mLifeformTex);
	ReleaseCOM(mCarnTex);
	ReleaseCOM(mSightTex);
	ReleaseCOM(mEggTex);

	DestroyAllVertexDeclarations();
}

void Evolution::init() 
{
	mFoodAmount = 500;
	mMaxBio = 520;
	mFoodSpawnTime = 0.0f;
	mFoodSpawnCycleLength = 0.0f;

	//int j = 0;
	for (int i=0; i<mMaxBio-mFoodAmount; ++i)
	{
		Stat stat;
		stat.pos = GetRandPos();
		//stat.canSee = false;
		//stat.eggEater = false;
		stat.carnivore = false;
		stat.lifeTimeLength = 180.0f;
		stat.eggTimeLength = 60.0f;
		stat.eggCycleLength = 30.0f;//30.0f
		stat.foodCycleLength = 45.0f; 
		stat.speed = 20.0f;//20.0f
		stat.sightDistance = 0.0f;//0.0f
		//stat.species = j;
		stat.fearDistance = 17.0f;//17.0f
		stat.pursuitLength = 5.0f;
		//stat.foodBeforeEgg = 2;
		//stat.eyeParts = 0;
		//stat.eggEatParts = 0;
		//stat.carnivore = true;
		stat.carnivoreParts = 0;//0.0f
		//stat.family.pParent = NULL;
		stat.nameber = GetRandFloat()*1000.0f;

		Lifeform lifeform;
		lifeform.spawn(stat);
		lifeformList.push_back(lifeform);
		//++j;
		//if (j > 4) j = 0;
	}

	for (int i=0; i<mFoodAmount; ++i)
	{
		Food food;
		food.spawn(GetRandPos());
		foodList.push_back(food);
	}
}

bool Evolution::checkDeviceCaps()
{
	// Can it run a hamster wheel?
	return true;
}

void Evolution::onLostDevice()
{
	mGfxStats->onLostDevice();
	HR(mFont->OnResetDevice());
	HR(mFX->OnLostDevice());
}

void Evolution::onResetDevice()
{
	mGfxStats->onResetDevice();
	HR(mFont->OnResetDevice());

	HR(mFX->OnResetDevice());

	buildProjMtx();
}

void Evolution::updateScene(float dt)
{
	gDInput->poll();
	
	mTrueTime = dt;
	mCoolTime += mTrueTime;

	//mBellyFoods = 0;
	
	mGfxStats->setTriCount((lifeformList.size()+foodList.size()+eggList.size())*2);
	mGfxStats->setVertexCount((lifeformList.size()+foodList.size()+eggList.size())*4);
	mGfxStats->update(mTrueTime);

	dt *= mTimeSpeed;

	if (gDInput->keyDown(DIK_LEFT) && mCoolTime > 0.1f)
	{
		--mTimeSpeed;
		if (mTimeSpeed < 1.0f)
			mTimeSpeed = 1.0f;
		mCoolTime = 0.0f;
	}

	if (gDInput->keyDown(DIK_RIGHT) && mCoolTime > 0.1f)
	{
		++mTimeSpeed;
		if (mTimeSpeed > 30.0f)
			mTimeSpeed = 30.0f;
		mCoolTime = 0.0f;
	}
	

	POINT mpos;
	GetCursorPos(&mpos);
	ScreenToClient(mhMainWnd, &mpos);
	//mpos.x -= 400;
	////mpos.y = -mpos.y + 300;
	//D3DXVECTOR4 mousePos;
	//D3DXVec3Transform(&mousePos, &(D3DXVECTOR3(mpos.x, mpos.y, 0.0f)), &(mView*mProj));
	
	//Mouse position and clicking stuff
	D3DXVECTOR3 mousePos(0.0f, 0.0f, 0.0f);
	static float w = (float)md3dPP.BackBufferWidth;
	static float h = (float)md3dPP.BackBufferHeight;
	float x = (2.0f*mpos.x/w - 1.0f) / mProj(0,0);
	float y = (-2.0f*mpos.y/h + 1.0f) / mProj(1,1);
	
	D3DXVECTOR3 dir(x, y, 1.0f);

	D3DXMATRIX invView;
	D3DXMatrixInverse(&invView, 0, &mView);

	D3DXVec3TransformNormal(&mousePos, &dir, &invView);
	D3DXVec3Normalize(&mousePos, &mousePos);

	float ratio = -mCameraPos.z/mousePos.z;

	mousePos.x = mousePos.x*ratio + mCameraPos.x;
	mousePos.y = mousePos.y*ratio + mCameraPos.y;

	//Zooming in/out
	// Try making a ray to each of the four corners and go from there
	if (gDInput->mouseDZ() != 0.0f)
	{
		mCameraPos.z += gDInput->mouseDZ()*mTrueTime*400.000f;
		mCameraPos.x += (gDInput->mouseDZ()*0.5000f*(mousePos.x-mCameraPos.x))*mTrueTime;
		mCameraPos.y += (gDInput->mouseDZ()*0.5000f*(mousePos.y-mCameraPos.y))*mTrueTime;
	}

	if (mbFollow)
	{
		mCameraPos.x = mFollowPos.x;
		mCameraPos.y = mFollowPos.y;
	}
	
	if (mCameraPos.z > -300.0f)
		mCameraPos.z = -300.0f;
	if (mCameraPos.z < -MapX)		
		mCameraPos.z = -MapX;		

	if (mCameraPos.x < -HalfMapX-mCameraPos.z/2.0f)
		mCameraPos.x = -HalfMapX-mCameraPos.z/2.0f;
	if (mCameraPos.x > HalfMapX+mCameraPos.z/2.0f)
		mCameraPos.x = HalfMapX+mCameraPos.z/2.0f;

	if (mCameraPos.y < -HalfMapY-mCameraPos.z*3.0f/8.0f)
		mCameraPos.y = -HalfMapY-mCameraPos.z*3.0f/8.0f;
	if (mCameraPos.y > HalfMapY+mCameraPos.z*3.0f/8.0f)
		mCameraPos.y = HalfMapY+mCameraPos.z*3.0f/8.0f;

	if (gDInput->mouseButtonDown(0) && mCoolTime > 1.0f)
	{
		mbDrawInfo = false;
	}

	if (gDInput->mouseButtonDown(1) && mCoolTime > 1.0f)
	{
		mbFollow = false;
	}

	/*if (gDInput->mouseButtonDown(2) && mCoolTime > 0.4f) // Add rightclick functions
	{
		Stat newStat;
		newStat.carnivoreParts = GetRandFloat()*20 + 1;
		newStat.eggCycleLength = GetRandFloat()*60.0f;
		newStat.lifeTimeLength = GetRandFloat()*360.0f;
		newStat.eggTimeLength  = GetRandFloat()*120.0f;
		newStat.foodCycleLength= GetRandFloat()*90.0f;
		newStat.speed	       = GetRandFloat()*40.0f;
		newStat.sightDistance  = GetRandFloat()*20.0f;
		newStat.fearDistance   = GetRandFloat()*30.0f;
		newStat.pursuitLength  = GetRandFloat()*20.0f;
		newStat.pos			   = D3DXVECTOR3(mousePos.x, mousePos.y, 0.0f);

		Egg egg;
		egg.spawn(newStat);
		eggList.push_back(egg);

		mCoolTime = 0.0f;
	}*/

	bool followFlag = true;
	// Update the lovely lifeforms.
	std::list<Lifeform>::iterator iLifeform = lifeformList.begin(); 
	while (iLifeform != lifeformList.end())
	{
		iLifeform->updateLifeform(dt, mTrueTime);

		// Did the mouse CLICK on this lifeform?
		if ( gDInput->mouseButtonDown(0) )
			if (!mbDrawInfo)
				if (Distance(mousePos, iLifeform->getPosition()) < 10.0f)
				{
					mDrawInfo = iLifeform->getStat();
					mbDrawInfo = true;
					mCoolTime = 0.0f;
				}
		if ( gDInput->mouseButtonDown(1) )
			if (!mbFollow)
				if (Distance(mousePos, iLifeform->getPosition()) < 10.0f)
				{
					mbFollow = true;
					mFollowNumber = iLifeform->getNameber();
					mCoolTime = 0.0f;
				}
		if (mbFollow)
			if (mFollowNumber == iLifeform->getNameber())
			{
				mFollowPos = iLifeform->getPosition();
				followFlag = false;
			}





		if (iLifeform->isDead())
			iLifeform = lifeformList.erase(iLifeform);

		else
			++iLifeform;
	}

	if (followFlag && mbFollow)
		mbFollow = false;

	// Run through the egg list and update them
	std::list<Egg>::iterator iEgg = eggList.begin();
	while (iEgg != eggList.end())
	{
		iEgg->updateEgg(dt);

		if (iEgg->isHatched())
			iEgg = eggList.erase(iEgg);

		else
			++iEgg;
	}

	// Collect total food in stomach!
	/*std::list<Lifeform>::iterator iCheck = lifeformList.begin(); 
	while (iCheck != lifeformList.end())
	{
		if (iCheck->isDead())
			iCheck = lifeformList.erase(iCheck);

		else 
		{
			mBellyFoods += iCheck->getStomach();		
			++iCheck;
		}
	}*/

	// Update food and the amount of them
	//mFoodSpawnTime += dt;
	//if (mFoodSpawnTime > mFoodSpawnCycleLength && 
		//foodList.size() < mFoodAmount && 
	//while(foodList.size() + mBellyFoods + eggList.size() < mMaxBio)
	//{
		//for (int i=0; i<mFoodAmount; ++i)
		//{
	//	Food food;
	//	food.spawn(GetRandPos());
	//	foodList.push_back(food);
		//}
		///mFoodSpawnTime = 0.0f;
	//}
	
	// Updating the food
	std::list<Food>::iterator iFood = foodList.begin();
	while (iFood != foodList.end())
	{
		if (iFood->isEaten())
			iFood = foodList.erase(iFood);
		else
			++iFood;
	}
	
	while(foodList.size() + BellyFoods + eggList.size() < mMaxBio)
	{
		Food food;
		food.spawn(GetRandPos());
		foodList.push_back(food);
	}


	buildViewMtx();
}

void Evolution::drawScene()
{
	// Clear the backbuffer and depth buffer.
	HR(gd3dDevice->Clear(0, 0, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0xffffffff, 1.0f, 0)); 

	HR(gd3dDevice->BeginScene());

	static char buffer[255];
	static RECT clientRect = {0, 0, 0, 0};
	GetClientRect(mhMainWnd, &clientRect);


	HR(gd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, true));
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHAREF, 25));
	HR(gd3dDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER));
	
	if (foodList.size() != 0 || eggList.size() != 0)
		drawEggsFood();

	if (lifeformList.size() != 0)
		drawLifeforms();
	

	if (mbDrawInfo)
	{		
		sprintf(buffer, "Carnivore: %d\n"
						"Carnivore Parts: %d\n"
						"Egg Cycle Length: %f\n"
						"Egg Time Length: %f\n"
						"Food Cycle Length: %f\n"
						"Lifetime Length: %f\n"
						"Sight Distance: %f\n"
						"Fear Distance: %f\n"
						"Pursuit Length: %f\n"
						"Speed: %f",
							mDrawInfo.carnivore,
							mDrawInfo.carnivoreParts,
							mDrawInfo.eggCycleLength,
							mDrawInfo.eggTimeLength,
							mDrawInfo.foodCycleLength,
							mDrawInfo.lifeTimeLength,
							mDrawInfo.sightDistance,
							mDrawInfo.fearDistance,
							mDrawInfo.pursuitLength,
							mDrawInfo.speed);
		RECT N = {clientRect.left, clientRect.top+(4*18), clientRect.left+300, clientRect.top+(14*18)};
		HR(mFont->DrawText(0, buffer, -1, &N, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(0, 0, 0)));
	}

	/*if (foodList.size() + BellyFoods + eggList.size() != mMaxBio || BellyFoods < 0)
	{
		sprintf(buffer, "BIOMASS LOST/GAINED");
		RECT N = {clientRect.left, clientRect.top+(4*18), clientRect.left+300, clientRect.top+(13*18)};
		HR(mFont->DrawText(0, buffer, -1, &N, DT_LEFT | DT_NOCLIP, D3DCOLOR_XRGB(255, 0, 0)));
	}*/

	mGfxStats->display();

	HR(gd3dDevice->SetRenderState(D3DRS_ALPHATESTENABLE, false));

	HR(gd3dDevice->EndScene());

	// Present the backbuffer.
	HR(gd3dDevice->Present(0, 0, 0, 0));
}

void Evolution::drawEggsFood()
{
	if (!mbEFStart)
	{
		mEFVB->Release();
		mEFIB->Release();
	}

	int Number = 0;
	int FNumber = 0;
	int ENumber = 0;

	const D3DXVECTOR3 baseV[4] = {D3DXVECTOR3(-1.00000f, -1.00000f, 0.0f),
						   D3DXVECTOR3(-1.00000f,  1.00000f, 0.0f),
						   D3DXVECTOR3( 1.00000f,  1.00000f, 0.0f),
						   D3DXVECTOR3( 1.00000f, -1.00000f, 0.0f)};

	const D3DXVECTOR2 baseT[4] = {D3DXVECTOR2(0.0f, 1.0f),
							D3DXVECTOR2(0.0f, 0.0f),
							D3DXVECTOR2(1.0f, 0.0f),
							D3DXVECTOR2(1.0f, 1.0f)};


	HR(gd3dDevice->CreateVertexBuffer((foodList.size()+eggList.size()) *4* sizeof(VertexPTEF), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &mEFVB, 0));
	VertexPTEF * efv = 0;
	HR(mEFVB->Lock(0, 0, (void**)&efv, 0));

	for (std::list<Food>::iterator it = foodList.begin(); it != foodList.end(); ++it)
	{
		D3DXVECTOR3 pos = it->getPosition();
		D3DXMATRIX T, S, F;
		D3DXVECTOR3 V[4];
		D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
		D3DXMatrixScaling(&S, 2.0f, 2.0f, 0.0f);
		F = S*(T);
		
		for (int k=0; k<4; ++k)
			D3DXVec3TransformCoord(&V[k], &baseV[k], &F);

		for (int k=0; k<4; ++k)
			efv[Number+k]   = VertexPTEF( V[k], baseT[k]);	

		//*v = iv; 
		//++fv;
		Number+=4;
		++FNumber;
	}

	/*if (eggList.size() != 0)
	{
	HR(gd3dDevice->CreateVertexBuffer(eggList.size() * sizeof(VertexPTEF), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &mEVB, 0));
	VertexPTEF * ev = 0;
	HR(mEVB->Lock(0, 0, (void**)&ev, 0));*/

	ENumber=FNumber;
	for (std::list<Egg>::iterator it = eggList.begin(); it != eggList.end(); ++it)
	{		
		D3DXVECTOR3 pos = it->getPosition();
		D3DXMATRIX T, S, F;
		D3DXVECTOR3 V[4];
		D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
		D3DXMatrixScaling(&S, 2.0f, 2.0f, 0.0f);
		F = S*(T);
		
		for (int k=0; k<4; ++k)
			D3DXVec3TransformCoord(&V[k], &baseV[k], &F);

		for (int k=0; k<4; ++k)
			efv[Number+k]   = VertexPTEF( V[k], baseT[k]);	

		//*v = iv; 
		//++ev;
		Number+=4;
		++ENumber;
	}

	HR(mEFVB->Unlock());

	Number = 0;
	
	HR(gd3dDevice->CreateIndexBuffer((foodList.size()+eggList.size()) *6* sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16,	D3DPOOL_MANAGED, &mEFIB, 0));

	WORD* find = 0;
	HR(mEFIB->Lock(0, 0, (void**)&find, 0));
	
	int k=0;
	int m=0;
	int g=0;
	while (m<FNumber)
	{
		find[k] = g; find[k+1] = g+1; find[k+2] = g+2;
		find[k+3] = g; find[k+4] = g+2; find[k+5] = g+3;
		k+=6;
		g+=4;
		++m;
	}

	int j=k;
	int n=m;
	int h=g;
	while (n<ENumber)
	{
		find[j] = h; find[j+1] = h+1; find[j+2] = h+2;
		find[j+3] = h; find[j+4] = h+2; find[j+5] = h+3;
		j+=6;
		h+=4;
		++n;
	}

	HR(mEFIB->Unlock());

	// Set up the geometry data stream
	
	HR(mFX->SetTechnique(mhEFTech));
	HR(mFX->SetTexture(mhTex,  mFoodTex));
	HR(gd3dDevice->SetStreamSource(0, mEFVB, 0, sizeof(VertexPTEF)));
	HR(gd3dDevice->SetIndices(mEFIB));
	HR(gd3dDevice->SetVertexDeclaration(VertexPTEF::Decl));

	HR(mFX->SetMatrix(mhWVP, &(mView*mProj)));
	
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for (UINT i=0; i<numPasses;++i)
	{
		HR(mFX->BeginPass(i));
		
		if (foodList.size() != 0)
			HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4*FNumber, 0, 2*FNumber));

		if (eggList.size() != 0)
		{
			HR(mFX->SetTexture(mhTex,  mEggTex));
			HR(mFX->CommitChanges());
			HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, FNumber*4, 4*(ENumber-FNumber), FNumber*6, 2*(ENumber-FNumber)));
		}
		HR(mFX->EndPass());
	}
	HR(mFX->End());

	if (mbEFStart)
		mbEFStart = false;
}

void Evolution::drawLifeforms()
{
	if (!mbLStart)
	{
		mLVB->Release();
		mLIB->Release();
	}

	int Number = 0;
	int LNumber = 0;

	const D3DXVECTOR3 baseV[4] = {D3DXVECTOR3(-1.00000f, -1.00000f, 0.0f),
						   D3DXVECTOR3(-1.00000f,  1.00000f, 0.0f),
						   D3DXVECTOR3( 1.00000f,  1.00000f, 0.0f),
						   D3DXVECTOR3( 1.00000f, -1.00000f, 0.0f)};

	const D3DXVECTOR2 baseT[4] = {D3DXVECTOR2(0.0f, 1.0f),
							D3DXVECTOR2(0.0f, 0.0f),
							D3DXVECTOR2(1.0f, 0.0f),
							D3DXVECTOR2(1.0f, 1.0f)};

	HR(gd3dDevice->CreateVertexBuffer((lifeformList.size()) *4* sizeof(VertexPTL), D3DUSAGE_WRITEONLY, 0, D3DPOOL_MANAGED, &mLVB, 0));
	VertexPTL * lv = 0;
	HR(mLVB->Lock(0, 0, (void**)&lv, 0));

	for (std::list<Lifeform>::iterator it = lifeformList.begin(); it != lifeformList.end(); ++it)
	{		
		Stat stat = it->getStat();
		float cParts = (float)(stat.carnivoreParts)/10.0f;
		if (stat.carnivoreParts >= 10)
			cParts = 1.0f;

		float sightDist = (stat.sightDistance)/10.0f;
		if (stat.sightDistance >= 10.0f)
			sightDist = 1.0f;	
		if (stat.sightDistance >= 10.0f && !(stat.carnivore))
			cParts = 0.0f;


		
		D3DXVECTOR3 pos = it->getPosition();
		D3DXMATRIX T, R, S, F;
		D3DXMatrixRotationZ(&R, it->getRotation());
		D3DXVECTOR3 V[4];
		D3DXMatrixTranslation(&T, pos.x, pos.y, pos.z);
		D3DXMatrixScaling(&S, 8.0f, 8.0f, 0.0f);
		F = S*(R*T);
		
		for (int k=0; k<4; ++k)
			D3DXVec3TransformCoord(&V[k], &baseV[k], &F);

		for (int k=0; k<4; ++k)
			lv[Number+k]   = VertexPTL( V[k], baseT[k], cParts, sightDist);	

		/*v[Number]   = VertexPTID(-1.0f, -1.0f, 0.0f, 0.0f, 1.0f, F, 2.0f);
		v[Number+1] = VertexPTID(-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, F, 2.0f);
		v[Number+2] = VertexPTID( 1.0f,  1.0f, 0.0f, 1.0f, 0.0f, F, 2.0f);
		v[Number+3] = VertexPTID( 1.0f, -1.0f, 0.0f, 1.0f, 1.0f, F, 2.0f);*/

		//++lv;
		Number+=4;
		++LNumber;
	}

	HR(mLVB->Unlock());
	//Number = 0;

	HR(gd3dDevice->CreateIndexBuffer(lifeformList.size() *6* sizeof(WORD), D3DUSAGE_WRITEONLY, D3DFMT_INDEX16, D3DPOOL_MANAGED, &mLIB, 0));

	WORD* lind = 0;
	HR(mLIB->Lock(0, 0, (void**)&lind, 0));
	int k=0;
	int g=0;
	for (int n=0; n<LNumber; ++n)
	{
		lind[k] = g; lind[k+1] = g+1; lind[k+2] = g+2;
		lind[k+3] = g; lind[k+4] = g+2; lind[k+5] = g+3;
		k+=6;
		g+=4;
	}
	
	HR(mLIB->Unlock());	


	HR(mFX->SetTechnique(mhLTech));
	HR(mFX->SetTexture(mhTex,  mLifeformTex));
	HR(mFX->SetTexture(mhTex2, mCarnTex));
	HR(mFX->SetTexture(mhTex3, mSightTex));

	HR(mFX->SetMatrix(mhWVP, &(mView*mProj)));
	
	HR(gd3dDevice->SetVertexDeclaration(VertexPTL::Decl));
	HR(gd3dDevice->SetStreamSource(0, mLVB, 0, sizeof(VertexPTL)));
	HR(gd3dDevice->SetIndices(mLIB));
	
	UINT numPasses = 0;
	HR(mFX->Begin(&numPasses, 0));
	for (UINT i=0; i<numPasses;++i)
	{
		HR(mFX->BeginPass(i));
		
		HR(gd3dDevice->DrawIndexedPrimitive(D3DPT_TRIANGLELIST, 0, 0, 4*LNumber, 0, 2*LNumber));
		
		HR(mFX->EndPass());
	}
	HR(mFX->End());
	
	if (mbLStart)
		mbLStart = false;
}


void Evolution::buildBuffers()
{
}

void Evolution::buildFX()
{
	ID3DXBuffer* errors = 0;
	if ( D3DXCreateEffectFromFile(gd3dDevice, "EvolutionShader.fx", 0, 0, D3DXSHADER_DEBUG, 0, &mFX, &errors) )
		if ( !errors )
		{
			MessageBox(0, "EvolutionShader.fx not found! D:", 0, 0);
			PostQuitMessage(0);
		}
	
	if( errors )
		MessageBox(0, (char*)errors->GetBufferPointer(), 0, 0);

	mhEFTech = mFX->GetTechniqueByName("EFEvolutionShader");
	mhLTech = mFX->GetTechniqueByName("LEvolutionShader");


	mhWVP			 = mFX->GetParameterByName(0, "gWVP");
	mhTex            = mFX->GetParameterByName(0, "gTex");
	mhTex2           = mFX->GetParameterByName(0, "gTex2");
	mhTex3           = mFX->GetParameterByName(0, "gTex3");

	HR(mFX->SetTexture(mhTex,  mFoodTex));
	HR(mFX->SetTexture(mhTex2, mEggTex));
	HR(mFX->SetTexture(mhTex3, mLifeformTex));
}


void Evolution::buildViewMtx()
{
	D3DXVECTOR3 pos(mCameraPos.x, mCameraPos.y, mCameraPos.z);
	D3DXVECTOR3 target(mCameraPos.x, mCameraPos.y, 0.0f);
	D3DXVECTOR3 up(0.0f, 1.0f, 0.0f);
	D3DXMatrixLookAtLH(&mView, &pos, &target, &up);
}

void Evolution::buildProjMtx()
{
	float w = (float)md3dPP.BackBufferWidth;
	float h = (float)md3dPP.BackBufferHeight;
	D3DXMatrixPerspectiveFovLH(&mProj, 0.717541f, w/h, 1.0f, MapX+1.0f);
}


/*
//-----------------------------------------------------------------------------
// Name: VS_HWInstancing
// Type: Vertex shader (HW Instancing)
// Desc: This shader computes standard transform and lighting for unlit, texture-mapped triangles.
//-----------------------------------------------------------------------------
void VS_HWInstancing( float4 vPos : POSITION,
					float3 vNormal : NORMAL,
					float2 vTex0 : TEXCOORD0,
					float4 vColor : COLOR0,
					float4 vBoxInstance : COLOR1,
					out float4 oPos : POSITION,
					out float4 oColor : COLOR0,
					out float2 oTex0 : TEXCOORD0 )
{
	//Use the fourth component of the vBoxInstance to rotate the box:
	vBoxInstance.w *= 2 * 3.1415;
	float4 vRotatedPos = vPos;
	vRotatedPos.x = vPos.x * cos(vBoxInstance.w) + vPos.z * sin(vBoxInstance.w);
	vRotatedPos.z = vPos.z * cos(vBoxInstance.w) - vPos.x * sin(vBoxInstance.w);
	
	//Use the instance position to offset the incoming box corner position:
	//  The "* 32 - 16" is to scale the incoming 0-1 intrapos range so that it maps to 8 box widths, covering
	//  the signed range -8 to 8. Boxes are 2 word units wide.
	vRotatedPos += float4( vBoxInstance.xyz * 32 - 16, 0 );
	
	// Transform the position from object space to homogeneous projection space
	oPos = mul( vRotatedPos, g_mWorld );
	oPos = mul( oPos, g_mView );
	oPos = mul( oPos, g_mProj );
	
	// Just copy the texture coordinate & color through
	oTex0 = vTex0;
	oColor = vColor;
}
*/

// Some Old shader code - might re-use later
/*
float3 c1 = tex2D(TexS4, tex0).rgb;
		float3 c2 = tex2D(TexS5, tex0).rgb;
		
		float totalInverse = 1.0f / (whichTex.z + whichTex.y);
		float alpha = (whichTex.y+whichTex.z);
		//float zAlpha = (whichTex.z);
		//float yAlpha = (whichTex.y);

		c1 *= whichTex.z * totalInverse;
		c2 *= whichTex.y * totalInverse;

		texColor.rgb = ((c1.rgb) + (c2.rgb))*alpha + texColor.rgb*(1.0f-alpha);
*/
