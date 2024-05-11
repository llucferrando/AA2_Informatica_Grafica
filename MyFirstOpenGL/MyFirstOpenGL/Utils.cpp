#include "Utils.h"


struct ShaderProgram {

	GLuint vertexShader = 0;
	GLuint geometryShader = 0;
	GLuint fragmentShader = 0;
};

Troll Utils::LoadOBJModel(const std::string& filePath)
{
	std::ifstream file(filePath);
	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	std::string line;
	std::stringstream ss;
	std::string prefix;
	glm::vec3 tmpVector3;
	glm::vec2 tmpVector2;

	std::vector<float> vertexs;
	std::vector<float> vertexNormal;
	std::vector<float> textureCoordinates;

	std::vector<float> tmpVertexs;
	std::vector<float> tmpNormals;
	std::vector<float> tmptextureCoordinates;

	while (std::getline(file, line)) {

		ss.clear();
		ss.str(line);
		ss >> prefix;

		if (prefix == "v") {
			ss >> tmpVector3.x >> tmpVector3.y >> tmpVector3.z;
			tmpVertexs.push_back(tmpVector3.x);
			tmpVertexs.push_back(tmpVector3.y);
			tmpVertexs.push_back(tmpVector3.z);
		}
		else if (prefix == "vt") {
			ss >> tmpVector2.x >> tmpVector2.y;
			tmptextureCoordinates.push_back(tmpVector2.x);
			tmptextureCoordinates.push_back(tmpVector2.y);
		}
		else if (prefix == "vn") {
			ss >> tmpVector3.x >> tmpVector3.y >> tmpVector3.z;
			tmpNormals.push_back(tmpVector3.x);
			tmpNormals.push_back(tmpVector3.y);
			tmpNormals.push_back(tmpVector3.z);
		}
		else if (prefix == "f") {
			int vertexData;
			short counter = 0;

			while (ss >> vertexData) {
				switch (counter) 
				{
					case 0:
						vertexs.push_back(tmpVertexs[(vertexData - 1) * 3]);
						vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 1]);
						vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 2]);
						ss.ignore(1, '/');
						counter++;
						break;

					case 1:
						textureCoordinates.push_back(tmptextureCoordinates[(vertexData - 1) * 2]);
						textureCoordinates.push_back(tmptextureCoordinates[((vertexData - 1) * 2) + 1]);
						ss.ignore(1, '/');
						counter++;
						break;

					case 2:
						vertexNormal.push_back(tmpNormals[(vertexData - 1) * 3]);
						vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 1]);
						vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 2]);
						counter = 0;
						break;
				}

			}

		}

	}

	return Troll(vertexs, textureCoordinates, vertexNormal);
}

//Funcion que genera una smatriz de traslación
glm::mat4 Utils::GenerateTranslationMatrix(glm::vec3 translation)
{
	return glm::translate(glm::mat4(1.f), translation);
}

//Funcion que genera una smatriz de rotación
glm::mat4 Utils::GenerateRotationMatrix(glm::vec3 axis, float fDegrees)
{
	return glm::rotate(glm::mat4(1.f), glm::radians(fDegrees), glm::normalize(axis));
}


//Funcion que genera una smatriz de escalado
glm::mat4 Utils::GenerateScaleMatrix(glm::vec3 scaleAxis)
{
	return glm::scale(glm::mat4(1.f), scaleAxis);
}

//Funcion que devolvera una string con todo el archivo leido
std::string Utils::Load_File(const std::string& filePath)
{
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

GLuint Utils::LoadFragmentShader(const std::string& filePath)
{
	//Crear un fragment shader
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

GLuint Utils::LoadGeometryShader(const std::string& filePath)
{
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

GLuint Utils::LoadVertexShader(const std::string& filePath)
{
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

float Utils::GetElapsedTime()
{
	static double startTime = glfwGetTime();
	return static_cast<float>(glfwGetTime() - startTime);
}
