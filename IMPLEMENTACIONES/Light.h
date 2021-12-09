#pragma once

#include <glew.h>
#include <glm.hpp>

//Para todas las luces, por ello vamos a heredar las la información de luz y reutilizarla. 

class Light
{
public:
	Light();
	Light(GLfloat red, GLfloat green, GLfloat blue, 
			GLfloat aIntensity, GLfloat dIntensity);

	~Light();

protected:
	glm::vec3 color;
	GLfloat ambientIntensity;
	GLfloat diffuseIntensity;
};

