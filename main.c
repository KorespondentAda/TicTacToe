#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "shadertools.h"
#include "utils.h"

#define WIN_WIDTH 800
#define WIN_HEIGHT 600
#define WIN_NAME "TicTacToe"
#define CIRCLE_VERTEX_COUNT 16
#define PI 3.14159265f

/* TODO Move to structure */
GLuint progBorders;
int gameState;
int winner;
int tiles[9];

void renderMainMenu() {
	/* Main menu consists of:
	 * background
	 * title
	 * play button
	 * exit button
	 *
	 * Background will be just black.
	 * Title is "TicTacToe", but how to draw it properly?
	 * "Play" and "Exit" buttons will be under title as texts,
	 * highligted under cursor.
	 * TODO
	 */

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

}

void drawBorders() {
	static const GLfloat ptsBordersVertex[] = {
		-.25f, -.6, 0.0f,
		-.25f,  .6, 0.0f,
		 .25f, -.6, 0.0f,
		 .25f,  .6, 0.0f,
		-.5, -.3f, 0.0f,
		 .5, -.3f, 0.0f,
		-.5,  .3f, 0.0f,
		 .5,  .3f, 0.0f
	};
	GLuint vaTileBorders; /* TODO Export buffer gen to be done once */
	glGenBuffers(1, &vaTileBorders);
	glBindBuffer(GL_ARRAY_BUFFER, vaTileBorders);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ptsBordersVertex), ptsBordersVertex, GL_STATIC_DRAW);
	glLineWidth(10.0f);
	glUseProgram(progBorders);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 4*2);
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &vaTileBorders);
}

void drawCross(GLfloat dx, GLfloat dy) {
	const float tx = 0.8f * 0.25f;
	const float ty = 0.8f * 0.3f;
	GLfloat vertices[] = {
		-1.0f*tx+dx, -1.0f*ty+dy, 0.0f,
		 1.0f*tx+dx,  1.0f*ty+dy, 0.0f,
		 1.0f*tx+dx, -1.0f*ty+dy, 0.0f,
		-1.0f*tx+dx,  1.0f*ty+dy, 0.0f,
	};
	GLuint buf;
	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glLineWidth(10.0f);
	glUseProgram(progBorders);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glDrawArrays(GL_LINES, 0, 2*2);
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &buf);
}

void fillCircleBuffer(GLfloat dx, GLfloat dy, GLfloat tx, GLfloat ty, GLfloat *buffer) {
	GLfloat x, y, z = 0.0f;
	static const GLfloat ω = 2 * PI / CIRCLE_VERTEX_COUNT;
	for (int i = 0; i < CIRCLE_VERTEX_COUNT; i++) {
		x = tx * cos(i * ω) + dx;
		y = ty * sin(i * ω) + dy;
		buffer[(3*i)+0] = x;
		buffer[(3*i)+1] = y;
		buffer[(3*i)+2] = z;
	}
}

void drawCircle(GLfloat dx, GLfloat dy) {
	const float tx = 0.8f * 0.25f;
	const float ty = 0.8f * 0.3f;
	static GLfloat vertices[CIRCLE_VERTEX_COUNT * 3];
	fillCircleBuffer(dx, dy, tx, ty, vertices);
	GLuint buf;
	glGenBuffers(1, &buf);
	glBindBuffer(GL_ARRAY_BUFFER, buf);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	glUseProgram(progBorders);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat)*3, 0);
	glDrawArrays(GL_LINE_LOOP, 0, CIRCLE_VERTEX_COUNT);
	glDisableVertexAttribArray(0);
	glDeleteBuffers(1, &buf);
}

void drawTile(int type, GLfloat dx, GLfloat dy) {
	if (type == 0) {
		return;
	}
	else if (type == 1) { /* Cross */
		drawCross(dx, dy);
	}
	else if (type == 2) { /* Circle */
		drawCircle(dx, dy);
	}
	else {
		Fail("Unknown tile fill type");
	}
}

void renderPlayground() {
	/* Game screen consists of:
	 * Table with 3x3 tiles;
	 * Players crosses and circles in that tiles
	 * TODO
	 */

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	/* Draw tile borders TODO
	 * Need to define border width and its coordinates
	 * Tile size is .25×.3
	 */
	drawBorders();

	const GLfloat dx = 0.25f * 2;
	const GLfloat dy = 0.3f * 2;
	drawTile(tiles[0], -dx, dy);
	drawTile(tiles[1], 0, dy);
	drawTile(tiles[2], dx, dy);
	drawTile(tiles[3], -dx, 0);
	drawTile(tiles[4], 0, 0);
	drawTile(tiles[5], dx, 0);
	drawTile(tiles[6], -dx, -dy);
	drawTile(tiles[7], 0, -dy);
	drawTile(tiles[8], dx, -dy);
}

void clearPlayground() {
	for (int i = 0; i < 9; i++) {
		tiles[i] = 0;
	}
}

