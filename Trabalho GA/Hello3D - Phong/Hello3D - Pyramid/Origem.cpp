/* Hello Triangle - c�digo adaptado de https://learnopengl.com/#!Getting-started/Hello-Triangle
 *
 * Adaptado por Rossana Baptista Queiroz
 * para a disciplina de Processamento Gr�fico/Computa��o Gr�fica - Unisinos
 * Vers�o inicial: 7/4/2017
 * �ltima atualiza��o em 20/04/2023
 *
 */

#include <iostream>
#include <string>
#include <assert.h>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

// GLAD
#include <glad/glad.h>

// GLFW
#include <GLFW/glfw3.h>

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

#include "Mesh.h"


struct Vertex
{
	glm::vec3 position;
	glm::vec3 v_color;
};

// Prot�tipo da fun��o de callback de teclado
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);

// Prot�tipos das fun��es
int setupGeometry();
int loadSimpleObj(string filePath, int& nVertices, glm::vec3 color = glm::vec3(1.0,0.0,0.0));

// Dimens�es da janela (pode ser alterado em tempo de execu��o)
const GLuint WIDTH = 1000, HEIGHT = 1000;

bool rotateX=false, rotateY=false, rotateZ=false;

bool rotateY_mario = false, rotateZ_mario = false;
bool rotateY_pikachu = false, rotateZ_pikachu = false;
bool rotateY_joystick = false, rotateZ_joystick = false;
bool rotateY_todos = false;

glm::vec3 cameraPos = glm::vec3(0.0, 0.0, 3.0);
glm::vec3 cameraFront = glm::vec3(0.0, 0.0, -1.0);
glm::vec3 cameraUp = glm::vec3(0.0, 1.0, 0.0);
float cameraSpeed = 0.1;

bool firstMouse = true;
float lastX = 0.0, lastY = 0.0;
float yaw = -90.0, pitch = 0.0;

