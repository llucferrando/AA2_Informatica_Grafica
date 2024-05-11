#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include "Camera.h"
#include <vector>
#include <chrono>
#include "Primitive.h"
#include "InputManager.h"
#include "Utils.h"
#include "Troll.h"

#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480
std::vector<GLuint> compiledPrograms;
std::vector<Troll> trolls;

struct ShaderProgram {

	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {

	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);

	glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);

}


//Función que dado un struct que contiene los shaders de un programa generara el programa entero de la GPU
GLuint CreateProgram(const ShaderProgram& shaders) {

	//Crear programa de la GPU
	GLuint program = glCreateProgram();

	//Verificar que existe un vertex shader y adjuntarlo al programa
	if (shaders.vertexShader != 0) {
		glAttachShader(program, shaders.vertexShader);
	}

	/*if (shaders.geometryShader != 0) {
		glAttachShader(program, shaders.geometryShader);
	}*/

	if (shaders.fragmentShader != 0) {
		glAttachShader(program, shaders.fragmentShader);
	}

	// Linkear el programa
	glLinkProgram(program);

	//Obtener estado del programa
	GLint success;
	glGetProgramiv(program, GL_LINK_STATUS, &success);

	//Devolver programa si todo es correcto o mostrar log en caso de error
	if (success) {

		//Liberamos recursos
		if (shaders.vertexShader != 0) {
			glDetachShader(program, shaders.vertexShader);
		}

		////Liberamos recursos
		//if (shaders.geometryShader != 0) {
		//	glDetachShader(program, shaders.geometryShader);
		//}

		//Liberamos recursos
		if (shaders.fragmentShader != 0) {
			glDetachShader(program, shaders.fragmentShader);
		}

		return program;
	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);

		//Almacenamos log
		std::vector<GLchar> errorLog(logLength);
		glGetProgramInfoLog(program, logLength, nullptr, errorLog.data());

		std::cerr << "Error al linkar el programa:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


void main() {

	//Definir semillas del rand según el tiempo
	srand(static_cast<unsigned int>(time(NULL)));
	//Instance the inputs
	InputManager myInputManager;
	//Instance the utils
	Utils myUtils;

	//Inicializamos GLFW para gestionar ventanas e inputs
	glfwInit();

	//Configuramos la ventana
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 4);
	glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

	//Inicializamos la ventana
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "My Engine", NULL, NULL);

	//Asignamos función de callback para cuando el frame buffer es modificado
	glfwSetFramebufferSizeCallback(window, Resize_Window);

	//Definimos espacio de trabajo
	glfwMakeContextCurrent(window);

	//Permitimos a GLEW usar funcionalidades experimentales
	glewExperimental = GL_TRUE;

	//Activamos cull face
	glEnable(GL_CULL_FACE);

	//Indicamos lado del culling
	glCullFace(GL_BACK);

	//Activamos el depth
	glEnable(GL_DEPTH_TEST);

	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {
		Camera* myCamera = new Camera;
		//Declarar instancia del cubo
		Primitive * cube = new Primitive;
		//Declarar instancia de la piramide
		Primitive * pyramid = new Primitive;
		//Declarar instancia del ortoedro
		Primitive * ortoedro = new Primitive;

		//Compilar shaders
		ShaderProgram myCubeProgram, myPyramidProgram, myOrtoedroProgram;
		myCubeProgram.vertexShader = myUtils.LoadVertexShader("CubeMovementVS.glsl");
		myCubeProgram.fragmentShader = myUtils.LoadFragmentShader("BlueTransformFS.glsl");
		myCubeProgram.geometryShader = myUtils.LoadGeometryShader("MyFirstGeometryShader.glsl");

		myPyramidProgram.vertexShader = myUtils.LoadVertexShader("PyramidMovementVS.glsl");
		myPyramidProgram.fragmentShader = myUtils.LoadFragmentShader("DynamicColorsFS.glsl");
		
		myOrtoedroProgram.vertexShader = myUtils.LoadVertexShader("OrtoedroMovementVS.glsl");
		myOrtoedroProgram.fragmentShader = myUtils.LoadFragmentShader("YellowTransformFS.glsl");

		trolls.push_back(myUtils.LoadOBJModel("Assets/Models/troll.obj"));



		//Compilar programa
		compiledPrograms.push_back(CreateProgram(myCubeProgram));
		compiledPrograms.push_back(CreateProgram(myPyramidProgram));
		compiledPrograms.push_back(CreateProgram(myOrtoedroProgram));
		

		//Definimos color para limpiar el buffer de color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		GLuint cubeVao,  cubeVbo, pyramidVao, pyramidVbo;

		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &cubeVao);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(cubeVao);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &cubeVbo);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, cubeVbo);

		//Posición X e Y del punto
		GLfloat cubePoints[] = {
			-0.2f, +0.2f, -0.2f, // 3
			+0.2f, +0.2f, -0.2f, // 2
			-0.2f, -0.2f, -0.2f, // 6
			+0.2f, -0.2f, -0.2f, // 7
			+0.2f, -0.2f, +0.2f, // 4
			+0.2f, +0.2f, -0.2f, // 2
			+0.2f, +0.2f, +0.2f, // 0
			-0.2f, +0.2f, -0.2f, // 3
			-0.2f, +0.2f, +0.2f, // 1
			-0.2f, -0.2f, -0.2f, // 6
			-0.2f, -0.2f, +0.2f, // 5
			+0.2f, -0.2f, +0.2f, // 4
			-0.2f, +0.2f, +0.2f, // 1
			+0.2f, +0.2f, +0.2f  // 0
		};
		
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cubePoints), cubePoints, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		//Desvinculamos VBO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		//Desvinculamos VAO
		glBindVertexArray(0);

		///////////-------------PYRAMID--------------///////////
		//Definimos cantidad de vao a crear y donde almacenarlos 
		glGenVertexArrays(1, &pyramidVao);

		//Indico que el VAO activo de la GPU es el que acabo de crear
		glBindVertexArray(pyramidVao);

		//Definimos cantidad de vbo a crear y donde almacenarlos
		glGenBuffers(1, &pyramidVbo);

		//Indico que el VBO activo es el que acabo de crear y que almacenará un array. Todos los VBO que genere se asignaran al último VAO que he hecho glBindVertexArray
		glBindBuffer(GL_ARRAY_BUFFER, pyramidVbo);

		GLfloat pyramidPoints[] = {
					
			-0.2f, -0.2f, -0.2f,
			+0.2f, -0.2f, -0.2f,
			+0.2f, -0.2f, +0.2f,
			-0.2f, -0.2f, +0.2f,

			+0.0f, +0.2f, +0.0f,

			-0.2f, -0.2f, -0.2f,
			+0.2f, -0.2f, -0.2f,
			+0.2f, -0.2f, +0.2f,
			-0.2f, -0.2f, +0.2f,

			+0.0f, +0.2f, +0.0f,

			-0.2f, -0.2f, -0.2f,
			+0.2f, -0.2f, -0.2f,
			+0.2f, -0.2f, +0.2f,
			-0.2f, -0.2f, +0.2f,

			+0.0f, +0.2f, +0.0f,
			   
		};

		
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPoints), pyramidPoints, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);



		
		
		
		//Timer set
		auto lastTime = std::chrono::system_clock::now();
		//GAME LOOP
		while (!glfwWindowShouldClose(window)) {
			
			//Delta time set
			auto currentTime = std::chrono::system_clock::now();
			std::chrono::duration<double> dTime = currentTime - lastTime;
			lastTime = currentTime;

			float elapsedTime = myUtils.GetElapsedTime();

			//Instance the function where we do the inputs
			myInputManager.InputTransforms(window, cube, ortoedro, pyramid, myCamera);
			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();
			if(!myInputManager.isStoped)
			{
				//Limpiamos los buffers
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				/////////-------CUBE MATRIX-------/////////
				//Definimos que queremos usar el VAO con los puntos

				if (cube->isRendering())
				{
					glBindVertexArray(cubeVao);
					glUseProgram(compiledPrograms[0]);
					glm::mat4 cubeModelMatrix = glm::mat4(1.f);
					glm::mat4 viewMatrix = glm::lookAt(myCamera->position, myCamera->position + glm::vec3(0.f, 0.f, -1.f), myCamera->localVectorUp);
					glm::mat4 projectionMatrix = glm::perspective(glm::radians(myCamera->fov), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, myCamera->fNear, myCamera->fFar);
					
					// Aplico velocidad hacia el forward
					cube->setPosition(cube->getPosition() + cube->getForward() * cube->getYVelocity() * static_cast<float>(dTime.count()));
					cube->setRotation(cube->getRotation() + glm::vec3(0.f, 1.f, 0.f) * cube->getFAngularVelocity() * static_cast<float>(dTime.count()));
					// Invierto direccion al llegar  a los limites
					if (cube->getPosition().y >= 0.5f || cube->getPosition().y <= -0.5f) {
						cube->setForward(cube->getForward() * -1.f);
					}

					// Genero matriz que definde la translacion del cubo
					glm::mat4 cubeTranslationMatrix = myUtils.GenerateTranslationMatrix(cube->getPosition() + glm::vec3(-0.7f, 0.f, 0.f));
					glm::mat4 rotationMatrix = myUtils.GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), cube->getRotation().y);

					//Aplicamos la matriz al shader
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "translation"), 1, GL_FALSE, glm::value_ptr(cubeTranslationMatrix));
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "rotation"), 1, GL_FALSE, glm::value_ptr(rotationMatrix));
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "viewMatrix"), 1, GL_FALSE, glm::value_ptr(viewMatrix));
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "projectionMatrix"), 1, GL_FALSE, glm::value_ptr(projectionMatrix));
					
					
					//Asignar valores iniciales al programa
					glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
					

					//Definimos que queremos dibujar
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);

					std::cout << myCamera->position.x << myCamera->position.y;


				}


				if (pyramid->isRendering())
				{
					/////////-------PYRAMID MATRIX-------/////////
					glBindVertexArray(pyramidVao);
					glUseProgram(compiledPrograms[1]);
			
					// Aplico velocidad hacia el forward
					pyramid->setPosition(pyramid->getPosition() + pyramid->getForward() * pyramid->getYVelocity() * static_cast<float>(dTime.count()));
					// Aplico rotacion sobre los ejes X y Y
					pyramid->setRotation(pyramid->getRotation() + glm::vec3(1.f, 1.f, 0.f) * pyramid->getFAngularVelocity() * static_cast<float>(dTime.count()));
					
					// Invierto direccion al llegar  a los limites
					if (pyramid->getPosition().y >= 0.5f || pyramid->getPosition().y <= -0.5f) {
						pyramid->setForward(pyramid->getForward() * -1.f);
					}

					// Genero matriz que definde la translacion del cubo
					glm::mat4 pyramidTranslationMatrix = myUtils.GenerateTranslationMatrix(pyramid->getPosition() + glm::vec3(0.7f, 0.f, 0.f));
					glm::mat4 pyramidRotationMatrix = myUtils.GenerateRotationMatrix(glm::vec3(1.f, 1.f, 0.f), pyramid->getRotation().y);

					//Aplicamos la matriz al shader
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[1], "translation"), 1, GL_FALSE, glm::value_ptr(pyramidTranslationMatrix));
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[1], "rotation"), 1, GL_FALSE, glm::value_ptr(pyramidRotationMatrix));
					glUniform2f(glGetUniformLocation(compiledPrograms[1], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
					//Asignar valores iniciales al programa
					glUniform1f(glGetUniformLocation(compiledPrograms[1], "elapsedTime"), elapsedTime);
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 15);
					glBindVertexArray(0);
					
					

				}


				if (ortoedro->isRendering())
				{
					/////////-------ORTOEDRO MATRIX-------/////////
					glBindVertexArray(cubeVao);
					glUseProgram(compiledPrograms[2]);
				
					// Aplico velocidad hacia el forward
					ortoedro->setScale(ortoedro->getScale() + glm::vec3(0.f, 1.f, 0.f) * ortoedro->getFScaleVelocity() * static_cast<float>(dTime.count()));
					ortoedro->setRotation(ortoedro->getRotation() + glm::vec3(0.f, 0.f, 1.f) * ortoedro->getFAngularVelocity() * static_cast<float>(dTime.count()));

					
					//Invierto esccalado al llegar a los límites
					if (ortoedro->getScale().y >= 1.9f || ortoedro->getScale().y <= 0.99f) {
						ortoedro->setFScaleVelocity(ortoedro->getFScaleVelocity() * -1.f);
					}

					// Genero matriz que definde la translacion del cubo
					glm::mat4 ortoedroScaleMatrix = myUtils.GenerateScaleMatrix(ortoedro->getScale());
					glm::mat4 ortoedroRotationMatrix = myUtils.GenerateRotationMatrix(glm::vec3(0.f, 0.f, 1.f), ortoedro->getRotation().z);

					//Aplicamos la matriz al shader
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[2], "scale"), 1, GL_FALSE, glm::value_ptr(ortoedroScaleMatrix));
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[2], "rotation"), 1, GL_FALSE, glm::value_ptr(ortoedroRotationMatrix));
					//Asignar valores iniciales al programa
					glUniform2f(glGetUniformLocation(compiledPrograms[2], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);

					//Definimos que queremos dibujar
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
				}	

				trolls[0].Render();
				//Cambiamos buffers
				glFlush();
				glfwSwapBuffers(window);
			
			}
			
		}

		//Desactivar y eliminar programa
		glUseProgram(0);
		glDeleteProgram(compiledPrograms[0]);
		glDeleteProgram(compiledPrograms[1]);
		glDeleteProgram(compiledPrograms[2]);
		
		
	}
	else {
		std::cout << "Crashed" << std::endl;
		glfwTerminate();
	}

	

	//Finalizamos GLFW
	glfwTerminate();

}