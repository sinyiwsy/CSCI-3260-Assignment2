/*********************************************************
FILE : main.cpp (csci3260 2019-2020 Assignment 2)
*********************************************************/
/*********************************************************
Student Information
Student ID: 1155110677
Student Name: Wong Sin Yi
*********************************************************/

#include "Dependencies/glew/glew.h"
#include "Dependencies/freeglut/freeglut.h"
#include "Dependencies/glm/glm.hpp"
#include "Dependencies/glm/gtc/matrix_transform.hpp"
#include "Dependencies/glm/gtc/type_ptr.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "Dependencies/stb_image/stb_image.h"

#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <time.h>

#define no 12
using namespace std;
clock_t now_t;
int st_time = 0;
//int i = 0;
GLint programID;
// Could define the Vao&Vbo and interaction parameter here

int isRotate = 1;
int lightbrightness = 3;
//camera rotation
float radius = 10.0f;
float camX;
float camY;
float camZ;
float yaw = -90.0f;
float pitch = 17.0f;
int original_x;
int original_y;
int event = 0;
int event2 = 0;
int cat_tx = 1;
float cat_r = 0;
float cat_x;
float cat_z;
float cat_delta = 0.1f;

glm::vec3 lightPosition = glm::vec3(8, 3, 0);


struct MultiLight
{
	glm::vec3 position;
	glm::vec3 color;
	glm::vec3 formula;
};

MultiLight lp[no];
GLint LittlelightPosition[no];
GLint Littlelightcolor[no];


float random(int max)
{
	srand(time(NULL));
	int min = 1;
	int x = rand() % (max - min + 1) + min;
	return x;
}
string InttoStringp(int n)
{
	char tmp[256];
	string str;
	sprintf_s(tmp, "littleLight[%d].position", n);
	str = tmp;
	//cout << str << endl;
	return str;
}
string InttoStringc(int n)
{
	char tmp[256];
	string str;
	sprintf_s(tmp, "littleLight[%d].color", n);
	str = tmp;
	//cout << str << endl;
	return str;
}
//a series utilities for setting shader parameters 
void setMat4(const std::string& name, glm::mat4& value)
{
	unsigned int transformLoc = glGetUniformLocation(programID, name.c_str());
	glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(value));
}
void setVec4(const std::string& name, glm::vec4 value)
{
	glUniform4fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void setVec3(const std::string& name, glm::vec3 value)
{
	glUniform3fv(glGetUniformLocation(programID, name.c_str()), 1, &value[0]);
}
void setFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(programID, name.c_str()), value);
}
void setInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(programID, name.c_str()), value);
}

bool checkStatus(
	GLuint objectID,
	PFNGLGETSHADERIVPROC objectPropertyGetterFunc,
	PFNGLGETSHADERINFOLOGPROC getInfoLogFunc,
	GLenum statusType)
{
	GLint status;
	objectPropertyGetterFunc(objectID, statusType, &status);
	if (status != GL_TRUE)
	{
		GLint infoLogLength;
		objectPropertyGetterFunc(objectID, GL_INFO_LOG_LENGTH, &infoLogLength);
		GLchar* buffer = new GLchar[infoLogLength];

		GLsizei bufferSize;
		getInfoLogFunc(objectID, infoLogLength, &bufferSize, buffer);
		cout << buffer << endl;

		delete[] buffer;
		return false;
	}
	return true;
}

bool checkShaderStatus(GLuint shaderID)
{
	return checkStatus(shaderID, glGetShaderiv, glGetShaderInfoLog, GL_COMPILE_STATUS);
}

bool checkProgramStatus(GLuint programID)
{
	return checkStatus(programID, glGetProgramiv, glGetProgramInfoLog, GL_LINK_STATUS);
}

string readShaderCode(const char* fileName)
{
	ifstream meInput(fileName);
	if (!meInput.good())
	{
		cout << "File failed to load..." << fileName;
		exit(1);
	}
	return std::string(
		std::istreambuf_iterator<char>(meInput),
		std::istreambuf_iterator<char>()
	);
}

