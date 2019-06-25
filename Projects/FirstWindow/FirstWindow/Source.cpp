#include <glad/glad.h>
#include <GLFW\glfw3.h>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

// The fragment shader only requires one output variable and that is a vector of size 4 that defines the final color output 
// that we should calculate ourselves. We can declare output values with the out keyword, that we here promptly named FragColor.
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

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


//---------------------------------------------VERTEX SHADER--------------------------------------------------
	unsigned int vertexShader;

	// Creates an empty shader object behind the scenes and returns an unsigned reference ID to this shader
	// returns 0 on error
	vertexShader = glCreateShader(GL_VERTEX_SHADER);

	// Replaces the source code of a given shader object
	// Whenever the shader is compiled, it's compiled using the given source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);

	// Compiles the given shader object
	glCompileShader(vertexShader);

	// Check for successful compilation of the shader
	int  success_vertex_shader;
	char infoLog_vertex_shader[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_vertex_shader);
	if (!success_vertex_shader)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog_vertex_shader);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog_vertex_shader << std::endl;
	}

//---------------------------------------------FRAGMENT SHADER--------------------------------------------------
	unsigned int fragmentShader;
	fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	glCompileShader(fragmentShader);

	int  success_frag_shader;
	char infoLog_frag_shader[512];
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success_frag_shader);
	if (!success_frag_shader)
	{
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog_frag_shader);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog_frag_shader << std::endl;
	}
//---------------------------------------------SHADER PROGRAM--------------------------------------------------
	// Create a program object
	unsigned int shaderProgram;
	shaderProgram = glCreateProgram();

	// Link previously compiled shaders to the program object and link them
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	glLinkProgram(shaderProgram);

	int success_shader_prog;
	char infoLog_shader_prog[512];

	// Check for success of the shader program linking
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success_shader_prog);
	if (!success_shader_prog) {
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog_shader_prog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog_shader_prog << std::endl;
	}
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

//---------------------------------------------VERTEX DATA--------------------------------------------------

	// Specifying the vertices of a triangle
	float vertices[] = {
	-0.5f, -0.5f, 0.0f,
	 0.5f, -0.5f, 0.0f,
	 0.0f,  0.5f, 0.0f
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
	// 6. Offset of where the position data begins in the buffer. Position data is at the start of the array for this case. 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

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


		// Drawing a triangle
		glUseProgram(shaderProgram);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		//-----------------Rendering Commands-----------------
		// Swaps the color buffer (a large buffer that contains color values for each pixel in GLFW's window) that has been used to draw in during this iteration and show it as output to the screen.
		glfwSwapBuffers(window);

		//-----------------Check and call events and swap buffers-----------------
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