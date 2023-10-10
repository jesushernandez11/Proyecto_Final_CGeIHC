/*---------------------------------------------------------*/
/* ----------------   Práctica 2 --------------------------*/
/*-----------------    2024-1   ---------------------------*/
/*------------- Hernández Lara Jesús Eduardo ---------------*/
#include <glew.h>
#include <glfw3.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 800;
int SCR_HEIGHT = 600;

GLFWmonitor *monitors;
GLuint VBO[2], VAO[2], EBO[2];
GLuint shaderProgramYellow, shaderProgramColor;

static const char* myVertexShader = "										\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
																			\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);						\n\
}";
//Recibe la posición de los vértices
//Recibe un vector que es el color
//Tiene la salida (outColor) del color
static const char* myVertexShaderColor = "									\n\
#version 330 core															\n\
																			\n\
layout (location = 0) in vec3 aPos;											\n\
layout (location = 1) in vec3 aColor;										\n\
out vec3 ourColor;															\n\
void main()																	\n\
{																			\n\
    gl_Position = vec4(aPos, 1.0);											\n\
	ourColor = aColor;														\n\
}";
//Cambia la apariencia de la figura
// Fragment Shader
static const char* myFragmentShaderYellow = "								\n\
#version 330																\n\
																			\n\
out vec3 finalColor;														\n\
																			\n\
void main()																	\n\
{																			\n\
    finalColor = vec3(1.0f, 0.0f, 0.0f);									\n\
}";
					 //rojo, verde, azul
//tiene una entrada que es la salida del anterior
//Recibe los colores RGB y el canal alfa en 1 y la apariencia se al fragmento de objetos
static const char* myFragmentShaderColor = "								\n\
#version 330 core															\n\
out vec4 FragColor;															\n\
in vec3 ourColor;															\n\
																			\n\
void main()																	\n\
{																			\n\
	FragColor = vec4(ourColor, 1.0f);										\n\
}";

void myData(void);
void setupShaders(void);
void display(void);
void getResolution(void);