void checkCondition() {
	if (tiles[0] == tiles[1] && tiles[1] == tiles[2] && tiles[2] != 0) {
		winner = tiles[0];
		clearPlayground();
	}
	else if (tiles[3] == tiles[4] && tiles[4] == tiles[5] && tiles[5] != 0) {
		winner = tiles[3];
		clearPlayground();
	}
	else if (tiles[6] == tiles[7] && tiles[7] == tiles[8] && tiles[8] != 0) {
		winner = tiles[8];
		clearPlayground();
	}
	else if (tiles[0] == tiles[3] && tiles[3] == tiles[6] && tiles[6] != 0) {
		winner = tiles[0];
		clearPlayground();
	}
	else if (tiles[1] == tiles[4] && tiles[4] == tiles[7] && tiles[7] != 0) {
		winner = tiles[1];
		clearPlayground();
	}
	else if (tiles[2] == tiles[5] && tiles[5] == tiles[8] && tiles[8] != 0) {
		winner = tiles[2];
		clearPlayground();
	}
	else if (tiles[0] == tiles[4] && tiles[4] == tiles[8] && tiles[8] != 0) {
		winner = tiles[4];
		clearPlayground();
	}
	else if (tiles[6] == tiles[4] && tiles[4] == tiles[2] && tiles[2] != 0) {
		winner = tiles[4];
		clearPlayground();
	}
	if (tiles[0] != 0 && tiles[1] != 0 && tiles[2] != 0 &&
			tiles[3] != 0 && tiles[4] != 0 && tiles[5] != 0 &&
			tiles[6] != 0 && tiles[7] != 0 && tiles[8] != 0) {
		winner = 0;
		clearPlayground();
	}
	gameState ^= 1;
	fprintf(stderr, "[Debug] gameState = %d\n", gameState);
}

void doTurn() {
	int choise = rand() % 9;
	while (tiles[choise] != 0) {
		choise = (choise + 1) % 9;
	}
	tiles[choise] = 2;
	checkCondition();
}

void renderWin(GLFWwindow *win, int state) {
	int width, height;
	glfwGetFramebufferSize(win, &width, &height);
	glViewport(0, 0, width, height);
	/* RENDERING:
	 * switch (gameState) {
	 * 	case gameMainMenu
	 * 	case gamePlayAI
	 * 	case gamePlayP
	 * 	case gameEnd
	 * }
	 */
	switch (state) {
		case 1: /* Main Menu (Currently without it) */
			renderMainMenu();
			break;
		case 2: /* Game itself */
			doTurn();
			[[fallthrough]];
		case 3:
			renderPlayground();
			break;
		default:
			Fail("Rendering error: unknown game state");
	}

	glfwSwapBuffers(win);
	return;
}

int getTileByCoord(GLFWwindow *win, double x, double y) {
	int width, height;
	glfwGetFramebufferSize(win, &width, &height);
	int x0 = width / 8 * 1;
	int x1 = width / 8 * 3;
	int x2 = width / 8 * 5;
	int x3 = width / 8 * 7;
	int y0 = height / 8 * 1;
	int y1 = height / 8 * 3;
	int y2 = height / 8 * 5;
	int y3 = height / 8 * 7;

	int res;
	if (x > x0 && x < x1) res = 0;
	else if (x > x1 && x < x2) res = 1;
	else if (x > x2 && x < x3) res = 2;
	else return -1;
	if (y > y0 && y < y1) res += 0;
	else if (y > y1 && y < y2) res += 3;
	else if (y > y2 && y < y3) res += 6;
	else return -1;
	return res;
}

void cbMouseButton(GLFWwindow *win, int button, int action, int mods) {
	/* Can determine which button pressed:
	 * GLFW_MOUSE_BUTTON_LEFT == GLFW_MOUSE_BUTTON_1
	 * GLFW_MOUSE_BUTTON_MIDDLE == GLFW_MOUSE_BUTTON_3
	 * GLFW_MOUSE_BUTTON_RIGHT == GLFW_MOUSE_BUTTON_2
	 */
	if (action == GLFW_RELEASE) {
		if (button == GLFW_MOUSE_BUTTON_LEFT && gameState == 3) {
			double cx, cy;
			glfwGetCursorPos(win, &cx, &cy);
			DebugPos(cx, cy);
			int tile = getTileByCoord(win, cx, cy);
			if (tile == -1) {
				return;
			}
			if (tiles[tile] == 0) {
				tiles[tile] = 1;
				checkCondition();
			}
		}
	}
}

int main(void) {
	// Init graphics
	if (!glfwInit()) {
		Fail("Failed to initialize GLFW");
	}
	atexit(glfwTerminate);

	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	Debug("GLFW Initialized");

	// Prepare window rendering
	GLFWwindow *win = glfwCreateWindow(WIN_WIDTH, WIN_HEIGHT, WIN_NAME, NULL, NULL);
	if (win == NULL) {
		Fail("Failed to open main window");
	}
	glfwMakeContextCurrent(win);
	glfwSwapInterval(1);
	glClearColor(0, 0, 0, 1);
	/* This is simple 2D game, do not need depth test
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	*/
	Debug("Window created");

	if (glewInit() != GLEW_OK) {
		Fail("Failed to initialize GLEW");
	}
	Debug("GLEW initialized");

	/* Set callbacks
	 */
	glfwSetMouseButtonCallback(win, cbMouseButton);
	Debug("Input callbacks initialized");

	/* Prepare shaders
	 */
	progBorders = LoadShaders("shaderBordersVertex.glsl", "shaderBordersFragment.glsl");
	Debug("Shaders initialized");

	/* TODO */
	gameState = 3;

	GLuint vaId;
	glGenVertexArrays(1, &vaId);
	glBindVertexArray(vaId);

	clearPlayground();
	srand(glfwGetTime());

	Debug("Main game loop starting");
	while (!glfwWindowShouldClose(win)) {
		renderWin(win, gameState);
		glfwPollEvents();
	}
	Debug("Main game loop ended");

	glDeleteVertexArrays(1, &vaId);

	return 0;
}
