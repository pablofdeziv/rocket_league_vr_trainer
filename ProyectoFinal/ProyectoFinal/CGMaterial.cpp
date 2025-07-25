#include "CGMaterial.h"
#include <GL/glew.h>
#include <Windows.h>
#include "resource.h"
#include <FreeImage.h>



//
// FUNCI�N: CGMaterial::CGMaterial()
//
// PROP�SITO: Construye un material con los valores por defecto
//
CGMaterial::CGMaterial()
{
	Ka = glm::vec3(1.0f, 1.0f, 1.0f);
	Kd = glm::vec3(1.0f, 1.0f, 1.0f);
	Ks = glm::vec3(0.8f, 0.8f, 0.8f);
	Shininess = 16.0f;
	textureId = 0;
}

//
// FUNCI�N: CGMaterial::SetAmbientReflect(GLfloat r, GLfloat g, GLfloat b)
//
// PROP�SITO: Asigna la reflectividad ambiental (color ante la luz ambiental)
//
void CGMaterial::SetAmbientReflect(GLfloat r, GLfloat g, GLfloat b)
{
	Ka = glm::vec3(r, g, b);
}

//
// FUNCI�N: CGMaterial::SetDifusseReflect(GLfloat r, GLfloat g, GLfloat b)
//
// PROP�SITO: Asigna la reflectividad difusa (color ante la luz difusa)
//
void CGMaterial::SetDifusseReflect(GLfloat r, GLfloat g, GLfloat b)
{
	Kd = glm::vec3(r, g, b);
}

//
// FUNCI�N: CGMaterial::SetSpecularReflect(GLfloat r, GLfloat g, GLfloat b)
//
// PROP�SITO: Asigna la reflectividad especular (color ante la luz especular)
//
void CGMaterial::SetSpecularReflect(GLfloat r, GLfloat g, GLfloat b)
{
	Ks = glm::vec3(r, g, b);
}

//
// FUNCI�N: CGMaterial::SetShininess(GLfloat f)
//
// PROP�SITO: Asigna el factor de brillo (comportamiento ante la luz especular)
//
void CGMaterial::SetShininess(GLfloat f)
{
	Shininess = f;
}

//
// FUNCI�N: CGMaterial::SetUniforms(CGShaderProgram* program)
//
// PROP�SITO: Configura las propiedades de material en el programa gr�fico
//
void CGMaterial::SetUniforms(CGShaderProgram* program)
{
	program->SetUniformVec3("Material.Ka", Ka);
	program->SetUniformVec3("Material.Kd", Kd);
	program->SetUniformVec3("Material.Ks", Ks);
	program->SetUniformF("Material.Shininess", Shininess);
	program->SetUniformI("BaseTex", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureId);
}

//
// FUNCI�N: CGMaterial::SetTexture(GLuint id)
//
// PROP�SITO: Asigna el identificador de la textura b�sica
//
void CGMaterial::SetTexture(GLuint id)
{
	textureId = id;
}

//
// FUNCI�N: CGMaterial::GetTexture()
//
// PROP�SITO: Obtiene el identificador de la textura b�sica
//
GLuint CGMaterial::GetTexture()
{
	return textureId;
}

//
// FUNCI�N: void CGMAterial::InitTexture(const char* filename)
//
// PROP�SITO: Carga una textura a partir de un fichero
//
void CGMaterial::InitTexture(const char* filename)
{
	FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename, 0);
	FIBITMAP* bitmap = FreeImage_Load(format, filename);
	FIBITMAP* pImage = FreeImage_ConvertTo32Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

	FreeImage_Unload(pImage);
}

//
// FUNCI�N: void CGMAterial::InitTexture(int idr)
//
// PROP�SITO: Carga una textura a partir de un recurso
//
void CGMaterial::InitTexture(int idr)
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

	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, (void*)FreeImage_GetBits(pImage));

	FreeImage_Unload(pImage);
	FreeImage_CloseMemory(hmem);
	free(mem_buffer);
}