bool m_pressed = false, rotatey = false, p_pressed = false, g_pressed = false, r_pressed = false, y_pressed = false;
float mario_rotacao = 0.0;
float pikachu_rotacao = 0.0;
float joystick_rotacao = 0.0;
// Fun��o MAIN
int main()
{
	// Inicializa��o da GLFW
	glfwInit();

	//Muita aten��o aqui: alguns ambientes n�o aceitam essas configura��es
	//Voc� deve adaptar para a vers�o do OpenGL suportada por sua placa
	//Sugest�o: comente essas linhas de c�digo para desobrir a vers�o e
	//depois atualize (por exemplo: 4.5 com 4 e 5)
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	//glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	//glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//Essencial para computadores da Apple
//#ifdef __APPLE__
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//#endif

	// Cria��o da janela GLFW
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Ola Visualizador 3D!", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Fazendo o registro da fun��o de callback para a janela GLFW
	glfwSetKeyCallback(window, key_callback);
	//glfwSetCursorPos(window, WIDTH / 2, HEIGHT / 2);
	glfwSetCursorPosCallback(window, mouse_callback);


	//Desabilita o desenho do cursor do mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// GLAD: carrega todos os ponteiros d fun��es da OpenGL
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;

	}

	// Obtendo as informa��es de vers�o
	const GLubyte* renderer = glGetString(GL_RENDERER); /* get renderer string */
	const GLubyte* version = glGetString(GL_VERSION); /* version as a string */
	cout << "Renderer: " << renderer << endl;
	cout << "OpenGL version supported " << version << endl;

	// Definindo as dimens�es da viewport com as mesmas dimens�es da janela da aplica��o
	int width, height;
	glfwGetFramebufferSize(window, &width, &height);
	glViewport(0, 0, width, height);


	// Compilando e buildando o programa de shader
	Shader shader("Hello3D.vs", "Hello3D.fs");

	// Gerando um buffer simples, com a geometria de um tri�ngulo
	int nVertices;
	//GLuint VAO = loadSimpleObj("../../3D_Models/Classic/bunny.obj", nVertices);
	//GLuint VAO = loadSimpleObj("../../3D_Models/Cube/cube.obj", nVertices);
	
	//GLuint VAO = loadSimpleObj("../../3D_Models/Suzanne/SuzanneTriLowPoly.obj", nVertices);
	//GLuint VAO2 = loadSimpleObj("../../3D_Models/Suzanne/SuzanneTriLowPoly.obj", nVertices,glm::vec3(0.0,1.0,0.0));
	//GLuint VAO3 = loadSimpleObj("../../3D_Models/Suzanne/SuzanneTriLowPoly.obj", nVertices, glm::vec3(0.0, 0.0, 1.0));
	
	
	GLuint VAO4 = loadSimpleObj("../../3D_Models/Pokemon/Pikachu.obj", nVertices);
	GLuint VAO3 = loadSimpleObj("../../3D_Models/joystick/joystick.obj", nVertices);
	GLuint VAO = loadSimpleObj("../../3D_Models/Mario/Mario.obj", nVertices);

	glUseProgram(shader.ID);

	glm::mat4 model = glm::mat4(1); //matriz identidade;
	GLint modelLoc = glGetUniformLocation(shader.ID, "model");
	//
	model = glm::rotate(model, /*(GLfloat)glfwGetTime()*/glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	glUniformMatrix4fv(modelLoc, 1, FALSE, glm::value_ptr(model));

	//Definindo a matriz de view (posi��o e orienta��o da c�mera)
	glm::mat4 view = glm::lookAt(glm::vec3(0.0, 0.0, 3.0), glm::vec3(0.0, 0.0, 0.0), glm::vec3(0.0, 1.0, 0.0));
	GLint viewLoc = glGetUniformLocation(shader.ID, "view");
	glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

	//Definindo a matriz de proje��o perpectiva
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)width / (GLfloat)height, 0.1f, 100.0f);
	GLint projLoc = glGetUniformLocation(shader.ID, "projection");
	glUniformMatrix4fv(projLoc, 1, FALSE, glm::value_ptr(projection));

	//Definindo as propriedades do material 
	shader.setFloat("ka", 0.4);
	shader.setFloat("kd", 0.5);
	shader.setFloat("ks", 0.5);
	shader.setFloat("q", 10);

	//Definindo as propriedades da fonte de luz
	shader.setVec3("lightPos", -2.0f, 10.0f, 3.0f);
	shader.setVec3("lightColor", 1.0f, 1.0f, 1.0f);

	//Mesh suzanne1, suzanne2, suzanne3, pikachu, mario;
	Mesh mario, pikachu, joystick;
	//suzanne1.initialize(VAO, nVertices, &shader, glm::vec3(-3.0, 0.0, 0.0));
	//suzanne2.initialize(VAO2, nVertices, &shader);
	//suzanne3.initialize(VAO3, nVertices, &shader, glm::vec3(3.0, 0.0, 0.0));
	pikachu.initialize(VAO4, nVertices, &shader, glm::vec3(-6.0, -3.0, -10.0));
	mario.initialize(VAO, nVertices, &shader, glm::vec3(0.0, -3.0, -10.0));
	joystick.initialize(VAO3, nVertices, &shader, glm::vec3(6.0, 0.0, -10.0), glm::vec3(10.0, 10.0, 10.0));
	

	glEnable(GL_DEPTH_TEST);
	float mario_rotacao = 0.0;
	float pikachu_rotacao = 0.0;
	float joystick_rotacao = 0.0;
	
	//Rotação camera
	glm::vec3 objectPos = glm::vec3(0.0, 0.0, -10.0); // posição do objeto
	float angle = 0.0f; // ângulo de rotação em graus

	// Loop da aplica��o - "game loop"
	while (!glfwWindowShouldClose(window))
	{
		

		// Checa se houveram eventos de input (key pressed, mouse moved etc.) e chama as fun��es de callback correspondentes
		glfwPollEvents();

		// Limpa o buffer de cor
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f); //cor de fundo
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLineWidth(10);
		glPointSize(20);

		//Alterando a matriz de view (posi��o e orienta��o da c�mera) 
		glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
		glUniformMatrix4fv(viewLoc, 1, FALSE, glm::value_ptr(view));

		//Enviando a posi��o da camera para o shader
		shader.setVec3("cameraPos", cameraPos.x, cameraPos.y, cameraPos.z);

		// Chamada de desenho - drawcall
		//suzanne1.update();
		//suzanne1.draw();

		//suzanne2.update();
		//suzanne2.draw();

		//suzanne3.update();
		//suzanne3.draw();

		if (p_pressed) {

			pikachu.update();
			pikachu.draw();
			
		}


		if (m_pressed){
			mario.update();
			mario.draw();
			
		}
		
		//trocando posição da camera
		if (g_pressed) {
			cameraPos = glm::vec3(9.0, 0.0, -10.0);
			

		}

		//Rotação da camera
		if (r_pressed) {
			cameraPos = glm::vec3(13.0f * cos(glm::radians(angle)), 0.0f, -15.0f * sin(glm::radians(angle))) + objectPos;
			
			// Atualizar o ângulo de rotação para o próximo quadro
			angle += 0.2f; // altere o valor para ajustar a velocidade de rotação

		}
		
		joystick.update();
		joystick.draw();

		//PIKACHU
		//press C
		if (rotateY_pikachu) {
			pikachu.initialize(VAO4, nVertices, &shader, glm::vec3(-6.0, -3.0, -10.0), glm::vec3(1.0, 1.0, 1.0), float(pikachu_rotacao), glm::vec3(0.0, 1.0, 0.0));
			pikachu_rotacao = pikachu_rotacao + 10.0;
			rotateY_pikachu = false;

		}

		//press V
		if (rotateZ_pikachu) {
			pikachu.initialize(VAO4, nVertices, &shader, glm::vec3(-6.0, -3.0, -10.0), glm::vec3(1.0, 1.0, 1.0), float(pikachu_rotacao), glm::vec3(0.0, 0.0, 1.0));
			pikachu_rotacao = pikachu_rotacao + 10.0;
			rotateZ_pikachu = false;

		}

		//----------------------------------------------------
		//MARIO
		//press Z
		if (rotateY_mario){
			mario.initialize(VAO, nVertices, &shader, glm::vec3(-0.0, -3.0, -10.0), glm::vec3(1.0, 1.0, 1.0), float(mario_rotacao), glm::vec3(0.0, 1.0, 0.0));
			mario_rotacao = mario_rotacao + 10.0;
			rotateY_mario = false;
				
		}

		//press X
		if (rotateZ_mario) {
			mario.initialize(VAO, nVertices, &shader, glm::vec3(-0.0, -3.0, -10.0), glm::vec3(1.0, 1.0, 1.0), float(mario_rotacao), glm::vec3(0.0, 0.0, 1.0));
			mario_rotacao = mario_rotacao + 10.0;
			rotateZ_mario = false;

		}
		
		//joystick 
		//press B
		if (rotateY_joystick) {
			joystick.initialize(VAO3, nVertices, &shader, glm::vec3(6.0, 0.0, -10.0), glm::vec3(10.0, 10.0, 10.0), float(joystick_rotacao), glm::vec3(0.0, 1.0, 0.0));
			joystick_rotacao = joystick_rotacao + 10.0;
			rotateY_joystick = false;

		}

		//press N
		if (rotateZ_joystick) {
			joystick.initialize(VAO3, nVertices, &shader, glm::vec3(6.0, 0.0, -10.0), glm::vec3(10.0, 10.0, 10.0), float(joystick_rotacao), glm::vec3(0.0, 0.0, 1.0));
			joystick_rotacao = joystick_rotacao + 10.0;
			rotateZ_joystick = false;

		}
		//todos giram
		if (rotateY_todos) {
			joystick.initialize(VAO3, nVertices, &shader, glm::vec3(6.0, 0.0, -10.0), glm::vec3(10.0, 10.0, 10.0), float(joystick_rotacao), glm::vec3(0.0, 1.0, 0.0));
			joystick_rotacao = joystick_rotacao + 10.0;
			mario.initialize(VAO, nVertices, &shader, glm::vec3(-0.0, -3.0, -10.0), glm::vec3(1.0, 1.0, 1.0), float(mario_rotacao), glm::vec3(0.0, 1.0, 0.0));
			mario_rotacao = mario_rotacao + 10.0;
			pikachu.initialize(VAO4, nVertices, &shader, glm::vec3(-6.0, -3.0, -10.0), glm::vec3(1.0, 1.0, 1.0), float(pikachu_rotacao), glm::vec3(0.0, 1.0, 0.0));
			pikachu_rotacao = pikachu_rotacao + 10.0;
			
			//todos param de girar
			if (y_pressed) {
				joystick_rotacao = false;
				mario_rotacao = false;
				pikachu_rotacao = false;
				rotateY_todos = false;
			}
		}

		// Troca os buffers da tela
		glfwSwapBuffers(window);
	}
	// Pede pra OpenGL desalocar os buffers
	glDeleteVertexArrays(1, &VAO);
	// Finaliza a execu��o da GLFW, limpando os recursos alocados por ela
	glfwTerminate();
	return 0;
}