void installShaders()
{
	GLuint vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	const GLchar* adapter[1];
	string temp = readShaderCode("VertexShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(vertexShaderID, 1, adapter, 0);
	temp = readShaderCode("FragmentShaderCode.glsl");
	adapter[0] = temp.c_str();
	glShaderSource(fragmentShaderID, 1, adapter, 0);

	glCompileShader(vertexShaderID);
	glCompileShader(fragmentShaderID);

	if (!checkShaderStatus(vertexShaderID) || !checkShaderStatus(fragmentShaderID))
		return;

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	if (!checkProgramStatus(programID))
		return;

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	glUseProgram(programID);
}

void mouse_callback(int button, int state, int x, int y)
{
	//TODO: Use mouse to do interactive events and animation
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		//printf("%d %d\n", x, y);
		original_x = x;
		original_y = y;
	}
}

void motion_callback(int x, int y)
{
	//TODO: Use mouse to do interactive events and animation
	
	yaw += 0.03f * (original_x - x);
	pitch += 0.03f * (y - original_y);
	pitch = glm::clamp(pitch, -89.0f, 89.0f);
	//printf("%d %d %f %f\n", x, y, yaw, pitch);
}

void keyboard_callback(unsigned char key, int x, int y)
{
	//TODO: Use keyboard to do interactive events and animation
	if (key == '1')
	{
		cat_tx = 1;
	}
	if (key == '2')
	{
		cat_tx = 2;
	}
	if (key == 'p')
	{
		if (isRotate == 1) {
			isRotate = 0;
			
		}else {
			isRotate = 1;
		
		}
	}
	if (key == 'w' && (event == 0))
	{
		lightbrightness += 1;
		if (lightbrightness > 10)lightbrightness = 10;
	}
	if (key == 's' && (event == 0))
	{
		lightbrightness -= 1;
		if (lightbrightness < 0)lightbrightness = 0;

	}
	if (key == 'h')
	{
		if (event == 0) {
			event = 1;
			lightbrightness = -10;
			for (int i = 0; i < no; i++) {
				lp[i].position = glm::vec3(cat_x, 0.0f, cat_z);
			}
		}
	}
	if (key == 'j')
	{

		event = 0;
		event2 = 0;
		lightbrightness = 3;
	}
}

void special_callback(int key, int x, int y)
{
	//TODO: Use keyboard to do interactive events and animation
	if (key == GLUT_KEY_UP) {
		cat_z += 0.1f * cos(cat_delta * cat_r);
		cat_x += 0.1f * sin(cat_delta * cat_r);
	}

	if (key == GLUT_KEY_DOWN) {
		cat_z -= 0.1f * cos(cat_delta * cat_r);
		cat_x -= 0.1f * sin(cat_delta * cat_r);
	}

	if (key == GLUT_KEY_LEFT) {
		cat_r += 1;
	}

	if (key == GLUT_KEY_RIGHT) {
		cat_r -= 1;
	}
}

// struct for storing the obj file
struct Vertex {
	glm::vec3 position;
	glm::vec2 uv;
	glm::vec3 normal;
};

struct Model {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
};

