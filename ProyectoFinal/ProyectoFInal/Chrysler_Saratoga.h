#pragma once

#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

class Chrysler_Saratoga : public CGObject {
private:
	CGMaterial* mtl[3];
	CGPiece* pieces[4];

public:
	Chrysler_Saratoga();
	~Chrysler_Saratoga();
	virtual int GetNumPieces();
	virtual CGPiece* GetPiece(int i);
};

