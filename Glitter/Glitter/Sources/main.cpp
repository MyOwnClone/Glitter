// Local Headers
#include "glitter.hpp"
#include "shader.h"
#include <iostream>

// System Headers
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true,
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto window = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(window);
    //gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    glfwSetKeyCallback(window, key_callback);
    
    Shader shaderProgram("shader.vs", "shader.frag");
    
    GLuint VAO0;
    glGenVertexArrays(1, &VAO0);
    
    GLuint VBO0;
    glGenBuffers(1, &VBO0);
    
    GLuint EBO0;
    glGenBuffers(1, &EBO0);
    
    GLfloat vertices0[] = {
        -0.25, -0.25, 0.0,    1, 0, 0,
         0.00,  0.50, 0.0,    0, 1, 0,
         0.25, -0.25, 0.0,    0, 0, 1
    };
    
    GLuint indices0[] = {  // Note that we start from 0!
        0, 1, 2,   // First Triangle
    };
    
    glBindVertexArray(VAO0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO0);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices0), indices0, GL_STATIC_DRAW);
    
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Rendering Loop
    while (glfwWindowShouldClose(window) == false)
    {
        glfwPollEvents();
        
        // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUniform1f(glGetUniformLocation(shaderProgram.Program, "offset"), 0.5);
        shaderProgram.Use();
        
        glBindVertexArray(VAO0);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}
