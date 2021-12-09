/*
Semestre 2022-1
Práctica : Iluminación
Cambios en el shader, en lugar de enviar la textura en el shader de fragmentos, enviaremos el finalcolor
*/
//para cargar imagen
#define STB_IMAGE_IMPLEMENTATION

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>
#include <Windows.h>

#include <glew.h>
#include <glfw3.h>

#include <glm.hpp>
#include <gtc\matrix_transform.hpp>
#include <gtc\type_ptr.hpp>
//para probar el importer
//#include<assimp/Importer.hpp>

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"


//para iluminación
#include "CommonValues.h"
#include "DirectionalLight.h" //Siempre debe existir, ilumina todo el escenario. Es el sol de luz blanca
#include "PointLight.h"       //Como un foco de vulbo. Ilumina a su alrrededor como una esfera. Su alcance se maneja con "atenuación lineal", no con radio. 
#include "SpotLight.h"        //Ilumina por medio de un cono. Usamos ángulo de apertura del cono. Linterna de mano 
//Todas las luces se les declara con la intensidad ambiental y difusa. 

#include "Material.h"

const float toRadians = 3.14159265f / 180.0f;
const float PI = 3.14159265f;

float movOffset;
float movHeli;
float giroArbol = 0.0;
float arribaAbajo = 0.0;
float subir = 0.0f;
float bandera = 0.0f;
float banderaA = 0.0f;
float x = 0.0f;
float y = 0.0f;
float banderaSkybox = 0.0f;
float skyboxCont = 0.0f;
float banderaPajaro;
float contadorPajaro;

float giroA = 0.0f;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture dadoTexture;
Texture pisoTexture;
Texture Tagave;
//PARIS
Texture arbolGenerico;
Texture arbolTodo;
Texture asfaltoObscuro;
Texture asfalto;
Texture concreto;
Texture esferas;
Texture frente;
Texture pasto;
Texture polvorienta;
Texture texturaArbol;
Texture texturaTorreEiffel;
Texture mikuu;

///////////////winhouse
Texture wh;
Model CasaM;
Model train;
Model flame;
Model e1;
Model e2;
Model e3;
Model e4;
Model e5;
Model e6;
Model e7;

///////////

//// MIKU
//
Model MT;
Model RH_M;
Model LH_M;
Model RA;
Model LA;
Model RL;
Model LL;
//

///////////////////////////Snowman

Sphere sp = Sphere(1, 20, 20);

///////////////////

Model Camino_M;
Model Paris;
Model ArbolNavidad;
Model TorreEiffel;
Model GloboAerostatico;
Model LamparaAlumbra;
Model Arboles;
Model Pajaro;
Model tren;
//PARIS


Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;

//luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;


// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";


//cálculo del promedio de las normales para sombreado de Phong
void calcAverageNormals(unsigned int* indices, unsigned int indiceCount, GLfloat* vertices, unsigned int verticeCount,
	unsigned int vLength, unsigned int normalOffset)
{
	for (size_t i = 0; i < indiceCount; i += 3)
	{
		unsigned int in0 = indices[i] * vLength;
		unsigned int in1 = indices[i + 1] * vLength;
		unsigned int in2 = indices[i + 2] * vLength;
		glm::vec3 v1(vertices[in1] - vertices[in0], vertices[in1 + 1] - vertices[in0 + 1], vertices[in1 + 2] - vertices[in0 + 2]);
		glm::vec3 v2(vertices[in2] - vertices[in0], vertices[in2 + 1] - vertices[in0 + 1], vertices[in2 + 2] - vertices[in0 + 2]);
		glm::vec3 normal = glm::cross(v1, v2);
		normal = glm::normalize(normal);

		in0 += normalOffset; in1 += normalOffset; in2 += normalOffset;
		vertices[in0] += normal.x; vertices[in0 + 1] += normal.y; vertices[in0 + 2] += normal.z;
		vertices[in1] += normal.x; vertices[in1 + 1] += normal.y; vertices[in1 + 2] += normal.z;
		vertices[in2] += normal.x; vertices[in2 + 1] += normal.y; vertices[in2 + 2] += normal.z;
	}

	for (size_t i = 0; i < verticeCount / vLength; i++)
	{
		unsigned int nOffset = i * vLength + normalOffset;
		glm::vec3 vec(vertices[nOffset], vertices[nOffset + 1], vertices[nOffset + 2]);
		vec = glm::normalize(vec);
		vertices[nOffset] = vec.x; vertices[nOffset + 1] = vec.y; vertices[nOffset + 2] = vec.z;
	}
}




