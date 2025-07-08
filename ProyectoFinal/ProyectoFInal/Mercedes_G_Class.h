#pragma once

#include <GL/glew.h>
#include "CGObject.h"
#include "CGPiece.h"

class Mercedes_G_Class : public CGObject {
private:
	CGMaterial* mtl[1];
	CGPiece* pieces[15];

public:
	Mercedes_G_Class();
	~Mercedes_G_Class();
	virtual int GetNumPieces();
	virtual CGPiece* GetPiece(int i);
};

