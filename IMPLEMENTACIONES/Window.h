#pragma once
#include<stdio.h>
#include<glew.h>
#include<glfw3.h>

class Window
{
public:
	Window();
	Window(GLint windowWidth, GLint windowHeight);
	int Initialise();
	GLfloat getBufferWidth() { return bufferWidth; }
	GLfloat getBufferHeight() { return bufferHeight; }
	GLfloat getXChange();
	GLfloat getYChange();

	//VARIABLES DE IVAN
	GLfloat getmuevex(){ return muevex; }
	GLfloat getmuevey() { return muevey; }
	GLfloat getmuevez(){ return muevez; }
	GLfloat getaccion() { return accion; }
	GLfloat gethi() { return hi; }
	//////////////////////////

	GLfloat getmueveAbajo() { return mueveabajo; }
	GLfloat getgiraHelice() { return giraHelice; }
	GLfloat getiniciaLuz() { return luz; }

	//VARIABLES DE ALICIA
	GLfloat getiniciaArbol() { return iniciaArbol; }
	GLfloat getiniciaGlobo() { return iniciaGlobo; }
	GLfloat getprueba() { return prueba; }
	//
	bool getShouldClose() {
		return  glfwWindowShouldClose(mainWindow);}
	bool* getsKeys() { return keys; }
	void swapBuffers() { return glfwSwapBuffers(mainWindow); }
	
	~Window();
private: 
	GLFWwindow *mainWindow;
	GLint width, height;
	bool keys[1024];
	int accion; 
	GLint bufferWidth, bufferHeight;
	void createCallbacks();
	GLfloat lastX;
	GLfloat lastY;
	GLfloat xChange;
	GLfloat yChange;

	GLfloat muevex;
	GLfloat muevey;
	GLfloat muevez;

	GLfloat mueveabajo;
	GLfloat muevearriba;
	GLfloat giraHelice;
	GLfloat prueba;

	GLfloat rotaxF;
	GLfloat rotaxA;
	GLfloat luz = 0;

	GLfloat iniciaArbol = 0; //alicia
	GLfloat iniciaGlobo = 0; //alicia
	GLfloat hi;
	bool flag; //IVAN

	bool mouseFirstMoved;
	static void ManejaTeclado(GLFWwindow* window, int key, int code, int action, int mode);
	static void ManejaMouse(GLFWwindow* window, double xPos, double yPos);

};

