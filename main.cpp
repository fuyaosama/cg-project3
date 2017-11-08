//-------------------------------------------------------------
// CAP5705 University of Florida
// copyright 2017 Corey Toler-Franklin 
// main.cpp for environment mapping program
//-------------------------------------------------------------






#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL.h>
#include "stb_image.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>
#include <iostream>
#include <string>
#include <vector>
#include "Shader.h"
//#include <model.h>
#include "Camera.h"
#include "Geometry.h"
#include "MetaballFactory.h"








//----------------------------------------------
// windows
//----------------------------------------------
int windowHeight = 1200;
int windowLength = 1200;


//----------------------------------------------
// Callback functions
//----------------------------------------------
void KeyCallback(GLFWwindow *window, int key, int scan, int act, int mode);
void MouseCallback(GLFWwindow *window, double xPosition, double yPosition);
void ScrollCallback(GLFWwindow *window, double deltaX, double deltaY);
void KeyMovement(void);
bool keys[1024];
GLfloat lastX = windowLength / 2, lastY = windowHeight / 2;
bool firstMouse = true;


//----------------------------------------------
// cube maps
//----------------------------------------------
std::vector<glm::vec3> InitCubeCoords(void);
GLuint GenerateCubeMap(std::vector<const char*> faces);
GLuint LoadTextures(char *textPath);
std::vector<const char*> cmapFiles      = { "../../data/images/negx.jpg",
                                            "../../data/images/posx.jpg", 
                                            "../../data/images/posy.jpg", 
                                            "../../data/images/negy.jpg", 
                                            "../../data/images/negz.jpg", 
                                            "../../data/images/posz.jpg" };




//----------------------------------------------
// Camera
//----------------------------------------------
Camera camera(glm::vec3(0.0f, 0.0f, 2.5f), glm::vec3(0.0f, 1.0f, 0.0f), -90.0f, 0.0f);
GLfloat deltaTime = 0.0f;
GLfloat lastFrame = 0.0f;