void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	float vertices[] = 
	{
		// positions		 //color				Vértice
	/*	-0.6f,  0.6f, 0.0f,	 1.0f, 0.0f, 0.0f,		//0
		0.8f, 0.6f, 0.0f,	 1.0f, 1.0f, 0.0f,		//1
		0.8f, 0.5f, 0.0f,	 0.0f, 1.0f, 0.0f,		//2
		0.0f, 0.5f, 0.0f,	 0.0f, 0.0f, 1.0f,		//3
		0.0f, 0.3f, 0.0f,	 1.0f, 0.0f, 1.0f,		//4
		0.5f, 0.3f, 0.0f,	 0.0f, 1.0f, 1.0f,		//5
		0.5f, 0.0f, 0.0f,	 0.54f, 0.9f, 0.23f,	//6
		0.0f, 0.0f, 0.0f,	 1.0f, 1.0f, 1.0f,		//7
		0.0f, -0.6f, 0.0f,	 0.67f, 0.35f, 0.47f,	//8
		-0.6f, -0.6f, 0.0f,	 0.45f, 0.45f, 0.69f,	//9*/
		-0.20282f,0.69917f, 0.0f,  0.8f, 0.4f, 0.0f, //vertice A 0
		-0.04203f,0.4605f, 0.0f, 0.8f, 0.4f, 0.0f, //vertice B 1
		-0.10233f,0.46662f, 0.0f, 1.0f, 0.6f, 0.8f, //vértice C rosa 2
		-0.10233f,0.46662f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice C naranja 3
		-0.17724f,0.60431f, 0.0f, 1.0f, 0.6f, 0.8f, //vértice D rosa 4
		-0.17724f,0.60431f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice D naranja 5
		-0.21561f,0.42684f, 0.0f, 0.8f, 0.4f, 0.0f, //vertice E 6
		-0.1809f,0.43296f, 0.0f, 1.0f, 0.6f, 0.8f, //vértice F rosa 7
		-0.1809f,0.43296f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice F naranja 8
		-0.17359f,0.33811f, 0.0f, 1.0f, 0.6f, 0.8f, //vértice G rosa 9
		-0.17359f,0.33811f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice G naranja 10
		0.01462f,0.4605f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice H 11
		0.07857f,0.4605f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice I 12
		0.23754f,0.69917f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice J 13
		0.21379f,0.60431f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice K naranja 14
		0.21379f,0.60431f, 0.0f, 1.0f, 0.6f, 0.8f, //vértice K rosa 15 
		0.13887f,0.46662f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice L naranja 16
		0.13887f,0.46662f, 0.0f, 1.0f, 0.6f, 0.8f, //vértice L rosa 17
		0.20282f,0.33811f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice M naranja 18
		0.20282f,0.33811f, 0.0f, 1.0f, 0.6f, 0.8f, //vértice M rosa 19
		0.22292f,0.43296f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice N naranja 20
		0.22292f,0.43296f, 0.0f, 1.0f, 0.6f, 0.8f, //vértice N rosa 21
		0.01462f,0.30139f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice O naranja 22
		0.01462f,0.30139f, 0.0f, 0.6f, 0.8f, 1.0f, //vértice O azul 23
		0.10415f,0.2249f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice P naranja 24
		0.10415f,0.2249f, 0.0f, 0.0823f, 0.3960f, 0.7529f, //vértice P granate 25
		-0.07309f,0.2249f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice Q naranja 26
		-0.07309f,0.2249f, 0.0f, 0.0f, 0.0f, 0.8f, //vértice Q azul 27
		-0.22292f,0.2249f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice R 28
		-0.12608f,0.13004f,0.0f, 0.8f, 0.4f, 0.0f, //vértice S 29
		-0.12608f,0.13004f,0.0f, 0.1083f, 0.4901f, 0.1960f, //vértice S verde 30
		-0.04934f,0.10862f,0.0f, 0.8f, 0.4f, 0.0f, //vértice T naranja 31
		-0.04934f,0.10862f,0.0f, 0.4f, 0.6f, 0.0f, //vértice T verde 32
		0.08405f,0.10862f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice U naranja 33
		0.08405f,0.10862f, 0.0f, 0.4f, 1.0f, 0.0f, //vértice U fosfo 34
		0.15714f,0.13004f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice v naranja 35
		0.15714f,0.13004f, 0.0f, 1.0f, 0.9176f, 0.8039f, //vértice v amarillo  36
		0.25581f,0.2249f,0.0f, 0.8f, 0.4f, 0.0f, //vértice w naranja 37
		0.3f,-0.01377f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice z naranja 38
		0.12791f,0.03519f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice a1 naranja 39
		0.12791f,0.03519f, 0.0f, 0.8f, 1.0f, 0.4f, //vértice a1 verde fosfo 40
		0.07309f,0.04131f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice b1 naranja 41
		0.07309f,0.04131f, 0.0f, 0.8f, 1.0f, 0.8f, //vértice b1 verde claro 42
		0.1206f,-0.2249f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice c1 naranja 43
		0.1206f,-0.2249f, 0.0f, 0.5019f, 0.4f, 0.0f, //vértice c1 granate 44
		0.20831f,-0.12698f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice d1 naranja 45
		-0.2613f,-0.01377f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice e1 naranja 46
		-0.16993f,-0.12698f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice f1 naranja 47
		-0.09319f,0.03519f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice g1 naranja 48
		-0.09319f,0.03519f, 0.0f, 0.8156f, 0.9411f, 0.7529f, //vértice g1 verde clarito 49
		-0.03472f, 0.04131f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice h1 naranja 50
		-0.03472f, 0.04131f, 0.0f, 0.0f, 0.3921f, 0.0f, //vértice h1 verde oscuro 51
		-0.08223f,-0.2249f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice i1 naranja 52
		-0.08223f,-0.2249f, 0.0f, 0.8274f, 0.1843f, 0.1843f, //vértice i1 naranja 53
		-0.06395f, -0.32893f, 0.0f, 1.0f, 0.0f, 0.2f, //vértice j1 tonalidad roja 54
		-0.06395f, -0.32893f,0.0f, 0.8f, 0.4f, 0.0f, //vértice j1 naranja 55
		0.01645f, -0.29221f, 0.0f, 0.3058f, 0.0f, 0.5098f, //vértice k1 tonalidad morada 56
		0.01645f, -0.29221f, 0.0f, 1.0f, 0.0f, 0.2f, //vértice k1 tonalidad roja 57
		0.01645f, -0.29221f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice k1 naranja 58
		-0.02193f, -0.26161f, 0.0f, 0.3058f, 0.0f, 0.5098f, //vértice l1 tonalidad morada 59
		-0.02193f, -0.26161f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice l1 tonalidad roja 60
		-0.01645f,-0.20348f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice m1 tonalidad roja 61
		-0.01645f,-0.20348f, 0.0f, 0.3058f, 0.0f, 0.5098f, //vértice m1 tonalidad morada 62
		0.05299f, -0.20348f, 0.0f, 0.8f, 0.4f, 0.0f, //vertice n1 tonalidad rojo.rosa 63
		0.05299f, -0.20348f, 0.0f, 0.3058f, 0.0f, 0.5098f, //vértice n1 tonalidad morada 64
		0.06761f, -0.26161f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice 01 tonalidad rosa claro 65
		0.06761f, -0.26773f, 0.0f, 0.3058f, 0.0f, 0.5098f, //vértice 01 tonalidad morada 66
		0.09319f, -0.32893f, 0.0f, 0.2f, 0.0f, 0.0f, //vértice p1 rojo muy oscuro 67
		0.09319f, -0.32893f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice p1 naranja 68
		0.01645f, -0.37177f, 0.0f, 0.6f, 0.2f, 0.1f, //vértice q1 marrón 69
		0.25033f,0.42684f, 0.0f, 0.8f, 0.4f, 0.0f, //vértice r1 naranja 70
	}; 


	unsigned int indices[] =
	{
		0,1,3,5, //naranja
		0,5,8,6, //naranja
		6,28,10,8, //naranja
		4,2,9,7, // rosa
		10,3,11, //naranja
		11,16,18, //naranja
		10,11,22,26,
		22,24,18,11, //ok
		10,28,29,31,26, //
		29,28,46,
		29,46,47,48,
		48,50,52,47,
		30,49,51,32, //verde ojo izquierdo
		12,13,14,16,
		21,19,17,15, //rosa
		70,13,14,20,
		70,37,18,20,
		18,24,33,35,37,
		35,37,38,
		35,38,45,39,
		39,41,43,45,
		36,34,42,40,//ojo verde derecho
		22,26,31,33,24,
		31,50,41,33,
		50,52,55,
	//  h1,i1,j1
		52,55,60,61,
		55,58,60,
		61,50,41,63,
		50,52,61,
		41,63,43,
		41,43,68,
		43,68,65,63,
		68,65,58,
		66,64,62,59,56,
		54,69,67,57
	};

	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);
	glGenBuffers(2, EBO);



	glBindVertexArray(VAO[0]);
	glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// position attribute				cuantas lecturas necesito para volver a leer otro dato de posición		Empieza a leer desde la posición 0
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// color attribute					cuantas lecturas necesito para volver a leer otro dato de posición		Empieza a leer desde la posición 3
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float))); //Manda la información del arreglo al shader de vértices
	glEnableVertexAttribArray(1);

	//Para trabajar con indices (Element Buffer Object)
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);

}

