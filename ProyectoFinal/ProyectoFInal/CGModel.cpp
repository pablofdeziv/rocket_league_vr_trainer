#include "CGModel.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <GLFW/glfw3.h>
#include "CGCamera.h"
#include "CGScene.h"
#include "CGSkybox.h"
#include "resource.h"
#include <iostream>


//
// FUNCIÓN: CGModel::initialize(int, int)
//
// PROPÓSITO: Initializa el modelo 3D
//
void CGModel::initialize(int w, int h)
{
    // Crea el programa gráfico para el entorno
    skyboxProgram = new CGShaderProgram(IDR_SHADER1, IDR_SHADER2, -1, -1, -1);
    if (skyboxProgram->IsLinked() == GL_FALSE) return;

    // Crea el programa gráfico para la sombra
    shadowProgram = new CGShaderProgram(IDR_SHADER5, IDR_SHADER6, -1, -1, -1);
    if (shadowProgram->IsLinked() == GL_FALSE) return;

    // Crea el programa gráfico para la escena
    sceneProgram = new CGShaderProgram(IDR_SHADER3, IDR_SHADER4, -1, -1, -1);
    if (sceneProgram->IsLinked() == GL_FALSE) return;

    // Crea la cámara
    camera = new CGCamera();
    camera->SetPosition(0.0f, 20.0f, 100.0f);

    // Crea el skybox
    skybox = new CGSkybox();

    // Crea la escena
    scene = new CGScene();

    // Crea el Framebuffer de la sombra
    bool frameBufferStatus = InitShadowMap();
    if (!frameBufferStatus) return;

    // Asigna el viewport y el clipping volume
    resize(w, h);

    // Opciones de dibujo
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CCW);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

//
// FUNCIÓN: CGModel::finalize()
//
// PROPÓSITO: Libera los recursos del modelo 3D
//
void CGModel::finalize()
{
    delete camera;
    delete scene;
    delete skybox;
    delete sceneProgram;
    delete skyboxProgram;
    delete shadowProgram;
}

//
// FUNCIÓN: CGModel::resize(int w, int h)
//
// PROPÓSITO: Asigna el viewport y el clipping volume
//
void CGModel::resize(int w, int h)
{
    double fov = glm::radians(15.0);
    double sin_fov = sin(fov);
    double cos_fov = cos(fov);
    if (h == 0) h = 1;
    GLfloat aspectRatio = (GLfloat)w / (GLfloat)h;
    GLfloat wHeight = (GLfloat)(sin_fov * 0.2 / cos_fov);
    GLfloat wWidth = wHeight * aspectRatio;

    wndWidth = w;
    wndHeight = h;

    glViewport(0, 0, w, h);
    projection = glm::frustum(-wWidth, wWidth, -wHeight, wHeight, 0.2f, 400.0f);
}

//
// FUNCIÓN: CGModel::render()
//
// PROPÓSITO: Genera la imagen
//
void CGModel::render()
{
    //*********************************************************//
    //                  Genera el ShadowMap                    //
    //*********************************************************//

    // Activa el framebuffer de la sombra
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    // Activa el programa que genera el shadowmap
    shadowProgram->Use();

    // Asigna las matrices Viewport, View y Projection de la luz.
    glm::mat4 lightViewMatrix = scene->GetLightViewMatrix();
    glm::mat4 lightPerspective = glm::ortho(-150.0f, 150.0f, -150.0f, 150.0f, 0.0f, 400.0f);
    glm::mat4 lightMVP = lightPerspective * lightViewMatrix;

    // Limpia la información de profundidad
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activa front-face culling
    glCullFace(GL_FRONT);

    //Asigna el viewport
    glViewport(0, 0, 1024, 1024);

    // Dibuja la escena
    scene->DrawShadow(shadowProgram, lightMVP);

    //*********************************************************//
    //                  Dibuja el skybox                       //
    //*********************************************************//
    
    // Activa el framebuffer de la imagen
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // Limpia el framebuffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Activa back-face culling
    glCullFace(GL_BACK);

    // Asigna el viewport
    glViewport(0, 0, wndWidth, wndHeight);

    // Dibuja el skybox
    glm::mat4 view = camera->ViewMatrix();
    skyboxProgram->Use();
    skybox->Draw(skyboxProgram, projection, view);

    //*********************************************************//
    //                  Dibuja la escena                       //
    //*********************************************************//

    // Activa el programa de la escena
    sceneProgram->Use();
    sceneProgram->SetUniformI("ShadowMap", 1);

    // Dibuja la escena
    glm::mat4 viewMatrix = camera->ViewMatrix();
    scene->Draw(sceneProgram, projection, viewMatrix, lightMVP);
}

