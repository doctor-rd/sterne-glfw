#include <stdlib.h>
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

int main() {
    if (!glfwInit())
        return 1;
    GLFWwindow* window = glfwCreateWindow(640, 480, "Sterne", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