void setupShaders()
{
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &myVertexShader, NULL);
	glCompileShader(vertexShader);

	unsigned int vertexShaderColor = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderColor, 1, &myVertexShaderColor, NULL);
	glCompileShader(vertexShaderColor);

	unsigned int fragmentShaderYellow = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderYellow, 1, &myFragmentShaderYellow, NULL);
	glCompileShader(fragmentShaderYellow);

	unsigned int fragmentShaderColor = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderColor, 1, &myFragmentShaderColor, NULL);
	glCompileShader(fragmentShaderColor);


	//Crear el Programa que combina Geometría con Color
	shaderProgramYellow = glCreateProgram();
	glAttachShader(shaderProgramYellow, vertexShader);
	glAttachShader(shaderProgramYellow, fragmentShaderYellow);
	glLinkProgram(shaderProgramYellow);

	shaderProgramColor = glCreateProgram();
	glAttachShader(shaderProgramColor, vertexShaderColor);
	glAttachShader(shaderProgramColor, fragmentShaderColor);
	glLinkProgram(shaderProgramColor);
	//Check for errors 

	//ya con el Programa, el Shader no es necesario
	glDeleteShader(vertexShader);
	glDeleteShader(vertexShaderColor);
	glDeleteShader(fragmentShaderYellow);
	glDeleteShader(fragmentShaderColor);

}

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
	monitors = glfwGetPrimaryMonitor();
	getResolution();

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 2",NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
	glfwSetWindowPos(window, 0, 30);
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, resize);

	glewInit();


	//My Functions
	//Setup Data to use
	myData();
	//To Setup Shaders
	setupShaders();
    
    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

		//Display Section
		glUseProgram(shaderProgramColor);

		glBindVertexArray(VAO[0]);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO[0]);

		glPointSize(10.0);
		//glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLE_FAN, 11, GL_UNSIGNED_INT, 0);
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(12 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(16 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(19 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(22 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(26 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void*)(30 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(35 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 8, GL_UNSIGNED_INT, (void*)(38 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(46 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(50 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(54 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(58 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(62 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void*)(66 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(71 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(74 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(78 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(82 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void*)(86 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(91 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(95 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(98 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(102 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(105 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(109 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(112 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(115 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(118 * sizeof(float)));
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, (void*)(122 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 5, GL_UNSIGNED_INT, (void*)(125 * sizeof(float)));
		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(130 * sizeof(float)));
		//glDrawArrays(GL_POINTS, 59, 1);
		//glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (void*)(82 * sizeof(float)));
		//glDrawArrays(GL_POINTS, 22, 1);
		//glDrawArrays(GL_TRIANGLE_FAN, 0, 6);


		glBindVertexArray(0);
		glUseProgram(0);

		//End of Display Section

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void my_input(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)  //GLFW_RELEASE
        glfwSetWindowShouldClose(window, true);  //Close
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}