// Fun��o de callback de teclado - s� pode ter uma inst�ncia (deve ser est�tica se
// estiver dentro de uma classe) - � chamada sempre que uma tecla for pressionada
// ou solta via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key == GLFW_KEY_M && action == GLFW_PRESS)
	{
		m_pressed = true;
	}
	
	if (key == GLFW_KEY_P && action == GLFW_PRESS)
	{
		p_pressed = true;
	}

	if (key == GLFW_KEY_G && action == GLFW_PRESS)
	{
		g_pressed = true;
	}

	if (key == GLFW_KEY_R)
	{
		r_pressed = true;
	}
	if (key == GLFW_KEY_Y)
	{
		y_pressed = true;
	}

	if (key == GLFW_KEY_Y && action == GLFW_PRESS)
	{
		rotateX = false;
		rotateY = true;
		rotateZ = false;
	}
	//rotação no eixo Y do mario	
	if (key == GLFW_KEY_Z)
	{
		rotateY_mario = true;

	}
	//rotação no eixo Z do mario	
	if (key == GLFW_KEY_X)
	{
		rotateZ_mario = true;

	}
	//rotação no eixo Y do pikachu	
	if (key == GLFW_KEY_C)
	{
		rotateY_pikachu = true;

	}
	//rotação no eixo Z do pikachu	
	if (key == GLFW_KEY_V)
	{
		rotateZ_pikachu = true;

	}
	
	//rotação no eixo Z do joystick	
	if (key == GLFW_KEY_B)
	{
		rotateY_joystick = true;

	}

	//rotação no eixo Z do joystick	
	if (key == GLFW_KEY_T)
	{
		rotateY_todos = true;

	}

	if (key == GLFW_KEY_N)
	{
		rotateZ_joystick = true;

	}

	if (key == GLFW_KEY_W)
	{
		cameraPos += cameraSpeed * cameraFront;
	}

	if (key == GLFW_KEY_S)
	{
		cameraPos -= cameraSpeed * cameraFront;
	}

	if (key == GLFW_KEY_A)
	{
		cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	if (key == GLFW_KEY_D)
	{
		cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}



}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{

	// cout << xpos << " " << ypos << endl;

	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float offsetx = xpos - lastX;
	float offsety = lastY - ypos;
	lastX = xpos;
	lastY = ypos;

	offsetx *= cameraSpeed;
	offsety *= cameraSpeed;

	pitch += offsety;
	yaw += offsetx;

	glm::vec3 front;
	front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	front.y = sin(glm::radians(pitch));
	front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	cameraFront = glm::normalize(front);

}


