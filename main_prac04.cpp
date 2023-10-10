/*---------------------------------------------------------*/
/* ----------------   Práctica 4 --------------------------*/
/*-----------------    2024-1   ---------------------------*/
/*------------- Alumno: Hernández Lara Jesús Eduardo ---------------*/
/*------------- No. de cuenta:316143671---------------*/
#include <glew.h>
#include <glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <shader_m.h>

#include <iostream>

void resize(GLFWwindow* window, int width, int height);
void my_input(GLFWwindow *window);

// settings
// Window size
int SCR_WIDTH = 3800;
int SCR_HEIGHT = 7600;

GLFWmonitor *monitors;
GLuint VBO, VAO, EBO;

void myData(void);
void getResolution(void);

//For Keyboard
float	movX = 0.0f,
		movY = 0.0f,
		movZ = -25.0f;

float rotX = 0.0f,
	  rotY = 0.0f,
	  rotZ = 0.0f;

void getResolution()
{
	const GLFWvidmode * mode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	SCR_WIDTH = mode->width;
	SCR_HEIGHT = (mode->height) - 80;
}

void myData()
{
	GLfloat verticesCubo[] = {
		//Position				//Color
		-0.5f, -0.5f, 0.5f,		//V0 - Frontal
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, 0.5f, 0.5f,		//V5
		-0.5f, -0.5f, 0.5f,		//V0
		-0.5f, 0.5f, 0.5f,		//V4
		0.5f, 0.5f, 0.5f,		//V5

		0.5f, -0.5f, -0.5f,		//V2 - Trasera
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, -0.5f,		//V7

		-0.5f, 0.5f, 0.5f,		//V4 - Izq
		-0.5f, 0.5f, -0.5f,		//V7
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, -0.5f, -0.5f,	//V3
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, -0.5f, 0.5f,		//V0

		0.5f, 0.5f, 0.5f,		//V5 - Der
		0.5f, -0.5f, 0.5f,		//V1
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		0.5f, -0.5f, -0.5f,		//V2

		-0.5f, 0.5f, 0.5f,		//V4 - Sup
		0.5f, 0.5f, 0.5f,		//V5
		0.5f, 0.5f, -0.5f,		//V6
		-0.5f, 0.5f, 0.5f,		//V4
		-0.5f, 0.5f, -0.5f,		//V7
		0.5f, 0.5f, -0.5f,		//V6

		-0.5f, -0.5f, 0.5f,		//V0 - Inf
		-0.5f, -0.5f, -0.5f,	//V3
		0.5f, -0.5f, -0.5f,		//V2
		-0.5f, -0.5f, 0.5f,		//V0
		0.5f, -0.5f, -0.5f,		//V2
		0.5f, -0.5f, 0.5f,		//V1
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(1, &VBO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(verticesCubo), verticesCubo, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
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

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Practica 4 2024-1", NULL, NULL);
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


	//Mis funciones
	//Datos a utilizar
	myData();
	glEnable(GL_DEPTH_TEST);

	Shader myShader("shaders/shader_projection.vs", "shaders/shader_projection.fs");
	myShader.use();

	glm::mat4 viewOp = glm::mat4(1.0f);			//Use this matrix for ALL models
	glm::mat4 projectionOp = glm::mat4(1.0f);	//This matrix is for Projection

	//Use "projection" in order to change how we see the information
	projectionOp = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
	glm::mat4 Temp01 = glm::mat4(1.0f);
	glm::mat4 TempCad = glm::mat4(1.0f);
	glm::mat4 TempPI = glm::mat4(1.0f);
	glm::mat4 TempPD = glm::mat4(1.0f);
	glm::mat4 TempBD = glm::mat4(1.0f);
	glm::mat4 TempBI = glm::mat4(1.0f);
	glm::mat4 TempABD = glm::mat4(1.0f);
	glm::mat4 TempM = glm::mat4(1.0f);
	glm::mat4 TempBE = glm::mat4(1.0f);
	glm::mat4 TempCP = glm::mat4(1.0f);
    glm::mat4 TempCuP = glm::mat4(1.0f); 
    // render loop
    // While the windows is not closed
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        my_input(window);

        // render
        // Background color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//Mi bloque de dibujo
		/*******************************************/
		glm::mat4 modelOp = glm::mat4(1.0f);		// initialize Matrix, Use this matrix for individual models
		//Use "view" in order to affect all models
		viewOp = glm::translate(glm::mat4(1.0f), glm::vec3(movX, movY, movZ));
		viewOp = glm::rotate(viewOp, glm::radians(rotX), glm::vec3(1.0f, 0.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(rotY), glm::vec3(0.0f, 1.0f, 0.0f));
		viewOp = glm::rotate(viewOp, glm::radians(rotZ), glm::vec3(0.0f, 0.0f, 1.0f));
		// pass them to the shaders
		myShader.setMat4("model", modelOp);
		myShader.setMat4("view", viewOp);
		myShader.setMat4("projection", projectionOp);

		//Model
		glBindVertexArray(VAO);
		modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 5.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //pecho


		Temp01 = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 2.75f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 1.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //cuello

		//modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 4.25f, 0.0f));
		//modelOp = glm::scale(modelOp, glm::vec3(1.5f, 2.5f, 1.0f));
		modelOp = glm::translate(Temp01, glm::vec3(0.0f, 1.5f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 2.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //cabeza
		
		TempCad = modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -3.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.0f, 1.0f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //cadera

		TempPI=modelOp = glm::translate(TempCad, glm::vec3(-1.25f, -2.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.5019f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //pierna izquierda
		
		TempPD=modelOp = glm::translate(TempCad, glm::vec3(1.25f, -2.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.5019f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //pierna derecha

		modelOp = glm::translate(TempPI, glm::vec3(-0.5f, -2.125f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.75f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.5019f, 0.5019f, 0.8f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //pie izquierdo

		modelOp = glm::translate(TempPD, glm::vec3(0.5f, -2.125f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.5f, 0.75f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.5019f, 0.5019f, 0.8f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //pie derecho
		
		TempBD=modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(2.75f, 2.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.2f, 0.4f, 0.6f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //brazo derecho

		TempBI=modelOp = glm::translate(glm::mat4(1.0f), glm::vec3(-2.75f, 2.25f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.5f, 0.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.2f, 0.4f, 0.6f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //brazo izquierdo

		TempABD = modelOp=glm::translate(TempBD, glm::vec3(0.25f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.8f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //brazo derecho

		modelOp = glm::translate(TempBI, glm::vec3(-0.25f, -2.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 3.5f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.8f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //brazo derecho
		
		TempM=modelOp = glm::translate(TempABD, glm::vec3(0.0f, -1.9f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 0.3f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.1f, 0.2f, 0.3f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //mango espada

		TempBE=modelOp = glm::translate(TempM, glm::vec3(0.65f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(0.3f, 0.6f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.4f, 0.5f, 0.6f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //mango espada

		modelOp = glm::translate(TempBE, glm::vec3(1.85f, 0.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(3.5f, 0.3f, 1.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.7f, 0.8f, 0.9f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //mango espada

		TempCP = modelOp = glm::translate(glm::mat4(1.0), glm::vec3(13.0f, 0.0f, 0.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.35f, 8.45f, 6.05f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Cabeza Patito

		TempCuP=modelOp = glm::translate(TempCP, glm::vec3(0.81f, -6.5f, -1.1f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(4.35f, 4.95f, 8.7f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Cuerpo Patito

		modelOp = glm::translate(TempCuP, glm::vec3(3.0f, -5.5f, -1.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.9f, 5.9f, 1.9f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.5019f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pata derecha patito

		modelOp = glm::translate(TempCuP, glm::vec3(-2.0f, -3.0f, -1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.9f, 3.0f, 1.9f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.5019f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pata derecha patito

		modelOp = glm::translate(TempCuP, glm::vec3(-3.0f, -4.0f, -1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.5f, 2.0f, 7.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.5019f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pata derecha patito

		modelOp = glm::translate(TempCuP, glm::vec3(3.0f, -8.0f, -1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(5.5f, 2.0f, 7.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.5019f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pata derecha patito

		modelOp = glm::translate(TempCuP, glm::vec3(1.81f, 1.0f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.8f, 3.2f, 5.55f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pata derecha patito

		modelOp = glm::translate(TempCuP, glm::vec3(1.81f, 1.0f, 1.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.8f, 3.2f, 5.55f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 1.0f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pata derecha patito

		modelOp = glm::translate(TempCP, glm::vec3(-1.0f, 1.0f, 3.0f));
		modelOp = glm::scale(modelOp, glm::vec3(1.0f, 1.0f, 1.0f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(0.5019f, 0.2519f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pata derecha patito

		modelOp = glm::translate(TempCP, glm::vec3(-3.5f, 1.0f, 3.0f));
		modelOp = glm::scale(modelOp, glm::vec3(2.6f, 1.15f, 3.35f));
		modelOp = glm::rotate(modelOp, glm::radians(-60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		myShader.setMat4("model", modelOp);
		myShader.setVec3("aColor", glm::vec3(1.0f, 0.5019f, 0.0f));
		glDrawArrays(GL_TRIANGLES, 0, 36); //Pata derecha patito





		
		glBindVertexArray(0);
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
        glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		movX += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		movX -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_UP) == GLFW_PRESS)
		movY += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_PAGE_DOWN) == GLFW_PRESS)
		movY -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		movZ -= 0.08f;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		movZ += 0.08f;
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
		rotX -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
		rotX += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
		rotY -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
		rotY += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_HOME) == GLFW_PRESS)
		rotZ -= 2.0f;
	if (glfwGetKey(window, GLFW_KEY_END) == GLFW_PRESS)
		rotZ += 2.0f;
	if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
		rotY += 2.0f;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void resize(GLFWwindow* window, int width, int height)
{
    // Set the Viewport to the size of the created window
    glViewport(0, 0, width, height);
}