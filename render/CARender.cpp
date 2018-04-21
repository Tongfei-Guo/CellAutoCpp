#include "CArender.hpp"
#include <glm/glm.hpp>

void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}
// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

std::vector<GLfloat> CARender::interpretbitmap(std::vector<int> bitindex)
{
    int elnum = bitindex.size();
    std::vector<GLfloat>bitmap(elnum*4);
    for(int i = 0; i < elnum; i++)
    {
        int colorind = bitindex[i];
        bitcolor colorinrgb = palette[colorind];
        bitmap[i*4] = (GLfloat)(colorinrgb.R/255.0);
        bitmap[i*4 + 1] = (GLfloat)(colorinrgb.G/255.0);
        bitmap[i*4 + 2] = (GLfloat)(colorinrgb.B/255.0);
        bitmap[i*4 + 3] = (GLfloat)(colorinrgb.alpha/255.0);
    }

    return std::move(bitmap);
}

CARender::CARender(int gridweigh, int gridlength,std::vector<bitcolor>  worldpalette)
	:gridsize(gridweigh,gridlength),SCR_WIDTH(gridweigh * 10),SCR_HEIGHT(gridlength * 10),palette(std::move(worldpalette))
{
	static const GLfloat vertices[] =
    {
        -0.5f, -0.5f, 0.0f, 1.0f,
         0.5f, -0.5f, 0.0f, 1.0f,
         0.5f,  0.5f, 0.0f, 1.0f,
        -0.5f,  0.5f, 0.0f, 1.0f
    };

	for(int i = 0; i < 4 * 4; i++)
	{
		square_vertices[i] = vertices[i];
	}



	glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    // Create a GLFWwindow object that we can use for GLFW's functions
	_window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(_window);




    glfwSetInputMode(_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED );//GLFW_CURSOR_DISABLED);
    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return ;
	}    

	_shader.loadshader("instancing.vs","instancing.fs");


	glGenVertexArrays(1, &square_vao);
    glGenBuffers(1, &square_vbo);

    glBindVertexArray(square_vao);
    glBindBuffer(GL_ARRAY_BUFFER, square_vbo);

	glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices) + sizeof(GLfloat)*gridsize[0]*gridsize[1]*4, NULL, GL_STREAM_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(square_vertices), square_vertices);

	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 0, 0);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (GLvoid *)sizeof(square_vertices));
    
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    
	glVertexAttribDivisor(1, 1);
       
	gridsizeID = glGetUniformLocation(_shader.Program, "gridsize");

}

CARender::~CARender()
{
	glDeleteVertexArrays(1, &square_vao);
    glDeleteBuffers(1, &square_vbo);
	glfwTerminate();
}



bool CARender::Renderworld(std::vector<int> & bitindex)
{
        auto colormap = interpretbitmap(bitindex);

	if(!glfwWindowShouldClose(_window))
	{
		processInput(_window);

		glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		glBindVertexArray(square_vao);
		glBindBuffer(GL_ARRAY_BUFFER, square_vbo);

		//glBufferData(GL_ARRAY_BUFFER, sizeof(square_vertices) + sizeof(GLfloat)*colormap.size(), NULL, GL_STREAM_DRAW);
		int offset = sizeof(square_vertices);
		glBufferSubData(GL_ARRAY_BUFFER, offset, sizeof(GLfloat)*colormap.size(), colormap.data());
	

		_shader.Use();
		glUniform2iv(gridsizeID, 1, &gridsize[0]);
		glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 4, gridsize[0]*gridsize[1]);
                   
                //glFlush();
		glfwSwapBuffers(_window);
		glfwPollEvents();

	}

	return !glfwWindowShouldClose(_window);

}