void CreateObjects()
{
	unsigned int indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2
	};

	GLfloat vertices[] = {
		//	x      y      z			u	  v			nx	  ny    nz
			-1.0f, -1.0f, -0.6f,	0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, -1.0f, 1.0f,		0.5f, 0.0f,		0.0f, 0.0f, 0.0f,
			1.0f, -1.0f, -0.6f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f,		0.5f, 1.0f,		0.0f, 0.0f, 0.0f
	};

	unsigned int floorIndices[] = {
		0, 2, 1,
		1, 2, 3
	};

	GLfloat floorVertices[] = {
		-10.0f, 0.0f, -10.0f,	0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, -10.0f,	10.0f, 0.0f,	0.0f, -1.0f, 0.0f,
		-10.0f, 0.0f, 10.0f,	0.0f, 10.0f,	0.0f, -1.0f, 0.0f,
		10.0f, 0.0f, 10.0f,		10.0f, 10.0f,	0.0f, -1.0f, 0.0f
	};

	unsigned int vegetacionIndices[] = {
		0, 1, 2,
		0, 2, 3,
		4,5,6,
		4,6,7
	};

	GLfloat vegetacionVertices[] = {
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, -0.5f, 0.0f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.5f, 0.5f, 0.0f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		-0.5f, 0.5f, 0.0f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,

		0.0f, -0.5f, -0.5f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, -0.5f, 0.5f,		1.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, 0.5f,		1.0f, 1.0f,		0.0f, 0.0f, 0.0f,
		0.0f, 0.5f, -0.5f,		0.0f, 1.0f,		0.0f, 0.0f, 0.0f,
	};
	calcAverageNormals(indices, 12, vertices, 32, 8, 5);

	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	calcAverageNormals(vegetacionIndices, 12, vegetacionVertices, 64, 8, 5);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);

}

void CrearCubo()
{
	unsigned int cubo_indices[] = {
		// front
		0, 1, 2,
		2, 3, 0,
		// right
		4, 5, 6,
		6, 7, 4,
		// back
		8, 9, 10,
		10, 11, 8,

		// left
		12, 13, 14,
		14, 15, 12,
		// bottom
		16, 17, 18,
		18, 19, 16,
		// top
		20, 21, 22,
		22, 23, 20,
	};


	GLfloat cubo_vertices[] = {
		// front
		//x		y		z		S		T			NX		NY		NZ
		-0.5f, -0.5f,  0.5f,	0.27f,  0.35f,		0.0f,	0.0f,	-1.0f,	//0
		0.5f, -0.5f,  0.5f,		0.48f,	0.35f,		0.0f,	0.0f,	-1.0f,	//1
		0.5f,  0.5f,  0.5f,		0.48f,	0.64f,		0.0f,	0.0f,	-1.0f,	//2
		-0.5f,  0.5f,  0.5f,	0.27f,	0.64f,		0.0f,	0.0f,	-1.0f,	//3
		// right
		//x		y		z		S		T
		0.5f, -0.5f,  0.5f,	    0.52f,  0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f, -0.5f,  -0.5f,	0.73f,	0.35f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  -0.5f,	0.73f,	0.64f,		-1.0f,	0.0f,	0.0f,
		0.5f,  0.5f,  0.5f,	    0.52f,	0.64f,		-1.0f,	0.0f,	0.0f,
		// back
		-0.5f, -0.5f, -0.5f,	0.77f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f, -0.5f, -0.5f,		0.98f,	0.35f,		0.0f,	0.0f,	1.0f,
		0.5f,  0.5f, -0.5f,		0.98f,	0.64f,		0.0f,	0.0f,	1.0f,
		-0.5f,  0.5f, -0.5f,	0.77f,	0.64f,		0.0f,	0.0f,	1.0f,

		// left
		//x		y		z		S		T
		-0.5f, -0.5f,  -0.5f,	0.0f,	0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f, -0.5f,  0.5f,	0.23f,  0.35f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  0.5f,	0.23f,	0.64f,		1.0f,	0.0f,	0.0f,
		-0.5f,  0.5f,  -0.5f,	0.0f,	0.64f,		1.0f,	0.0f,	0.0f,

		// bottom
		//x		y		z		S		T
		-0.5f, -0.5f,  0.5f,	0.27f,	0.02f,		0.0f,	1.0f,	0.0f,
		0.5f,  -0.5f,  0.5f,	0.48f,  0.02f,		0.0f,	1.0f,	0.0f,
		 0.5f,  -0.5f,  -0.5f,	0.48f,	0.31f,		0.0f,	1.0f,	0.0f,
		-0.5f, -0.5f,  -0.5f,	0.27f,	0.31f,		0.0f,	1.0f,	0.0f,

		//UP
		 //x		y		z		S		T
		 -0.5f, 0.5f,  0.5f,	0.27f,	0.68f,		0.0f,	-1.0f,	0.0f,
		 0.5f,  0.5f,  0.5f,	0.48f,  0.68f,		0.0f,	-1.0f,	0.0f,
		  0.5f, 0.5f,  -0.5f,	0.48f,	0.98f,		0.0f,	-1.0f,	0.0f,
		 -0.5f, 0.5f,  -0.5f,	0.27f,	0.98f,		0.0f,	-1.0f,	0.0f,

	};

	Mesh* cubo = new Mesh();
	cubo->CreateMesh(cubo_vertices, cubo_indices, 192, 36);
	meshList.push_back(cubo);

}

