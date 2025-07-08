#pragma once

#include <GL/glew.h>
#include "CGFigure.h"

//
// CLASE: CGCone
//
// DESCRIPCI�N: Representa un cono de altura 'h', radio 'r', 
//              dividido en 'm' l�neas y 'p' rodajas. 
//
class CGCone : public CGFigure {
public:
	CGCone(GLint p, GLint m, GLfloat h, GLfloat r);
};

