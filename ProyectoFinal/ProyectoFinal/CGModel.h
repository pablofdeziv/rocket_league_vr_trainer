#pragma once

#include <GL/glew.h>
#include "CGShaderProgram.h"
#include "CGScene.h"
#include "CGSkybox.h"
#include "CGCamera.h"

class CGModel
{
public:
	void initialize(int w, int h);
	void finalize();
	void render();
	void update();
	void key_pressed(int key);
	void mouse_button(int button, int action);
	void mouse_move(double xpos, double ypos);
	void colocarCamara();
	void BallHits();
	void CarConstraints();
	void BallConstraints();
	void resize(int w, int h);

private:
	CGShaderProgram* sceneProgram;
	CGShaderProgram* skyboxProgram;
	CGShaderProgram* shadowProgram;
	CGScene* scene;
	CGCamera* camera;
	CGSkybox* skybox;
	glm::mat4 projection;
	bool fueracoche = false;
	bool marchaatras = false;
	bool avancePelota = false;
	bool retrocesoPelota = false;

	GLsizei wndWidth;
	GLsizei wndHeight;
	GLuint shadowFBO;
	GLuint depthTexId;

	bool InitShadowMap();
	//void CameraConstraints();
};