/// /////////////////////////////Snowman
void CrearCilindro(int res, float height, float R) {

	//constantes utilizadas en los ciclos for
	int n, i, coordenada = 0;
	//número de vértices ocupados
	int verticesBase = (res + 1) * 6;
	//cálculo del paso interno en la circunferencia y variables que almacenarán cada coordenada de cada vértice
	GLfloat dt = 2 * PI / res, x, z, y = -0.5f;
	//apuntadores para guardar todos los vértices e índices generados
	GLfloat* vertices = (GLfloat*)calloc(sizeof(GLfloat*), (verticesBase) * 3);
	unsigned int* indices = (unsigned int*)calloc(sizeof(unsigned int*), verticesBase);

	//ciclo for para crear los vértices de las paredes del cilindro
	for (n = 0; n <= (res); n++) {
		if (n != res) {
			x = R * cos((n)*dt);
			z = R * sin((n)*dt);
		}
		//caso para terminar el círculo
		else {
			x = R * cos((0) * dt);
			z = R * sin((0) * dt);
		}
		for (i = 0; i < 6; i++) {
			switch (i) {
			case 0:
				vertices[i + coordenada] = x;
				break;
			case 1:
				vertices[i + coordenada] = y;
				break;
			case 2:
				vertices[i + coordenada] = z;
				break;
			case 3:
				vertices[i + coordenada] = x;
				break;
			case 4:
				vertices[i + coordenada] = 0.5;
				break;
			case 5:
				vertices[i + coordenada] = z;
				break;
			}
		}
		coordenada += 6;
	}

	//ciclo for para crear la circunferencia inferior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices[coordenada + i] = x;
				break;
			case 1:
				vertices[coordenada + i] = -0.5f;
				break;
			case 2:
				vertices[coordenada + i] = z;
				break;
			}
		}
		coordenada += 3;
	}

	//ciclo for para crear la circunferencia superior
	for (n = 0; n <= (res); n++) {
		x = R * cos((n)*dt);
		z = R * sin((n)*dt);
		for (i = 0; i < 3; i++) {
			switch (i) {
			case 0:
				vertices[coordenada + i] = x;
				break;
			case 1:
				vertices[coordenada + i] = 0.5;
				break;
			case 2:
				vertices[coordenada + i] = z;
				break;
			}
		}
		coordenada += 3;
	}

	//Se generan los indices de los vértices
	for (i = 0; i < verticesBase; i++) {
		indices[i] = i;
	}

	//se genera el mesh del cilindro
	Mesh* cilindro = new Mesh();
	cilindro->CreateMesh(vertices, indices, coordenada, verticesBase);
	meshList.push_back(cilindro);
}

void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}



