#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform vec4 colour;
void main() {
    FragColor = colour; // Set color
}
)";

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    int height = 600;
    int width = 800;
    GLFWwindow* window = glfwCreateWindow(width, height, "Demo", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load all OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the viewport size and callback
    glViewport(0, 0, width, height);

    // Build and compile the shader program
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Vertex data for point, line, and triangle
    float vertices[] = {
        // Point
        -0.7f,  0.0f, 0.0f, 

        // Line
        -0.3f, -0.5f, 0.0f,  
        -0.3f,  0.5f, 0.0f,

        // Triangle
         0.3f, -0.5f, 0.0f,  
         0.7f, -0.5f, 0.0f,  
         0.5f,  0.5f, 0.0f 
    };

    // Index data for point, line, and triangle
    unsigned int pointIndices[] = { 0 };
    unsigned int lineIndices[] = { 1, 2 };
    unsigned int triangleIndices[] = { 3, 4, 5 };

    unsigned int VAO, VBO; // Common VBO different EBO for objects
    unsigned int EBO_point, EBO_line, EBO_triangle;

    // Generate and bind VAO
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO_point);
    glGenBuffers(1, &EBO_line);
    glGenBuffers(1, &EBO_triangle);

    glBindVertexArray(VAO);

    // Vertex Buffer Object
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // Point EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_point);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(pointIndices), pointIndices, GL_STATIC_DRAW);

    // Line EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_line);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_STATIC_DRAW);

    // Triangle EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_triangle);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(triangleIndices), triangleIndices, GL_STATIC_DRAW);

    // Vertex Attribute Pointer
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {

        // Clear colour of screen, set a background colour
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Bind the Shader and set Uniforms
        glUseProgram(shaderProgram);
        std::string colourName = "colour";
        int colourLoc = glGetUniformLocation(shaderProgram, colourName.c_str());
        glUniform4f(colourLoc, 1.0f, 0.0f, 0.0f, 1.0f);

        // Bind the VAO (automatically binds the assosiated VBO)
        glBindVertexArray(VAO);

        // Draw the point by binding its EBO and issuing draw call
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_point);
        glPointSize(10.0f); // Make the point larger for visibility
        glDrawElements(GL_POINTS, 1, GL_UNSIGNED_INT, 0);

        // Draw the line by binding its EBO and issuing draw call
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_line);
        glDrawElements(GL_LINES, 2, GL_UNSIGNED_INT, 0);

        // Draw the triangle by binding its EBO and issuing draw call
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO_triangle);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO_point);
    glDeleteBuffers(1, &EBO_line);
    glDeleteBuffers(1, &EBO_triangle);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
