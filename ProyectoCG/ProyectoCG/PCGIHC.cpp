#define STB_IMAGE_IMPLEMENTATION

#include <iostream>
#include <random>  // Biblioteca para números aleatorios
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

#include "Window.h"
#include "Mesh.h"
#include "Shader_light.h"
#include "Camera.h"
#include "Texture.h"
#include "Sphere.h"
#include"Model.h"
#include "Skybox.h"

#include "CommonValues.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "Material.h"
const float toRadians = 3.14159265f / 180.0f;




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

bool dadof=false;

float rotCasaArbolKND;

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

//Modelos Los chicos del barrio
Model CasaArbolKND;
Model CommonCold;
Model CoolBus;
Model Dragstuh;
Model Elimonater;
Model Musket;
Model Mosquito;
Model Mushi;
Model N2;
Model N4;
Model N9;
Model Roadstar;
Model SCAMPP;
Model SPLANKE;
Model GumGun;
Model Machineggun;
Model SimioArcoiris;
Model Visor;
//Modelos Bob Esponja



//Modelos Alicia en el pais de las maravillas

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
	

	unsigned int tableroIndices[] = {
	   0, 1, 2,
	   0, 2, 3,
	};

	GLfloat tableroVertices[] = {
	   -60.0f,  0.0f,  60.0f,		0.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		60.0f,  0.0f,  60.0f,		1.0f, 0.0f,		0.0f, -1.0f, 0.0f,
		60.0f,  0.0f, -60.0f,		1.0f, 1.0f,		0.0f, -1.0f, 0.0f,
	   -60.0f,  0.0f, -60.0f,		0.0f, 1.0f,		0.0f, -1.0f, 0.0f,

	};


	Mesh *obj1 = new Mesh();
	obj1->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj1);

	Mesh *obj2 = new Mesh();
	obj2->CreateMesh(vertices, indices, 32, 12);
	meshList.push_back(obj2);

	Mesh *obj3 = new Mesh();
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
	Shader *shader1 = new Shader();
	shader1->CreateFromFiles(vShader, fShader);
	shaderList.push_back(*shader1);
}




