#include <glad/glad.h> 
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "include/stb_image.h"
#include <iostream>
#include <string>
#include <fstream>

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

class Shader
{
private:
    unsigned int ID;

    bool checkIfCompiled(GLuint shader, const std::string &type)
    {
        GLint success;
        GLchar infoLog[1024];
        if(type != "PROGRAM")
        {
            glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
            if(!success)
            {
                glGetShaderInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << std::endl;
                return false;
            }
        }
        else
        {
            glGetProgramiv(shader, GL_LINK_STATUS, &success);
            if(!success)
            {
                glGetProgramInfoLog(shader, 1024, NULL, infoLog);
                std::cerr << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << std::endl;
                return false;
            }
        }
        return true;
    }

public:
    Shader(const std::string &vertexShaderFile, const std::string &fragmentShaderFile)
    {
        std::fstream inFile;
        std::string sourceCode;

        inFile.open(vertexShaderFile);
        if(!inFile.is_open())
        {
            std::cerr << "Unable to open " << vertexShaderFile << std::endl;
            return;
        }
        sourceCode = std::string(
            std::istreambuf_iterator<char>(inFile),
            std::istreambuf_iterator<char>()
        );
        inFile.close();

        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        const char *vertexShaderCode = sourceCode.c_str();
        glShaderSource(vertexShader, 1, &vertexShaderCode, NULL);
        glCompileShader(vertexShader);
        if(!checkIfCompiled(vertexShader, "VERTEX"))
        {
            return;
        }

        inFile.open(fragmentShaderFile);
        if(!inFile.is_open())
        {
            std::cerr << "Unable to open " << fragmentShaderFile << std::endl;
            return;
        }
        sourceCode = std::string(
            std::istreambuf_iterator<char>(inFile),
            std::istreambuf_iterator<char>()
        );

        unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        const char *fragmentShaderCode = sourceCode.c_str();
        glShaderSource(fragmentShader, 1, &fragmentShaderCode, NULL);
        glCompileShader(fragmentShader);
        if(!checkIfCompiled(fragmentShader, "FRAGMENT"))
        {
            return;
        }

        this->ID = glCreateProgram();
        glAttachShader(this->ID, fragmentShader);
        glAttachShader(this->ID, vertexShader);
        glLinkProgram(this->ID);
        checkIfCompiled(this->ID, "PROGRAM");

        glDeleteShader(fragmentShader);
        glDeleteShader(vertexShader);
    }

    void use()
    {
        glUseProgram(this->ID);
    }

    unsigned int getID()
    {
        return this->ID;
    }
};

unsigned int loadTexture(const std::string &file)
{
    unsigned int texture;
    int width, height, nChannels;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    unsigned char *data = stbi_load(file.c_str(), &width, &height, &nChannels, 0);
    if(!data)
    {
        std::cerr << "Unable to load texture from " << file << std::endl;
    }
    else
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    stbi_image_free(data);
    return texture;
}

void initGL(void)
{
    glViewport(0, 0,  WINDOW_WIDTH, WINDOW_HEIGHT);
    glEnable(GL_DEPTH_TEST);
}

void onResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    initGL();
    glfwSetFramebufferSizeCallback(window, onResize);

    Shader shader("cube_hitman_ver.glsl", "cube_hitman_frag.glsl");

    float vertices[] = {
        -0.5f, -0.5f, 0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, 0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, 0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, 0.5f,  0.0f, 1.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };  

    unsigned int indices[] = {
        0, 1, 3,
        2, 3, 1,
        2, 1, 5, 
        6, 2, 5, 
        6, 5, 4, 
        7, 4, 6,
        7, 3, 4,
        0, 3, 4,
        7, 3, 2,
        6, 7, 2,
        0, 4, 1,
        4, 5, 1
    };

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(float)));

    unsigned int texture = loadTexture("resources/wall2.png");

    while(!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        shader.use();

        glm::mat4 transform(1.0);        
        //transform = glm::translate(transform, glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::rotate(transform, (float)glfwGetTime(), glm::vec3(1.0f, 0.5f, 0.25f));
        unsigned int transformLoc = glGetUniformLocation(shader.getID(), "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));
        // delete this line to fill the polygons
        // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (const void*)(0 * sizeof(unsigned int)));

        glfwSwapBuffers(window);
        glfwPollEvents();    
    }

    return 0;
};