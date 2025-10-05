#include "pre_hdr.h"
#include "Log.h"

const char *vertexShaderSrc = R"(
#version 330 core
layout (location = 0) in vec2 pos;
layout (location = 1) in vec2 texCoord;
out vec2 vTexCoord;
void main() {
    vTexCoord = texCoord;
    gl_Position = vec4(pos, 0.0, 1.0);
}
)";

const char *fragmentShaderSrc = R"(
#version 330 core
in vec2 vTexCoord;
out vec4 FragColor;
uniform sampler2D tex;
void main() {
    FragColor = texture(tex, vTexCoord);
}
)";

int main(int argc, const char *argv[]) {
    if (!glfwInit()) {
        MY_LOG_ERROR("Failed to initialize GLFW");
        return 1;
    }
    // 使用Core Profile+OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // init window
    GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World!", NULL, NULL);
    if (!window) {
        MY_LOG_ERROR("Could not create GLFW window");
        glfwTerminate();
        return 1;
    }
    glfwMakeContextCurrent(window);
    // load OpenGL functions
    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        MY_LOG_ERROR("Failed to initialize GLAD");
        return 1;
    }
    // 创建数据
    uint8_t *data = new uint8_t[100 * 100 * 3];
    for (int y = 0; y < 100; y++) {
        for (int x = 0; x < 100; x++) {
            data[(y * 100 + x) * 3 + 0] = 0xff; // R
            data[(y * 100 + x) * 3 + 1] = 0x00; // G
            data[(y * 100 + x) * 3 + 2] = 0xff; // B
        }
    }
    // 创建纹理
    GLuint tex;
    glGenTextures(1, &tex);
    glBindTexture(GL_TEXTURE_2D, tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 100, 100, 0, GL_RGB, GL_UNSIGNED_BYTE, data);

    // 顶点数据 全屏矩形
    // clang-format off
    float vertices[] = {
         // pos                // tex
        -1.0f, -1.0f,  0.0f,  0.0f,
         1.0f, -1.0f,  1.0f,  0.0f,
         1.0f,  1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  0.0f,  1.0f
    };
    // clang-format on
    unsigned int indices[] = {0, 1, 2, 2, 3, 0};

    GLuint vao, vbo, ebo;
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    glGenBuffers(1, &ebo);

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) 0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *) (2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // 编译着色器
    auto compileShader = [](GLenum type, const char *src) {
        GLuint shader = glCreateShader(type);
        glShaderSource(shader, 1, &src, NULL);
        glCompileShader(shader);
        GLint success;
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success) {
            char log[512];
            glGetShaderInfoLog(shader, 512, NULL, log);
            MY_LOG_ERROR("Shader compile error: {}", log);
        }
        return shader;
    };

    GLuint vs = compileShader(GL_VERTEX_SHADER, vertexShaderSrc);
    GLuint fs = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSrc);
    GLuint program = glCreateProgram();
    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    // 渲染循环
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(program);
        glBindTexture(GL_TEXTURE_2D, tex);
        glBindVertexArray(vao);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    delete[] data;
    glfwTerminate();
    return 0;
}
