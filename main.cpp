
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <C:/FAQULTATE/PG LAB/proiect final/glm/glm.hpp>
#include <C:/FAQULTATE/PG LAB/proiect final/glm/gtc/matrix_transform.hpp>
#include <C:/FAQULTATE/PG LAB/proiect final/glm/gtc/matrix_inverse.hpp>
#include <C:/FAQULTATE/PG LAB/proiect final/glm/gtc/type_ptr.hpp>

#include "Window.h"
#include "Shader.hpp"
#include "Model3D.hpp"
#include "Camera.hpp"
#include "SkyBox.hpp"

#include <iostream>
#include <list>

// window
gps::Window myWindow;

const unsigned int SHADOW_WIDTH = 5096;
const unsigned int SHADOW_HEIGHT = 5096;

unsigned int SCR_WIDTH = 1024;
unsigned int SCR_HEIGHT = 768;

glm::mat4 model;
glm::mat4 modelOZN;
glm::mat4 modelSimba;
GLuint modelLoc;
GLuint modelLoc2;
glm::mat4 view;
GLuint viewLoc;
glm::mat4 projection;
GLuint projectionLoc;
glm::mat3 normalMatrix;
glm::mat3 normalMatrixOZN;
glm::mat3 normalMatrixSimba;
GLuint normalMatrixLoc;
glm::mat4 lightRotation;

glm::vec3 lightDir;
GLuint lightDirLoc;
glm::vec3 lightColor;
GLuint lightColorLoc;

bool pressedKeys[1024];
float angleY = 0.0f;
GLfloat lightAngle;
GLfloat simbaX;


gps::Camera myCamera(
	glm::vec3(13.20f, 2.07f, 29.29f),
	glm::vec3(-1.5f, 0.0f, 0.0f),
	glm::vec3(0.0f, 1.0f, 0.0f));


gps::Model3D scene;
gps::Model3D ozn;
gps::Model3D raindrop;
gps::Model3D simba;
gps::Model3D lightCube;
gps::Model3D screenQuad;

gps::Shader myCustomShader;
gps::Shader depthMapShader;
gps::Shader lightShader;
gps::Shader screenQuadShader;
gps::Shader skyboxShader;


gps::SkyBox mySkyBox;
std::vector<const GLchar*> faces;

//cameraMoves
float yaw = -90.0f;
float pitch = 0.0f;

float cameraSpeed = 0.3f;


GLuint shadowMapFBO;
GLuint depthMapTexture;

bool showDepthMap;

int spotlight = 0;
GLint spotlightLoc;
int bool_cameraPresentation = 0;
int fog = 0;
GLint fogLoc;


bool cameraAnimation = false;
double x_current = 0.0f;
double y_current = 0.0f;

GLfloat angle;


int bool_raining = 0;
int nr_raindrops = 40;

std::list< std::pair< glm::vec3, float> > raindrops;
std::pair< glm::vec3, float> generateRaindrop() {
	return { glm::vec3(rand() % 90 - 40, rand() % 25 + 5, rand() % 70 - 30) , 0.2f };
}


GLenum glCheckError_(const char* file, int line)
{
	GLenum errorCode;
	while ((errorCode = glGetError()) != GL_NO_ERROR) {
		std::string error;
		switch (errorCode) {
		case GL_INVALID_ENUM:
			error = "INVALID_ENUM";
			break;
		case GL_INVALID_VALUE:
			error = "INVALID_VALUE";
			break;
		case GL_INVALID_OPERATION:
			error = "INVALID_OPERATION";
			break;
		case GL_STACK_OVERFLOW:
			error = "STACK_OVERFLOW";
			break;
		case GL_STACK_UNDERFLOW:
			error = "STACK_UNDERFLOW";
			break;
		case GL_OUT_OF_MEMORY:
			error = "OUT_OF_MEMORY";
			break;
		case GL_INVALID_FRAMEBUFFER_OPERATION:
			error = "INVALID_FRAMEBUFFER_OPERATION";
			break;
		}
		std::cout << error << " | " << file << " (" << line << ")" << std::endl;
	}
	return errorCode;
}
#define glCheckError() glCheckError_(__FILE__, __LINE__)

void windowResizeCallback(GLFWwindow* window, int width, int height) {
	fprintf(stdout, "window resized to width: %d , and height: %d\n", width, height);
	//TODO	
	glViewport(0, 0, width, height);
}

void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mode) {
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
		showDepthMap = !showDepthMap;
	
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			pressedKeys[key] = true;
		else if (action == GLFW_RELEASE)
			pressedKeys[key] = false;
	}
}