Model loadOBJ(const char* objPath)
{
	// function to load the obj file
	// Note: this simple function cannot load all obj files.

	struct V {
		// struct for identify if a vertex has showed up
		unsigned int index_position, index_uv, index_normal;
		bool operator == (const V& v) const {
			return index_position == v.index_position && index_uv == v.index_uv && index_normal == v.index_normal;
		}
		bool operator < (const V& v) const {
			return (index_position < v.index_position) ||
				(index_position == v.index_position && index_uv < v.index_uv) ||
				(index_position == v.index_position && index_uv == v.index_uv && index_normal < v.index_normal);
		}
	};

	std::vector<glm::vec3> temp_positions;
	std::vector<glm::vec2> temp_uvs;
	std::vector<glm::vec3> temp_normals;

	std::map<V, unsigned int> temp_vertices;

	Model model;
	unsigned int num_vertices = 0;

	std::cout << "\nLoading OBJ file " << objPath << "..." << std::endl;

	std::ifstream file;
	file.open(objPath);

	// Check for Error
	if (file.fail()) {
		std::cerr << "Impossible to open the file! Do you use the right path? See Tutorial 6 for details" << std::endl;
		exit(1);
	}

	while (!file.eof()) {
		// process the object file
		char lineHeader[128];
		file >> lineHeader;

		if (strcmp(lineHeader, "v") == 0) {
			// geometric vertices
			glm::vec3 position;
			file >> position.x >> position.y >> position.z;
			temp_positions.push_back(position);
		}
		else if (strcmp(lineHeader, "vt") == 0) {
			// texture coordinates
			glm::vec2 uv;
			file >> uv.x >> uv.y;
			temp_uvs.push_back(uv);
		}
		else if (strcmp(lineHeader, "vn") == 0) {
			// vertex normals
			glm::vec3 normal;
			file >> normal.x >> normal.y >> normal.z;
			temp_normals.push_back(normal);
		}
		else if (strcmp(lineHeader, "f") == 0) {
			// Face elements
			V vertices[3];
			for (int i = 0; i < 3; i++) {
				char ch;
				file >> vertices[i].index_position >> ch >> vertices[i].index_uv >> ch >> vertices[i].index_normal;
			}

			// Check if there are more than three vertices in one face.
			std::string redundency;
			std::getline(file, redundency);
			if (redundency.length() >= 5) {
				std::cerr << "There may exist some errors while load the obj file. Error content: [" << redundency << " ]" << std::endl;
				std::cerr << "Please note that we only support the faces drawing with triangles. There are more than three vertices in one face." << std::endl;
				std::cerr << "Your obj file can't be read properly by our simple parser :-( Try exporting with other options." << std::endl;
				exit(1);
			}

			for (int i = 0; i < 3; i++) {
				if (temp_vertices.find(vertices[i]) == temp_vertices.end()) {
					// the vertex never shows before
					Vertex vertex;
					vertex.position = temp_positions[vertices[i].index_position - 1];
					vertex.uv = temp_uvs[vertices[i].index_uv - 1];
					vertex.normal = temp_normals[vertices[i].index_normal - 1];

					model.vertices.push_back(vertex);
					model.indices.push_back(num_vertices);
					temp_vertices[vertices[i]] = num_vertices;
					num_vertices += 1;
				}
				else {
					// reuse the existing vertex
					unsigned int index = temp_vertices[vertices[i]];
					model.indices.push_back(index);
				}
			} // for
		} // else if
		else {
			// it's not a vertex, texture coordinate, normal or face
			char stupidBuffer[1024];
			file.getline(stupidBuffer, 1024);
		}
	}
	file.close();

	std::cout << "There are " << num_vertices << " vertices in the obj file.\n" << std::endl;
	return model;
}

GLuint loadTexture(const char* texturePath)
{
	// tell stb_image.h to flip loaded texture's on the y-axis.
	stbi_set_flip_vertically_on_load(true);
	// load the texture data into "data"
	int Width, Height, BPP;
	unsigned char* data = stbi_load(texturePath, &Width, &Height, &BPP, 0);
	// Please pay attention to the format when sending the data to GPU
	GLenum format = 3;
	switch (BPP) {
	case 1: format = GL_RED; break;
	case 3: format = GL_RGB; break;
	case 4: format = GL_RGBA; break;
	}
	if (!data) {
		std::cout << "Failed to load texture: " << texturePath << std::endl;
		exit(1);
	}

	//GLuint textureID = 0;
	//TODO: Create one OpenGL texture and set the texture parameter 
	GLuint textureID;
	glGenTextures(1, &textureID);
	// "Bind" the newly created texture :
	// to indicate all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, textureID);
	// Give the image to OpenGL
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB,
		GL_UNSIGNED_BYTE, data);
	// OpenGL has now copied the data. Free our own version


	stbi_image_free(data);

	std::cout << "Load " << texturePath << " successfully!" << std::endl;


	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
		GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
		GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}


GLuint VAO, VBO, EBO;
Model obj;
GLuint Texture0;
GLuint Texture1;
GLuint jeepVAO, jeepVBO, jeepEBO;
Model jeepobj;
GLuint jeepTexture0;

GLuint grassVAO, grassVBO, grassEBO;
Model grassobj;
GLuint grassTexture0;

GLuint ccVAO, ccVBO, ccEBO;
Model ccobj;
GLuint ccTexture0;

GLuint lightVAO, lightVBO, lightEBO;
Model lightobj;
GLuint lightTexture0;