// Esta fun��o est� bastante harcoded - objetivo � criar os buffers que armazenam a 
// geometria de um tri�ngulo
// Apenas atributo coordenada nos v�rtices
// 1 VBO com as coordenadas, VAO com apenas 1 ponteiro para atributo
// A fun��o retorna o identificador do VAO
int setupGeometry()
{
	// Aqui setamos as coordenadas x, y e z do tri�ngulo e as armazenamos de forma
	// sequencial, j� visando mandar para o VBO (Vertex Buffer Objects)
	// Cada atributo do v�rtice (coordenada, cores, coordenadas de textura, normal, etc)
	// Pode ser arazenado em um VBO �nico ou em VBOs separados
	GLfloat vertices[] = {

		//Base da pir�mide: 2 tri�ngulos
		//x    y    z    r    g    b
		-0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		-0.5, -0.5,  0.5, 0.0, 1.0, 1.0,
		 0.5, -0.5, -0.5, 1.0, 0.0, 1.0,

		 -0.5, -0.5, 0.5, 1.0, 1.0, 0.0,
		  0.5, -0.5,  0.5, 0.0, 1.0, 1.0,
		  0.5, -0.5, -0.5, 1.0, 0.0, 1.0,

		 //
		 -0.5, -0.5, -0.5, 1.0, 1.0, 0.0,
		  0.0,  0.5,  0.0, 1.0, 1.0, 0.0,
		  0.5, -0.5, -0.5, 1.0, 1.0, 0.0,

		  -0.5, -0.5, -0.5, 1.0, 0.0, 1.0,
		  0.0,  0.5,  0.0, 1.0, 0.0, 1.0,
		  -0.5, -0.5, 0.5, 1.0, 0.0, 1.0,

		   -0.5, -0.5, 0.5, 1.0, 1.0, 0.0,
		  0.0,  0.5,  0.0, 1.0, 1.0, 0.0,
		  0.5, -0.5, 0.5, 1.0, 1.0, 0.0,

		   0.5, -0.5, 0.5, 0.0, 1.0, 1.0,
		  0.0,  0.5,  0.0, 0.0, 1.0, 1.0,
		  0.5, -0.5, -0.5, 0.0, 1.0, 1.0,

		  //"Chão"
		  -5.0, -0.5, -5.0, 0.5, 0.5, 0.5,
		  -5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		   5.0, -0.5, -5.0, 0.5, 0.5, 0.5,

		  -5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		   5.0, -0.5,  5.0, 0.5, 0.5, 0.5,
		   5.0, -0.5, -5.0, 0.5, 0.5, 0.5


	};

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);
	
	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 
	
	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3*sizeof(GLfloat)));
	glEnableVertexAttribArray(1);



	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;
}

