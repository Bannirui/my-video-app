#include <GLFW/glfw3.h>

#include "Log.h"

int main(int argc, const char *argv[]) {
    GLFWwindow *window;
    if (!glfwInit()) {
        MY_LOG_ERROR("Failed to initialize GLFW");
        return 1;
    }
    window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
    if (!window) {
        MY_LOG_ERROR("could not create GLFW window");
        return 1;
    }
    MY_LOG_INFO("HELLO WORLD!");
    return 0;
}