GLuint catVAO, catVBO, catEBO;
Model catobj;
GLuint catTexture0, catTexture1;
void sendDataToOpenGL()
{
	//TODO:
	//Load objects and bind to VAO & VBO
	//Load texture
	
	obj = loadOBJ("resources/floor/floor.obj");
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);
	//Create Vertex Buffer Objects
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, obj.vertices.size() * sizeof(Vertex), &obj.vertices[0], GL_STATIC_DRAW);
	//Create Element array Buffer Objects
	glGenBuffers(1, &EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj.indices.size() * sizeof(unsigned int), &obj.indices[0], GL_STATIC_DRAW);
	// 1st attribute buffer : position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, position) // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, uv) // array buffer offset
	);
	Texture0 = loadTexture("resources/floor/floor_spec.jpg");
	Texture1 = loadTexture("resources/floor/floor_diff.jpg");
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, normal) // array buffer offset
	);

	jeepobj = loadOBJ("resources/others/chair.obj");
	glGenVertexArrays(1, &jeepVAO);
	glBindVertexArray(jeepVAO);
	//Create Vertex Buffer Objects
	glGenBuffers(1, &jeepVBO);
	glBindBuffer(GL_ARRAY_BUFFER, jeepVBO);
	glBufferData(GL_ARRAY_BUFFER, jeepobj.vertices.size() * sizeof(Vertex), &jeepobj.vertices[0], GL_STATIC_DRAW);
	//Create Element array Buffer Objects
	glGenBuffers(1, &jeepEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, jeepEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, jeepobj.indices.size() * sizeof(unsigned int), &jeepobj.indices[0], GL_STATIC_DRAW);
	// 1st attribute buffer : position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, position) // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, uv) // array buffer offset
	);
	jeepTexture0 = loadTexture("resources/others/wood.jpg");
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, normal) // array buffer offset
	);

	grassobj = loadOBJ("resources/others/grass.obj");
	glGenVertexArrays(1, &grassVAO);
	glBindVertexArray(grassVAO);
	//Create Vertex Buffer Objects
	glGenBuffers(1, &grassVBO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, grassobj.vertices.size() * sizeof(Vertex), &grassobj.vertices[0], GL_STATIC_DRAW);
	//Create Element array Buffer Objects
	glGenBuffers(1, &grassEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, grassEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, grassobj.indices.size() * sizeof(unsigned int), &grassobj.indices[0], GL_STATIC_DRAW);
	// 1st attribute buffer : position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, position) // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, uv) // array buffer offset
	);
	grassTexture0 = loadTexture("resources/others/grass.jpg");
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, normal) // array buffer offset
	);


	ccobj = loadOBJ("resources/others/cc.obj");
	glGenVertexArrays(1, &ccVAO);
	glBindVertexArray(ccVAO);
	//Create Vertex Buffer Objects
	glGenBuffers(1, &ccVBO);
	glBindBuffer(GL_ARRAY_BUFFER, ccVBO);
	glBufferData(GL_ARRAY_BUFFER, ccobj.vertices.size() * sizeof(Vertex), &ccobj.vertices[0], GL_STATIC_DRAW);
	//Create Element array Buffer Objects
	glGenBuffers(1, &ccEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ccEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, ccobj.indices.size() * sizeof(unsigned int), &ccobj.indices[0], GL_STATIC_DRAW);
	// 1st attribute buffer : position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, position) // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, uv) // array buffer offset
	);
	ccTexture0 = loadTexture("resources/others/cc.jpg");
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, normal) // array buffer offset
	);

	lightobj = loadOBJ("resources/others/light.obj");
	glGenVertexArrays(1, &lightVAO);
	glBindVertexArray(lightVAO);
	//Create Vertex Buffer Objects
	glGenBuffers(1, &lightVBO);
	glBindBuffer(GL_ARRAY_BUFFER, lightVBO);
	glBufferData(GL_ARRAY_BUFFER, lightobj.vertices.size() * sizeof(Vertex), &lightobj.vertices[0], GL_STATIC_DRAW);
	//Create Element array Buffer Objects
	glGenBuffers(1, &lightEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lightEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, lightobj.indices.size() * sizeof(unsigned int), &lightobj.indices[0], GL_STATIC_DRAW);
	// 1st attribute buffer : position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, position) // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, uv) // array buffer offset
	);
	lightTexture0 = loadTexture("resources/others/light.jpg");
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, normal) // array buffer offset
	);
	
	//CAT OBJ
	catobj = loadOBJ("resources/cat/cat.obj");
	glGenVertexArrays(1, &catVAO);
	glBindVertexArray(catVAO);
	//Create Vertex Buffer Objects
	glGenBuffers(1, &catVBO);
	glBindBuffer(GL_ARRAY_BUFFER, catVBO);
	glBufferData(GL_ARRAY_BUFFER, catobj.vertices.size() * sizeof(Vertex), &catobj.vertices[0], GL_STATIC_DRAW);
	//Create Element array Buffer Objects
	glGenBuffers(1, &catEBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, catEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, catobj.indices.size() * sizeof(unsigned int), &catobj.indices[0], GL_STATIC_DRAW);
	// 1st attribute buffer : position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(
		0, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, position) // array buffer offset
	);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(
		1, // attribute
		2, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, uv) // array buffer offset
	);
	catTexture0 = loadTexture("resources/cat/cat_01.jpg");
	catTexture1 = loadTexture("resources/cat/cat_02.jpg");
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(
		2, // attribute
		3, // size
		GL_FLOAT, // type
		GL_FALSE, // normalized?
		sizeof(Vertex), // stride
		(void*)offsetof(Vertex, normal) // array buffer offset
	);
	

}
void matrix(string obj) {
	glm::mat4 modelTransformMatrix = glm::mat4(1.0f);
	glm::mat4 modelRotationMatrix = glm::mat4(1.0f);
	glm::mat4 modelScalingMatrix = glm::mat4(1.0f);
	unsigned int slot = 0;

	if (obj == "test") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(-5.0f, -1.0f, -3.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.2f, 0.2f, 0.2f));
		modelRotationMatrix = glm::rotate(glm::mat4(), 0.5f, glm::vec3(0, 1, 0));
	}
	else if (obj == "grass1") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(8.0f, -1.0f, 1.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(1.0f, 1.0f, 1.0f));
	}
	else if (obj == "grass2") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(6.0f, -1.0f, 0.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(1.5f, 1.5f, 1.5f));
	}
	else if (obj == "grass3") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(0.0f, -1.0f, -5.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(1.6f, 1.6f, 1.6f));
	}
	else if (obj == "grass4") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(-2.0f, -1.0f, -4.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(1.3f, 1.3f, 1.3f));
	}
	else if (obj == "grass5") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(2.0f, -1.0f, -1.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.7f, 0.7f, 0.7f));
	}
	else if (obj == "cc") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(5.0f, 0.5f, -4.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.4f, 0.4f, 0.4f));
		modelRotationMatrix = glm::rotate(glm::mat4(), -0.5f, glm::vec3(0, 1, 0));
	}
	else if (obj == "light") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(6.0f, -1.2f, -6.0f));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.8f, 0.8f, 0.8f));
		//modelRotationMatrix = glm::rotate(glm::mat4(), -0.5f, glm::vec3(0, 1, 0));
	}
	else if (obj == "cat") {
		modelTransformMatrix = glm::translate(glm::mat4(), glm::vec3(cat_x, -1.0f, cat_z));
		modelScalingMatrix = glm::scale(glm::mat4(), glm::vec3(0.08f, 0.08f, 0.08f));
		modelRotationMatrix = glm::rotate(glm::mat4(), -14.0f, glm::vec3(1, 0, 0));
		modelRotationMatrix *= glm::rotate(glm::mat4(), cat_r*cat_delta, glm::vec3(0, 0, 1));
	}


	GLint modelTransformMatrixUniformLocation = glGetUniformLocation(programID, "modelTransformMatrix");
	GLint modelRotateMatrixUniformLocation = glGetUniformLocation(programID, "modelRotationMatrix");
	GLint modelScalingMatrixUniformLocation = glGetUniformLocation(programID, "modelScalingMatrix");

	glUniformMatrix4fv(modelTransformMatrixUniformLocation, 1, GL_FALSE, &modelTransformMatrix[0][0]);
	glUniformMatrix4fv(modelRotateMatrixUniformLocation, 1, GL_FALSE, &modelRotationMatrix[0][0]);
	glUniformMatrix4fv(modelScalingMatrixUniformLocation, 1, GL_FALSE, &modelScalingMatrix[0][0]);

	camX = 20.0* sin(cos(glm::radians(yaw)) * cos(glm::radians(pitch)));
	camY = 20.0* sin(glm::radians(pitch));
	camZ = -20.0*cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	glm::mat4 Projection = glm::perspective(glm::radians(45.0f), (float)640/(float)480 , 0.1f, 100.0f);
	glm::mat4 Lookat = glm::lookAt(
		glm::vec3(camX, camY , camZ), //cam
		glm::vec3(0.0f, 5.0f, -5.0f), //look
		glm::vec3(0, 1, 0) 
	);
	glm::mat4 Tmp = glm::translate(glm::mat4(), glm::vec3(0.0f, 0.5f, -5.0f));
	glm::mat4 ProjectionMatrix = Projection * Lookat * Tmp;
	GLuint MatrixID = glGetUniformLocation(programID, "projectionMatrix");
	glUniformMatrix4fv(MatrixID, 1, GL_FALSE, &ProjectionMatrix[0][0]);

	GLuint TexLoc = glGetUniformLocation(programID, "myTextureSampler0");
	glActiveTexture(GL_TEXTURE0 + slot);
	glUniform1i(TexLoc, 0);

	//Light Effect
	
	//GLint ambientLightUniformLocation = glGetUniformLocation(programID, "ambientLight");
	//glm::vec4 ambientLight(0.1f * lightbrightness, 0.1f * lightbrightness, 0.1f *lightbrightness, 1.0f);
	//glUniform4fv(ambientLightUniformLocation, 1, &ambientLight[0]);

	GLint lightPositionUniformLocation = glGetUniformLocation(programID, "lightPositionWorld");
	glm::mat4 rotationMat = glm::rotate(glm::mat4(),  0.00015f , glm::vec3(0, isRotate, 0));
	if (isRotate == 1) lightPosition = glm::vec3(rotationMat * glm::vec4(lightPosition, 1));
	glUniform3fv(lightPositionUniformLocation, 1, &lightPosition[0]);

	GLint eyePositionUniformLocation = glGetUniformLocation(programID, "eyePositionWorld");
	glm::vec3 eyePosition(camX, camY, camZ);
	glUniform3fv(eyePositionUniformLocation, 1, &eyePosition[0]);
	
	//direactional light
	GLint ambientLightUniformLocation0 = glGetUniformLocation(programID, "ambientLight0");
	glm::vec4 ambientLight0(0.1f * lightbrightness, 0.1f * lightbrightness, 0.1f * lightbrightness, 1.0f);
	glUniform4fv(ambientLightUniformLocation0, 1, &ambientLight0[0]);

	GLint lightPositionUniformLocation0 = glGetUniformLocation(programID, "lightPositionWorld0");
	glm::vec3 lightPosition0 = glm::vec3(cat_x, 5.0f, cat_z);
	glUniform3fv(lightPositionUniformLocation0, 1, &lightPosition0[0]);

	GLint eyePositionUniformLocation0 = glGetUniformLocation(programID, "eyePositionWorld0");
	glm::vec3 eyePosition0(camX, camY, camZ);
	glUniform3fv(eyePositionUniformLocation0, 1, &eyePosition0[0]);
	if (event == 1) {
		for (int i = 0; i < no; i++) {
			if (i % 3 == 0) lp[i].color = glm::vec3(1.0f, 0.0f, 0.0f); //color setting
			else if (i % 3 == 1) lp[i].color = glm::vec3(0.0f, 1.0f, 0.0f);
			else lp[i].color = glm::vec3(0.0f, 0.0f, 1.0f);

			string str = InttoStringp(i);
			const char* c_str = str.c_str();
			string str2 = InttoStringc(i);
			const char* c_str2 = str2.c_str();
			LittlelightPosition[i] = glGetUniformLocation(programID, c_str);
			glm::mat4 lprotationMat = glm::rotate(glm::mat4(), 0.0001f, glm::vec3(0, 1, 0));
			lp[i].position = glm::vec3(lp[i].formula + lp[i].position);
			lp[i].position = glm::vec3(lprotationMat * glm::vec4(lp[i].position, 1));
			glUniform3fv(LittlelightPosition[i], 1, &lp[i].position[0]);
			Littlelightcolor[i] = glGetUniformLocation(programID, c_str2);
			glUniform3fv(Littlelightcolor[i], 1, &lp[i].color[0]);
		}
	}
	if (event == 0) {
		for (int i = 0; i < no; i++) {
			string str = InttoStringc(i);
			const char* c_str = str.c_str();
			lp[i].color = glm::vec3(0.0f,0.0f,0.0f);
			Littlelightcolor[i] = glGetUniformLocation(programID, c_str);
			glUniform3fv(Littlelightcolor[i], 1, &lp[i].color[0]);
		}
	}
}
void paintGL(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//cout << i <<": "<<clock() - st_time  << endl;
	//TODO:
	//Set lighting information, such as position and color of lighting source
	//Set transformation matrix
	//Bind different textures
	
	//Deph test
	glClearDepth(1.0f);
	glDepthFunc(GL_LESS);
	glEnable(GL_DEPTH_TEST);
	
	matrix("floor");
	glBindVertexArray(VAO);
	glBindTexture(GL_TEXTURE_2D, Texture1);
	glDrawElements(GL_TRIANGLES, obj.indices.size(), GL_UNSIGNED_INT, 0);
	
	matrix("test");
	glBindVertexArray(jeepVAO);
	glBindTexture(GL_TEXTURE_2D, jeepTexture0);
	glDrawElements(GL_TRIANGLES, jeepobj.indices.size(), GL_UNSIGNED_INT, 0);

	matrix("grass1");
	glBindVertexArray(grassVAO);
	glBindTexture(GL_TEXTURE_2D, grassTexture0);
	glDrawElements(GL_TRIANGLES, grassobj.indices.size(), GL_UNSIGNED_INT, 0);

	matrix("grass2");
	glBindVertexArray(grassVAO);
	glBindTexture(GL_TEXTURE_2D, grassTexture0);
	glDrawElements(GL_TRIANGLES, grassobj.indices.size(), GL_UNSIGNED_INT, 0);

	matrix("grass3");
	glBindVertexArray(grassVAO);
	glBindTexture(GL_TEXTURE_2D, grassTexture0);
	glDrawElements(GL_TRIANGLES, grassobj.indices.size(), GL_UNSIGNED_INT, 0);

	matrix("grass4");
	glBindVertexArray(grassVAO);
	glBindTexture(GL_TEXTURE_2D, grassTexture0);
	glDrawElements(GL_TRIANGLES, grassobj.indices.size(), GL_UNSIGNED_INT, 0);

	matrix("grass5");
	glBindVertexArray(grassVAO);
	glBindTexture(GL_TEXTURE_2D, grassTexture0);
	glDrawElements(GL_TRIANGLES, grassobj.indices.size(), GL_UNSIGNED_INT, 0);
	
	matrix("cc");
	glBindVertexArray(ccVAO);
	glBindTexture(GL_TEXTURE_2D, ccTexture0);
	glDrawElements(GL_TRIANGLES, ccobj.indices.size(), GL_UNSIGNED_INT, 0);

	matrix("light");
	glBindVertexArray(lightVAO);
	glBindTexture(GL_TEXTURE_2D, lightTexture0);
	glDrawElements(GL_TRIANGLES, lightobj.indices.size(), GL_UNSIGNED_INT, 0);

	//cout << jeepobj.indices.size() << endl;
	
	matrix("cat");
	glBindVertexArray(catVAO);
	if (cat_tx == 1)
		glBindTexture(GL_TEXTURE_2D, catTexture0);
	if (cat_tx == 2)
		glBindTexture(GL_TEXTURE_2D, catTexture1);

	glDrawElements(GL_TRIANGLES, catobj.indices.size(), GL_UNSIGNED_INT, 0);
	
	//cout << cat_tx << endl;
	//cout << catobj.indices.size()<< endl;
	glFlush();
	glutPostRedisplay();
}

