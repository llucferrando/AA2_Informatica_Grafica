#include "MyShaderProgram.h"

MyShaderProgram::MyShaderProgram()
{
	ShaderProgram myFirstProgram;
	myFirstProgram.vertexShader = LoadVertexShader("MyFirstVertexShader.glsl");
	myFirstProgram.geometryShader = LoadGeometryShader("MyFirstGeometryShader.glsl");
	myFirstProgram.fragmentShader = LoadFragmentShader("MyFirstFragmentShader.glsl");

	compiledPrograms.push_back(CreateProgram(myFirstProgram));
}
GLuint MyShaderProgram:: LoadVertexShader(const std::string& filePath)
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
GLuint MyShaderProgram::LoadFragmentShader(const std::string& filePath)
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
std::string MyShaderProgram::Load_File(const std::string& filePath)
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
glm::mat4 MyShaderProgram::GenerateTranslationMatrix(glm::vec3 translation)
{
	return glm::mat4();
}
glm::mat4 MyShaderProgram::GenerateRotationMatrix(glm::vec3 axis, float fDegrees)
{
	return glm::mat4();
}
GLuint MyShaderProgram::LoadGeometryShader(const std::string& filePath)
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
GLuint MyShaderProgram::CreateProgram(const ShaderProgram& shaders)
{
	//Crear programa de la GPU
	GLuint program = glCreateProgram();

	//Verificar que existe un vertex shader y adjuntarlo al programa
	if (shaders.vertexShader != 0) {
		glAttachShader(program, shaders.vertexShader);
	}

	if (shaders.geometryShader != 0) {
		glAttachShader(program, shaders.geometryShader);
	}

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

		//Liberamos recursos
		if (shaders.geometryShader != 0) {
			glDetachShader(program, shaders.geometryShader);
		}

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

Model MyShaderProgram::LoadOBJModel(MyShaderProgram* program, int programID, const std::string& filePath, const char* texturefilePath) {

	//Verifico archivo y si no puedo abrirlo cierro aplicativo
	std::ifstream file(filePath);

	if (!file.is_open()) {
		std::cerr << "No se ha podido abrir el archivo: " << filePath << std::endl;
		std::exit(EXIT_FAILURE);
	}

	//Variables lectura fichero
	std::string line;
	std::stringstream ss;
	std::string prefix;
	glm::vec3 tmpVec3;
	glm::vec2 tmpVec2;

	//Variables elemento modelo
	std::vector<float> vertexs;
	std::vector<float> vertexNormal;
	std::vector<float> textureCoordinates;

	//Variables temporales para algoritmos de sort
	std::vector<float> tmpVertexs;
	std::vector<float> tmpNormals;
	std::vector<float> tmpTextureCoordinates;

	//Recorremos archivo linea por linea
	while (std::getline(file, line)) {

		//Por cada linea reviso el prefijo del archivo que me indica que estoy analizando
		ss.clear();
		ss.str(line);
		ss >> prefix;

		//Estoy leyendo un vertice
		if (prefix == "v") {

			//Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			//Almaceno en mi vector de vertices los valores
			tmpVertexs.push_back(tmpVec3.x);
			tmpVertexs.push_back(tmpVec3.y);
			tmpVertexs.push_back(tmpVec3.z);
		}

		//Estoy leyendo una UV (texture coordinate)
		else if (prefix == "vt") {

			//Las UVs son siempre imagenes 2D asi que uso el tmpvec2 para almacenarlas
			ss >> tmpVec2.x >> tmpVec2.y;

			//Almaceno en mi vector temporal las UVs
			tmpTextureCoordinates.push_back(tmpVec2.x);
			tmpTextureCoordinates.push_back(tmpVec2.y);

		}

		//Estoy leyendo una normal
		else if (prefix == "vn") {

			//Asumo que solo trabajo 3D así que almaceno XYZ de forma consecutiva
			ss >> tmpVec3.x >> tmpVec3.y >> tmpVec3.z;

			//Almaceno en mi vector temporal de normales las normales
			tmpNormals.push_back(tmpVec3.x);
			tmpNormals.push_back(tmpVec3.y);
			tmpNormals.push_back(tmpVec3.z);

		}

		//Estoy leyendo una cara
		else if (prefix == "f") {

			int vertexData;
			short counter = 0;

			//Obtengo todos los valores hasta un espacio
			while (ss >> vertexData) {

				//En orden cada numero sigue el patron de vertice/uv/normal
				switch (counter) {
				case 0:
					//Si es un vertice lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec3, salto 1 / y aumento el contador en 1
					vertexs.push_back(tmpVertexs[(vertexData - 1) * 3]);
					vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 1]);
					vertexs.push_back(tmpVertexs[((vertexData - 1) * 3) + 2]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 1:
					//Si es un uv lo almaceno - 1 por el offset y almaceno dos seguidos al ser un vec2, salto 1 / y aumento el contador en 1
					textureCoordinates.push_back(tmpTextureCoordinates[(vertexData - 1) * 2]);
					textureCoordinates.push_back(tmpTextureCoordinates[((vertexData - 1) * 2) + 1]);
					ss.ignore(1, '/');
					counter++;
					break;
				case 2:
					//Si es una normal la almaceno - 1 por el offset y almaceno tres seguidos al ser un vec3, salto 1 / y reinicio
					vertexNormal.push_back(tmpNormals[(vertexData - 1) * 3]);
					vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 1]);
					vertexNormal.push_back(tmpNormals[((vertexData - 1) * 3) + 2]);
					counter = 0;
					break;
				}
			}
		}
	}

	return Model(program, programID, texturefilePath, vertexs, textureCoordinates, vertexNormal);
}