int main()
{
	std::mt19937 generator(static_cast<unsigned int>(std::time(0)));

	int RD8 = 0;
	int RD4 = 0;

	mainWindow = Window(1366, 768);
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
	pisoTexture = Texture("Textures/piso.tga");
	pisoTexture.LoadTextureA();

	Tablero0 = Texture("Textures/Tablero0.tga");
	Tablero0.LoadTextureA();

	/*
	.
	.
	.
	instanciar texturas del tablero
	*/

	D8 = Texture("Textures/D8C.jpg");
	D8.LoadTextureA();
	D4 = Texture("Textures/D4C.jpg");
	D4.LoadTextureA();

	Dado8c = Model();
	Dado8c.LoadModel("Models/Dado8C.obj");
	Dado4c = Model();
	Dado4c.LoadModel("Models/Dado4C.obj");


	CasaArbolKND = Model();
	CasaArbolKND.LoadModel("Models/CasaArbolKND.obj");
	CommonCold = Model();
	CommonCold.LoadModel("Models/CommonCold.obj");
	CoolBus = Model();
	CoolBus.LoadModel("Models/CoolBus.obj");
	Dragstuh = Model();
	Dragstuh.LoadModel("Models/Dragstuh.obj");
	Elimonater = Model();
	Elimonater.LoadModel("Models/Elimonater.obj");
	Musket = Model();
	Musket.LoadModel("Models/Musket.obj");
	Mosquito = Model();
	Mosquito.LoadModel("Models/Mosquito.obj");
	Mushi = Model();
	Mushi.LoadModel("Models/Mushi.obj");
	N2 = Model();
	N2.LoadModel("Models/N2.obj");
	N4 = Model();
	N4.LoadModel("Models/N4.obj");
	N9 = Model();
	N9.LoadModel("Models/N9.obj");
	Roadstar = Model();
	Roadstar.LoadModel("Models/Roadstar.obj");
	SCAMPP = Model();
	SCAMPP.LoadModel("Models/SCAMPP.obj");
	SPLANKE = Model();
	SPLANKE.LoadModel("Models/SPLANKE.obj");
	GumGun = Model();
	GumGun.LoadModel("Models/GumGun.obj");
	Machineggun = Model();
	Machineggun.LoadModel("Models/Machineggun.obj");
	SimioArcoiris = Model();
	SimioArcoiris.LoadModel("Models/SimioArcoiris.obj");
	Visor = Model();
	Visor.LoadModel("Models/Visor.obj");


	std::vector<std::string> skyboxFaces;
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_rt.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_lf.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_dn.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_up.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_bk.tga");
	skyboxFaces.push_back("Textures/Skybox/cupertin-lake_ft.tga");

	skybox = Skybox(skyboxFaces);

	Material_brillante = Material(4.0f, 256);
	Material_opaco = Material(0.3f, 4);


	//luz direccional, sólo 1 y siempre debe de existir
	mainLight = DirectionalLight(1.0f, 1.0f, 1.0f,
		0.4f, 0.4f,
		0.0f, 0.0f, -1.0f);
	//contador de luces puntuales
	unsigned int pointLightCount = 0;
	//Declaración de primer luz puntual
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
		uniformSpecularIntensity = 0, uniformShininess = 0, uniformTextureOffset=0;
	GLuint uniformColor = 0;
	glm::mat4 projection = glm::perspective(45.0f, (GLfloat)mainWindow.getBufferWidth() / mainWindow.getBufferHeight(), 0.1f, 1000.0f);
	
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

	rotCasaArbolKND = 0.0f;

	indiceCasillas = 0;
	texturaTablero = 0;
	float rotOffDado = 15.0f;
	float rotOffCasaArbolKND = 2.0f;

	////Loop mientras no se cierra la ventana
	while (!mainWindow.getShouldClose())
	{
		GLfloat now = glfwGetTime();
		deltaTime = now - lastTime;
		deltaTime += (now - lastTime) / limitFPS;
		lastTime = now;
		double temp = glfwGetTime();
		animacion();

		rotCasaArbolKND += rotOffCasaArbolKND * deltaTime;

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
				
				// Distribución para el dado de 8 caras en el rango [1, 8]
				std::uniform_int_distribution<int> distribution1(1, 8);
				RD8 = distribution1(generator);

				// Distribución para el dado de 4 caras en el rango [1, 4]
				std::uniform_int_distribution<int> distribution2(1, 4);
				RD4 = distribution2(generator);
				indiceCasillas = RD4 + RD8;
			}
			else { 
				dadof = true; 
			}
			if (dadof) {

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
					movDado8y =0.3;
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

		} else {
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
		camera.keyControl(mainWindow.getsKeys(), deltaTime);
		camera.mouseControl(mainWindow.getXChange(), mainWindow.getYChange());

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

		//información en el shader de intensidad especular y brillo
		uniformSpecularIntensity = shaderList[0].GetSpecularIntensityLocation();
		uniformShininess = shaderList[0].GetShininessLocation();

		glUniformMatrix4fv(uniformProjection, 1, GL_FALSE, glm::value_ptr(projection));
		glUniformMatrix4fv(uniformView, 1, GL_FALSE, glm::value_ptr(camera.calculateViewMatrix()));
		glUniform3f(uniformEyePosition, camera.getCameraPosition().x, camera.getCameraPosition().y, camera.getCameraPosition().z);

		// luz ligada a la cámara de tipo flash
		glm::vec3 lowerLight = camera.getCameraPosition();
		lowerLight.y -= 0.3f;
		spotLights[0].SetFlash(lowerLight, camera.getCameraDirection());

		//información al shader de fuentes de iluminación
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

		//Objetos MAPA
		//CasaArbolKND
		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100, 10, -100.0f));
		model = glm::rotate(model, rotCasaArbolKND * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		CasaArbolKND.RenderModel();

		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(80.0f, -2.0f, -90.0f));
		model = glm::scale(model, glm::vec3(3.5f, 3.5f, 3.5f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		GumGun.RenderModel();

		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(-100.0f, -2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(2.5f, 2.5f, 2.5f));
		model = glm::rotate(model, 90 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		Visor.RenderModel();

		model = glm::mat4(1.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(100.0f, -2.0f, 100.0f));
		model = glm::scale(model, glm::vec3(4.5f, 4.5f, 4.5f));
		model = glm::rotate(model, -135 * toRadians, glm::vec3(0.0f, 1.0f, 0.0f));
		glUniformMatrix4fv(uniformModel, 1, GL_FALSE, glm::value_ptr(model));
		SimioArcoiris.RenderModel();




		toffset = glm::vec2(0.0, 0.0);
		model = glm::mat4(1.0);
		model = glm::translate(model, glm::vec3(0.0f, -1.9f, 0.0f));
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