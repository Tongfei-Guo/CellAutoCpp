#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "shader.h"
#include <vector>

struct bitcolor
{
	unsigned char R;
	unsigned char G;
	unsigned char B;
	unsigned char alpha;
} ;



class CARender
{
private:
	GLFWwindow* _window;
	unsigned int SCR_WIDTH;
	unsigned int SCR_HEIGHT;
	GLuint square_vao;
	GLuint square_vbo;
	Shader _shader;
	glm::i32vec2 gridsize;
	GLfloat square_vertices[4*4];
	GLuint gridsizeID;
        std::vector<bitcolor> palette;
        std::vector<GLfloat> interpretbitmap(std::vector<int> bitindex);
public:
	CARender(int gridweigh, int gridlength, std::vector<bitcolor> worldpalette);
	~CARender();
	bool Renderworld(std::vector<int> & colormap);
};
