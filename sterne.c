#include <stdlib.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

typedef struct {
  GLfloat x;
  GLfloat y;
  GLfloat z;
} Coord;

GLfloat rnd() {
    return (GLfloat)rand() / (GLfloat)RAND_MAX;
}

Coord* createStars(int n, GLfloat r, GLfloat m) {
    Coord* result = malloc(n*sizeof(Coord));
    for (int i=0; i<n; i++)
        result[i] = (Coord){r*(2.0*rnd()-1.0), r*(2.0*rnd()-1.0), m*rnd()};
    return result;
}

const char* vertex_shader =
"#version 400\n"
"in vec3 coord;"
"void main() {"
"  gl_Position = vec4(coord.x, coord.y, -0.1*coord.z, 2.0*coord.z-1.0);"
"}";

int main() {
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(640, 480, "Sterne", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    glewInit();

    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vertex_shader, NULL);
    glCompileShader(vs);
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, vs);
    glLinkProgram(shader_programme);

    const int n_stars = 5000;
    Coord *stars = createStars(n_stars, 4.0, 10.0);

    while (!glfwWindowShouldClose(window)) {
        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, n_stars*sizeof(Coord), stars, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glUseProgram(shader_programme);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDrawArrays(GL_POINTS, 0, n_stars);
        glDisableVertexAttribArray(0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
