#include "CGScene.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CGShaderProgram.h"
#include "CGFigure.h"
#include "CGLight.h"
#include "CGMaterial.h"
#include "Porteria.h"
#include "CGCone.h"
#include "CGCylinder.h"
#include "CGSphere.h"
#include "CGTorus.h"
#include "CGIcosahedron.h"
#include "CGGround.h"
#include "resource.h"
#include "Mercedes_G_Class.h"
#include "Chrysler_Saratoga.h"

//
// FUNCIÓN: CGScene::CGScene()
//
// PROPÓSITO: Construye el objeto que representa la escena
//
CGScene::CGScene()
{
    glm::vec3 Ldir = glm::vec3(0.0f, -0.8f, -1.0f);
    Ldir = glm::normalize(Ldir);
    light = new CGLight();
    light->SetLightDirection(Ldir);
    light->SetAmbientLight(glm::vec3(0.2f, 0.2f, 0.2f));
    light->SetDifusseLight(glm::vec3(0.8f, 0.8f, 0.8f));
    light->SetSpecularLight(glm::vec3(1.0f, 1.0f, 1.0f));

    object = new Chrysler_Saratoga(); // COCHE
    object->Translate(glm::vec3(0.0f, 0.0f, 0.0f));
    object->Rotate(-180.0f, glm::vec3(0.0f, 10.0f, 0.0f));

    /*object = new Mercedes_G_Class(); // COCHE
    object->Translate(glm::vec3(0.0f, 1.0f, 0.0f));
    object->Rotate(180.0f, glm::vec3(0.0f, 12.0f, 0.0f));*/


    matg = new CGMaterial();
    matg->SetAmbientReflect(1.0f, 1.0f, 1.0f);
    matg->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    matg->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    matg->SetShininess(16.0f);
    matg->InitTexture("textures/prueba.jpg");
    //matg->InitTexture(IDR_IMAGE2);

    ground = new CGGround(128.0f, 155.0f);
    ground->SetMaterial(matg);

    matg2 = new CGMaterial();  //fondos
    matg2->SetAmbientReflect(5.0f, 5.0f, 5.0f);
    matg2->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    matg2->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    matg2->SetShininess(16.0f);
    matg2->InitTexture("textures/roca1.jpg");
    //matg->InitTexture(IDR_IMAGE2);

    matg3 = new CGMaterial();  //pared porteria
    matg3->SetAmbientReflect(5.0f, 5.0f, 5.0f);
    matg3->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    matg3->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    matg3->SetShininess(16.0f);
    matg3->InitTexture("textures/roca3.jpg");
    //matg->InitTexture(IDR_IMAGE2);


    matg4 = new CGMaterial(); // porteria
    matg4->SetAmbientReflect(3.0f, 3.0f, 3.0f);
    matg4->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    matg4->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    matg4->SetShininess(16.0f);
    matg4->InitTexture("textures/roca2.jpg");
    //matg->InitTexture(IDR_IMAGE2);


    pared1 = new CGGround(38.0f, 155.0f); //el 25 es la altura de la pared 
    pared1->SetMaterial(matg2);
    pared1->Translate(glm::vec3(-128.0f, 38.0f, 0.0f));
    pared1->Rotate(-90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    pared2 = new CGGround(38.0f, 155.0f);
    pared2->SetMaterial(matg2);
    pared2->Translate(glm::vec3(128.0f, 38.0f, 0.0f));
    pared2->Rotate(90.0f, glm::vec3(0.0f, 0.0f, 1.0f));

    pared3 = new CGGround(128.0f, 38.0f);
    pared3->SetMaterial(matg4);
    pared3->Translate(glm::vec3(0.0f, 38.0f, 155.0f));
    pared3->Rotate(-90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    pared4 = new CGGround(40.0f, 38.0f);
    pared4->SetMaterial(matg2);
    pared4->Translate(glm::vec3(-88.0f, 38.0f, -155.0f));
    pared4->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    pared5 = new CGGround(42.0f, 38.0f);
    pared5->SetMaterial(matg2);
    pared5->Translate(glm::vec3(86.0f, 38.0f, -155.0f));
    pared5->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    pared6 = new CGGround(46.0f, 18.0f);
    pared6->SetMaterial(matg3);
    pared6->Translate(glm::vec3(-2.0f, 58.0f, -155.0f));
    pared6->Rotate(90.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    porteria = new Porteria(20.0f, 15.0f);
    porteria->SetMaterial(matg4);
    porteria->Translate(glm::vec3(-32.0f, 20.0f, -170.0f));
    porteria->Rotate(180.0f, glm::vec3(1.0f, 0.0f, 0.0f));

    mat4 = new CGMaterial();
    mat4->SetAmbientReflect(1.0f, 1.0f, 1.0f);
    mat4->SetDifusseReflect(1.0f, 1.0f, 1.0f);
    mat4->SetSpecularReflect(0.8f, 0.8f, 0.8f);
    mat4->SetShininess(16.0f);
    mat4->InitTexture("textures/ball.jpg"); //textura pelota
    //mat4->InitTexture(IDR_IMAGE3);

    fig4 = new CGSphere(20, 40, 6.5f); //pelota
    fig4->SetMaterial(mat4);
    fig4->Translate(glm::vec3(2.0f, 30.0f, -50.0f));
}

//
// FUNCIÓN: CGScene3:~CGScene()
//
// PROPÓSITO: Destruye el objeto que representa la escena
//
CGScene::~CGScene()
{
    delete ground;
    delete fig4;
    delete light;
    delete matg;
    delete matg2;
    delete matg3;
    delete matg4;
    delete mat4;
    delete object;
    delete pared1;
    delete pared2;
    delete pared3;
    delete pared4;
    delete pared5;
    delete pared6;
    delete porteria;
}   


//
// FUNCIÓN: CGScene::Draw()
//
// PROPÓSITO: Dibuja la escena
//
void CGScene::Draw(CGShaderProgram* program, glm::mat4 proj, glm::mat4 view, glm::mat4 shadowViewMatrix)
{
    light->SetUniforms(program);
    object->Draw(program, proj, view);
    pared1->Draw(program, proj, view, shadowViewMatrix);
    pared2->Draw(program, proj, view, shadowViewMatrix);
    pared3->Draw(program, proj, view, shadowViewMatrix);
    pared4->Draw(program, proj, view, shadowViewMatrix);
    pared5->Draw(program, proj, view, shadowViewMatrix);
    pared6->Draw(program, proj, view, shadowViewMatrix);
    porteria->Draw(program, proj, view, shadowViewMatrix);
    ground->Draw(program, proj, view, shadowViewMatrix);
    fig4->Draw(program, proj, view, shadowViewMatrix);
}

//
// FUNCIÓN: CGScene::DrawShadow()
//
// PROPÓSITO: Dibuja las sombras de la escena
//
void CGScene::DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix)
{
    fig4->DrawShadow(program, shadowMatrix);
    pared1->DrawShadow(program, shadowMatrix);
    pared2->DrawShadow(program, shadowMatrix);
    pared3->DrawShadow(program, shadowMatrix);
    pared4->DrawShadow(program, shadowMatrix);
    pared5->DrawShadow(program, shadowMatrix);
    pared6->DrawShadow(program, shadowMatrix);
    object->DrawShadow(program, shadowMatrix);
}

//
// FUNCIÓN: CGScene::GetLightViewMatrix()
//
// PROPÓSITO: Obtiene la matriz de posicionamiento de la luz
//
glm::mat4 CGScene::GetLightViewMatrix()
{
    glm::vec3 Zdir = -(light->GetLightDirection());
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 Xdir = glm::normalize(glm::cross(Up, Zdir));
    glm::vec3 Ydir = glm::cross(Zdir, Xdir);
    glm::vec3 Zpos = 150.0f * Zdir;
    glm::vec3 Center = glm::vec3(0.0f, 0.0f, 0.0f);

    glm::mat4 view = glm::lookAt(Zpos, Center, Ydir);
    return view;
}

CGObject* CGScene::getCoche()
{
    return object;
}

CGFigure* CGScene::getBalon()
{
    return fig4;
}


