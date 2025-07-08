#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "CGModel.h"

//
// CLASE: CGApplication
//
// DESCRIPCI�N: Construye una aplicaci�n gr�fica basada en una ventana creada con GLFW 
//              y un modelo realizado con OpenGL
//
class CGApplication
{
public:
	void run();

private:
	GLFWwindow* window;
	int windowWidth;
	int windowHeight;
	int windowXpos;
	int windowYpos;
	bool fullScreen;
	CGModel model;

	double limitFPS;
	double lastTime;
	double deltaTime;

	// M�todos principales
	void initWindow();
	void initOpenGL();
	void initModel();
	void mainLoop();
	void timing();
	void cleanup();
	void swapFullScreen();

	// Respuesta a eventos
	static void keyCallback(GLFWwindow* window, int key, int scan, int act, int mods);
	static void mouseButtonCallback(GLFWwindow* window, int bt, int action, int mods);
	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void framebufferResizeCallback(GLFWwindow* window, int width, int height);
};


