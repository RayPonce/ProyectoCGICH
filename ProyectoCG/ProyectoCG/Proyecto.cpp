#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <random>  // Biblioteca para n�meros aleatorios
#include <ctime>   // Biblioteca para obtener la semilla basada en el tiempo actual

#include <stdio.h>
#include <string.h>
#include <cmath>
#include <vector>
#include <math.h>

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

//para iluminaci�n
#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;

//variables para animaci�n

float movOffset;


float angulovaria = 0.0f;
float angulovariaALA1 = 0.001f;
float angulovariaALA2 = 0.0f;
float rotDragon = 0.0f;
float movDragonZ = 0.0f;
float movDragonX = 0.0f;

bool circuito = false;
bool recorrido1 = true;
bool recorrido2 = false;
bool recorrido3 = false;
bool recorrido4 = false;
bool recorrido5 = false;


float movDadoX;
float movDadoy;
float movDado8y;
float movDado4y;
float rotDado8;
float rotDado4;

float rotDado8x;
float rotDado8y;
float rotDado8z;

float rotDado4x;
float rotDado4y;
float rotDado4z;

bool dadof = false;

int indiceCasillas;
int texturaTablero;

Window mainWindow;
std::vector<Mesh*> meshList;
std::vector<Shader> shaderList;

Camera camera;

Texture brickTexture;
Texture dirtTexture;
Texture plainTexture;
Texture pisoTexture;



Texture Tablero0;
Texture Tablero1;
Texture Tablero2;
Texture Tablero3;
Texture Tablero4;
Texture Tablero5;
Texture Tablero6;
Texture Tablero7;
Texture Tablero8;
Texture Tablero9;

Texture Tablero10;
Texture Tablero11;
Texture Tablero12;
Texture Tablero13;
Texture Tablero14;
Texture Tablero15;
Texture Tablero16;
Texture Tablero17;
Texture Tablero18;
Texture Tablero19;

Texture Tablero20;
Texture Tablero21;
Texture Tablero22;
Texture Tablero23;
Texture Tablero24;
Texture Tablero25;
Texture Tablero26;
Texture Tablero27;
Texture Tablero28;
Texture Tablero29;

Texture Tablero30;
Texture Tablero31;
Texture Tablero32;
Texture Tablero33;
Texture Tablero34;
Texture Tablero35;
Texture Tablero36;
Texture Tablero37;
Texture Tablero38;
Texture Tablero39;
Texture Tablero40;


Texture D4;
Texture D8;

Model Dado8c;
Model Dado4c;
Model Tablero;

Model Dragon_M;
Model Dragon_Ala1;
Model Dragon_Ala2;
Model Dragon_C1;
Model Dragon_C2;
Model Dragon_C3;
Model Dragon_C4;
Model Dragon_C5;

Model Patricio;
Model PieDerPat;
Model BrazoDerPat;
Model PieIzqPat;
Model BrazoIzqPat;

Model Larry;
Model CasaBob;
Model CasaPat;
Model Crustaceo;
Model CasaCala;
Model OsoMar;
Model GymLarry;
Model BaldeCar;
Model CasaDonC;
Model Medusa;
Model islaBobInicio;
Model ChozaBob;
Model Gary;
Model CangreBur;

Skybox skybox;

//materiales
Material Material_brillante;
Material Material_opaco;


//Sphere cabeza = Sphere(0.5, 20, 20);
GLfloat deltaTime = 0.0f;
GLfloat lastTime = 0.0f;
static double limitFPS = 1.0 / 60.0;

// luz direccional
DirectionalLight mainLight;
//para declarar varias luces de tipo pointlight
PointLight pointLights[MAX_POINT_LIGHTS];
SpotLight spotLights[MAX_SPOT_LIGHTS];

// Vertex Shader
static const char* vShader = "shaders/shader_light.vert";

// Fragment Shader
static const char* fShader = "shaders/shader_light.frag";

void animacion();

