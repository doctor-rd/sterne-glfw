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

void moveStars(Coord* coord, int n, GLfloat m, GLfloat d) {
    for (int i=0; i<n; i++) {
        coord[i].z+=d;
        while (coord[i].z>m)
            coord[i].z-=m;
        while (coord[i].z<0.0)
            coord[i].z+=m;
    }
}

const char* vertex_shader =
"#version 400\n"
"in vec3 coord;"
"void main() {"
"  gl_Position = vec4(coord.x, coord.y, -0.1*coord.z, 2.0*coord.z-1.0);"
"}";

const char* geometry_shader =
"#version 400\n"
"layout(points) in;"
"layout(line_strip, max_vertices = 2) out;"
"out float vz;"
"uniform float speed;"
"void main() {"
"  gl_Position = gl_in[0].gl_Position;"
"  vz = gl_Position.z;"
"  EmitVertex();"
"  gl_Position = gl_in[0].gl_Position + vec4(0.0, 0.0, 0.1*speed, -2.0*speed);"
"  vz = gl_Position.z;"
"  EmitVertex();"
"  EndPrimitive();"
"}";

const char* fragment_shader =
"#version 400\n"
"in float vz;"
"out vec4 frag_colour;"
"void main() {"
"  float vBright = 1.2-(-10.0*vz)/4.5;"
"  frag_colour = vec4(vBright, vBright, vBright, 1.0);"
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
    GLuint gs = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(gs, 1, &geometry_shader, NULL);
    glCompileShader(gs);
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fragment_shader, NULL);
    glCompileShader(fs);
    GLuint shader_programme = glCreateProgram();
    glAttachShader(shader_programme, fs);
    glAttachShader(shader_programme, vs);
    glAttachShader(shader_programme, gs);
    glLinkProgram(shader_programme);

    const int n_stars = 5000;
    const GLfloat d = 10.0;
    Coord *stars = createStars(n_stars, 4.0, d);
    GLfloat speed = -0.1;

    double then = 0.0;
    while (!glfwWindowShouldClose(window)) {
        double now = glfwGetTime();
        double deltaTime = now - then;
        then = now;
        GLuint vbo = 0;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, n_stars*sizeof(Coord), stars, GL_STATIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);
        glUseProgram(shader_programme);
        GLint loc_speed = glGetUniformLocation(shader_programme, "speed");
        glUniform1f(loc_speed, speed);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);
        glDrawArrays(GL_POINTS, 0, n_stars);
        glDisableVertexAttribArray(0);
        glfwSwapBuffers(window);
        moveStars(stars,n_stars, d, speed*deltaTime/0.2);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
