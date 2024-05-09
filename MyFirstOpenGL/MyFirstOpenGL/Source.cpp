#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtc/type_ptr.hpp>
#include <gtc/matrix_transform.hpp>
#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <chrono>
#include "Primitive.h"
#include "InputManager.h"


#define WINDOW_WIDTH 640
#define WINDOW_HEIGHT 480




	

std::vector<GLuint> compiledPrograms;


struct ShaderProgram {

	GLuint vertexShader = 0;
	//GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};

void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {

	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);

	glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);

}


//Funcion que genera una matriz de translacion
glm::mat4 GenerateTranslationMatrix(glm::vec3 translation) {

	return glm::translate(glm::mat4(1.f), translation);
}

//Funcion que genera una matriz de rotacion
glm::mat4 GenerateRotationMatrix(glm::vec3 axis, float fDegrees) {

	return glm::rotate(glm::mat4(1.f), glm::radians(fDegrees), glm::normalize(axis));
}
//Funcion que genera una matriz de escalado
glm::mat4 GenerateScaleMatrix(glm::vec3 scaleAxis) {

	return glm::scale(glm::mat4(1.f),scaleAxis);
}


//Funcion que devolvera una string con todo el archivo leido
std::string Load_File(const std::string& filePath) {

	std::ifstream file(filePath);

	std::string fileContent;
	std::string line;

	//Lanzamos error si el archivo no se ha podido abrir
	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Leemos el contenido y lo volcamos a la variable auxiliar
	while (std::getline(file, line)) {
		fileContent += line + "\n";
	}

	//Cerramos stream de datos y devolvemos contenido
	file.close();

	return fileContent;
}

