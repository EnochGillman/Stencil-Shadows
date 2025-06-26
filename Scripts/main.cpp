/*
 * File used to organize project.
 *
 */


// https:// bcmpinc.wordpress.com/2015/08/18/creating-an-opengl-4-5-context-using-sdl2-and-glad/
	// jan/07/25

// Definitions
#define GLM_FORCE_RADIANS	1
#define GLM_ENABLE_EXPERIMENTAL

#define PI float(3.14159265358979323846)

// Includes
#include <algorithm>
#include <SDL3/SDL.h>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

#include "stb_image/stb_image.h"

#include "model.hpp"
#include "light.hpp"
#include "camera.hpp"

int SCREEN_FULLSCREEN = 0;
int SCREEN_VSYNC = 0;
int SCREEN_WIDTH = 960;
int SCREEN_HEIGHT = 540;
int RESOLUTION_WIDTH = 1280;
int RESOLUTION_HEIGHT = 720;

int FPS_CAP = 60;
unsigned int LAST_TIME = SDL_GetTicks();
double FRAME_TIME = double(1000) / FPS_CAP;
double FPS_TRACK = 0.0f;
unsigned long FPS_NEXT = 0;
int FRAME_DELAY = 0;

float MOUSE_X = 0.0f;
float MOUSE_Y = 0.0f;
float MOUSE_WHEEL_Y = 0;

SDL_Window *window = NULL;
SDL_GLContext maincontext;

static void sdlDie(const char* message) {
	fprintf(stderr, "%s: %s\n", message, SDL_GetError());
	exit(2);
}

// https:// stackoverflow.com/questions/32294913/getting-continuous-window-resize-event-in-sdl-2
	// feb/05/25
bool resizingEventWatcher(void* data, SDL_Event* event) {
	if(event->type == SDL_EVENT_WINDOW_RESIZED) {
		if (window == (SDL_Window*)data) {
			SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
			glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		}
	}
	return 0;
}

void initScreen(const char* caption) {
	// initialize sdl3
	if(!SDL_Init(SDL_INIT_VIDEO)) {
		sdlDie("Couldn't initialize SDL");
	}
	atexit(SDL_Quit);
	SDL_GL_LoadLibrary(NULL);

	// set up an OpenGL 4.6 core context
	SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	// get depth buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	// make sure there is no multisampling
	SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	// gamma correction
	SDL_GL_SetAttribute(SDL_GL_FRAMEBUFFER_SRGB_CAPABLE, 1);

	// create window
	window = SDL_CreateWindow(
		caption,
		SCREEN_WIDTH, SCREEN_HEIGHT,
		SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN * SCREEN_FULLSCREEN
	);
	if(window == NULL) {
		sdlDie("Could not set video mode");
	}

	SDL_SetWindowResizable(window, true);
	SDL_AddEventWatch(resizingEventWatcher, window);

	maincontext = SDL_GL_CreateContext(window);
	if(maincontext == NULL) {
		sdlDie("Cou not create context");
	}

	SDL_ShowCursor();

	// get window properties
	printf("OpenGL loaded\n");
	gladLoadGLLoader(GLADloadproc(SDL_GL_GetProcAddress));
	printf("Vendor:   %s\n", glGetString(GL_VENDOR));
	printf("Renderer: %s\n", glGetString(GL_RENDERER));
	printf("Version:  %s\n", glGetString(GL_VERSION));

	// v-sync
	SDL_GL_SetSwapInterval(SCREEN_VSYNC);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	// enable blending
	glEnable(GL_BLEND);

	// enable face culling
	glEnable(GL_CULL_FACE);

	//gamma correction
	glEnable(GL_FRAMEBUFFER_SRGB);

	// disable multisampling
	glDisable(GL_MULTISAMPLE);

	// glEnable(GL_PRIMITIVE_RESTART);
	// glPrimitiveRestartIndex(0);

	SDL_GetWindowSize(window, &SCREEN_WIDTH, &SCREEN_HEIGHT);
	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
}

#define CUBE_CNT 80
glm::vec3 cubePositions[CUBE_CNT];
glm::vec3 grassPositions[CUBE_CNT];

