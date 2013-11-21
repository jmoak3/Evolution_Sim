//=============================================================================
// Vertex.h by Frank Luna (C) 2005 All Rights Reserved.
//
// This file contains all of our vertex structures.  We will add to it
// as needed throughout the book.
//=============================================================================

#include <d3dx9.h>

#ifndef VERTEX_H
#define VERTEX_H

// Call in constructor and destructor, respectively, of derived application class.
void InitAllVertexDeclarations();
void DestroyAllVertexDeclarations();


//===============================================================
struct VertexPos
{
	VertexPos():pos(0.0f, 0.0f, 0.0f){}
	VertexPos(float x, float y, float z):pos(x,y,z){}
	VertexPos(const D3DXVECTOR3& v):pos(v){}

	D3DXVECTOR3 pos;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexCol
{
	VertexCol():pos(0.0f, 0.0f, 0.0f),col(0x00000000){}
	VertexCol(float x, float y, float z, D3DCOLOR c):pos(x,y,z), col(c){}
	VertexCol(const D3DXVECTOR3& v, D3DCOLOR c):pos(v),col(c){}

	D3DXVECTOR3 pos;
	D3DCOLOR    col;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPN
{
	VertexPN()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f){}
	VertexPN(float x, float y, float z, 
		float nx, float ny, float nz):pos(x,y,z), normal(nx,ny,nz){}
	VertexPN(const D3DXVECTOR3& v, const D3DXVECTOR3& n)
		:pos(v),normal(n){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	static IDirect3DVertexDeclaration9* Decl;
};

//===============================================================
struct VertexPNT
{
	VertexPNT()
		:pos(0.0f, 0.0f, 0.0f),
		normal(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f){}
	VertexPNT(float x, float y, float z, 
		float nx, float ny, float nz,
		float u, float v):pos(x,y,z), normal(nx,ny,nz), tex0(u,v){}
	VertexPNT(const D3DXVECTOR3& v, const D3DXVECTOR3& n, const D3DXVECTOR2& uv)
		:pos(v),normal(n), tex0(uv){}

	D3DXVECTOR3 pos;
	D3DXVECTOR3 normal;
	D3DXVECTOR2 tex0;

	static IDirect3DVertexDeclaration9* Decl;
};

struct VertexPTID // RESOLVE WITH ID
{
	VertexPTID()
		:pos(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f),
		tex1(0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 0.0f), // Matricize
		tex5(0.0f, 0.0f, 0.0f){}
	//VertexPTID(float x, float y, float z,
	//		 float u, float v, float x1, float y1, float z1, float w1, 
	//		 float x2, float y2, float z2)
	//		 :pos(x,y,z), tex0(u,v), 
	//		 tex1(x1,y1,z1,w1), tex5(x2,y2,z2){}

	VertexPTID(const D3DXVECTOR3& v, const D3DXVECTOR2& uv, 
			   const D3DXMATRIX& t1, const D3DXVECTOR3& t5)// Matricize
				:pos(v), tex0(uv),
				tex1(t1), tex5(t5){}

	VertexPTID(float x, float y, float z,
			 float u, float v,
			 const D3DXMATRIX& t1, const D3DXVECTOR3& t5)// Matricize
				:pos(x,y,z), tex0(u,v),
				 tex1(t1), tex5(t5){}


	D3DXVECTOR3 pos; //12	
	D3DXVECTOR2 tex0; //8
	D3DXMATRIX tex1; //64
	D3DXVECTOR3 tex5; //12
	// 96/8 = 12
	static IDirect3DVertexDeclaration9* Decl;
};

struct VertexPTEF
{
	VertexPTEF()
		:pos(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f) {}
	//VertexPTID(float x, float y, float z,
	//		 float u, float v, float x1, float y1, float z1, float w1, 
	//		 float x2, float y2, float z2)
	//		 :pos(x,y,z), tex0(u,v), 
	//		 tex1(x1,y1,z1,w1), tex5(x2,y2,z2){}

	VertexPTEF(const D3DXVECTOR3& v, const D3DXVECTOR2& uv)
				:pos(v), tex0(uv) {}

	VertexPTEF(float x, float y, float z,
			 float u, float v)// Matricize
				:pos(x,y,z), tex0(u,v) {}


	D3DXVECTOR3 pos; //12	
	D3DXVECTOR2 tex0; //8

	static IDirect3DVertexDeclaration9* Decl;
};


struct VertexPTL
{
	VertexPTL()
		:pos(0.0f, 0.0f, 0.0f),
		tex0(0.0f, 0.0f),
		tex1(0.0f, 0.0f) {}
	//VertexPTID(float x, float y, float z,
	//		 float u, float v, float x1, float y1, float z1, float w1, 
	//		 float x2, float y2, float z2)
	//		 :pos(x,y,z), tex0(u,v), 
	//		 tex1(x1,y1,z1,w1), tex5(x2,y2,z2){}

	VertexPTL(const D3DXVECTOR3& v, const D3DXVECTOR2& uv, 
			   const D3DXMATRIX& t1)// Matricize
				:pos(v), tex0(uv),
				tex1(t1){}

	VertexPTL(const D3DXVECTOR3& v, const D3DXVECTOR2& uv, 
			  float a, float b)// Matricize
				:pos(v), tex0(uv),
				tex1(a, b){}

	VertexPTL(float x, float y, float z,
			 float u, float v, float a, float b)
			 :pos(x,y,z), tex0(u,v),
				 tex1(a, b){}


	D3DXVECTOR3 pos; //12	
	D3DXVECTOR2 tex0; //8
	D3DXVECTOR2 tex1; //64
	// 96/8 = 12
	static IDirect3DVertexDeclaration9* Decl;
};

#endif // VERTEX_H