#include <GL/glew.h>
#include <iostream>
#include <SDKDDKVer.h>
#include <Windows.h>
#include <FreeImage.h>
#include "CGSkybox.h"
#include "resource.h"

//
// FUNCI�N: CGSkybox::CGSkybox()
//
// PROP�SITO: Construye el objeto que describe la imagen de fondo
//
CGSkybox::CGSkybox()
{
	InitCubemap();
	InitCube();
}

//
// FUNCI�N: CGSkybox::~CGSkybox()
//
// PROP�SITO: Destruye el objeto que describe la imagen de fondo
//
CGSkybox::~CGSkybox()
{
	// Delete buffers
	glDeleteBuffers(2, VBO);
	glDeleteVertexArrays(1, &VAO);
	glDeleteTextures(1, &cubemap);
}

//
// FUNCI�N: CGSkybox::InitCube()
//
// PROP�SITO: Inicialliza los buffers con los v�rtices del tel�n
//
void CGSkybox::InitCube()
{
	GLfloat vertices[12] = {
		-1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f,
		1.0f, 1.0f, -1.0f,
		-1.0f, 1.0f, -1.0f
	};

	GLushort indexes[6] = {
		0,1,2,
		0,2,3
	};

	// Create the Vertex Array Object
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// Create the Vertex Buffer Objects
	glGenBuffers(2, VBO);

	// Copy data to video memory
	// Vertex data
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, vertices, GL_STATIC_DRAW);

	// Indexes
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VBO[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort) * 6, indexes, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0); // Vertex position
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
}

//
// FUNCI�N: CGSkybox::InitCubemap()
//
// PROP�SITO: Inicialliza las texturas del cubo
//
void CGSkybox::InitCubemap()
{
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &cubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	// Versi�n con recursos
	/*InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, IDR_IMAGE4);
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, IDR_IMAGE5);
	InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, IDR_IMAGE6);
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, IDR_IMAGE7);
	InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, IDR_IMAGE8);
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, IDR_IMAGE9);*/

	// Versi�n con ficheros
	InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_X, "textures/posx.jpg");
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, "textures/negx.jpg");
	InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, "textures/posy.jpg");
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, "textures/negy.jpg");
	InitTexture(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, "textures/negz.jpg");
	InitTexture(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, "textures/posz.jpg");

	// Typical cube map settings
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

//
// FUNCI�N: CGSkybox::InitTexture(GLuint target, const char* filename)
//
// PROP�SITO: Carga una textura a partir de un fichero
//
void CGSkybox::InitTexture(GLuint target, const char* filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	FIBITMAP* bitmap = FreeImage_Load(format, filename);
	FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	glTexImage2D(target, 0, GL_RGBA8, nWidth, nHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

	FreeImage_Unload(pImage);
}

//
// FUNCI�N: void CGSkybox::InitTexture(GLuint target, int idr)
//
// PROP�SITO: Carga una textura a partir de un recurso
//
void CGSkybox::InitTexture(GLuint target, int idr)
{
	HRSRC handle = FindResource(NULL, MAKEINTRESOURCE(idr), L"IMAGE");
	HGLOBAL hGlobal = LoadResource(NULL, handle);
	LPCTSTR rsc_ptr = static_cast<LPCTSTR>(LockResource(hGlobal));
	DWORD mem_size = SizeofResource(NULL, handle);
	BYTE* mem_buffer = (BYTE*)malloc((mem_size) * sizeof(BYTE));
	memcpy(mem_buffer, rsc_ptr, mem_size * sizeof(BYTE));
	FreeResource(hGlobal);

	FIMEMORY* hmem = FreeImage_OpenMemory(mem_buffer, mem_size * sizeof(BYTE));
	FREE_IMAGE_FORMAT fif = FreeImage_GetFileTypeFromMemory(hmem, 0);
	FIBITMAP* check = FreeImage_LoadFromMemory(fif, hmem, 0);
	FIBITMAP* pImage = FreeImage_ConvertTo32Bits(check);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	glTexImage2D(target, 0, GL_RGBA8, nWidth, nHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

	FreeImage_Unload(pImage);
	FreeImage_CloseMemory(hmem);
	free(mem_buffer);
}

//
// FUNCI�N: CGSkybox::Draw()
//
// PROP�SITO: Dibuja la imagen de fondo
//
void CGSkybox::Draw(CGShaderProgram* program, glm::mat4 projection, glm::mat4 view)
{
	glm::mat3 rot3 = glm::mat3(view); // Parte rotacional de la matriz View
	glm::mat4 rot4 = glm::mat4(rot3);
	glm::mat4 mvp = projection * rot4; // Transformaci�n del Skybox a coordenadas Clip
	glm::mat4 inv = glm::inverse(mvp); // Transformaci�n de coordenadas Clip a coordenadas de modelo del Skybox

	program->SetUniformMatrix4("Inverse", inv);
	program->SetUniformI("CubemapTex", 0);


	glDepthMask(GL_FALSE);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemap);

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_SHORT, NULL);
	glDepthMask(GL_TRUE);
}