void mouseCallback(GLFWwindow* window, double xpos, double ypos) {
	if (cameraAnimation == false) {

		if (x_current == 0.0f && y_current == 0.0f) { //verific daca pozitia initiala a mouse ului a fost setata
			x_current = xpos;
			y_current = ypos;
		}
		else {
			double incr_x = (xpos - x_current) / 10; // impart la o val mai mica ca sa fie mai sensibil la miscarea mouseului
			double incr_y = (ypos - y_current) / 50;
			if ((pitch + incr_y < 79.0f) && (pitch + incr_y) > -89.0f) {
				pitch += incr_y;
			}

			yaw += incr_x;
			myCamera.rotate(pitch, yaw);

			x_current = xpos;
			y_current = ypos;
		}
	}
}

void processMovement()
{

	if (pressedKeys[GLFW_KEY_8]) {
		myCamera.move(gps::MOVE_UP, cameraSpeed);
		
		
	}

	if (pressedKeys[GLFW_KEY_9]) {
		myCamera.move(gps::MOVE_DOWN, cameraSpeed);
		
		
	}

	if (pressedKeys[GLFW_KEY_W]) {
		myCamera.move(gps::MOVE_FORWARD, cameraSpeed);
		
	}

	if (pressedKeys[GLFW_KEY_S]) {
		myCamera.move(gps::MOVE_BACKWARD, cameraSpeed);
		
	}

	if (pressedKeys[GLFW_KEY_A]) {
		myCamera.move(gps::MOVE_LEFT, cameraSpeed);
		
	}

	if (pressedKeys[GLFW_KEY_D]) {
		myCamera.move(gps::MOVE_RIGHT, cameraSpeed);
	
	}

	if (pressedKeys[GLFW_KEY_Q]) {
		angleY -= 1.0f;
	}

	if (pressedKeys[GLFW_KEY_E]) {
		angleY += 1.0f;
	}

	if (pressedKeys[GLFW_KEY_T]) {
		bool_cameraPresentation = 1;
		glfwSetTime(0.0f);
	}

	if (pressedKeys[GLFW_KEY_R]) {
		bool_cameraPresentation = 0;
		myCamera.setCameraPosition(glm::vec3(-1.5f, 0.0f, 1.75f));
		myCamera.setCameraTarget(glm::vec3(-1.5f, 0.0f, 0.0f));
		myCamera.setCameraUp(glm::vec3(0.0f, 1.0f, 0.0f));
		myCamera.setCameraFrontDirection(glm::normalize(myCamera.getCameraTarget() - myCamera.getCameraPosition()));
		myCamera.setCameraRightDirection(glm::normalize(glm::cross(myCamera.getCameraFrontDirection(), myCamera.getCameraUp())));
		
	}

	if (pressedKeys[GLFW_KEY_J]) {
		lightAngle -= 0.1f;
		//if(lightAngle <)
	}

	if (pressedKeys[GLFW_KEY_L]) {
		lightAngle += 0.1f;
	}

	
	
	if (pressedKeys[GLFW_KEY_N]) {
		spotlight = 1;
		myCustomShader.useShaderProgram();
		spotlightLoc = glGetUniformLocation(myCustomShader.shaderProgram, "bool_spotlight");
		glUniform1i(spotlightLoc, spotlight);
	}

	if (pressedKeys[GLFW_KEY_B]) {
		spotlight = 0;
		myCustomShader.useShaderProgram();
		spotlightLoc = glGetUniformLocation(myCustomShader.shaderProgram, "bool_spotlight");
		glUniform1i(spotlightLoc, spotlight);
	}
	
	if (pressedKeys[GLFW_KEY_C]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}

	if (pressedKeys[GLFW_KEY_X]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}

	if (pressedKeys[GLFW_KEY_Z]) {
		glPolygonMode(GL_FRONT_AND_BACK, GL_POINT);
	}
	



	if (pressedKeys[GLFW_KEY_0]) {
		std::cout << myCamera.getCameraPosition().x << " ";
		std::cout << myCamera.getCameraPosition().y << " ";
		std::cout << myCamera.getCameraPosition().z << " ";
		std::cout << myCamera.getCameraTarget().x << " ";
		std::cout << myCamera.getCameraTarget().y << " ";
		std::cout << myCamera.getCameraTarget().z << " ";

	}

	if (pressedKeys[GLFW_KEY_G]) {
		fog = 1;
		myCustomShader.useShaderProgram();
		fogLoc = glGetUniformLocation(myCustomShader.shaderProgram, "bool_fog");
		glUniform1i(fogLoc, fog);
	}

	if (pressedKeys[GLFW_KEY_H])
	{
		fog = 0;
		myCustomShader.useShaderProgram();
		fogLoc = glGetUniformLocation(myCustomShader.shaderProgram, "bool_fog");
		glUniform1i(fogLoc, fog);
		bool_raining = 0;
	}
	if (pressedKeys[GLFW_KEY_P]) {
		if (bool_raining) return;

		bool_raining = 1;
		raindrops.clear();
		for (int i = 1; i <= nr_raindrops; i++) {
			raindrops.push_back(generateRaindrop());
		}
	}


	if (pressedKeys[GLFW_KEY_UP]) {
		simbaX -= 0.05f;
	}
	if (pressedKeys[GLFW_KEY_DOWN]) {
		simbaX += 0.05f;
	}

}

