#include <GL/glew.h>
#include <iostream>
#include <SDKDDKVer.h>
#include <Windows.h>
#include "CGShaderProgram.h"
#include "resource.h"

//
// FUNCI�N: CGShaderProgram::CGShaderProgram(int vs, int fs, int gs, int tcs, int tes)
//
// PROP�SITO: Crea un programa gr�fico cargando y compilando los shaders que lo forman
//
CGShaderProgram::CGShaderProgram(int vs, int fs, int gs, int tcs, int tes)
{
	vertexShader = NO_SHADER;
	fragmentShader = NO_SHADER;
	geometryShader = NO_SHADER;
	tessControlShader = NO_SHADER;
	tessEvaluationShader = NO_SHADER;
	linked = GL_FALSE;

	// Crea y compila los shaders
	if (vs != -1) vertexShader = CreateShader(GL_VERTEX_SHADER, vs);
	if (fs != -1) fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fs);
	if (gs != -1) geometryShader = CreateShader(GL_GEOMETRY_SHADER, gs);
	if (tcs != -1) tessControlShader = CreateShader(GL_TESS_CONTROL_SHADER, tcs);
	if (tes != -1) tessEvaluationShader = CreateShader(GL_TESS_EVALUATION_SHADER, tes);

	//Crea el programa y carga los shaders
	program = glCreateProgram();
	if (vertexShader != NO_SHADER) glAttachShader(program, vertexShader);
	if (fragmentShader != NO_SHADER) glAttachShader(program, fragmentShader);
	if (geometryShader != NO_SHADER) glAttachShader(program, geometryShader);
	if (tessControlShader != NO_SHADER) glAttachShader(program, tessControlShader);
	if (tessEvaluationShader != NO_SHADER) glAttachShader(program, tessEvaluationShader);
	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		linked = GL_FALSE;
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		char* logInfo = (char*)malloc(sizeof(char) * logLength);
		GLsizei written;
		glGetProgramInfoLog(program, logLength, &written, logInfo);
		std::cout << logInfo << std::endl;
		return;
	}

	linked = GL_TRUE;
}

//
// FUNCI�N: CGShaderProgram::CGShaderProgram(int vs, int fs, int gs, int tcs, int tes)
//
// PROP�SITO: Crea un programa gr�fico cargando y compilando los shaders que lo forman
//
CGShaderProgram::CGShaderProgram(const char* vs, const char* fs, const char* gs, const char* tcs, const char* tes)
{
	vertexShader = NO_SHADER;
	fragmentShader = NO_SHADER;
	geometryShader = NO_SHADER;
	tessControlShader = NO_SHADER;
	tessEvaluationShader = NO_SHADER;
	linked = GL_FALSE;

	// Crea y compila los shaders
	if (vs != NULL) vertexShader = CreateShader(GL_VERTEX_SHADER, vs);
	if (fs != NULL) fragmentShader = CreateShader(GL_FRAGMENT_SHADER, fs);
	if (gs != NULL) geometryShader = CreateShader(GL_GEOMETRY_SHADER, gs);
	if (tcs != NULL) tessControlShader = CreateShader(GL_TESS_CONTROL_SHADER, tcs);
	if (tes != NULL) tessEvaluationShader = CreateShader(GL_TESS_EVALUATION_SHADER, tes);

	//Crea el programa y carga los shaders
	program = glCreateProgram();
	if (vertexShader != NO_SHADER) glAttachShader(program, vertexShader);
	if (fragmentShader != NO_SHADER) glAttachShader(program, fragmentShader);
	if (geometryShader != NO_SHADER) glAttachShader(program, geometryShader);
	if (tessControlShader != NO_SHADER) glAttachShader(program, tessControlShader);
	if (tessEvaluationShader != NO_SHADER) glAttachShader(program, tessEvaluationShader);
	glLinkProgram(program);

	GLint status;
	glGetProgramiv(program, GL_LINK_STATUS, &status);
	if (status == GL_FALSE)
	{
		linked = GL_FALSE;
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
		char* logInfo = (char*)malloc(sizeof(char) * logLength);
		GLsizei written;
		glGetProgramInfoLog(program, logLength, &written, logInfo);
		std::cout << logInfo << std::endl;
		return;
	}

	linked = GL_TRUE;
}

//
// FUNCI�N: CGShaderProgram::CreateShader(int mode, int idr)
//
// PROP�SITO: Crea un shader de un cierto tipo a partir de un recurso
//
GLuint CGShaderProgram::CreateShader(int mode, int idr) 
{
	GLint status;
	GLuint shader = glCreateShader(mode);
	char* code = GetShaderCodeFromResource(idr);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	free(code);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char* logInfo = (char*)malloc(sizeof(char) * logLength);
		GLsizei written;
		glGetShaderInfoLog(shader, logLength, &written, logInfo);
		std::cout << logInfo << std::endl;
		return NO_SHADER;
	}
	return shader;
}

//
// FUNCI�N: CGShaderProgram::CreateShader(int mode, const char* filename)
//
// PROP�SITO: Crea un shader de un cierto tipo a partir de un fichero
//
GLuint CGShaderProgram::CreateShader(int mode, const char* filename)
{
	GLint status;
	GLuint shader = glCreateShader(mode);
	char* code = GetShaderCodeFromFile(filename);
	glShaderSource(shader, 1, &code, NULL);
	glCompileShader(shader);
	free(code);
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
	if (status == GL_FALSE)
	{
		GLint logLength;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
		char* logInfo = (char*)malloc(sizeof(char) * logLength);
		GLsizei written;
		glGetShaderInfoLog(shader, logLength, &written, logInfo);
		std::cout << logInfo << std::endl;
		return NO_SHADER;
	}
	return shader;
}