void drawStuff(Shader* program, std::vector<Model>& test) {
	for(int i = 0; i < CUBE_CNT; ++i) {
		float angle_pos = 0.0002f * (int(FPS_NEXT) + FPS_TRACK) * (1-2*((int)i%2)) + 2*i;
		test[i%test.size()].model = glm::translate(glm::mat4(1.0f), cubePositions[i] + glm::vec3(std::sin(angle_pos * 1.2f), std::sin(angle_pos * 2) * 2.0f, std::cos(angle_pos * 1.2f)) * 0.2f);
		float angle = 0.0006f * (int(FPS_NEXT) + FPS_TRACK) + i;
		test[i%test.size()].model = glm::rotate(
			test[i%test.size()].model,
							angle,
					  glm::vec3(std::sin(angle), std::sin(angle*0.5f+1), std::sin(angle*2.0f+2))
		);
		test[i%test.size()].use(program);
	}
}

int main() {
	srand(time(0));

	for(int i = 0; i < CUBE_CNT; ++i) {
		cubePositions[i] = glm::vec3(std::sin(i*7) * 2.3f * std::sqrt(float(i)), std::sin(i*70) * 3.0f + 4.0f, std::cos(i*5) * 2.3f * std::sqrt(float(i))) * 4.0f;
	}
	for(int i = 0; i < CUBE_CNT; ++i) {
		grassPositions[i] = glm::vec3(std::sin(i*5) * 1.3f * i, 0.0f, std::cos(i*5) * 1.3f * i) * (1.0f/(1+i)) * 2.0f * std::sqrt(float(i)) + glm::vec3(0.0f, 0.5f, 0.0f);
	}

	initScreen("OpenGL 4.6");

	Model floor("Resources/Models/Floor/floor.gltf");
	std::vector<Model> test {
		Model("Resources/Models/Graffiti/graffiti.gltf"),
		Model("Resources/Models/Torus/torus.gltf"),
		Model("Resources/Models/Circle/circle.gltf"),
		Model("Resources/Models/outsourced_bunny/scene.gltf")
	};
	floor.model = glm::scale(glm::translate(floor.model, glm::vec3(0.0f, -1.0f, 0.0f)), glm::vec3(25.0f));

	glm::vec3 ambientColor;
	Camera::init();
	Camera camera {&RESOLUTION_WIDTH, &RESOLUTION_HEIGHT, 70.0f, &ambientColor};

	Light::init(&camera.pos, &camera.view, &camera.billboard);
	std::vector<Light> lights;
	if(rand() % 2 == 0) {
		std::cout << "Day" << std::endl;
		ambientColor = {0.4f, 0.55f, 0.75f};
		// ambientColor = {1.0f, 1.0f, 1.0f};
		lights.push_back({	// point
			glm::vec4(glm::normalize(glm::vec3(0.2f, 1.0f, -0.2f)), 0.0f),
				 glm::vec3(0.0f),
				 glm::vec3(1.0f, 0.7f, 0.3f)*2.0f,
				 0.0f,
				 0.0f,
				 0.0f
		});
		// for(int i = 0; i < 10; ++i) {
		// 	lights.push_back({	// point
		// 		glm::vec4(glm::normalize(glm::vec3(0.2f + std::sin(i*PI*0.2f)*0.01f, 1.0f, -0.2f + std::cos(i*PI*0.2f)*0.01f)), 0.0f),
		// 			 glm::vec3(0.0f),
		// 			 glm::vec3(1.0f, 0.7f, 0.3f)*0.1f,
		// 			 0.0f,
		// 			 0.0f,
		// 			 0.0f
		// 	});
		// }
	} else {
		std::cout << "Night" << std::endl;
		ambientColor = {0.0f, 0.0f, 0.0f};
		lights.push_back({	// point
			glm::vec4(glm::vec3(42.0f, 50.0f, 0.0f), 1.0f),
				 glm::vec3(0.0f),
				 glm::vec3(1.0f, 0.7f, 0.3f)*8.0f,
				 400.0f,
				 0.0f,
				 0.0f
		});
		lights.push_back({	// point
			glm::vec4(glm::vec3(0.0f, 52.0f, 42.0f), 1.0f),
				 glm::vec3(0.0f),
				 glm::vec3(1.0f, 0.7f, 1.0f)*7.0f,
				 400.0f,
				 0.0f,
				 0.0f
		});
	}

	Line::init(&camera.view, &camera.screen);
	// glm::vec3 lineColor(1.0f, 0.7f, 0.2f);

	std::vector<Line> lines {
		{
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.1f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			true
		},
		{
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.1f, 0.0f),
			glm::vec3(0.0f, 1.0f, 0.0f),
			true
		},
		{
			glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3(0.0f, 0.0f, 0.1f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			true
		}
	};

	Shader litProgram(
		"Resources/Shaders/Model/lit_basic.vert",
		"Resources/Shaders/Model/lit_basic.frag"
	);
	Shader unlitProgram(
		"Resources/Shaders/Model/unlit_basic.vert",
		"Resources/Shaders/Model/unlit_basic.frag"
	);
	Shader stencilProgram(
		"Resources/Shaders/Model/stencil.vert",
		"Resources/Shaders/Model/stencil.geom",
		"Resources/Shaders/Model/stencil.frag"
	);

	stencilProgram.use();
	stencilProgram.add_uniform3f("unViewPos", &camera.pos);
	stencilProgram.add_uniform4m("unView", &camera.view);

	litProgram.use();
	litProgram.add_uniform3f("unViewPos", &camera.pos);
	litProgram.add_uniform4m("unView", &camera.view);

	unlitProgram.use();
	unlitProgram.add_uniform3f("unViewPos", &camera.pos);
	unlitProgram.add_uniform1f("unClipNear", &camera.clipNear);
	unlitProgram.add_uniform1f("unClipFar", &camera.clipFar);
	unlitProgram.add_uniform4m("unView", &camera.view);

	glPolygonMode(GL_FRONT, GL_FILL);
	// glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	// Input
	const bool* KEYS = SDL_GetKeyboardState(nullptr);
	SDL_Event event;
	bool quit = false;

	Shader screenProgram(
		"Resources/Shaders/Screen/screen.vert",
		"Resources/Shaders/Screen/screen.frag"
	);
	screenProgram.use();
	screenProgram.add_uniform3f("unLightAc", &ambientColor);

	glBindVertexArray(0);

	LAST_TIME = SDL_GetTicks();
	FPS_NEXT = LAST_TIME + 1000;
	while (!quit) {
		MOUSE_WHEEL_Y = 0;
		// process inputs
		while (SDL_PollEvent(&event)) {
			if(event.type == SDL_EVENT_QUIT) {
				quit = true;
			}
			if(event.type == SDL_EVENT_MOUSE_WHEEL) {
				MOUSE_WHEEL_Y -= event.wheel.y;
			}
			if(event.type == SDL_EVENT_MOUSE_BUTTON_DOWN) {
				switch((int)SDL_CursorVisible()) {
					case 0:
						SDL_ShowCursor();
						MOUSE_X = 0.0f;
						MOUSE_Y = 0.0f;
						break;
					case 1:
						SDL_HideCursor();
						SDL_WarpMouseInWindow(window, SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f);
						SDL_WarpMouseInWindow(window, SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f);
						break;
				}
			}
		}
		if (KEYS[SDL_SCANCODE_ESCAPE]) {
			quit = true;
		}

		// compute game

		// return mouse to center
		if(!SDL_CursorVisible()) {
			SDL_GetMouseState(&MOUSE_X, &MOUSE_Y);
			MOUSE_X -= SCREEN_WIDTH*0.5f;
			MOUSE_Y -= SCREEN_HEIGHT*0.5f;
			SDL_WarpMouseInWindow(window, SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f);
		}

		for(size_t i = 0; i < lights.size(); ++i) {
			if(lights[i].pos.w == 1.0f) {
				lights[i].pos = glm::rotate(lights[i].pos, 0.005f+int(i)*0.001f, glm::normalize(glm::vec3(0.1f, 1.0f, 0.0f)));
			}
		}

		// render
		camera.use(
			MOUSE_X,
			MOUSE_Y,
			MOUSE_WHEEL_Y,
			float(KEYS[SDL_SCANCODE_S] - KEYS[SDL_SCANCODE_W]),
			float(KEYS[SDL_SCANCODE_D] - KEYS[SDL_SCANCODE_A]),
			float(KEYS[SDL_SCANCODE_SPACE] - KEYS[SDL_SCANCODE_LSHIFT])
		);
		// camera.up = glm::normalize(glm::rotate(camera.up, 0.01f, glm::vec3(1.0f, 0.0f, 0.0f)));

		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);

		glDisable(GL_STENCIL_TEST);
		glDepthMask(GL_TRUE);
		glDepthFunc(GL_LESS);

		camera.unlitPass();
		for(size_t i = 0; i < lines.size(); ++i) {
			lines[i].use();
		}
		for(size_t i = 0; i < lights.size(); ++i) {
			lights[i].use();
		}

		drawStuff(&unlitProgram, test);
		floor.use(&unlitProgram);

		glBlendFunc(GL_ONE, GL_ONE);
		glBlendEquation(GL_FUNC_ADD);

		for(size_t i = 0; i < lights.size(); ++i) {
			glDepthFunc(GL_LESS);
			stencilProgram.use();
			stencilProgram.use_uniform4f("unLightPos", &lights[i].pos);
			glEnable(GL_STENCIL_TEST);
			glDepthMask(GL_FALSE);
			glEnable(GL_DEPTH_CLAMP);

			glDisable(GL_CULL_FACE);
			glStencilMask(0xFF);
			glClear(GL_STENCIL_BUFFER_BIT);
			glStencilFunc(GL_ALWAYS, 0, 0xFF);
			glStencilOpSeparate(GL_FRONT, GL_KEEP, GL_DECR_WRAP, GL_KEEP);
			glStencilOpSeparate(GL_BACK, GL_KEEP, GL_INCR_WRAP, GL_KEEP);
			drawStuff(&stencilProgram, test);
			glEnable(GL_CULL_FACE);
			glStencilMask(0x00);

			glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
			glStencilFunc(GL_EQUAL, 0, 0xFF);

			glEnable(GL_CULL_FACE);
			glDepthMask(GL_TRUE);
			glDisable(GL_DEPTH_CLAMP);
			floor.use(&stencilProgram);
			litProgram.use();
			glActiveTexture(GL_TEXTURE0 + 2);
			glUniform1i(
				glGetUniformLocation(litProgram.ID, "unScreen"), 2
			);
			lights[i].bind(&litProgram);
			glDepthFunc(GL_EQUAL);
			drawStuff(&litProgram, test);
			floor.use(&litProgram);
		}
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glBlendEquation(GL_FUNC_ADD);

		glDisable(GL_STENCIL_TEST);

		glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		glDisable(GL_CULL_FACE);
		glDepthMask(GL_TRUE);
		camera.show(&screenProgram);

		SDL_GL_SwapWindow(window);

		// increment the float tracker of how long it has been
		FPS_TRACK += FRAME_TIME;
		if(FPS_NEXT + 1000 < SDL_GetTicks()) {
			std::cout << "detected sizeable lag, resetting FPS_NEXT..." << std::endl;
			FPS_NEXT = SDL_GetTicks();
		}
		FPS_NEXT += int(FPS_TRACK);
		FPS_TRACK -= int(FPS_TRACK);

		// delay to prevent high cpu usage
		FRAME_DELAY = int(FRAME_TIME) - int(SDL_GetTicks() - LAST_TIME) - 2;
		if(0 < FRAME_DELAY) {
			SDL_Delay(FRAME_DELAY);
		}

		// indicate that we slept too long
		if(!(SDL_GetTicks() < FPS_NEXT)) {
			std::cout << "jitter at " << LAST_TIME << std::endl;
		}

		// utilize busy wait for maximum accuracy
		while(SDL_GetTicks() < FPS_NEXT) {}
		LAST_TIME = SDL_GetTicks();
	}

	SDL_GL_DestroyContext(maincontext);
	SDL_DestroyWindow(window);

	return 0;
}