int loadSimpleObj(string filePath, int& nVertices, glm::vec3 color)
{
	ifstream inputFile;
	inputFile.open(filePath);
	vector <GLfloat> vertbuffer;

	vector <Vertex> vertices;
	vector <int> indices;
	vector <glm::vec3> normals;
	vector <glm::vec2> texCoord;

	if (inputFile.is_open())
	{
		char line[100];
		string sline;
		

		while (!inputFile.eof())
		{
			inputFile.getline(line, 100);
			sline = line;

			string word;
			istringstream ssline(sline);

			ssline >> word;

			if (word == "v")
			{
				Vertex v;
				ssline >> v.position.x >> v.position.y >> v.position.z;
				v.v_color.r = color.r; v.v_color.g = color.g; v.v_color.b = color.b;
				vertices.push_back(v);
			}
			if (word == "vt")
			{
				glm::vec2 vt;
				ssline >> vt.s >> vt.t;
				texCoord.push_back(vt);
			}
			if (word == "vn")
			{
				glm::vec3 vn;
				ssline >> vn.x >> vn.y >> vn.z;
				normals.push_back(vn);
			}
			else if (word == "f")
			{
				string tokens[3];
				for (int i = 0; i < 3; i++)
				{
					ssline >> tokens[i];
					int pos = tokens[i].find("/");
					string token = tokens[i].substr(0, pos);
					int index = atoi(token.c_str()) - 1;
					indices.push_back(index);
					vertbuffer.push_back(vertices[index].position.x);
					vertbuffer.push_back(vertices[index].position.y);
					vertbuffer.push_back(vertices[index].position.z);
					vertbuffer.push_back(vertices[index].v_color.r);
					vertbuffer.push_back(vertices[index].v_color.g);
					vertbuffer.push_back(vertices[index].v_color.b);

					tokens[i] = tokens[i].substr(pos + 1);
					pos = tokens[i].find("/");
					token = tokens[i].substr(0, pos);
					int indexT = atoi(token.c_str()) - 1;

					vertbuffer.push_back(texCoord[indexT].s);
					vertbuffer.push_back(texCoord[indexT].t);

					tokens[i] = tokens[i].substr(pos + 1);
					token = tokens[i].substr(0, pos);
					int indexN = atoi(token.c_str()) - 1;

					vertbuffer.push_back(normals[indexN].x);
					vertbuffer.push_back(normals[indexN].y);
					vertbuffer.push_back(normals[indexN].z);
					
				}
			
			}

		}

		inputFile.close();
	}
	else
	{
		cout << "N�o foi possivel abrir o arquivo " << filePath << endl;
	}


	nVertices = vertbuffer.size() / 11;

	GLuint VBO, VAO;

	//Gera��o do identificador do VBO
	glGenBuffers(1, &VBO);

	//Faz a conex�o (vincula) do buffer como um buffer de array
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	//Envia os dados do array de floats para o buffer da OpenGl
	glBufferData(GL_ARRAY_BUFFER, vertbuffer.size() * sizeof(GLfloat), vertbuffer.data(), GL_STATIC_DRAW);

	//Gera��o do identificador do VAO (Vertex Array Object)
	glGenVertexArrays(1, &VAO);

	// Vincula (bind) o VAO primeiro, e em seguida  conecta e seta o(s) buffer(s) de v�rtices
	// e os ponteiros para os atributos 
	glBindVertexArray(VAO);

	//Para cada atributo do vertice, criamos um "AttribPointer" (ponteiro para o atributo), indicando: 
	// Localiza��o no shader * (a localiza��o dos atributos devem ser correspondentes no layout especificado no vertex shader)
	// Numero de valores que o atributo tem (por ex, 3 coordenadas xyz) 
	// Tipo do dado
	// Se est� normalizado (entre zero e um)
	// Tamanho em bytes 
	// Deslocamento a partir do byte zero 

	//Atributo posi��o (x, y, z)
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	//Atributo cor (r, g, b)
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	//Atributo coordenadas de textura (s, t)
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	//Atributo vetor normal (x, y e z)
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 11 * sizeof(GLfloat), (GLvoid*)(8 * sizeof(GLfloat)));
	glEnableVertexAttribArray(3);

	// Observe que isso � permitido, a chamada para glVertexAttribPointer registrou o VBO como o objeto de buffer de v�rtice 
	// atualmente vinculado - para que depois possamos desvincular com seguran�a
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// Desvincula o VAO (� uma boa pr�tica desvincular qualquer buffer ou array para evitar bugs medonhos)
	glBindVertexArray(0);

	return VAO;

}