void initOpenGLWindow()
{
	myWindow.Create(1024, 768, "My Scene");
	//glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void setWindowCallbacks() {
	glfwSetWindowSizeCallback(myWindow.getWindow(), windowResizeCallback);
	glfwSetKeyCallback(myWindow.getWindow(), keyboardCallback);
	glfwSetCursorPosCallback(myWindow.getWindow(), mouseCallback);
	glfwSetInputMode(myWindow.getWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	//glfwSetScrollCallback(myWindow.getWindow(), scroll_callback);

}

void initOpenGLState()
{
	glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
	glViewport(0, 0, myWindow.getWindowDimensions().width, myWindow.getWindowDimensions().height);

	glEnable(GL_DEPTH_TEST); // enable depth-testing
	glDepthFunc(GL_LESS); // depth-testing interprets a smaller value as "closer"
	//glEnable(GL_CULL_FACE); // cull face
	//glCullFace(GL_BACK); // cull back face
	glFrontFace(GL_CCW); // GL_CCW for counter clock-wise

	glEnable(GL_FRAMEBUFFER_SRGB);
}

void initModels() {

	scene.LoadModel("models/scena_FINALA_ULTIMA_VERSIUNE2.obj");
	ozn.LoadModel("models/ozn.obj");
	//cat.LoadModel("models/matza.obj");
	lightCube.LoadModel("models/cube/cube.obj");
	screenQuad.LoadModel("models/quad/quad.obj");
	raindrop.LoadModel("models/picur.obj");
	simba.LoadModel("models/simba.obj");
	//glCheckError();
}

void initShaders() {
	myCustomShader.loadShader("shaders/shaderStart.vert", "shaders/shaderStart.frag");
	myCustomShader.useShaderProgram();
	lightShader.loadShader("shaders/lightCube.vert", "shaders/lightCube.frag");
	lightShader.useShaderProgram();
	screenQuadShader.loadShader("shaders/screenQuad.vert", "shaders/screenQuad.frag");
	screenQuadShader.useShaderProgram();
	depthMapShader.loadShader("shaders/depthMap.vert", "shaders/depthMap.frag");
	depthMapShader.useShaderProgram();
	skyboxShader.loadShader("shaders/skyboxShader.vert", "shaders/skyboxShader.frag");
	skyboxShader.useShaderProgram();
}

void initUniforms() {
	myCustomShader.useShaderProgram();

	model = glm::mat4(1.0f);
	modelOZN = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f, 1.0f, 0.0f));
	modelSimba = glm::translate(glm::mat4(1.0f), glm::vec3(simbaX, 0.0f, 0.0f));
	modelLoc = glGetUniformLocation(myCustomShader.shaderProgram, "model");
	modelLoc2 = glGetUniformLocation(depthMapShader.shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	view = myCamera.getViewMatrix();
	viewLoc = glGetUniformLocation(myCustomShader.shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

	normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
	normalMatrixLoc = glGetUniformLocation(myCustomShader.shaderProgram, "normalMatrix");
	normalMatrixOZN = glm::mat3(glm::inverseTranspose(view * modelOZN));
	normalMatrixSimba = glm::mat3(glm::inverseTranspose(view * modelSimba));
	glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

	projection = glm::perspective(glm::radians(45.0f), (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height, 0.1f, 1000.0f);
	projectionLoc = glGetUniformLocation(myCustomShader.shaderProgram, "projection");
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));

	//set the light direction (direction towards the light)
	lightDir = glm::vec3(0.0f, 1.0f, 1.0f);
	lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(1.0f, 0.0f, 0.0f));
	lightDirLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightDir");
	glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

	//set light color
	lightColor = glm::vec3(1.0f, 1.0f, 1.0f); //white light
	lightColorLoc = glGetUniformLocation(myCustomShader.shaderProgram, "lightColor");
	glUniform3fv(lightColorLoc, 1, glm::value_ptr(lightColor));

	spotlightLoc = glGetUniformLocation(myCustomShader.shaderProgram, "bool_spotlight");
	glUniform1i(spotlightLoc, spotlight);

	fogLoc = glGetUniformLocation(myCustomShader.shaderProgram, "bool_fog");
	glUniform1i(fogLoc, fog);

	lightShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
}

