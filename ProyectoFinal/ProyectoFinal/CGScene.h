#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "CGShaderProgram.h"
#include "CGLight.h"
#include "CGMaterial.h"
#include "CGFigure.h"
#include "CGObject.h"


class CGScene {
public:
    CGScene();
    ~CGScene();
    void Draw(CGShaderProgram* program, glm::mat4 proj, glm::mat4 view, glm::mat4 shadowViewMatrix);
    void DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix);
    glm::mat4 GetLightViewMatrix();

    CGObject* getCoche();
    CGFigure* getBalon();


private:
    CGFigure* ground;
    //CGFigure* fig0;
    //CGFigure* fig1;
    //CGFigure* fig2;
    //CGFigure* fig3;
    CGFigure* fig4;
    //CGFigure* fig5;
    CGMaterial* matb;
    CGMaterial* matg2;
    CGMaterial* matg3;
    CGMaterial* matg4;
    CGMaterial* matg;
    CGMaterial* mat0;
    CGMaterial* mat1;
    CGMaterial* mat2;
    CGMaterial* mat3;
    CGMaterial* mat4;
    CGMaterial* mat5;
    CGLight* light;
    CGObject* object;
    CGFigure* pared1;
    CGFigure* pared2;
    CGFigure* pared3;
    CGFigure* pared4;
    CGFigure* pared5;
    CGFigure* pared6;
    CGFigure* porteria;

};