//----------------------------------------------------
// Main entry point
// TO DO - Manage all OpenGL draw calls here
// Create shader programs
//          e.g. Shader shader("Shaders/shader.vert", "Shaders/shader.frag");
// Load and use shader programs
//          e.g. shader.Use();
// Get and set uniform variables
//          e.g. glGetUniformLocation(shader.Program, "model");
// Set triangles to be drawn
// Bind and draw cubemap texures
//----------------------------------------------------
int main()
{

    //instantiating the window
    //---------------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
   // glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    GLFWwindow* window = glfwCreateWindow(windowLength, windowHeight, "Environment Mapping", nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Could not create the window."  << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);



    // associate callbacks
    //--------------------------------------------------------------
    glfwSetKeyCallback(window, KeyCallback);
    glfwSetCursorPosCallback(window, MouseCallback);
    glfwSetScrollCallback(window, ScrollCallback);


    // set the input mode
    //---------------------------------------------------------------
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);



    // Init GLEW
    //--------------------------------------------------------------
    //glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW not initialized." << std::endl;
        return -1;
    }
	//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	//{
	//	std::cout << "Failed to initialize GLAD" << std::endl;
	//	return -1;
	//}

    // OpenGL Window
    int widthBuff, heightBuff;
    glfwGetFramebufferSize(window, &widthBuff, &heightBuff);
    glViewport(0, 0, widthBuff, heightBuff);
	Shader skyboxShader("../../src/mainsrc/Shaders/environment.vert", "../../src/mainsrc/Shaders/environment.frag");
	Shader reflection("../../src/mainsrc/Shaders/reflection.vert", "../../src/mainsrc/Shaders/reflection.frag");
	Geometry teapot;
	teapot.Initialize("../../data/models/teapot.obj");
	

	float skyboxVertices[] = {
		// positions          
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f,  10.0f
	};
	//std::vector<glm::vec3> skyboxVertices = InitCubeCoords();
	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


	unsigned int potvao;
	glGenVertexArrays(1, &potvao);
	//glGenBuffers(1, &rfragment);
	//glBindVertexArray(skyboxVAO);
	//glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
	//glEnableVertexAttribArray(0);
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	//unsigned int cubeTexture = LoadTextures("../../data/images/negz.jpg");

	unsigned int cubemapTexture = GenerateCubeMap(cmapFiles);
    //game loop, as long as window is open
	//skyboxShader.Use();
	//glUniform1i(glGetUniformLocation(skyboxShader.Program, "skybox"),0);

	glm::mat4 model(glm::mat3(1.0f));
	model = glm::translate<float>(model, glm::vec3(0.0f, 0.0f, -.5f));

	std::cout <<potvao << std::endl;

    while (!glfwWindowShouldClose(window))
    {
        //deltaTime is used to calculate camera movement frame independently
        GLfloat currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
		//glm::mat4 model;
        KeyMovement();
		glDepthFunc(GL_LESS);
		

		glEnable(GL_DEPTH_TEST);
		glClear(GL_DEPTH_BUFFER_BIT);
		//render
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//std::cout << glm::radians(45.0f)<< std::endl;
		glm::mat4 view = camera.viewMat();
		glm::mat4 projection = glm::perspective(1000*glm::radians(camera.zoomfactor), (float)windowLength / (float)windowHeight, 0.1f, 300.0f);
		glm::mat4 fixView = glm::mat4(glm::mat3(view));

		reflection.Use();
		glUniformMatrix4fv(glGetUniformLocation(reflection.Program, "model"), 1, GL_FALSE, glm::value_ptr(model));
		glUniformMatrix4fv(glGetUniformLocation(reflection.Program, "view"), 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(glGetUniformLocation(reflection.Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
		glUniform3fv(glGetUniformLocation(reflection.Program, "cameraPos"), 1, glm::value_ptr(camera.pos));
		glBindVertexArray(potvao);
		//glUniformMatrix4fv(glGetUniformLocation(reflection.Program, "model"),1,GL_FALSE,&model[0][0]);
		teapot.Draw(8,9);
		glActiveTexture(GL_TEXTURE0);
		glBindVertexArray(0);
        // reset
		glDepthFunc(GL_LEQUAL);
		// change depth function so depth test passes when values are equal to depth buffer's content
		skyboxShader.Use();
		//view = glm::mat4(glm::mat3(camera.viewMat())); // remove translation from the view matrix
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"),1,GL_FALSE, glm::value_ptr(fixView));
		//glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "view"), 1, GL_FALSE, &view[0][0]);
		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.Program, "projection"), 1, GL_FALSE, &projection[0][0]);
		//skyboxShader.setMat4("projection", projection);
		// skybox cube
		
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
		glBindVertexArray(skyboxVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glBindVertexArray(0);
	 // set depth function back to default

        // swap buffers
        glfwSwapBuffers(window);
		glfwPollEvents();
    }

	glDeleteVertexArrays(1, &potvao);
	glDeleteVertexArrays(1, &skyboxVAO);
	//	glDeleteBuffers(1, &cubeVBO);
	glDeleteBuffers(1, &skyboxVAO);


    // end process
    glfwTerminate();	

    // return
    return 0;
}



//=====================================================================
// Cameras and callbacks
//=====================================================================
void KeyCallback(GLFWwindow *window, int key, int scan, int act, int mode)
{
    //escape with escape key
    if (key == GLFW_KEY_ESCAPE && act == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);


    if (key == GLFW_KEY_UP && act == GLFW_PRESS)
    {

    }

    if (key == GLFW_KEY_DOWN && act == GLFW_PRESS)
    {

    }

    if (key == GLFW_KEY_LEFT && act == GLFW_PRESS)
    {

    }

    if (key == GLFW_KEY_RIGHT && act == GLFW_PRESS)
    {

    }
    //updating keys table 
    if (act == GLFW_PRESS)
        keys[key] = true;
    else if (act == GLFW_RELEASE)
        keys[key] = false;

}
void MouseCallback(GLFWwindow *window, double xPosition, double yPosition)
{
    //first time being encountered
    if (firstMouse)
    {
        lastX = xPosition;
        lastY = yPosition;
        firstMouse = false;
    }
    

    //finding change
    GLfloat deltaX = xPosition - lastX;
    GLfloat deltaY = lastY - yPosition;

    //updating last coords
    lastX = xPosition;
    lastY = yPosition;

    camera.mouseIn(deltaX, deltaY);

}
void ScrollCallback(GLFWwindow *window, double deltaX, double deltaY)
{
    camera.mouseScroll(deltaY);
}
void KeyMovement()
{
    //simple forwards, backwards and strafe
    //handled by camera object
    if (keys[GLFW_KEY_W])
        camera.keyIn(FORWARD, deltaTime);
    if (keys[GLFW_KEY_S])
        camera.keyIn(BACKWARD, deltaTime);
    if (keys[GLFW_KEY_A])
        camera.keyIn(LEFT, deltaTime);
    if (keys[GLFW_KEY_D])
        camera.keyIn(RIGHT, deltaTime);
    if (keys[GLFW_KEY_SPACE])
        camera.keyIn(UP, deltaTime);
    if (keys[GLFW_KEY_LEFT_CONTROL])
        camera.keyIn(DOWN, deltaTime);
}








GLuint GenerateCubeMap(std::vector<const char*> faces)
{

	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i], &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}