void initializedGL(void) //run only once
{
	glewInit();
	installShaders();
	sendDataToOpenGL();
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE | GLUT_DEPTH | GLUT_MULTISAMPLE);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Assignment 2");

	//TODO:
	/*Register different CALLBACK function for GLUT to response
	with different events, e.g. window sizing, mouse click or
	keyboard stroke */

	float r = 0.0006f;
	for (int i = 0; i < no; i++) {
		float angle = ((360 / no) * i) * (3.1415926 / 180);
		//cout << i << ": " << angle << endl;
		lp[i].position = glm::vec3(cat_x, 0.0f, cat_z);
		lp[i].formula = glm::vec3(cos(angle) * r, 0.0f, sin(angle) * r);
		if( i%3 == 0) lp[i].color = glm::vec3(1.0f, 0.0f, 0.0f);
		else if (i % 3 == 1) lp[i].color = glm::vec3(0.0f, 1.0f, 0.0f);
		else lp[i].color = glm::vec3(0.0f, 0.0f, 1.0f);
	}


	initializedGL();
	glutDisplayFunc(paintGL);

	glutMouseFunc(mouse_callback);
	glutMotionFunc(motion_callback);
	glutKeyboardFunc(keyboard_callback);
	glutSpecialFunc(special_callback);

	glutMainLoop();
	//system("PAUSE");
	return 0;
}

void mouseWheel_callback(int wheel, int direction, int x, int y)
{
	// Optional.
}