int main()
{
	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	//LINEAS DE ALEX------------------
	//wiPlaySound(TEXT("Navidad"), NULL, SND_ASYNC);///Para reproducir la cancion
	//--------------------------------

	CreateObjects();
	CrearCubo();
	/// /////////////////////////////////////Snowman
	CrearCilindro(36, 1.0f, 1.0f);
	//CrearCono(36, 3.0f, 2.0f);
	///////////////////


	CreateShaders();



	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 5.0f, 0.5f);

	//Texturas
	mikuu = Texture("Textures/miku.png");
	//mikuu.LoadTextureA();

	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	arbolGenerico = Texture("Textures/arbol_generico.jpg");
	arbolGenerico.LoadTextureA();

	arbolTodo = Texture("Textures/arbol_todo.png");
	arbolTodo.LoadTextureA();

	asfaltoObscuro = Texture("Textures/asfalto obscuro.jpg");
	asfaltoObscuro.LoadTextureA();

	asfalto = Texture("Textures/asfalto.jpg");
	asfalto.LoadTextureA();

	concreto = Texture("Textures/concreto.jpg");
	concreto.LoadTextureA();

	esferas = Texture("Textures/esferas.png");
	esferas.LoadTextureA();

	frente = Texture("Textures/frente.jpeg");
	frente.LoadTextureA();

	pasto = Texture("Textures/pasto.jpg");
	pasto.LoadTextureA();

	polvorienta = Texture("Textures/polvorienta.jpg");
	polvorienta.LoadTextureA();

	texturaArbol = Texture("Textures/textura_arbol.jpg");
	texturaArbol.LoadTextureA();

	texturaTorreEiffel = Texture("Textures/textura_torre_eiffel.png");
	texturaTorreEiffel.LoadTextureA();

	/////////////////////////////////////////////////////////////////miku
	//
	MT = Model();
	MT.LoadModel("Models/MT.obj");
	RH_M = Model();
	RH_M.LoadModel("Models/RH.obj");
	LH_M = Model();
	LH_M.LoadModel("Models/LH.obj");

	RA = Model();
	RA.LoadModel("Models/RA.obj");
	LA = Model();
	LA.LoadModel("Models/LA.obj");

	RL = Model();
	RL.LoadModel("Models/RL.obj");
	LL = Model();
	LL.LoadModel("Models/LL.obj");

	////////////Modelos de interior
	///////////////////winhouse
	CasaM = Model();
	//CasaM.LoadModel("Models/winhouse.obj");
	CasaM.LoadModel("Models/haus.obj");
	flame = Model();
	flame.LoadModel("Models/flame.obj");

	//CasaM.LoadModel("Models/Snow covered CottageOBJ.obj");
	//seat= Model();
	//seat.LoadModel("Models/Snow covered CottageOBJ.obj");
	e1 = Model();
	e1.LoadModel("Models/sillon.obj");
	e2 = Model();
	e2.LoadModel("Models/chim.obj");
	e3 = Model();
	e3.LoadModel("Models/13110_Fireplace_v2_l3.obj");
	e4 = Model();
	//e4.LoadModel("Models/sillon.obj");
	train.LoadModel("Models/tren.obj");
	train= Model();
	///////////////


	//
	
	//PARIS - Modelos *************************
	Paris = Model();
	Paris.LoadModel("Models/please.obj");

	ArbolNavidad = Model();
	ArbolNavidad.LoadModel("Models/arbolnavidad.obj");

	TorreEiffel = Model();
	TorreEiffel.LoadModel("Models/torreEiffel.obj");

	GloboAerostatico = Model();
	GloboAerostatico.LoadModel("Models/globoAerostatico.obj");

	LamparaAlumbra = Model();
	LamparaAlumbra.LoadModel("Models/lamparaAlumbra.obj");

	Pajaro = Model();
	Pajaro.LoadModel("Models/Ave.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox2/posx.jpg");
	skyboxFaces.push_back("Textures/Skybox2/negx.jpg");
	skyboxFaces.push_back("Textures/Skybox2/negy.jpg");
	skyboxFaces.push_back("Textures/Skybox2/posy.jpg");
	skyboxFaces.push_back("Textures/Skybox2/posz.jpg");
	skyboxFaces.push_back("Textures/Skybox2/negz.jpg");
	skybox = Skybox(skyboxFaces);
	
	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);

	/////////////////////////////////////miku
	glm::vec3 posmiku = glm::vec3(0.0f, 20.0f, -1.5f);
	////////////////////////////////////
	///////////////////////////Snowman
	glm::vec3 possm = glm::vec3(-5.0f, posmiku.y + 1.0, -5.0f);
	/////////////////// 


	//posición inicial del helicóptero
	glm::vec3 posblackhawk = glm::vec3(-20.0f, 6.0f, -1.0);

	//posición inicial del coche
	glm::vec3 poscoche = glm::vec3(0.0f, 0.5f, -1.5f);

	//pos ini winhouse
	//model = glm::translate(model, glm::vec3(-65.0f, posmiku.y - 2.0f, -250.0f));
	glm::vec3 posh = glm::vec3(-65.0f, posmiku.y - 2.0f, -250.0f);

	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.3f, 0.3f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	unsigned int spotLightCount = 0;

	//linterna camara
	spotLights[0] = SpotLight(0.0f, 0.0f, 1.0f,
		0.0f, 2.0f,
		-100.0f, 44.0f, 32.0f,//posicion
		0.0f, -1.0f, 0.0f,//direccion donde apunta
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;

	//poste izquierda
	spotLights[1] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		-380.0f, 50.0f, 40.0f,//posicion
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	//poste derecho
	spotLights[2] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		-100.0f, 50.0f, 40.0f,//posicion
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	//arbol
	spotLights[3] = SpotLight(0.0f, 1.0f, 0.0f,
		1.0f, 2.0f,
		-230.0f, 50.0f, 40.0f,//posicion
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;


	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

	///////////////////////////Snowman
	sp.init();
	sp.load();
	////////////////////////////////


	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;

		//Recibir eventos del usuario
		glfwPollEvents();
		camera.keyControl(mainWindow.getsKeys(), mainWindow.getaccion(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

		////////////////////////////////////////////////////////////////MIKU////
		
		if (!camera.type) {
			camera.position.x = posmiku.x + mainWindow.getmuevex() + 5.0f;
			camera.position.y = posmiku.y + mainWindow.getmuevey() + 5.0f;
			camera.position.z = posmiku.z + mainWindow.getmuevez();
			/*camera.front = glm::vec3(0.0f, 0.0f, -1.0f);
			camera.up = glm::vec3(0.0f, 1.0f, 0.0f);
			camera.right = glm::vec3(1.0f, 0.0f, 0.0f);*/
		}
		else {
			//camera.position.x = poscoche.x + mainWindow.getmuevex() + 20.0f;
			camera.position.y = posmiku.y + mainWindow.getmuevey() + 200.0f;
			//camera.position.z = poscoche.z + mainWindow.getmuevez() - 5.0f;
			camera.front = glm::vec3(0.0f, -1.0f, 0.0f);
			camera.up = glm::vec3(-1.0f, 0.0f, 0.0f);
			camera.right = glm::vec3(0.0f, 0.0f, -1.0f);
		}
		// 
		// 
		///////////////////////////////////////////////
		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		//luz ligada a la cámara de tipo flash 
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());


		//luz del faro del coche
		//spotLights[2].SetPos(glm::vec3(0.0f, 0.0f, mainWindow.getmueveFrente()));

		//luz del faro del helicoptero
		//spotLights[2].SetPos(glm::vec3(0.0f, 0.0f, mainWindow.getmueveFrente()));


		//información al shader de fuentes de iluminación
		shaderList[0].SetDirectionalLight(&mainLight);
	


		glm::mat4 model(1.0);



		


		////////////////////////////////////////////Snowman
		// model = glm::translate(model, glm::vec3(posmiku.x+4.0f, posmiku.y + 1.0f, posmiku.z-10.0f));
		//glm::mat4 model(1.0);
		model = glm::mat4(1.0);
		glm::mat4 matrizauxiliar(1.0);
		glm::mat4 matrizpalma(1.0);
		glm::mat4 ms(1.0);

		//model = glm::translate(model, glm::vec3(possm.x, possm.y, possm.z));
		model = glm::translate(model, glm::vec3(posh.x + 4.0, posh.y + 1.0f, posh.z - 10.0f));
		model = glm::rotate(model, glm::radians(mainWindow.gethi()),
			glm::vec3(0.0f, 0.0f, 1.0f));
		matrizauxiliar = model;
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		sp.render();



		////--------
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(possm.x, possm.y + 2.5, possm.z));
		model = glm::translate(model, glm::vec3(posh.x + 4.0, posh.y + 3.5, posh.z-10.0f));
		model = glm::rotate(model, glm::radians(mainWindow.gethi()),
			glm::vec3(1.0f, 0.0f, 0.0f));
		matrizauxiliar = model;
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		//glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));

		sp.render();

		model = glm::mat4(1.0f);
		//color = glm::vec3(0.75f, 0.8f, 0.8f);
		//model = glm::translate(model, glm::vec3(possm.x, possm.y + 4.5, possm.z));
		model = glm::translate(model, glm::vec3(posh.x + 4.0, posh.y + 5.5, posh.z-10.0f));
		model = glm::rotate(model, glm::radians(mainWindow.gethi() * 40 * toRadians), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla Y se rota sobre el eje y

		ms = model;
		model = glm::scale(model, glm::vec3(1.0f, 2.0f, 1.0f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay() * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.gethi() * toRadians), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz() * toRadians), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla Y se rota sobre el eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();

		model = glm::mat4(1.0f);
		//color = glm::vec3(0.75f, 0.8f, 0.8f);
		model = ms;
		model = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
		model = glm::scale(model, glm::vec3(1.5f, 1.0f, 1.5f));
		//model = glm::rotate(model, glm::radians(mainWindow.getrotay() * toRadians), glm::vec3(0.0f, 1.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.gethi() * toRadians), glm::vec3(1.0f, 0.0f, 0.0f));  //al presionar la tecla Y se rota sobre el eje y
		//model = glm::rotate(model, glm::radians(mainWindow.getrotaz() * toRadians), glm::vec3(0.0f, 0.0f, 1.0f));  //al presionar la tecla Y se rota sobre el eje y
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));//FALSE ES PARA QUE NO SEA TRANSPUESTA
		//glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		meshList[4]->RenderMesh();


		//////////////////////////////// snsn
		
		////////////////////////////winhouse
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-65.0f, posmiku.y - 2.0f, -250.0f));
		model = glm::translate(model, glm::vec3(posh.x, posh.y-2.0f , posh.z));
		model = glm::scale(model, glm::vec3(0.75f, 1.0f, 0.75f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));

		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//wh.UseTexture();
		CasaM.RenderModel();
		///
		model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(-65.0f, posmiku.y + 1.0f, -250.0f));
		//model = glm::translate(model, glm::vec3(-0.0f, posmiku.y + 1.0f, -0.0f));
		model = glm::translate(model, glm::vec3(posh.x-0.0f, posh.y + 1.0f, posh.z-0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));

		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		e1.RenderModel();
		///
		model = glm::mat4(1.0);
		// model = glm::translate(model, glm::vec3(-65.0f, posmiku.y + 1.0f, -240.0f));
		model = glm::translate(model, glm::vec3(posh.x+0.0f, posh.y + 1.0f, posh.z+10.0f));
		//model = glm::translate(model, glm::vec3(0.0f, posmiku.y + 0.0f, 10.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, -180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		e1.RenderModel();
		//train.RenderModel();

	///chim
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posh.x, posh.y + 1.0f, posh.z-10.0f));
		//model = glm::translate(model, glm::vec3(posmiku.x, posmiku.y + 1.0f, posmiku.z-10.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//train.RenderModel();
		e2.RenderModel();
	///	
		// Árbol navideño

		model = glm::mat4(1.0);
		//neg -delante, neg-abajo,neg-derecha
		//model = glm::translate(model, glm::vec3(100.0f, posmiku.y + 2.0f, 10.0f));
		model = glm::translate(model, glm::vec3(posh.x-8.0f, posh.y - 5.0f, posh.z+10.0f));
		//model = glm::rotate(model, giroArbol * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.0f, 2.0f, 2.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArbolNavidad.RenderModel();
	///
		model = glm::mat4(1.0);
		//Cuando multiplicas dentro de la sinodal lo hace lento porque cambia la velocidad, antes del seno es la altura de que tanto va a subir o bajar. 
		//model = glm::translate(model, glm::vec3(2.0f + x, 100.0f, 2.0f * 4.0f * sin(contadorPajaro * 0.6)));
		//model = glm::translate(model, glm::vec3(-0.5f* sin(contadorPajaro * 0.6), posmiku.y, 1.0f * sin(contadorPajaro * 0.6)));
		
		//model = glm::translate(model, glm::vec3(+5.0f+6.0f * (cos(contadorPajaro * 0.6)), posmiku.y, 15.0f+2.0f*sin(contadorPajaro * 0.6)));
		model = glm::translate(model, glm::vec3(posh.x+5.0f + 6.0f * (cos(contadorPajaro * 0.6)), posh.y, posh.z+15.0f + 2.0f * sin(contadorPajaro * 0.6)));

		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		model = glm::rotate(model, 180*toRadians, glm::vec3(0.0f, 8.0f*(sin(x*0.125f)), 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//train.RenderModel();
		Pajaro.RenderModel();
	///
		model = glm::mat4(1.0);
		//Cuando multiplicas dentro de la sinodal lo hace lento porque cambia la velocidad, antes del seno es la altura de que tanto va a subir o bajar. 
		//model = glm::translate(model, glm::vec3(2.0f + x, 100.0f, 2.0f * 4.0f * sin(contadorPajaro * 0.6)));
		//model = glm::translate(model, glm::vec3(-0.5f* sin(contadorPajaro * 0.6), posmiku.y, 1.0f * sin(contadorPajaro * 0.6)));
		//posmiku.x, posmiku.y + 1.0f, posmiku.z - 10.0f
		model = glm::translate(model, glm::vec3(posh.x, (posh.y + 2.0f + 1.0f * sin(mainWindow.gethi())), posh.z- 8.5f +1.0* mainWindow.gethi()));
		//model = glm::translate(model, glm::vec3(posmiku.x, (posmiku.y + 2.0f + 1.0f * sin(mainWindow.gethi())), posmiku.z-8.5f + 1.0f * mainWindow.gethi()));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 8.0f * (sin(x * 0.125f)), 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		flame.RenderModel();
		//////////////winwin

		////////////////////////////////////////////////////////////////MIKU
		
		glm::mat4 mara(1.0);
		glm::mat4 ma1(1.0);
		glm::mat4 ma2(1.0);
		glm::mat4 ma3(1.0);

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(posmiku.x + mainWindow.getmuevex(), posmiku.y, posmiku.z + mainWindow.getmuevez()));

		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		ma1 = model;
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		MT.RenderModel();

		model = glm::mat4(1.0);
		model = ma1;
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(mainWindow.gethi(), 0.0f, 0.0f));
		ma2 = model;
		//model = glm::translate(mara, glm::vec3(posmiku.x + mainWindow.getmuevex(), posmiku.y, posmiku.z + mainWindow.getmuevez()));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RA.RenderModel();

		model = glm::mat4(1.0);
		model = ma1;
		//model = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LA.RenderModel();

		model = glm::mat4(1.0);
		model = ma1;
		// = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RL.RenderModel();

		model = glm::mat4(1.0);
		model = ma1;
		//marl = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LL.RenderModel();

		///
		model = glm::mat4(1.0);
		model = ma2;
		//marl = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		RH_M.RenderModel();

		model = glm::mat4(1.0);
		model = ma1;
		//marl = glm::rotate(model, -90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		LH_M.RenderModel();

		/////////////////////////////////////////////////////////////////////////////MIKU

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//pisoTexture.UseTexture();
		//agregar material al plano de piso
		//Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);
		//meshList[2]->RenderMesh();


		////*************************************PARIS**********************************

		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));

		//model = glm::translate(model, glm::vec3(0.0f, mainWindow.getmueveAbajo(), 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, mainWindow.getmueveArriba(), 0.0f));

		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Paris.RenderModel();


		// Árbol navideño

		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-230.0f, 0.0f, 40.0f));
		
		if (mainWindow.getiniciaArbol() > 0) {
			if (banderaA > 0) {
				if (giroArbol < 20.0) {
					giroArbol += 0.009;
				}
				else {
					banderaA = -1.0f;
				}
			}
			else {
				if (giroArbol > -20.0) {
					giroArbol -= 0.009;
				}
				else {
					banderaA = 1.0f;
				}
			}
		}


		if (mainWindow.getiniciaLuz() > 0) {
			shaderList[0].SetPointLights(pointLights, pointLightCount);
			shaderList[0].SetSpotLights(spotLights, spotLightCount);
			}


		if (banderaSkybox > 0) {
			if (skyboxCont < 60.0)
			{	
				skyboxCont += 0.1;
			}
			else
			{
				std::vector<std::string> skyboxFaces;
				skyboxFaces.push_back("Textures/Skybox2/posx.jpg");
				skyboxFaces.push_back("Textures/Skybox2/negx.jpg");
				skyboxFaces.push_back("Textures/Skybox2/negy.jpg");
				skyboxFaces.push_back("Textures/Skybox2/posy.jpg");
				skyboxFaces.push_back("Textures/Skybox2/posz.jpg");
				skyboxFaces.push_back("Textures/Skybox2/negz.jpg");
				skybox = Skybox(skyboxFaces);
				banderaSkybox = -1.0f;
			}
		}
		else {
			if (skyboxCont > -60.0)
			{	
				skyboxCont -= 0.1;
			}
			else
			{
				std::vector<std::string> skyboxFaces;
				skyboxFaces.push_back("Textures/Skybox/noche.jpg");
				skyboxFaces.push_back("Textures/Skybox/noche1.jpg");
				skyboxFaces.push_back("Textures/Skybox/noche2.jpg");
				skyboxFaces.push_back("Textures/Skybox/noche3.jpg");
				skyboxFaces.push_back("Textures/Skybox/noche4.jpg");
				skyboxFaces.push_back("Textures/Skybox/noche5.jpg");
				skybox = Skybox(skyboxFaces);

				banderaSkybox = 1.0f;
			}
		}

		model = glm::rotate(model, giroArbol * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(8.0f, 8.0f, 8.0f));

		//model = glm::translate(model, glm::vec3(0.0f, mainWindow.getmueveAbajo(), 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, mainWindow.getmueveArriba(), 0.0f));

		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		ArbolNavidad.RenderModel();

		// Torre Eiffel

		model = glm::mat4(1.0);

		model = glm::translate(model, glm::vec3(-250.0f, 10.0f, -140.0f));
		model = glm::scale(model, glm::vec3(4.0f, 4.0f, 4.0f));

		//model = glm::translate(model, glm::vec3(0.0f, mainWindow.getmueveAbajo(), 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, mainWindow.getmueveArriba(), 0.0f));

		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		TorreEiffel.RenderModel();

		// Globo Aerostático

		model = glm::mat4(1.0);
		//arribaAbajo += 0.009;
		model = glm::translate(model, glm::vec3(-150.0f, 0.0f, 80.0f));
		
		if (mainWindow.getiniciaGlobo() > 0){
			if (bandera > 0) {
				if (subir < 100.0) {
					subir += 0.1f;
				}
				else {
					bandera = -1.0f;
				}
			}
			else {
				if (subir > 20.0) {
					subir -= 0.1;
				}
				else {
					bandera = 1.0f;
				}

			}
		}


		model = glm::translate(model, glm::vec3(-100.0f, subir, -140.0f));
		model = glm::scale(model, glm::vec3(15.0f, 15.0f, 15.0f));

		//model = glm::translate(model, glm::vec3(0.0f, mainWindow.getmueveAbajo(), 0.0f));
		//model = glm::translate(model, glm::vec3(0.0f, mainWindow.getmueveArriba(), 0.0f));

		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GloboAerostatico.RenderModel();

	// LAMPARA 1 PARA ALUMBRAR CALLES - Lado izquierdo

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-380.0f, 15.0f, 40.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
	//model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LamparaAlumbra.RenderModel();


	// LAMPARA 2 PARA ALUMBRAR CALLES - Lado derecho 

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, 15.0f, 40.0f));
		model = glm::scale(model, glm::vec3(20.0f, 20.0f, 20.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		LamparaAlumbra.RenderModel();
	//***********************************************************************************

		// PÁJARO


		if (banderaPajaro > 0) {
			if (contadorPajaro < 40.0)
			{
				contadorPajaro += 0.01*deltaTime;
				x += 0.1;
			}
			else
			{
				banderaPajaro = -1.0f;
			}
		}
		else {
			if (contadorPajaro > -40.0)
			{
				contadorPajaro -= 0.01 * deltaTime;
				x -= 0.1;
			}
			else
			{
				banderaPajaro = 1.0f;
			}
		}
		-
		model = glm::mat4(1.0);
		//Cuando multiplicas dentro de la sinodal lo hace lento porque cambia la velocidad, antes del seno es la altura de que tanto va a subir o bajar. 

		model = glm::translate(model, glm::vec3(2.0f + x,100.0f, 2.0f * 4.0f * sin(contadorPajaro * 0.6)));


		model = glm::scale(model, glm::vec3(30.0f, 30.0f, 30.0f));
		model = glm::rotate(model, 180 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Pajaro.RenderModel();

		glUseProgram(0);

		mainWindow.swapBuffers();
	}

	return 0;
}
