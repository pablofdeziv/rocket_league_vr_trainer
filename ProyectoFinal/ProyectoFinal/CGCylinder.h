#pragma once

#include <GL/glew.h>
#include "CGFigure.h"

//
// CLASE: CGCylinder
//
// DESCRIPCI�N: Representa un cilindro de radio 'r', longitud '2�l', 
//              dividido en 'p' capas y 'm' l�neas.
//
class CGCylinder : public CGFigure {
public:
	CGCylinder(GLint p, GLint m, GLfloat r, GLfloat l);
};