//=====================================================================
// TO DO:
// Set-up Cube Map
// 1. Generate the texture id and bind it
//    Use  glGenTextures and glBindTexture for  GL_TEXTURE_CUBE_MAP
// 2. Load all the cubemap textures (one for each face of the cube - cmapFiles stores these).
//    Use the SOIL API  
//    e.g. unsigned char* image = SOIL_load_image(faceLocations[i], &width, &height, 0, SOIL_LOAD_RGB);
//    Remember to free the image memory once you are done with the image.
//    e.g. SOIL_free_image_data(image);
//    Use glTexImage2D to set the texture images
// 3. Use glTexParameteri to set the filter parameters for the GL_TEXTURE_CUBE_MAP
//=====================================================================
GLuint LoadTextures(char *textPath)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	unsigned char *texData;
	int width, height, channel;
	texData = SOIL_load_image(textPath, &width, &height, 0, SOIL_LOAD_RGB);
	if (texData) {
		glTexImage2D(GL_TEXTURE_2D,
			0,
			GL_RGB,
			width, height,
			0,
			GL_RGB,
			GL_UNSIGNED_BYTE,
			texData);
		SOIL_free_image_data(texData);
	}
	else {
		std::cout << "The image is not found " << textPath << "." << std::endl;
		SOIL_free_image_data(texData);
	}
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return textureID;
}




//=====================================================================
// TO DO:
// Set-up Cube Coordiates
// Initialize the cubemap coordinates.
// Returns a std::vector<glm::vec3>  with the coordinates 
//=====================================================================
std::vector<glm::vec3> InitCubeCoords(void)
{

	std::vector<glm::vec3> input;
		// positions          
	input.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	input.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	input.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	input.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	input.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	input.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	input.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	input.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	input.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));

	input.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	input.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	input.push_back(glm::vec3(1.0f, -1.0f, -1.0f));

	input.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	input.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, -1.0f, 1.0f));
	input.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));

	input.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, -1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(-1.0f, 1.0f, 1.0f));
	input.push_back(glm::vec3(-1.0f, 1.0f, -1.0f));

	input.push_back(glm::vec3(-1.0f, -1.0f, -1.0f));
	input.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	input.push_back(glm::vec3(1.0f, -1.0f, -1.0f));
	input.push_back(glm::vec3(-1.0f, -1.0f, 1.0f));
	input.push_back(glm::vec3(1.0f, -1.0f, 1.0f));

    return input;
}














