#include "Chrysler_Saratoga.h"
#include "Chrysler_Saratoga_pieces.h"
#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

Chrysler_Saratoga::Chrysler_Saratoga()
{
	model = glm::mat4(1.0f);

	mtl[0] = new CGMaterial();
	mtl[0]->SetAmbientReflect(0.588f, 0.588f, 0.588f);
	mtl[0]->SetDifusseReflect(0.588f, 0.588f, 0.588f);
	mtl[0]->SetSpecularReflect(0.0f, 0.0f, 0.0f);
	mtl[0]->SetShininess(10.0f);
	mtl[0]->InitTexture("textures/Body_Base_Color.png");

	mtl[1] = new CGMaterial();
	mtl[1]->SetAmbientReflect(0.588f, 0.588f, 0.588f);
	mtl[1]->SetDifusseReflect(0.588f, 0.588f, 0.588f);
	mtl[1]->SetSpecularReflect(0.0f, 0.0f, 0.0f);
	mtl[1]->SetShininess(10.0f);
	mtl[1]->InitTexture("textures/Glass_Base_Color.png");

	mtl[2] = new CGMaterial();
	mtl[2]->SetAmbientReflect(0.588f, 0.588f, 0.588f);
	mtl[2]->SetDifusseReflect(0.588f, 0.588f, 0.588f);
	mtl[2]->SetSpecularReflect(0.0f, 0.0f, 0.0f);
	mtl[2]->SetShininess(10.0f);
	mtl[2]->InitTexture("textures/Tire_Base_Color.png");

	pieces[0] = new Chrysler_Saratoga_0(mtl[0]);
	pieces[1] = new Chrysler_Saratoga_1(mtl[2]);
	pieces[2] = new Chrysler_Saratoga_2(mtl[2]);
	pieces[3] = new Chrysler_Saratoga_3(mtl[1]);
}

Chrysler_Saratoga::~Chrysler_Saratoga()
{
	for (int i = 0; i < 4; i++) delete pieces[i];
	delete[] pieces;
	for (int i = 0; i < 3; i++) delete mtl[i];
	delete[] mtl;
}

int Chrysler_Saratoga::GetNumPieces()
{
	return 4;
}

CGPiece* Chrysler_Saratoga::GetPiece(int index)
{
	return pieces[index];
}