void initFBO() {
	//TODO - Create the FBO, the depth texture and attach the depth texture to the FBO
	glGenFramebuffers(1, &shadowMapFBO);
	glGenTextures(1, &depthMapTexture);
	glBindTexture(GL_TEXTURE_2D, depthMapTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMapTexture, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

glm::mat4 computeLightSpaceTrMatrix() {
	//TODO - Return the light-space transformation matrix
	glm::mat4 lightView = glm::lookAt(glm::mat3(lightRotation) * lightDir * 18.4f, glm::vec3(0.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	//const GLfloat near_plane = 1.0f, far_plane = 100.0f;
	glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, -50.0f, 50.0f, -150.0f, 180.0f);
	//matricea finala de transformare in spatiul luminii
	glm::mat4 lightSpaceTrMatrix = lightProjection * lightView;
	return lightSpaceTrMatrix;
}


void initSkyBox() {
	faces.push_back("textures/skybox/right.tga");
	faces.push_back("textures/skybox/left.tga");
	faces.push_back("textures/skybox/top.tga");
	faces.push_back("textures/skybox/bottom.tga");
	faces.push_back("textures/skybox/back.tga");
	faces.push_back("textures/skybox/front.tga");
	mySkyBox.Load(faces);
	//glCheckError();
}
 

void renderScene(gps::Shader shader, bool depthPass)
{
	shader.useShaderProgram();

	model = glm::rotate(glm::mat4(1.0f), glm::radians(angleY), glm::vec3(0.0f, 1.0f, 0.0f));
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

	//do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));
	}

	scene.Draw(shader);
}

void renderOZN(gps::Shader shader, bool depthPass)
{
	shader.useShaderProgram();

	modelOZN = glm::rotate(modelOZN, 0.02f, glm::vec3(0, 1, 0));

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelOZN));
	glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(modelOZN));

	//do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrixOZN = glm::mat3(glm::inverseTranspose(view * modelOZN));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrixOZN));
	}
	ozn.Draw(shader);
}

void renderSimba(gps::Shader shader, bool depthPass)
{
	shader.useShaderProgram();

	modelSimba = glm::translate(modelSimba, glm::vec3(simbaX, 0.0f, 0.0f));
	simbaX = 0.0f;
	

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(modelSimba));
	glUniformMatrix4fv(modelLoc2, 1, GL_FALSE, glm::value_ptr(modelSimba));

	//do not send the normal matrix if we are rendering in the depth map
	if (!depthPass) {
		normalMatrixSimba = glm::mat3(glm::inverseTranspose(view * modelSimba));
		glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrixSimba));
	}
	simba.Draw(shader);
}


