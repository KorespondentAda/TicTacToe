#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "utils.h"

GLuint LoadShaders(const char *vertexFile, const char *fragmentFile) {
	/* Load shader code from file */
	char *data[2];
	FILE *file = fopen(vertexFile, "rb");
	if (!file) Error("Can't open shader file");
	fseek(file, 0, SEEK_END);
	int length = ftell(file);
	rewind(file);
	data[0] = malloc((length + 1) * sizeof(char));
	fread(data[0], 1, length, file);
	fclose(file);
	Debug("Vertex shader file readed");
	file = fopen(fragmentFile, "rb");
	if (!file) Error("Can't open shader file");
	fseek(file, 0, SEEK_END);
	length = ftell(file);
	rewind(file);
	data[1] = malloc((length + 1) * sizeof(char));
	fread(data[1], 1, length, file);
	fclose(file);
	Debug("Fragment shader file readed");

	GLint res;
	GLuint shVertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(shVertex, 1, &(data[0]), NULL);
	free(data[0]);
	glCompileShader(shVertex);
	glGetShaderiv(shVertex, GL_COMPILE_STATUS, &res);
	if (!res) {
		Error("Can't compile vertex shader");
		/* TODO Compilation error message */
	}
	Debug("Vertex shader compiled");

	GLuint shFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shFragment, 1, &(data[1]), NULL);
	free(data[1]);
	glCompileShader(shFragment);
	glGetShaderiv(shFragment, GL_COMPILE_STATUS, &res);
	if (!res) {
		Error("Can't compile fragment shader");
		/* TODO Compilation error message */
	}
	Debug("Fragment shader compiled");

	/* Linking */
	GLuint program = glCreateProgram();
	glAttachShader(program, shVertex);
	glAttachShader(program, shFragment);
	glLinkProgram(program);
	glGetShaderiv(shFragment, GL_LINK_STATUS, &res);
	if (!res) {
		Error("Can't link shader program");
		/* TODO Compilation error message */
	}
	Debug("Shader program linked");

	glDetachShader(program, shVertex);
	glDetachShader(program, shFragment);
	glDeleteShader(shVertex);
	glDeleteShader(shFragment);

	return program;
}