GLuint LoadFragmentShader(const std::string& filePath) {

	// Crear un fragment shader
	GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

	//Usamos la funcion creada para leer el fragment shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el fragment shader con su código fuente
	glShaderSource(fragmentShader, 1, &cShaderSource, nullptr);

	// Compilar el fragment shader
	glCompileShader(fragmentShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el fragment shader
	if (success) {

		return fragmentShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(fragmentShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el fragment shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}


GLuint LoadGeometryShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint geometryShader = glCreateShader(GL_GEOMETRY_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su código fuente
	glShaderSource(geometryShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(geometryShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(geometryShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return geometryShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(geometryShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(geometryShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
}

GLuint LoadVertexShader(const std::string& filePath) {

	// Crear un vertex shader
	GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

	//Usamos la funcion creada para leer el vertex shader y almacenarlo 
	std::string sShaderCode = Load_File(filePath);
	const char* cShaderSource = sShaderCode.c_str();

	//Vinculamos el vertex shader con su código fuente
	glShaderSource(vertexShader, 1, &cShaderSource, nullptr);

	// Compilar el vertex shader
	glCompileShader(vertexShader);

	// Verificar errores de compilación
	GLint success;
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

	//Si la compilacion ha sido exitosa devolvemos el vertex shader
	if (success) {

		return vertexShader;

	}
	else {

		//Obtenemos longitud del log
		GLint logLength;
		glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &logLength);

		//Obtenemos el log
		std::vector<GLchar> errorLog(logLength);
		glGetShaderInfoLog(vertexShader, logLength, nullptr, errorLog.data());

		//Mostramos el log y finalizamos programa
		std::cerr << "Se ha producido un error al cargar el vertex shader:  " << errorLog.data() << std::endl;
		std::exit(EXIT_FAILURE);
	}
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

		//Declarar instancia del cubo
		Primitive * cube = new Primitive;
		//Declarar instancia de la piramide
		Primitive * pyramid = new Primitive;
		//Declarar instancia del ortoedro
		Primitive * ortoedro = new Primitive;


		//Declarar vec2 para definir el offset
		glm::vec2 offset = glm::vec2(0.f, 0.f);

		//Compilar shaders
		ShaderProgram myCubeProgram, myPyramidProgram, myOrtoedroProgram;
		myCubeProgram.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
		myCubeProgram.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");

		myPyramidProgram.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
		myPyramidProgram.fragmentShader = LoadFragmentShader("BlueTransformFS.glsl");
		
		myOrtoedroProgram.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
		myOrtoedroProgram.fragmentShader = LoadFragmentShader("PyramidColor.glsl");



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
			-0.3f, +0.3f, -0.3f, // 3
			+0.3f, +0.3f, -0.3f, // 2
			-0.3f, -0.3f, -0.3f, // 6
			+0.3f, -0.3f, -0.3f, // 7
			+0.3f, -0.3f, +0.3f, // 4
			+0.3f, +0.3f, -0.3f, // 2
			+0.3f, +0.3f, +0.3f, // 0
			-0.3f, +0.3f, -0.3f, // 3
			-0.3f, +0.3f, +0.3f, // 1
			-0.3f, -0.3f, -0.3f, // 6
			-0.3f, -0.3f, +0.3f, // 5
			+0.3f, -0.3f, +0.3f, // 4
			-0.3f, +0.3f, +0.3f, // 1
			+0.3f, +0.3f, +0.3f  // 0
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
					
			-0.3f, -0.3f, -0.3f,  
			+0.3f, -0.3f, -0.3f,  
			+0.3f, -0.3f, +0.3f,
			-0.3f, -0.3f, +0.3f,
			
			+0.0f, +0.3f, +0.0f,
			
			-0.3f, -0.3f, -0.3f,
			+0.3f, -0.3f, -0.3f,
			+0.3f, -0.3f, +0.3f,
			-0.3f, -0.3f, +0.3f,

			+0.0f, +0.3f, +0.0f,
			
			-0.3f, -0.3f, -0.3f,
			+0.3f, -0.3f, -0.3f,
			+0.3f, -0.3f, +0.3f,
			-0.3f, -0.3f, +0.3f,

			+0.0f, +0.3f, +0.0f,
			   
		};

		
		glBufferData(GL_ARRAY_BUFFER, sizeof(pyramidPoints), pyramidPoints, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);

		glEnableVertexAttribArray(0);

		glBindVertexArray(0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);


		//Asignar valores iniciales al programa
		glUniform2f(glGetUniformLocation(compiledPrograms[0], "windowSize"), WINDOW_WIDTH, WINDOW_HEIGHT);
		
		//Instance the inputs
		InputManager myInputManager;
		//Timer set
		auto lastTime = std::chrono::system_clock::now();
		//GAME LOOP
		while (!glfwWindowShouldClose(window)) {
			
			//Delta time set
			auto currentTime = std::chrono::system_clock::now();
			std::chrono::duration<double> dTime = currentTime - lastTime;
			lastTime = currentTime;

			//Instance the function where we do the inputs
			myInputManager.InputTransforms(window, cube, ortoedro, pyramid);
			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();
			if(!myInputManager.isStoped)
			{
				//Limpiamos los buffers
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

				/////////-------CUBE MATRIX-------/////////
				//Definimos que queremos usar el VAO con los puntos

				if (cube->isRendering)
				{
					glBindVertexArray(cubeVao);
					glUseProgram(compiledPrograms[0]);
					glm::mat4 cubeModelMatrix = glm::mat4(1.f);

					// Aplico velocidad hacia el forward
					cube->position = cube->position + cube->forward * cube->yVelocity * static_cast<float>(dTime.count());
					cube->rotation = cube->rotation + glm::vec3(0.f, 1.f, 0.f) * cube->fAngularVelocity * static_cast<float>(dTime.count());
					// Invierto direccion al llegar  a los limites
					if (cube->position.y >= 0.5f || cube->position.y <= -0.5f) {
						cube->forward = cube->forward * -1.f;
					}

					// Genero matriz que definde la translacion del cubo
					glm::mat4 cubeTranslationMatrix = GenerateTranslationMatrix(cube->position);
					glm::mat4 rotationMatrix = GenerateRotationMatrix(glm::vec3(0.f, 1.f, 0.f), cube->rotation.y);

					// Aplico las matrices
					cubeModelMatrix = cubeTranslationMatrix * rotationMatrix;

					//Aplicamos la matriz al shader
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(cubeModelMatrix));

					//Definimos que queremos dibujar
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
				}


				if (pyramid->isRendering)
				{
					/////////-------PYRAMID MATRIX-------/////////
					glBindVertexArray(pyramidVao);
					glUseProgram(compiledPrograms[1]);
					glm::mat4 pyramidModelMatrix = glm::mat4(1.f);

					// Aplico velocidad hacia el forward
					pyramid->position = pyramid->position + pyramid->forward * pyramid->yVelocity * static_cast<float>(dTime.count());
					pyramid->rotation = pyramid->rotation + glm::vec3(0.f, 1.f, 0.f) * pyramid->fAngularVelocity * static_cast<float>(dTime.count());
					pyramid->scale = pyramid->scale + glm::vec3(0.f, 1.f, 0.f) * pyramid->fScaleVelocity * static_cast<float>(dTime.count());
					// Invierto direccion al llegar  a los limites
					if (pyramid->position.y >= 0.5f || pyramid->position.y <= -0.5f) {
						pyramid->forward = pyramid->forward * -1.f;
					}

					// Genero matriz que definde la translacion del cubo
					glm::mat4 pyramidTranslationMatrix = GenerateTranslationMatrix(pyramid->position);
					glm::mat4 pyramidRotationMatrix = GenerateRotationMatrix(glm::vec3(1.f, 1.f, 0.f), pyramid->rotation.y);
					glm::mat4 pyramidScaleMatrix = GenerateScaleMatrix(pyramid->scale);

					// Aplico las matrices
					pyramidModelMatrix = pyramidTranslationMatrix * pyramidRotationMatrix;

					//Aplicamos la matriz al shader
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(pyramidModelMatrix));
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 15);
					glBindVertexArray(0);

				}


				if (ortoedro->isRendering)
				{
					/////////-------ORTOEDRO MATRIX-------/////////
					glBindVertexArray(cubeVao);
					glUseProgram(compiledPrograms[2]);
					glm::mat4 ortoedroModelMatrix = glm::mat4(1.f);

					// Aplico velocidad hacia el forward
					ortoedro->position = ortoedro->position + ortoedro->Zforward * ortoedro->zVelocity * static_cast<float>(dTime.count());
					ortoedro->scale = ortoedro->scale + glm::vec3(0.f, 1.f, 0.f) * ortoedro->fScaleVelocity * static_cast<float>(dTime.count());
					// Invierto direccion al llegar  a los limites
					if (ortoedro->position.z >= 0.5f || ortoedro->position.z <= -0.5f) {
						ortoedro->Zforward = ortoedro->Zforward * -1.f;
					}
					//Invierto esccalado al llegar a los límites
					if (ortoedro->scale.y >= 1.9f || ortoedro->scale.y <= 0.99f) {
						ortoedro->fScaleVelocity = ortoedro->fScaleVelocity * -1.f;
					}

					// Genero matriz que definde la translacion del cubo
					glm::mat4 ortoedroTranslationMatrix = GenerateTranslationMatrix(ortoedro->position);
					glm::mat4 ortoedroScaleMatrix = GenerateScaleMatrix(ortoedro->scale);

					// Aplico las matrices
					ortoedroModelMatrix = ortoedroTranslationMatrix * ortoedroScaleMatrix;

					//Aplicamos la matriz al shader
					glUniformMatrix4fv(glGetUniformLocation(compiledPrograms[0], "transform"), 1, GL_FALSE, glm::value_ptr(ortoedroModelMatrix));

					//Definimos que queremos dibujar
					glDrawArrays(GL_TRIANGLE_STRIP, 0, 14);
				}




				//Cambiamos buffers
				glFlush();
				glfwSwapBuffers(window);
				
			
			
			
			
			
			}
			
							
		}

		//Desactivar y eliminar programa
		glUseProgram(0);
		glDeleteProgram(compiledPrograms[0]);

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();

}