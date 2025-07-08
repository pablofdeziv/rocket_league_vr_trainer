#include "CGPiece.h"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "CGMaterial.h"
#include "CGFigure.h"

//
// FUNCI�N: CGPiece::~CGPiece()
//
// PROP�SITO: Destructor de la pieza
//
CGPiece::~CGPiece()
{
	if (vertices != NULL) delete[] vertices;
	if (indexes != NULL) delete[] indexes;
	if (normals != NULL) delete[] normals;
	if (textures != NULL) delete[] textures;

	// Delete vertex buffer objects
	glDeleteBuffers(4, VBO);
	glDeleteVertexArrays(1, &VAO);
}

//
// FUNCI�N: CGPiece::InitBuffers()
//
// PROP�SITO: Crea el VAO y los VBO y almacena todos los datos
//            en la GPU.
//
void CGPiece::InitBuffers()
{
	// Create the Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the Vertex Buffer Objects
	glGenBuffers(4, VBO);

	// Copy data to video memory
	// Vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_DATA]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 3, vertices, GL_STATIC_DRAW);

	// Normal data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL_DATA]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 3, normals, GL_STATIC_DRAW);

	// Texture coordinates
	glBindBuffer(GL_ARRAY_BUFFER, VBO[TEXTURE_DATA]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * numVertices * 2, textures, GL_STATIC_DRAW);

	// Indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[INDEX_DATA]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * numFaces * 3, indexes, GL_STATIC_DRAW);

	delete[] vertices;
	delete[] indexes;
	delete[] normals;
	delete[] textures;

	vertices = NULL;
	indexes = NULL;
	normals = NULL;
	textures = NULL;

	glEnableVertexAttribArray(0); // Vertex position
	glBindBuffer(GL_ARRAY_BUFFER, VBO[VERTEX_DATA]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(1); // Vertex normals
	glBindBuffer(GL_ARRAY_BUFFER, VBO[NORMAL_DATA]);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);

	glEnableVertexAttribArray(2); // Vertex textures
	glBindBuffer(GL_ARRAY_BUFFER, VBO[TEXTURE_DATA]);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, 0);

	location = glm::mat4(1.0f);
}

//
// FUNCI�N: CGPiece::SetMaterial(CGMaterial* m)
//
// PROP�SITO: Asigna el material de la pieza
//
void CGPiece::SetMaterial(CGMaterial* m)
{
	material = m;
}

//
// FUNCI�N: CGPiece::SetLocation(glm::mat4 loc)
//
// PROP�SITO: Asigna la matriz de posici�n de la pieza respecto a la posici�n de la
//            figura a la que pertenece.
//
void CGPiece::SetLocation(glm::mat4 loc)
{
	location = loc;
}

//
// FUNCI�N: CGPiece::GetLocation()
//
// PROP�SITO: Obtiene la matriz de posici�n de la pieza respecto a la posici�n de la
//            figura a la que pertenece.
//
glm::mat4 CGPiece::GetLocation()
{
	return location;
}

//
// FUNCI�N: CGPiece::Translate(glm::vec3 t)
//
// PROP�SITO: A�ade un desplazamiento a la matriz de posici�n de la pieza 
//            respecto a la posici�n de la figura a la que pertenece.
//
void CGPiece::Translate(glm::vec3 t)
{
	location = glm::translate(location, t);
}

//
// FUNCI�N: CGPiece::Rotate(GLfloat angle, glm::vec3 axis)
//
// PROP�SITO: A�ade una rotaci�n a la matriz de posici�n de la pieza 
//            respecto a la posici�n de la figura a la que pertenece.
//
void CGPiece::Rotate(GLfloat angle, glm::vec3 axis)
{
	location = glm::rotate(location, glm::radians(angle), axis);
}

//
// FUNCI�N: CGPiece::Draw(CGShaderProgram * program, glm::mat4 projection, glm::mat4 view, glm::mat4 model)
//
// PROP�SITO: Dibuja a pieza
//
void CGPiece::Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view, glm::mat4 model)
{
	glm::mat4 mvp = projection * view * model * location;
	program->SetUniformMatrix4("MVP", mvp);
	program->SetUniformMatrix4("ViewMatrix", view);
	program->SetUniformMatrix4("ModelViewMatrix", view * model * location);

	material->SetUniforms(program);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_SHORT, NULL);
}

void CGPiece::DrawShadow(CGShaderProgram* program, glm::mat4 shadowMatrix)
{
	glm::mat4 mvp = shadowMatrix * location;
	program->SetUniformMatrix4("MVP", mvp);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, numFaces * 3, GL_UNSIGNED_SHORT, NULL);
}