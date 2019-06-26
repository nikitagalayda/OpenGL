#include <glad/glad.h>
#include <GLFW\glfw3.h>
#include <iostream>
#include "Shader.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
	// Initialize GLFW
	glfwInit();
	// Client API Version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Using OpenGL core profile, we'll get access to a smaller subset of OpenGL features
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// Creating a window object
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// Context of the newly created window is the main context on the current thread
	glfwMakeContextCurrent(window);

	// Initialize GLAD
	// We pass GLAD the function to load the adress of the OpenGL function pointers which is OS-specific
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glViewport(0, 0, 800, 600);

	// Called on every window resize
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);


//---------------------------------------------SHADERS--------------------------------------------------
	Shader myShader("3.3.shader.vs", "3.3.shader.fs");

//---------------------------------------------VERTEX DATA--------------------------------------------------

	// Specifying the vertices of a triangle
	float vertices[] = {
		// positions         // colors
		 0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,   // bottom right
		-0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,   // bottom left
		 0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f    // top 
	};

	// VBO (Vertex Buffer Objects) keep large amounts of vertex data on the GPU memory. Vertex Data is then sent from CPU to these buffers in large batches.
	unsigned int VBO;
	glGenBuffers(1, &VBO);

	// VAO (Vertex Array Object) helps us with configuring vertex attributes
	unsigned int VAO;
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	// There are many types of buffers, and VBO buffer type is GL_ARRAY_BUFFER
	// Bind the buffer to GL_ARRAY_BUFFER type
	// From that point on any buffer calls we make (on the GL_ARRAY_BUFFER target) will be used to configure the currently bound buffer, which is VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	
	// Store our predefined vertex data into the bound buffer (VBO) in GPU memory
	// Parameters: 1. Type of buffer we want to copy data into 2. Size of data 3. Actual data 4. How graphics card should manage data
	// For parameter 4:
	// GL_STATIC_DRAW: the data will most likely not change at all or very rarely.
	// GL_DYNAMIC_DRAW: the data is likely to change a lot.
	// GL_STREAM_DRAW : the data will change every time it is drawn.
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// We need to tell OpenGL how to interpret the Vertex Data
	// Parameters:
	// 1. Which vertex attribute to configure. We already specified the location of the position vertex in the vertex shader (layout (location = 0))
	// 2. Size of the vertex attribute. Vertex attribute is a vec3 so it's composed of 3 values
	// 3. Type of data
	// 4. Whether or not data should be normalized. Ours is already normalized.
	// 5. Stride. Space between consecutive vertex attributes. In the vertex data array, next vertex data is located 3 * sizeof(float) units away.
	// 6. Offset of where the position data begins in the buffer. For position it's at the beginning (so 0) for color its 3 floats to the right. 
	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// Color attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	// Render loop
	while (!glfwWindowShouldClose(window))
	{
		//-----------------Input-----------------
		processInput(window);

		//-----------------Fill window with color----------------- 
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		// float timeValue = glfwGetTime();
		// float greenValue = (sin(timeValue) / 2.0f) + 0.5f;

		// Get the location of the uniform
		// int vertexColorLocation = glGetUniformLocation(shaderProgram, "ourColor");
		// Updating the uniform requires us to first use teh program (because it sets the uniform on the currently active shader program)
		myShader.use();
		// Sets a uniform value of the currently active shader program
		// f at the end is because there are no overloaded functions in OpenGL so f represents float (another example is glUniform4i for int)
		// glUniform4f(vertexColorLocation, 0.0f, greenValue, 0.0f, 1.0f);

		// Drawing a triangle
		// glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//-----------------Check and call events and swap buffers-----------------
		// Swaps the color buffer (a large buffer that contains color values for each pixel in GLFW's window) that has been used to draw in during this iteration and show it as output to the screen.
		glfwSwapBuffers(window);
		// Checks if any events are triggered (like keyboard input or mouse movement events), updates the window state, and calls the corresponding functions (which we can set via callback methods)
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}


// ------------------------------------------------FUNCTIONS-------------------------------------------------------------


// Changes the viewport dimensions to match the window size
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
}