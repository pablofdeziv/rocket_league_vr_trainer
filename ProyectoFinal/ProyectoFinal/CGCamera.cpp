#include "CGCamera.h"
#include <GL/glew.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

//
// FUNCI�N: CGCamera::CGCamera()
//
// PROP�SITO: Construye una c�mara
//
// COMENTARIOS: 
//     La posici�n inicial es (0,0,0).
//     La orientaci�n incial es el sistema de coordenadas del modelo
//     El tama�o del paso inicial es 0.1
//     El tama�o del giro inicial es 1.0 grados
//
CGCamera::CGCamera()
{
    Pos = glm::vec3(0.0f, 0.0f, 0.0f);
    Dir = glm::vec3(0.0f, 0.0f, 1.0f);
    Up = glm::vec3(0.0f, 1.0f, 0.0f);
    Right = glm::vec3(1.0f, 0.0f, 0.0f);

    moveStep = 0.0f;
    turnStep = 1.0f;
    cosAngle = (GLfloat)cos(glm::radians(turnStep));
    sinAngle = (GLfloat)sin(glm::radians(turnStep));
}

//
// FUNCI�N: CGCamera::ViewMatrix()
//
// PROP�SITO: Obtiene la matriz View para situar la c�mara.
//
glm::mat4 CGCamera::ViewMatrix()
{
    return glm::lookAt(Pos, Pos - Dir, Up);
}

//
// FUNCI�N: CGCamera::SetPosition(GLfloat x, GLfloat y, GLfloat z)
//
// PROP�SITO: Asigna la posici�n de la c�mara con respecto al 
//            sistema de coordenadas del modelo.
//
void CGCamera::SetPosition(GLfloat x, GLfloat y, GLfloat z)
{
    Pos = glm::vec3(x, y, z);
}

//
// FUNCI�N: CGCamera::SetDirection(GLfloat xDir, GLfloat yDir, GLfloat zDir, 
//                                 GLfloat xUp, GLfloat yUp, GLfloat zUp)
//
// PROP�SITO: Asigna la orientaci�n de la c�mara.
//
void CGCamera::SetDirection(GLfloat xD, GLfloat yD, GLfloat zD, GLfloat xU, GLfloat yU, GLfloat zU)
{
    Dir = glm::vec3(xD, yD, zD);
    Up = glm::vec3(xU, yU, zU);
    Right = glm::cross(Up, Dir);
}

//
// FUNCI�N: CGCamera::SetMoveStep(GLfloat step)
//
// PROP�SITO: Asigna el avance en cada paso.
//
void CGCamera::SetMoveStep(GLfloat step)
{
    moveStep = step;
}

//
// FUNCI�N: CGCamera::SetTurnStep(GLfloat step)
//
// PROP�SITO: Asigna el �ngulo de giro en cada paso.
//
void CGCamera::SetTurnStep(GLfloat step)
{
    turnStep = step;
    cosAngle = (GLfloat)cos(glm::radians(turnStep));
    sinAngle = (GLfloat)sin(glm::radians(turnStep));
}

//
// FUNCI�N: CGCamera::GetPosition()
//
// PROP�SITO: Obtiene la posici�n de la c�mara.
//
glm::vec3 CGCamera::GetPosition()
{
    return Pos;
}

//
// FUNCI�N: CGCamera::GetDirection()
//
// PROP�SITO: Obtiene la orientaci�n de la c�mara (eje Z).
//
glm::vec3 CGCamera::GetDirection()
{
    return Dir;
}

//
// FUNCI�N: CGCamera::GetUpDirection()
//
// PROP�SITO: Obtiene la orientaci�n cenital de la c�mara (eje Y).
//
glm::vec3 CGCamera::GetUpDirection()
{
    return Up;
}

//
// FUNCI�N: CGCamera::GetMoveStep()
//
// PROP�SITO: Obtiene el avance en cada paso.
//
GLfloat CGCamera::GetMoveStep()
{
    return moveStep;
}

//
// FUNCI�N: CGCamera::GetTurnStep()
//
// PROP�SITO: Obtiene el �ngulo de giro en cada paso.
//
GLfloat CGCamera::GetTurnStep()
{
    return turnStep;
}

//
// FUNCI�N: CGCamera::MoveFront()
//
// PROP�SITO: Mueve el observador un paso (moveStep) en la direcci�n -Dir 
//
//
void CGCamera::MoveFront()
{
    Pos -= moveStep * Dir;
}

