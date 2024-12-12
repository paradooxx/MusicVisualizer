#include "Window.h"

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}


void Window::run() 
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// glfw window creation
	GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "MusicVisualizer", NULL, NULL);
	if (window == NULL)
	{
		cout << "Failed to create GLFW window" << endl;
		glfwTerminate();
		exit(-1);
	}
	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		cout << "Failed to initialize GLAD" << endl;
		exit(-1);
	}

	Shader shader = Shader("shaders/ShaderVertex.glsl", "shaders/ShaderFragment.glsl");

	// create meshes
	int barNum = 10;
	Mesh* meshes = new Mesh[barNum];

	for (int idx = 0; idx < barNum; idx++) {
		meshes[idx].setVertices(idx, barNum); // set x coordinate
		meshes[idx].setupMesh();
		meshes[idx].loadTexture("textures/container.jpg");
	}

	MusicManager player;
	player.loadMusic("musics/music.mp3");
	player.playMusic();

	// initialize spectrum
	int size = player.initSpectrum(12);
	float* frequencies = new float[size]();

	// render loop
	while (!glfwWindowShouldClose(window))
	{
		// input
		processInput(window);

		// get frequency
		player.getFrequency(frequencies);

		float red = 0.02f + (frequencies[1] * 0.9f);
		float green = 0.4f + (frequencies[1] * 0.2f);
		float blue = 0.639f + (frequencies[1] * 0.1f);

		// render
		glClearColor(red, green, blue, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		shader.use();

		for (int i = 0; i < barNum; i++) {
			float yCoordinate = frequencies[i];
			float corr = yCoordinate * pow(2, i);
			meshes[i].draw(shader, corr);
		}

		// swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();

	delete[] meshes;
	delete[] frequencies;
}