//
// FUNCIÓN: CGModel::update()
//
// PROPÓSITO: Anima la escena
//
void CGModel::update()
{
    //camera->MoveFront();
    //CameraConstraints();
    scene->getCoche()->MoveFront();
    scene->getBalon()->MoveFront();
    CarConstraints();
    colocarCamara();
    BallHits();
    BallConstraints();
    CarConstraints();

}

//
// FUNCIÓN: CGModel::key_pressed(int)
//
// PROPÓSITO: Respuesta a acciones de teclado
//
void CGModel::key_pressed(int key)
{
    switch (key)
    {
    case GLFW_KEY_UP:
        scene->getCoche()->SetMoveStep(scene->getCoche()->GetMoveStep() + 0.2f);
        break;
    case GLFW_KEY_DOWN:
        scene->getCoche()->SetMoveStep(scene->getCoche()->GetMoveStep() - 0.2f);
        break;
    /*case GLFW_KEY_LEFT:
        if(scene->getCoche()->GetMoveStep() >= 0.4 || scene->getCoche()->GetMoveStep() <= -0.4)
           scene->getCoche()->Rotate(4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case GLFW_KEY_RIGHT:
        if (scene->getCoche()->GetMoveStep() >= 0.4 || scene->getCoche()->GetMoveStep() <= -0.4)
            scene->getCoche()->Rotate(-4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    }*/
    case GLFW_KEY_LEFT:
        //if (scene->getCoche()->GetMoveStep() >= 0.15 || scene->getCoche()->GetMoveStep() <= 0.15)
            scene->getCoche()->Rotate(4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    case GLFW_KEY_RIGHT:
        //if (scene->getCoche()->GetMoveStep() >= 0.15 || scene->getCoche()->GetMoveStep() <= 0.15)
            scene->getCoche()->Rotate(-4.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        break;
    }
}

//
//  FUNCIÓN: CGModel:::mouse_button(int button, int action)
//
//  PROPÓSITO: Respuesta del modelo a un click del ratón.
//
void CGModel::mouse_button(int button, int action)
{
}

//
//  FUNCIÓN: CGModel::mouse_move(double xpos, double ypos)
//
//  PROPÓSITO: Respuesta del modelo a un movimiento del ratón.
//
void CGModel::mouse_move(double xpos, double ypos)
{
}

void CGModel::colocarCamara() {

    glm::vec3 posicionReal = scene->getCoche()->GetRealPosition();
    glm::vec3 forward = scene->getCoche()->getForwardDirection();
    glm::vec3 up = scene->getCoche()->getUpDirection();
    glm::vec3 r = scene->getCoche()->getRightDirection();

    
    glm::vec3 cameraPosition = posicionReal - forward * 70.0f + up * 15.0f;
    
    camera->SetPosition(cameraPosition.x, (cameraPosition.y), (cameraPosition.z)); 

    camera->SetDir(-forward);
}

void CGModel::BallHits() {
    glm::vec3 posB = scene->getBalon()->GetRealPosition();
    glm::vec3 posC = scene->getCoche()->GetRealPosition();

    float dx = posB.x - posC.x;
    float dy = posB.y - posC.y;
    float dz = posB.z - posC.z;
    float distanciaXYZ = sqrt((dx * dx) + (dz * dz) + (dy * dy));

    if (distanciaXYZ <= 15.0f) {
        if(scene->getCoche()->GetMoveStep() < 0.0f)
            scene->getBalon()->SetDireccion(glm::vec3(-dx, -dy, -dz));
        else
            scene->getBalon()->SetDireccion(glm::vec3(dx, dy, dz));

        scene->getBalon()->SetMoveStep(scene->getCoche()->GetMoveStep() * 0.3f);
        scene->getBalon()->SetupStep(0.25f);
    }

}


void CGModel::CarConstraints() {
    glm::vec3 posS = scene->getCoche()->GetRealPosition();
    int constraint = 0;

    //std::cout << " movestep: " << scene->getCoche()->GetMoveStep() << std::endl;
    //std::cout << " x: " << posS.x << " y: " << posS.y << " z: " << posS.z << std::endl;

    if (fueracoche == true && scene->getCoche()->GetMoveStep() > 0)
        scene->getCoche()->SetMoveStep(-(scene->getCoche()->GetMoveStep()));

    if (marchaatras == true && scene->getCoche()->GetMoveStep() < 0)
        scene->getCoche()->SetMoveStep(-(scene->getCoche()->GetMoveStep()));

    if (posS.x > 113.0f) {
        constraint = 1;
    }
    if (posS.x < -113.0f) {
        constraint = 1;
    }
    if (posS.z > 145.0f) {
        constraint = 1;
    }
    if ((posS.x > 40 || posS.x < -40) && (posS.z < -145))
        constraint = 1;

    if ((posS.x < 40 || posS.x < -40) && (posS.z < -180))
        constraint = 1;
            
    

    if (constraint == 1) {
        if (scene->getCoche()->GetMoveStep() > 0 && !marchaatras && !fueracoche) {
            scene->getCoche()->SetMoveStep(-(scene->getCoche()->GetMoveStep() * 0.3f));
            fueracoche = true;
            
        }
        else if (scene->getCoche()->GetMoveStep() < 0 && marchaatras == false && !fueracoche)
        {
            scene->getCoche()->SetMoveStep(-(scene->getCoche()->GetMoveStep() * 0.4f));
            marchaatras = true;
        }
        
    }
    else if (constraint == 0) {
        fueracoche = false;
        marchaatras = false;
    }
}

void CGModel::BallConstraints() {  //cambiar
    glm::vec3 posS = scene->getBalon()->GetRealPosition();
    int constraint = 0;

    //std::cout << " movestep: " << scene->getBalon()->GetMoveStep() << std::endl;


    if (avancePelota == true && scene->getBalon()->GetMoveStep() > 0)
        scene->getBalon()->SetMoveStep(-(scene->getBalon()->GetMoveStep()));

    if (retrocesoPelota == true && scene->getBalon()->GetMoveStep() < 0)
        scene->getBalon()->SetMoveStep(-(scene->getBalon()->GetMoveStep()));

    if (posS.x > 113.0f) {
        constraint = 1;
    }
    if (posS.x < -113.0f) {
        constraint = 1;
    }
    if (posS.z > 145.0f) {
        constraint = 1;
    }
    if ((posS.x > 40 || posS.x < -40) && (posS.z < -145))
        constraint = 1;

    if ((posS.x < 40 || posS.x < -40) && (posS.z < -180))
        constraint = 1;

    if (constraint == 1) {
        if (scene->getBalon()->GetMoveStep() > 0 && !retrocesoPelota && !avancePelota) {
            scene->getBalon()->SetMoveStep(-(scene->getBalon()->GetMoveStep() * 0.4f));
            avancePelota = true;
        }
        else if (scene->getBalon()->GetMoveStep() < 0 && !avancePelota && !retrocesoPelota){
            scene->getBalon()->SetMoveStep(-(scene->getBalon()->GetMoveStep() * 0.4f));
            retrocesoPelota = true;
        }
    }
    else if (constraint == 0) {
        avancePelota = false;
        retrocesoPelota = false;
    }

    if ((posS.x < 40 || posS.x < -40) && (posS.z < -164)) {
        std::cout << " GOOOOOOOOOOOOOOOL " << std::endl;
        exit(0);
    }

}




//
// FUNCIÓN: CGModel::InitShadowMap()
//
// PROPÓSITO: Inicializa el FBO para almacenar la textura de sombra
//
bool CGModel::InitShadowMap()
{
    GLfloat border[] = { 1.0f, 1.0f, 1.0f, 1.0f };
    GLsizei shadowMapWidth = 1024;
    GLsizei shadowMapHeight = 1024;

    glGenFramebuffers(1, &shadowFBO);
    glBindFramebuffer(GL_FRAMEBUFFER, shadowFBO);

    glGenTextures(1, &depthTexId);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthTexId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT24, shadowMapWidth,
        shadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, border);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LESS);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D, depthTexId, 0);

    glDrawBuffer(GL_NONE);

    bool result = true;
    if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
    {
        result = false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return result;
}