//
// FUNCI�N: CGCamera::MoveBack()
//
// PROP�SITO: Mueve el observador un paso (moveStep) hacia atr�s en la direcci�n Dir 
//
//
void CGCamera::MoveBack()
{
    Pos += moveStep * Dir;
}

//
// FUNCI�N: CGCamera::MoveLeft()
//
// PROP�SITO: Mueve el observador un paso (moveStep) hacia la izquierda. 
//
//
void CGCamera::MoveLeft()
{
    Pos -= moveStep * Right;
}

//
// FUNCI�N: CGCamera::MoveRight()
//
// PROP�SITO: Mueve el observador un paso (moveStep) hacia la derecha. 
//
//
void CGCamera::MoveRight()
{
    Pos += moveStep * Right;
}

//
// FUNCI�N: CGCamera::MoveUp()
//
// PROP�SITO: Mueve el observador un paso (moveStep) hacia arriba. 
//
//
void CGCamera::MoveUp()
{
    Pos += moveStep * Up;
}

//
// FUNCI�N: CGCamera::MoveDown()
//
// PROP�SITO: Mueve el observador un paso (moveStep) hacia abajo. 
//
void CGCamera::MoveDown()
{
    Pos -= moveStep * Up;
}

//
// FUNCI�N: CGCamera::TurnRight()
//
// PROP�SITO: Rota el observador un paso (angleStep) hacia su derecha.
//
void CGCamera::TurnRight()
{
    Dir.x = cosAngle * Dir.x - sinAngle * Right.x;
    Dir.y = cosAngle * Dir.y - sinAngle * Right.y;
    Dir.z = cosAngle * Dir.z - sinAngle * Right.z;

    // Right = Up x Dir
    Right = glm::cross(Up, Dir);
}

//
// FUNCI�N: CGCamera::TurnLeft()
//
// PROP�SITO: Rota el observador un paso (angleStep) hacia su izquierda.
//
void CGCamera::TurnLeft()
{
    Dir.x = cosAngle * Dir.x + sinAngle * Right.x;
    Dir.y = cosAngle * Dir.y + sinAngle * Right.y;
    Dir.z = cosAngle * Dir.z + sinAngle * Right.z;

    // Right = Up x Dir
    Right = glm::cross(Up, Dir);
}

//
// FUNCI�N: CGCamera::TurnUp()
//
// PROP�SITO: Rota el observador un paso (angleStep) hacia arriba.
//
void CGCamera::TurnUp()
{
    Dir.x = cosAngle * Dir.x - sinAngle * Up.x;
    Dir.y = cosAngle * Dir.y - sinAngle * Up.y;
    Dir.z = cosAngle * Dir.z - sinAngle * Up.z;

    // Up = Dir x Right
    Up = glm::cross(Dir, Right);
}

//
// FUNCI�N: CGCamera::TurnDown()
//
// PROP�SITO: Rota el observador un paso (angleStep) hacia abajo.
//
void CGCamera::TurnDown()
{
    Dir.x = cosAngle * Dir.x + sinAngle * Up.x;
    Dir.y = cosAngle * Dir.y + sinAngle * Up.y;
    Dir.z = cosAngle * Dir.z + sinAngle * Up.z;

    // Up = Dir x Right
    Up = glm::cross(Dir, Right);
}

//
// FUNCI�N: CGCamera::TurnCW()
//
// PROP�SITO: Rota el observador un paso (angleStep) en sentido del reloj.
//
void CGCamera::TurnCW()
{
    Up.x = cosAngle * Up.x + sinAngle * Right.x;
    Up.y = cosAngle * Up.y + sinAngle * Right.y;
    Up.z = cosAngle * Up.z + sinAngle * Right.z;

    // Right = Up x Dir
    Right = glm::cross(Up, Dir);
}

//
// FUNCI�N: CGCamera::TurnDown()
//
// PROP�SITO: Rota el observador un paso (angleStep) en sentido contrario al reloj.
//
void CGCamera::TurnCCW()
{
    Up.x = cosAngle * Up.x - sinAngle * Right.x;
    Up.y = cosAngle * Up.y - sinAngle * Right.y;
    Up.z = cosAngle * Up.z - sinAngle * Right.z;

    // Right = Up x Dir
    Right = glm::cross(Up, Dir);
}

void CGCamera::SetDir(glm::vec3 up) {
    Dir = up;
}