//c�lculo del promedio de las normales para sombreado de Phong
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


	unsigned int tableroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat tableroVertices[] = {
	   -20.0f,  0.0f,  20.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		20.0f,  0.0f,  20.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		20.0f,  0.0f, -20.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	   -20.0f,  0.0f, -20.0f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};


	Mesh* obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh* obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh* obj3 = new Mesh();
	obj3->CreateMesh(floorVertices, floorIndices, 32, 6);
	meshList.push_back(obj3);

	Mesh* obj4 = new Mesh();
	obj4->CreateMesh(vegetacionVertices, vegetacionIndices, 64, 12);
	meshList.push_back(obj4);


	Mesh* obj5 = new Mesh();
	obj5->CreateMesh(tableroVertices, tableroIndices, 32, 6);
	meshList.push_back(obj5);

}


void CreateShaders()
{
	Shader* shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	std::mt19937 generator(static_cast<unsigned int>(std::time(0)));

	int RD8 = 0;
	int RD4 = 0;

	mainWindow = Window(1366, 768); // 1280, 1024 or 1024, 768
	mainWindow.Initialise();

	CreateObjects();
	CreateShaders();

	camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), -60.0f, 0.0f, 0.5f, 0.5f);

	brickTexture = Texture("Textures/brick.png");
	brickTexture.LoadTextureA();
	dirtTexture = Texture("Textures/dirt.png");
	dirtTexture.LoadTextureA();
	plainTexture = Texture("Textures/plain.png");
	plainTexture.LoadTextureA();
	pisoTexture = Texture("Textures/pisoaj.tga");
	pisoTexture.LoadTextureA();

	Tablero0 = Texture("Textures/tablero0.tga");
	Tablero0.LoadTextureA();
	Tablero1 = Texture("Textures/tablero1.tga");
	Tablero1.LoadTextureA();
	Tablero2 = Texture("Textures/tablero2.tga");
	Tablero2.LoadTextureA();
	Tablero3 = Texture("Textures/tablero3.tga");
	Tablero3.LoadTextureA();
	Tablero4 = Texture("Textures/tablero4.tga");
	Tablero4.LoadTextureA();
	Tablero5 = Texture("Textures/tablero5.tga");
	Tablero5.LoadTextureA();
	Tablero6 = Texture("Textures/tablero6.tga");
	Tablero6.LoadTextureA();
	Tablero7 = Texture("Textures/tablero7.tga");
	Tablero7.LoadTextureA();
	Tablero8 = Texture("Textures/tablero8.tga");
	Tablero8.LoadTextureA();
	Tablero9 = Texture("Textures/tablero9.tga");
	Tablero9.LoadTextureA();
	Tablero10 = Texture("Textures/tablero10.tga");
	Tablero10.LoadTextureA();
	Tablero11 = Texture("Textures/tablero11.tga");
	Tablero11.LoadTextureA();
	Tablero12 = Texture("Textures/tablero12.tga");
	Tablero12.LoadTextureA();
	Tablero13 = Texture("Textures/tablero13.tga");
	Tablero13.LoadTextureA();
	Tablero14 = Texture("Textures/tablero14.tga");
	Tablero14.LoadTextureA();
	Tablero15 = Texture("Textures/tablero15.tga");
	Tablero15.LoadTextureA();
	Tablero16 = Texture("Textures/tablero16.tga");
	Tablero16.LoadTextureA();
	Tablero17 = Texture("Textures/tablero17.tga");
	Tablero17.LoadTextureA();
	Tablero18 = Texture("Textures/tablero18.tga");
	Tablero18.LoadTextureA();
	Tablero19 = Texture("Textures/tablero19.tga");
	Tablero19.LoadTextureA();
	Tablero20 = Texture("Textures/tablero20.tga");
	Tablero20.LoadTextureA();
	Tablero21 = Texture("Textures/tablero21.tga");
	Tablero21.LoadTextureA();
	Tablero22 = Texture("Textures/tablero22.tga");
	Tablero22.LoadTextureA();
	Tablero23 = Texture("Textures/tablero23.tga");
	Tablero23.LoadTextureA();
	Tablero24 = Texture("Textures/tablero24.tga");
	Tablero24.LoadTextureA();
	Tablero25 = Texture("Textures/tablero25.tga");
	Tablero25.LoadTextureA();
	Tablero26 = Texture("Textures/tablero26.tga");
	Tablero26.LoadTextureA();
	Tablero27 = Texture("Textures/tablero27.tga");
	Tablero27.LoadTextureA();
	Tablero28 = Texture("Textures/tablero28.tga");
	Tablero28.LoadTextureA();
	Tablero29 = Texture("Textures/tablero29.tga");
	Tablero29.LoadTextureA();
	Tablero30 = Texture("Textures/tablero30.tga");
	Tablero30.LoadTextureA();
	Tablero31 = Texture("Textures/tablero31.tga");
	Tablero31.LoadTextureA();
	Tablero32 = Texture("Textures/tablero32.tga");
	Tablero32.LoadTextureA();
	Tablero33 = Texture("Textures/tablero33.tga");
	Tablero33.LoadTextureA();
	Tablero34 = Texture("Textures/tablero34.tga");
	Tablero34.LoadTextureA();
	Tablero35 = Texture("Textures/tablero35.tga");
	Tablero35.LoadTextureA();
	Tablero36 = Texture("Textures/tablero36.tga");
	Tablero36.LoadTextureA();
	Tablero37 = Texture("Textures/tablero37.tga");
	Tablero37.LoadTextureA();
	Tablero38 = Texture("Textures/tablero38.tga");
	Tablero38.LoadTextureA();
	Tablero39 = Texture("Textures/tablero39.tga");
	Tablero39.LoadTextureA();
	Tablero40 = Texture("Textures/tablero40.tga");
	Tablero40.LoadTextureA();


	D8 = Texture("Textures/D8C.jpg");
	D8.LoadTextureA();
	D4 = Texture("Textures/D4C.jpg");
	D4.LoadTextureA();

	Dado8c = Model();
	Dado8c.LoadModel("Models/Dado8C.obj");
	Dado4c = Model();
	Dado4c.LoadModel("Models/Dado4C.obj");

	Patricio = Model();
	Patricio.LoadModel("Models/patricio.obj");
	PieDerPat = Model();
	PieDerPat.LoadModel("Models/piederpat.obj");
	PieIzqPat = Model();
	PieIzqPat.LoadModel("Models/pieizqpat.obj");
	BrazoDerPat = Model();
	BrazoDerPat.LoadModel("Models/brazoderpat.obj");
	BrazoIzqPat = Model();
	BrazoIzqPat.LoadModel("Models/brazoizqpat.obj");

	//Modelos universo Bob
	Larry = Model();
	Larry.LoadModel("Models/Larry.obj");
	CasaBob = Model();
	CasaBob.LoadModel("Models/Pi�aBob.obj");
	CasaPat = Model();
	CasaPat.LoadModel("Models/CasaPat.obj");
	CasaCala = Model();
	CasaCala.LoadModel("Models/CasaCalamardo.obj");
	Crustaceo = Model();
	Crustaceo.LoadModel("Models/Crustaceo.obj");
	OsoMar = Model();
	OsoMar.LoadModel("Models/OsoMarino.obj");
	GymLarry = Model();
	GymLarry.LoadModel("Models/GymLarry.obj");
	BaldeCar = Model();
	BaldeCar.LoadModel("Models/BaldeCarnada.obj");
	CasaDonC = Model();
	CasaDonC.LoadModel("Models/CasaDonCangrejo.obj");
	Medusa = Model();
	Medusa.LoadModel("Models/Medusa.obj");
	islaBobInicio = Model();
	islaBobInicio.LoadModel("Models/Jardin.obj");
	ChozaBob = Model();
	ChozaBob.LoadModel("Models/Choza.obj");
	Gary = Model();
	Gary.LoadModel("Models/Gary.obj");
	CangreBur = Model();
	CangreBur.LoadModel("Models/CangreBurg.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");
	skyboxFaces.push_back("Textures/Skybox/cielo.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, s�lo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaraci�n de primer luz puntual
	/*pointLights[0] = PointLight(1.0f, 0.0f, 0.0f,
		0.0f, 1.0f,
		0.0f, 2.5f, 1.5f,
		0.3f, 0.2f, 0.1f);
	pointLightCount++;
	*/
	unsigned int spotLightCount = 0;
	//linterna
	spotLights[0] = SpotLight(1.0f, 1.0f, 1.0f,
		0.0f, 2.0f,
		0.0f, 0.0f, 0.0f,
		0.0f, -1.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		5.0f);
	spotLightCount++;
	/*
	//luz fija
	spotLights[1] = SpotLight(0.0f, 0.0f, 1.0f,
		1.0f, 2.0f,
		5.0f, 10.0f, 0.0f,
		0.0f, -5.0f, 0.0f,
		1.0f, 0.0f, 0.0f,
		15.0f);
	spotLightCount++;
	*/

	GLuint uniformProjection = 0, uniformModel = 0, uniformView = 0, uniformEyePosition = 0,
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset = 0;
	GLuint uniformColor = 0;
	glm::mat4 projection;
	

	movOffset = 0.3f;
	movDadoX = 0.0f;
	movDadoy = 0.0f;
	movDado8y = 0.0f;
	movDado4y = 0.0f;
	rotDado8 = 0.0f;
	rotDado4 = 0.0f;

	rotDado8x = 0.0f;
	rotDado8y = 0.0f;
	rotDado8z = 0.0f;

	rotDado4x = 0.0f;
	rotDado4y = 0.0f;
	rotDado4z = 0.0f;

	indiceCasillas = 0;
	texturaTablero = 0;
	float rotOffDado = 15.0f;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		double temp = glfwGetTime();
		animacion();
		float rotExtPat = 5 * sin(6.0f * glfwGetTime() * 1.0f);
		glm::vec3 avatarPos;


		//Va al principio del while y antes del if de getbandera
		if (mainWindow.getcamara() == 0)
		{
			projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
			glm::vec3 cameraPosition = avatarPos + glm::vec3(0.0f, 5.0f, -10.0f); // Detr�s y arriba del avatar
			glm::mat4 view = glm::lookAt(cameraPosition, avatarPos, glm::vec3(0.0f, 1.0f, 0.0f));
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}
		else if (mainWindow.getcamara() == 1)
		{
			// Proyecci�n ortogr�fica mirando el tablero desde arriba
			float orthoSize = 80.0f;
			projection = glm::ortho(-orthoSize, orthoSize, -orthoSize, orthoSize, 25.0f, 200.0f);

			// Definir la vista desde arriba
			glm::mat4 view = glm::lookAt(
				glm::vec3(0.0f,	90.0f, 0.0f), // Posici�n de la c�mara arriba del tablero
				glm::vec3(0.0f, 0.0f, 0.0f),  // Mirando al centro del tablero
				glm::vec3(0.0f, 0.0f, -1.0f) 
			);
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			camera.keyControl(mainWindow.getsKeys(), deltaTime);
		}
		else if (mainWindow.getcamara() == 2)
		{
			// C�mara est�tica
			projection = glm::perspective(90.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);

			// Calcular la posici�n de Patricio (asumiendo que tienes esta informaci�n)
			glm::vec3 patricioPosition = glm::vec3(5.0f, 0.0f, 1.0f); // Ajusta esto a la posici�n real de Patricio

			// Definir la vista siguiendo a Patricio
			glm::mat4 view = glm::lookAt(
				patricioPosition + glm::vec3(0.0f, 5.0f, -10.0f), // Posici�n de la c�mara detr�s y arriba de Patricio
				patricioPosition, // Mirando a Patricio
				glm::vec3(0.0f, 0.0f, 1.0f)  // Vector "arriba"
			);
			glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(view));
			camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());
		}

		if (mainWindow.getBandera()) //Presionar Z para mover el dado y X para reiniciar la tirada
		{
			if (movDadoy > 0.0 && movDadoX > -5.0)
			{
				movDadoX += movOffset * deltaTime;
				movDadoy -= movOffset * deltaTime;
				movDado8y -= movOffset * deltaTime;
				movDado4y -= movOffset * deltaTime;
				rotDado8 += rotOffDado * deltaTime;
				rotDado4 += rotOffDado * deltaTime;

				// Distribuci�n para el dado de 8 caras en el rango [1, 8]
				std::uniform_int_distribution<int> distribution1(1, 8);
				RD8 = distribution1(generator);

				// Distribuci�n para el dado de 4 caras en el rango [1, 4]
				std::uniform_int_distribution<int> distribution2(1, 4);
				RD4 = distribution2(generator);
				indiceCasillas = RD4 + RD8;
			}
			else {
				dadof = true;
			}
			if (dadof) {



				std::cout << "indiceCasillas: " << indiceCasillas << std::endl;
				if (RD8 == 1) { // D8=1
					movDado8y = -0.6;
					rotDado8 = 0;
					rotDado8x = -30;
					rotDado8y = 0;
					rotDado8z = -70;
					RD8 = 0;
				}
				if (RD8 == 2) {//D8=2
					movDado8y = -0.6;
					rotDado8 = 0;
					rotDado8x = 30;
					rotDado8y = 0;
					rotDado8z = 110;
					RD8 = 0;
				}
				if (RD8 == 3) { // D8=3
					movDado8y = -0.6;
					rotDado8 = 0;
					rotDado8x = -30;
					rotDado8y = 0;
					rotDado8z = 70;
					RD8 = 0;
				}
				if (RD8 == 4) { // D8=4
					movDado8y = -0.6;
					rotDado8 = 0;
					rotDado8x = 150;
					rotDado8y = 0;
					rotDado8z = 70;
					RD8 = 0;
				}
				if (RD8 == 5) {//D8=5
					movDado8y = -1.6;
					rotDado8 = 0;
					rotDado8x = 71;
					rotDado8y = 0;
					rotDado8z = 0;
					RD8 = 0;
				}
				if (RD8 == 6) {//D8=6
					movDado8y = 0.3;
					rotDado8 = 0;
					rotDado8x = -110;
					rotDado8y = 0;
					rotDado8z = 0;
					RD8 = 0;
				}
				if (RD8 == 7) {//D8=7
					movDado8y = 0.3;
					rotDado8 = 0;
					rotDado8x = 0;
					rotDado8y = 0;
					rotDado8z = -180;
					RD8 = 0;
				}
				if (RD8 == 8) {//D8=8
					movDado8y = -1.6;
					rotDado8 = 0;
					rotDado8x = 0;
					rotDado8y = 0;
					rotDado8z = 0;
					RD8 = 0;
				}

				if (RD4 == 1) {//D4=1
					movDado4y = -1.6;
					rotDado4 = 0;
					rotDado4x = 0;
					rotDado4y = 0;
					rotDado4z = 0;
					RD4 = 0;
				}
				if (RD4 == 2) {//D4=2
					movDado4y = -0.7;
					rotDado4 = 0;
					rotDado4x = 110;
					rotDado4y = -120;
					rotDado4z = 0;
					RD4 = 0;
				}
				if (RD4 == 3) {//D4=3
					movDado4y = -0.7;
					rotDado4 = 0;
					rotDado4x = 110;
					rotDado4y = 0;
					rotDado4z = 0;
					RD4 = 0;
				}
				if (RD4 == 4) {//D4=4
					movDado4y = -0.7;
					rotDado4 = 0;
					rotDado4x = 110;
					rotDado4y = 120;
					rotDado4z = 0;
					RD4 = 0;
				}
			}

		}
		else {
			movDadoX = 0.0;
			movDadoy = 5.0;
			movDado8y = 5.0;
			movDado4y = 5.0;
			rotDado8 = 0.0;
			rotDado4 = 0.0;
			dadof = false;

		}


		//Recibir eventos del usuario
		glfwPollEvents();
		

		// Clear the window
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		skybox.DrawSkybox(camera.calculateViewMatrix(), projection);
		shaderList[0].UseShader();
		uniformModel = shaderList[0].GetModelLocation();
		uniformProjection = shaderList[0].GetProjectionLocation();
		uniformView = shaderList[0].GetViewLocation();
		uniformEyePosition = shaderList[0].GetEyePositionLocation();
		uniformColor = shaderList[0].getColorLocation();
		uniformTextureOffset = shaderList[0].getOffsetLocation();

		//informaci�n en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la c�mara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//informaci�n al shader de fuentes de iluminaci�n
		shaderList[0].SetDirectionalLight(&mainLight);
		shaderList[0].SetPointLights(pointLights, pointLightCount);
		shaderList[0].SetSpotLights(spotLights, spotLightCount);



		glm::mat4 model(1.0);
		glm::mat4 modelaux(1.0);
		glm::vec3 color = glm::vec3(1.0f, 1.0f, 1.0f);
		glm::vec2 toffset = glm::vec2(0.0f, 0.0f);

		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(30.0f, 1.0f, 30.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		pisoTexture.UseTexture();
		Material_opaco.UseMaterial(uniformSpecularIntensity, uniformShininess);

		meshList[2]->RenderMesh();



		//Dado8c
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0f, 0.0f, 1.0f));
		model = glm::translate(model, glm::vec3(-movDadoX, movDadoy, -2.0f));
		model = glm::translate(model, glm::vec3(-movDadoX, movDado8y, -2.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, rotDado8 * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));


		model = glm::rotate(model, rotDado8x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotDado8y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotDado8z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado8c.RenderModel();

		//Dado4c
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-movDadoX, movDadoy, -2.0f));
		model = glm::translate(model, glm::vec3(-movDadoX, movDado4y, -2.0f));
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		model = glm::rotate(model, rotDado4 * toRadians, glm::vec3(1.0f, 1.0f, 1.0f));
		model = glm::rotate(model, rotDado4z * toRadians, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::rotate(model, rotDado4x * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotDado4y * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));

		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Dado4c.RenderModel();


		//Modelos Bob
		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Larry.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		//model = glm::rotate(model, -140 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//CasaBob.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//CasaPat.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.6f, 0.6f, 0.6f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//CasaCala.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Crustaceo.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//OsoMar.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//GymLarry.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(26.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.7f, 0.7f, 0.7f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//BaldeCar.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.2f, 0.2f, 0.2f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//CasaDonC.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Medusa.RenderModel();

		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		islaBobInicio.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.4f, 0.4f, 0.4f));
		//model = glm::rotate(model, -80 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//ChozaBob.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));
		//model = glm::rotate(model, 180 * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//Gary.RenderModel();

		//model = glm::mat4(1.0);
		//model = glm::translate(model, glm::vec3(28.0f, 1.0f, 45.0f));
		//model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		////model = glm::rotate(model, rotObjCasillaY * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		//glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		//CangreBur.RenderModel();

		//Patricio
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(5.0, 0.0f, 1.0f));
		modelaux = model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Patricio.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.6f, 0.52f, 0.03f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, rotExtPat * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieDerPat.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.38f, 0.52f, 0.005f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -rotExtPat * toRadians, glm::vec3(1.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		PieIzqPat.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(-0.84f, 1.91f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, -rotExtPat * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoDerPat.RenderModel();

		model = modelaux;
		model = glm::translate(model, glm::vec3(0.68f, 1.99f, 0.0f));
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		model = glm::rotate(model, rotExtPat * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		BrazoIzqPat.RenderModel();

		toffset = glm::vec2(0.0, 0.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
		model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		glUniform2fv(uniformTextureOffset, 1, glm::value_ptr(toffset));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		color = glm::vec3(1.0f, 1.0f, 1.0f);
		glUniform3fv(uniformColor, 1, glm::value_ptr(color));
		if (texturaTablero == 0) {
			Tablero0.UseTexture();
		}
		else if (texturaTablero == 1) {
			Tablero1.UseTexture();
		}
		else if (texturaTablero == 2) {
			Tablero2.UseTexture();
		}
		else if (texturaTablero == 3) {
			Tablero3.UseTexture();
		}
		else if (texturaTablero == 4) {
			Tablero4.UseTexture();
		}
		else if (texturaTablero == 5) {
			Tablero5.UseTexture();
		}
		else if (texturaTablero == 6) {
			Tablero6.UseTexture();
		}
		else if (texturaTablero == 7) {
			Tablero7.UseTexture();
		}
		else if (texturaTablero == 8) {
			Tablero8.UseTexture();
		}
		else if (texturaTablero == 9) {
			Tablero9.UseTexture();
		}
		else if (texturaTablero == 10) {
			Tablero10.UseTexture();
		}
		else if (texturaTablero == 11) {
			Tablero11.UseTexture();
		}
		else if (texturaTablero == 12) {
			Tablero12.UseTexture();
		}
		else if (texturaTablero == 13) {
			Tablero13.UseTexture();
		}
		else if (texturaTablero == 14) {
			Tablero14.UseTexture();
		}
		else if (texturaTablero == 15) {
			Tablero15.UseTexture();
		}
		else if (texturaTablero == 16) {
			Tablero16.UseTexture();
		}
		else if (texturaTablero == 17) {
			Tablero17.UseTexture();
		}
		else if (texturaTablero == 18) {
			Tablero18.UseTexture();
		}
		else if (texturaTablero == 19) {
			Tablero19.UseTexture();
		}
		else if (texturaTablero == 20) {
			Tablero20.UseTexture();
		}
		else if (texturaTablero == 21) {
			Tablero21.UseTexture();
		}
		else if (texturaTablero == 22) {
			Tablero22.UseTexture();
		}
		else if (texturaTablero == 23) {
			Tablero23.UseTexture();
		}
		else if (texturaTablero == 24) {
			Tablero24.UseTexture();
		}
		else if (texturaTablero == 25) {
			Tablero25.UseTexture();
		}
		else if (texturaTablero == 26) {
			Tablero26.UseTexture();
		}
		else if (texturaTablero == 27) {
			Tablero27.UseTexture();
		}
		else if (texturaTablero == 28) {
			Tablero28.UseTexture();
		}
		else if (texturaTablero == 29) {
			Tablero29.UseTexture();
		}
		else if (texturaTablero == 30) {
			Tablero30.UseTexture();
		}
		else if (texturaTablero == 31) {
			Tablero31.UseTexture();
		}
		else if (texturaTablero == 32) {
			Tablero32.UseTexture();
		}
		else if (texturaTablero == 33) {
			Tablero33.UseTexture();
		}
		else if (texturaTablero == 34) {
			Tablero34.UseTexture();
		}
		else if (texturaTablero == 35) {
			Tablero35.UseTexture();
		}
		else if (texturaTablero == 36) {
			Tablero36.UseTexture();
		}
		else if (texturaTablero == 37) {
			Tablero37.UseTexture();
		}
		else if (texturaTablero == 38) {
			Tablero38.UseTexture();
		}
		else if (texturaTablero == 39) {
			Tablero39.UseTexture();
		}
		else if (texturaTablero == 40) {
			Tablero40.UseTexture();
		}
		Material_brillante.UseMaterial(uniformSpecularIntensity, uniformShininess);
		meshList[4]->RenderMesh();




		glDisable(GL_BLEND);
		glUseProgram(0);
		mainWindow.swapBuffers();
	}

	return 0;
}

void animacion()
{

	if (recorrido1)
	{
		rotDragon = 0;
		angulovaria += 0.5f * deltaTime;
		angulovariaALA1 += 5.0f * deltaTime;
		angulovariaALA2 -= 5.0f * deltaTime;
		movDragonX += 0.5f * deltaTime;
		if (movDragonX > 50)
		{
			recorrido1 = false;
			recorrido2 = true;
		}
	}
	if (recorrido2)
	{
		rotDragon = 90;
		angulovaria += 0.5f * deltaTime;
		angulovariaALA1 += 5.0f * deltaTime;
		angulovariaALA2 -= 5.0f * deltaTime;
		movDragonZ += 0.01f;
		if (movDragonZ > 50)
		{
			recorrido2 = false;
			recorrido3 = true;

		}
	}

	if (recorrido3)
	{
		rotDragon = 180;
		angulovaria += 0.5f * deltaTime;
		angulovariaALA1 += 5.0f * deltaTime;
		angulovariaALA2 -= 5.0f * deltaTime;
		movDragonX -= 0.01f;
		if (movDragonX < 0)
		{
			recorrido3 = false;
			recorrido4 = true;
		}
	}

	if (recorrido4)
	{
		rotDragon = 270;
		angulovaria += 0.5f * deltaTime;
		angulovariaALA1 += 5.0f * deltaTime;
		angulovariaALA2 -= 5.0f * deltaTime;
		movDragonZ -= 0.01f;
		if (movDragonZ < 0)
		{
			recorrido4 = false;
			recorrido1 = true;
		}
	}
}