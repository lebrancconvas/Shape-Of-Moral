#include <iostream>
#include <stdio.h>
#include <string.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

const GLint WIDTH = 800, HEIGHT = 600;
const float toRadian = 3.14159265f / 180.0f;
GLuint VAO, VBO, shader, uniformModel;

//vShader
static const char* vShader =
"                                                               \n\
#version 330                                                    \n\
                                                                \n\
layout (location = 0) in vec3 pos;                              \n\
                                                                \n\
uniform mat4 model;                                             \n\
                                                                \n\
void main() {                                                   \n\
    gl_Position = model * vec4(pos, 1.0);                       \n\
}                                                               \n\
";

//fShader
static const char* fShader =
"                                                               \n\
                                                                \n\
#version 330                                                    \n\
out vec4 colour;                                                \n\
                                                                \n\
void main()                                                     \n\
{                                                               \n\
    colour = vec4(1.0, 0.0, 0.0, 1.0);                          \n\
}                                                               \n\
";

void CreateTriangle() {
    GLfloat vertices[] =
    {
        -1.0f, -1.0f, 0.0f,
        1.0f, -1.0f, 0.0f,
        0.0f, 1.0f, 0.0f
    };
    
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void AddShader(GLuint theProgram, const char* shaderCode, GLenum shaderType) {
    GLuint theShader = glCreateShader(shaderType);
    const GLchar* theCode[1];
    theCode[0] = shaderCode;
    
    GLint codeLength[1];
    codeLength[0] = strlen(shaderCode);
    
    glShaderSource(theShader, 1, theCode, codeLength);
    glCompileShader(theShader);
    
    GLint result = 0;
    GLchar elog[1024] = { 0 };
    
    glGetShaderiv(theShader, GL_COMPILE_STATUS, &result);
    if(!result) {
        glGetShaderInfoLog(theShader, sizeof(elog), NULL, elog);
        printf("Error compiler the %d shader: '%s'\n", shaderType, elog);
        return;
    }
    glAttachShader(theProgram, theShader);
}

void CompileShaders() {
    shader = glCreateProgram();
    
    if(!shader) {
        printf("Error Creating Shader Program!\n");
        return;
    }
    
    AddShader(shader, vShader, GL_VERTEX_SHADER);
    AddShader(shader, fShader, GL_FRAGMENT_SHADER);
    
    GLint result = 0;
    GLchar elog[1024] = { 0 };
    
    glLinkProgram(shader);
    glGetProgramiv(shader, GL_LINK_STATUS, &result);
    
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error linking program: '%s'\n", elog);
        return;
    }
    
    glValidateProgram(shader);
    glGetProgramiv(shader, GL_VALIDATE_STATUS, &result);
    
    if(!result) {
        glGetProgramInfoLog(shader, sizeof(elog), NULL, elog);
        printf("Error validating program : %s\n", elog);
        return;
    }
    
    uniformModel = glGetUniformLocation(shader, "model");
}

int main() {
    if(!glfwInit()) {
        printf("GLFW Installation Failed");
        glfwTerminate();
        return -1;
    }
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* mainWindow = glfwCreateWindow(WIDTH, HEIGHT, "CGLab05 - Poom Yimyuean 62050210", NULL, NULL);

    if(!mainWindow) {
        glfwTerminate();
        return -1;
    }
    
    int bufferWidth; int bufferHeight;
    glfwGetFramebufferSize(mainWindow, &bufferWidth, &bufferHeight);

    glfwMakeContextCurrent(mainWindow);
    
    glewExperimental = GL_TRUE;
    
    if(glewInit() != GLEW_OK) {
        printf("GLEW Installation Failed.");
        glfwDestroyWindow(mainWindow);
        glfwTerminate();
        return 1;
    }
    
    glViewport(0, 0, bufferWidth, bufferHeight);
    
    CreateTriangle();
    CompileShaders();
    
    while(!glfwWindowShouldClose(mainWindow)) {
        glfwPollEvents();
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        glUseProgram(shader);
        
        mat4 model (1.0f);
        model = scale(model, vec3(0.4f, 0.4f, 1.0f));
        
        glUniformMatrix4fv(uniformModel, 1, GL_FALSE, value_ptr(model));
        
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);
        glBindVertexArray(0);
        glUseProgram(0);
        glfwSwapBuffers(mainWindow);
    }
    return 0;
}
