
#define GL3_PROTOTYPES 1
#include "GL/glew.h"

#include "sdl2_util.h"

const Uint8* gCurrentKeyStates = nullptr;

constexpr unsigned int wndWidth{ 800 }, wndHeight{ 600 };

// An array of 3 vectors which represents 3 vertices
static const GLfloat g_vertex_buffer_data[] = {
	-1.0f, -1.0f, 0.0f,
	1.0f, -1.0f, 0.0f,
	0.0f,  1.0f, 0.0f,
};

class Game
{
private:
	SDL_Window *window = nullptr;
	SDL_GLContext glContext;

	GLuint VertexArrayID;
	// This will identify our vertex buffer
	GLuint vertexbuffer;

	//
	// init openGL stuff for SDL
	// return -1 on error
	//
	int initOpenGLAttrs()
	{
		// TODO - set this to enable GLES for mobile
		// Set our OpenGL version.
		// SDL_GL_CONTEXT_CORE gives us only the newer version, deprecated functions are disabled
		if (SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE) != 0)
		{
			return -1;
		}

		// Set up so that we use version 3.2 of OpenGL.
		// 3.2 is part of the modern versions of OpenGL, but most video cards whould be able to run it
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

		// Turn on double buffering with a 24bit Z buffer.
		if (SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1) != 0)
		{
			return -1;
		}
		if (SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24) != 0)
		{
			return -1;	
		}

		return 0;	// ok
	}

public:
	Game()	{ }

	// returns -1 on error
	int init()
	{
		// init SDL
		if (SDL_Init(SDL_INIT_EVERYTHING) != 0)
		{
			logSDLError(std::cout, "SDL_Init");
			return -1;
		}

		if (initOpenGLAttrs() < 0)
		{
			logSDLError(std::cout, "initOpenGLAttrs");
			return -1;
		}

		// Create window
		window = SDL_CreateWindow("SDL2_OPENGL", 100, 100, wndWidth, wndHeight, SDL_WINDOW_OPENGL);
		if (window == nullptr)
		{
			logSDLError(std::cout, "CreateWindow");
			SDL_Quit();
			return -1;
		}

		glContext = SDL_GL_CreateContext(window);
		if (glContext == nullptr)
		{
			return -1;
		}

		// This tells OpenGL that we want to use OpenGL 3.0 stuff and later.
		glewExperimental = GL_TRUE;
		glewInit();

		// create a Vertex Array Object and set it as the current one :
		glGenVertexArrays(1, &VertexArrayID);
		glBindVertexArray(VertexArrayID);

		// This makes our buffer swap syncronized with the monitor's vertical refresh
		SDL_GL_SetSwapInterval(1);

		//
		// create triangle buffer
		//
		// Generate 1 buffer, put the resulting identifier in vertexbuffer
		glGenBuffers(1, &vertexbuffer);
		// The following commands will talk about our 'vertexbuffer' buffer
		glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
		// Give our vertices to OpenGL.
		glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

		return 0;	// ok
	}

	void run()
	{
		SDL_Event e;
		bool quit = false;
		while (!quit)
		{
			while (SDL_PollEvent(&e))
			{
				//If user closes the window, presses escape
				if (e.type == SDL_QUIT ||
					(e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE))
				{
					quit = true;
				}
			}

			// key state
			gCurrentKeyStates = SDL_GetKeyboardState(NULL);
			if (gCurrentKeyStates[SDL_SCANCODE_ESCAPE])
				break;

			// Set background color as cornflower blue
			glClearColor(0.39f, 0.58f, 0.93f, 1.f);
			// Clear color buffer
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//
			// draw triangle
			//
			// 1st attribute buffer : vertices
			glEnableVertexAttribArray(0);
			glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
			glVertexAttribPointer(
				0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
				3,                  // size
				GL_FLOAT,           // type
				GL_FALSE,           // normalized?
				0,                  // stride
				(void*)0            // array buffer offset
			);
			// Draw the triangle !
			glDrawArrays(GL_TRIANGLES, 0, 3); // Starting from vertex 0; 3 vertices total -> 1 triangle
			glDisableVertexAttribArray(0);

			// Update window with OpenGL rendering
			SDL_GL_SwapWindow(window);
		}

		// cleanup
		SDL_GL_DeleteContext(glContext);
		SDL_DestroyWindow(window);
		SDL_Quit();
	}

};

int main(int, char **)
{
	Game game;
	if (game.init() < 0)
	{
		return -1;
	}
	game.run();
	return 0;
}

