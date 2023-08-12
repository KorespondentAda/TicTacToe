#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#include "utils.h"

static void PrintShaderError(GLuint shader) {
	int logLength;
	char *log;
	glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0){
		log = malloc(logLength);
		glGetShaderInfoLog(shader, logLength, NULL, log);
		Error("Can't compile vertex shader:");
		Error(log);
		free(log);
	}
}

static void PrintProgramError(GLuint program) {
	int logLength;
	char *log;
	glGetProgramiv(program, GL_INFO_LOG_LENGTH, &logLength);
	if (logLength > 0){
		log = malloc(logLength);
		glGetProgramInfoLog(program, logLength, NULL, log);
		Error("Can't link shader program:");
		Error(log);
		free(log);
	}
}

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
		PrintShaderError(shVertex);
		goto cleanup;
	}
	Debug("Vertex shader compiled");

	GLuint shFragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(shFragment, 1, &(data[1]), NULL);
	free(data[1]);
	glCompileShader(shFragment);
	glGetShaderiv(shFragment, GL_COMPILE_STATUS, &res);
	if (!res) {
		PrintShaderError(shFragment);
		goto cleanup;
	}
	Debug("Fragment shader compiled");

	/* Linking */
	GLuint program = glCreateProgram();
	glAttachShader(program, shVertex);
	glAttachShader(program, shFragment);
	glLinkProgram(program);
	glGetProgramiv(program, GL_LINK_STATUS, &res);
	if (!res) {
		PrintProgramError(program);
		goto cleanup;
	}
	Debug("Shader program linked");

cleanup:

	glDetachShader(program, shVertex);
	glDetachShader(program, shFragment);
	glDeleteShader(shVertex);
	glDeleteShader(shFragment);

	return program;
}
