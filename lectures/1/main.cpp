#include <iostream>
#include <fstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


int xMove;

std::string readFile(std::string& source)
{
    std::ifstream stream(source);

    std::string content((std::istreambuf_iterator<char>(stream)),
        (std::istreambuf_iterator<char>()));

    return content;
}

unsigned int compileShader(unsigned int type, std::string sourceFile)
{
    unsigned int id;

    id = glCreateShader(type);
    std::string code = readFile(sourceFile);
    const char* src = code.c_str();
    unsigned int count = 1;
    glShaderSource(id, count, &src, NULL);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length*sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " << 
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") 
            << "!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}

unsigned int createShaders(std::string& vertexFile, std::string& fragmentFile)
{
    unsigned int program, vId, fId;
    int result;
    char message[1024] = { 0 };

    program = glCreateProgram();
    vId = compileShader(GL_VERTEX_SHADER, vertexFile);
    fId = compileShader(GL_FRAGMENT_SHADER, fragmentFile);

    glAttachShader(program, vId);
    glAttachShader(program, fId);

    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(program, sizeof(message), NULL, message);
        printf("Error linking program: %s\n", message);
        return -1;
    }
    glValidateProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &result);
    if (!result)
    {
        glGetProgramInfoLog(program, sizeof(message), NULL, message);
        printf("Error validating program: %s\n", message);
        return -1;
    }

    xMove = glGetUniformLocation(program, "xMove");

    glDeleteShader(vId);
    glDeleteShader(fId);

    return program;
}

unsigned int createVertices(unsigned int *VAO, unsigned int *IBO)
{
    float buffer[] = {
        -1.0, -1.0,
        0.0, 1.0,
        1.0, -1.0
    };
    unsigned int indices[] = {
        0, 1, 2
    };
    unsigned int elemCount = 3;
    int bufferSize = 6 * sizeof(float);
    int indicesSize = elemCount * sizeof(unsigned int);

    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, GL_STATIC_DRAW);

    unsigned int attIndex = 0;
    int attComponents = 2;
    unsigned int stride = 2 * sizeof(float);
    int offset = 0;
    glVertexAttribPointer(attIndex, attComponents, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(attIndex);

    glGenBuffers(1, IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return elemCount;
}


unsigned int createVertices2(unsigned int* VAO, unsigned int* IBO)
{
    float buffer[] = {
        -1.0, 0.0,
        -1.0, 1.0,
        0.0, 1.0,
        1.0, 1.0,
        1.0, 0.0,
        1.0, -1.0,
        0.0, -1.0,
        -1.0, -1.0
    };
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 4,
        4, 5, 6,
        6, 7, 0
    };
    unsigned int elemCount = 12;
    int bufferSize = 16 * sizeof(float);
    int indicesSize = elemCount * sizeof(unsigned int);

    glGenVertexArrays(1, VAO);
    glBindVertexArray(*VAO);

    unsigned int VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, bufferSize, buffer, GL_STATIC_DRAW);

    unsigned int attIndex = 0;
    int attComponents = 2;
    unsigned int stride = 2 * sizeof(float);
    int offset = 0;
    glVertexAttribPointer(attIndex, attComponents, GL_FLOAT, GL_FALSE, stride, (void*)offset);
    glEnableVertexAttribArray(attIndex);

    glGenBuffers(1, IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indicesSize, indices, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return elemCount;
}

int main()
{
    GLFWwindow* window;

    /* Initialize the library */
    if (!glfwInit())
        return -1;

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(window);

    if (glewInit() != GLEW_OK)
    {
        glfwTerminate();
        return -1;
    }

    unsigned int VAO, IBO;
    unsigned int elemCount;
    elemCount = createVertices(&VAO, &IBO);
    //elemCount = createVertices2(&VAO, &IBO);

    unsigned int program;
    std::string vShader = "vertex.shader";
    std::string fShader = "fragment.shader";
    program = createShaders(vShader, fShader);

    bool movementEnabled = false;
    float center = 0.0;
    float move = 0.005;

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        /* Render here */
        glClear(GL_COLOR_BUFFER_BIT);

        int firstIndex = 0;

        if (movementEnabled) {
            center += move;
            if (center > 1.0 || center < -1.0)
            {
                move = -move;
            }
            std::cout << center << std::endl;
        }

        glUseProgram(program);

        glUniform1f(xMove, center);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
        //glDrawArrays(GL_TRIANGLES, firstIndex, elemCount);
        glDrawElements(GL_TRIANGLES, elemCount, GL_UNSIGNED_INT, NULL);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        /* Swap front and back buffers */
        glfwSwapBuffers(window);

        /* Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();

	return 0;
}