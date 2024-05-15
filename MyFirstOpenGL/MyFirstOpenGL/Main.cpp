
#include "Utils.h"
#include "Camera.h"
#include "InputManager.h"
#include "Model.h"
#include "MyShaderProgram.h"

#define WINDOW_WIDTH 1000
#define WINDOW_HEIGHT 880


MyShaderProgram * myProgram;
std::vector<Model> models;

using namespace Utils;
void Resize_Window(GLFWwindow* window, int iFrameBufferWidth, int iFrameBufferHeight) {
	//Definir nuevo tamaño del viewport
	glViewport(0, 0, iFrameBufferWidth, iFrameBufferHeight);
	glUniform2f(glGetUniformLocation(myProgram->compiledPrograms[0], "windowSize"), iFrameBufferWidth, iFrameBufferHeight);
}

void main() {
	
	InputManager myInputManager;
	Camera* myCamera = new Camera;
	
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

	//Indicamos lado del cullingS
	glEnable(GL_DEPTH_TEST);

	//Inicializamos GLEW y controlamos errores
	if (glewInit() == GLEW_OK) {

		models.push_back(myProgram->LoadOBJModel(myProgram,0,"Assets/Models/troll.obj", "Assets/Texturas/troll.png"));

		//Definimos color para limpiar el buffer de color
		glClearColor(1.f, 1.f, 1.f, 1.f);

		//Definimos modo de dibujo para cada cara
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		//Generamos el game loop
		while (!glfwWindowShouldClose(window)) {

			//Pulleamos los eventos (botones, teclas, mouse...)
			glfwPollEvents();

			//Limpiamos los buffers
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

			for (Model model : models)
			{
				model.Render();

			}
			
			
			myInputManager.InputTransforms(window, myCamera);
			
			glFlush();
			glfwSwapBuffers(window);
		}
		
	

	}
	else {
		std::cout << "Ha petao." << std::endl;
		glfwTerminate();
	}

	//Finalizamos GLFW
	glfwTerminate();

}