void renderSkyBox(gps::Shader shader) {
	shader.useShaderProgram();
	view = myCamera.getViewMatrix();

	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "view"), 1, GL_FALSE,
		glm::value_ptr(view));

	projection = glm::perspective(glm::radians(45.0f), (float)myWindow.getWindowDimensions().width / (float)myWindow.getWindowDimensions().height, 0.1f, 1000.0f);
	glUniformMatrix4fv(glGetUniformLocation(shader.shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	mySkyBox.Draw(shader, view, projection);

}

void moveRaindrops() {
	for (auto& it : raindrops) {
		it.first.y -= it.second;
		if (it.first.y <= -2.5) {
			raindrops.push_back(generateRaindrop());
		}
	}
	raindrops.remove_if([](auto it) {return it.first.y <= -2.5; });
}

void renderScene() {

	// depth maps creation pass
	//TODO - Send the light-space transformation matrix to the depth map creation shader and
	//		 render the scene in the depth map

	// rasterizati în harta de adancime
	//Schimbati portul de vizualizare pentru a acoperi dimensiunea texturarii de adancime;
	//Activati obiectul framebuffer;
	//Curatati(initializati) buffer - ul de adancime;
	//Configurati shader - ul(pentru completarea texturii de adancime) si matricile de transformare(pentru sistemul
	//	de coordinate al luminii);
	//Rasterizati scena;
	//Dezactivati obiectul framebuffer;

	depthMapShader.useShaderProgram();
	glUniformMatrix4fv(glGetUniformLocation(depthMapShader.shaderProgram, "lightSpaceTrMatrix"), 1, GL_FALSE, glm::value_ptr(computeLightSpaceTrMatrix()));
	glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, shadowMapFBO);
	glClear(GL_DEPTH_BUFFER_BIT);
	
	renderScene(depthMapShader, 1);
	renderOZN(depthMapShader, 1);
	renderSimba(depthMapShader, 1);
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// render depth map on screen - toggled with the M key

	if (showDepthMap) {
		glViewport(0, 0, (float)myWindow.getWindowDimensions().width , (float)myWindow.getWindowDimensions().height);

		glClear(GL_COLOR_BUFFER_BIT);

		screenQuadShader.useShaderProgram();

		//bind the depth map
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(screenQuadShader.shaderProgram, "depthMap"), 0);

		glDisable(GL_DEPTH_TEST);
		screenQuad.Draw(screenQuadShader);
		glEnable(GL_DEPTH_TEST);
	}
	else {


		// final scene rendering pass (with shadows)

		// rasterizati în framebuffer
		//Modificati portul de vizualizare pentru a acomoda obiectul framebuffer al ferestrei OpenGL;
		//Curatati(initializati) buffer - ul de adâncime si culoarea;
		//Configurasi shader - ul(pentru rasterizarea cu umbre) si matricile de transformare;
		//Activati(legati) textura de adancime;
		//Rasterizati scena;

		glViewport(0, 0, (float)myWindow.getWindowDimensions().width, (float)myWindow.getWindowDimensions().height);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		myCustomShader.useShaderProgram();

		if (bool_cameraPresentation == 1)
		{
			myCamera.cameraPresentation();
		}

		view = myCamera.getViewMatrix();
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));

		lightRotation = glm::rotate(glm::mat4(1.0f), glm::radians(lightAngle), glm::vec3(1.0f, 0.0f, 0.0f));
		glUniform3fv(lightDirLoc, 1, glm::value_ptr(glm::inverseTranspose(glm::mat3(view * lightRotation)) * lightDir));

		//bind the shadow map
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D, depthMapTexture);
		glUniform1i(glGetUniformLocation(myCustomShader.shaderProgram, "shadowMap"), 3);

		glUniformMatrix4fv(glGetUniformLocation(myCustomShader.shaderProgram, "lightSpaceTrMatrix"),
			1,
			GL_FALSE,
			glm::value_ptr(computeLightSpaceTrMatrix()));

		renderScene(myCustomShader, false);
		renderOZN(myCustomShader, false);
		renderSimba(myCustomShader, false);
	
		//draw a white cube around the light

		lightShader.useShaderProgram();

		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(view));

		model = lightRotation;
		model = glm::translate(model, 1.0f * lightDir);
		model = glm::scale(model, glm::vec3(0.0001f, 0.0001f, 1.0f));
		glUniformMatrix4fv(glGetUniformLocation(lightShader.shaderProgram, "model"), 1, GL_FALSE, glm::value_ptr(model));

		lightCube.Draw(lightShader);

		
	}

	if (bool_raining) {
		moveRaindrops();
		for (auto it : raindrops) {
			model = glm::translate(glm::mat4(1.0f), it.first);
			glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));
			normalMatrix = glm::mat3(glm::inverseTranspose(view * model));
			glUniformMatrix3fv(normalMatrixLoc, 1, GL_FALSE, glm::value_ptr(normalMatrix));

			raindrop.Draw(myCustomShader);
		}
	}

	renderSkyBox(skyboxShader);
	
}

void cleanup() {
	myWindow.Delete();
}

int main(int argc, const char* argv[]) {

	try {
		initOpenGLWindow();
	}
	catch (const std::exception & e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}

	setWindowCallbacks();
	initOpenGLState();
	initModels();
	initShaders();
	initUniforms();
	initFBO();
	initSkyBox();
	
	
	
	//glCheckError();

	while (!glfwWindowShouldClose(myWindow.getWindow())) {
		processMovement();
		renderScene();

		glfwPollEvents();
		glfwSwapBuffers(myWindow.getWindow());
		//glCheckError();
	}

	cleanup();

	return EXIT_SUCCESS;
}