//
// FUNCI�N: CGShaderProgram::GetShaderCodeFromResource(int idr)
//
// PROP�SITO: Obtiene el contenido de un fichero de texto almacenado como un recurso
//
char* CGShaderProgram::GetShaderCodeFromResource(int idr)
{
	HRSRC shaderHandle = FindResource(NULL, MAKEINTRESOURCE(idr), L"SHADER");
	HGLOBAL shaderGlobal = LoadResource(NULL, shaderHandle);
	LPCTSTR shaderPtr = static_cast<LPCTSTR>(LockResource(shaderGlobal));
	DWORD shaderSize = SizeofResource(NULL, shaderHandle);
	char* shaderCodeLine = (char*)malloc((shaderSize + 1) * sizeof(char));
	memcpy(shaderCodeLine, shaderPtr, shaderSize);
	shaderCodeLine[shaderSize] = '\0';
	FreeResource(shaderGlobal);
	return shaderCodeLine;
}

//
// FUNCI�N: CGShaderProgram::GetShaderCodeFromFile(const char* filename)
//
// PROP�SITO: Obtiene el contenido de un fichero de texto almacenado como un fichero
//
char* CGShaderProgram::GetShaderCodeFromFile(const char* filename)
{
	FILE* file = NULL;
	fopen_s(&file,filename, "r");
	fseek(file, 0L, SEEK_END);
	int size = ftell(file);
	fseek(file, 0L, SEEK_SET);
	char* code = (char*)malloc(sizeof(char) * (size + 1));
	int read = fread(code, sizeof(char), size, file);
	code[read] = '\0';
	fclose(file);
	return code;
}

//
// FUNCI�N: CGShaderProgram::~CGShaderProgram()
//
// PROP�SITO: Destruye el programa gr�fico
//
CGShaderProgram::~CGShaderProgram()
{
	if (vertexShader != NO_SHADER) glDeleteShader(vertexShader);
	if (fragmentShader != NO_SHADER) glDeleteShader(fragmentShader);
	if (geometryShader != NO_SHADER) glDeleteShader(geometryShader);
	if (tessControlShader != NO_SHADER) glDeleteShader(tessControlShader);
	if (tessEvaluationShader != NO_SHADER) glDeleteShader(tessEvaluationShader);

	glDeleteProgram(program);
}

//
// FUNCI�N: CGShaderProgram::IsLinked()
//
// PROP�SITO: Verifica si el programa se ha linkado de forma correcta
//
GLboolean CGShaderProgram::IsLinked()
{
	return linked;
}

//
// FUNCI�N: CGShaderProgram::Use()
//
// PROP�SITO: Activa el funcionamiento del programa en la tarjeta gr�fica
//
GLvoid CGShaderProgram::Use()
{
	glUseProgram(program);
}

//
// FUNCI�N: CGShaderProgram::SetUniformF(const char* name, GLfloat f)
//
// PROP�SITO: Asigna el valor de una variable uniforme de tipo float
//
GLvoid CGShaderProgram::SetUniformF(const char* name, GLfloat f)
{
	GLuint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniform1f(location, f);
}

//
// FUNCI�N: CGShaderProgram::SetUniformMatrix4(const char* name, glm::mat4 m)
//
// PROP�SITO: Asigna el valor de una variable uniforme de tipo mat4 (matriz 4x4)
//
GLvoid CGShaderProgram::SetUniformMatrix4(const char* name, glm::mat4 m)
{
	GLuint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniformMatrix4fv(location, 1, GL_FALSE, &m[0][0]);
}

//
// FUNCI�N: CGShaderProgram::SetUniformVec4(const char* name, glm::vec4 v)
//
// PROP�SITO: Asigna el valor de una variable uniforme de tipo vec4 (vector de 4 float)
//
GLvoid CGShaderProgram::SetUniformVec4(const char* name, glm::vec4 v)
{
	GLuint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniform4fv(location, 1, &v[0]);
}

//
// FUNCI�N: CGShaderProgram::SetUniformVec3(const char* name, glm::vec3 v)
//
// PROP�SITO: Asigna el valor de una variable uniforme de tipo vec3 (vector de 3 float)
//
GLvoid CGShaderProgram::SetUniformVec3(const char* name, glm::vec3 v)
{
	GLuint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniform3fv(location, 1, &v[0]);
}

//
// FUNCI�N: CGShaderProgram::SetUniformI(const char* name, GLint i)
//
// PROP�SITO: Asigna el valor de una variable uniforme de tipo entero
//
GLvoid CGShaderProgram::SetUniformI(const char* name, GLint i)
{
	GLuint location = glGetUniformLocation(program, name);
	if (location >= 0) glUniform1i(location, i);
}
//
// FUNCI�N: CGShaderProgram::SetUniformSubroutine(int mode, const char * name)
//
// PROP�SITO: Asigna una subrutina uno de los shaders
//
GLvoid CGShaderProgram::SetUniformSubroutine(int mode, const char* name)
{
	GLuint location = glGetSubroutineIndex(program, mode, name);
	if (location >= 0) glUniformSubroutinesuiv(mode, 1, &location);
}