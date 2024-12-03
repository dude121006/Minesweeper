#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <stdio.h>
 
static unsigned int CompileShader(unsigned int type, char* source)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    
    // querying the success of compilation
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);


    if (result == GL_FALSE)
    {
        printf("The compilation for type %s was unsuccessful!\n", 
                    (type == GL_VERTEX_SHADER) ? "vertex shader" : "fragment shader");

        // delete the shader bcos compilation was unsuccessful
        glDeleteShader(id);
        return 0;
    }

    return id;
}

static unsigned int CreateShader(char* vertShaderSource, char* fragShaderSource)
{
    unsigned int program = glCreateProgram();
    
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertShaderSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShaderSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    glValidateProgram(program);
    
    int validateResult;
    glGetProgramiv(program, GL_VALIDATE_STATUS, &validateResult);
    if (validateResult == GL_FALSE)
    {
        printf("Error in validation!\n");
    }
    
    glDeleteShader(vs);
    glDeleteShader(fs);
    
    return program;
}


int main() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow* window = glfwCreateWindow(800, 600, "this is NOT a window!", NULL, NULL);
    
    if (window == NULL) {
        printf("Failed to create GLFW window\n");
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }




    
    float vertPos[6] = {
        -0.5f, -0.5f,
         0.0f, 0.5f,
         0.5f, -0.5f
    };
    
    // creates 1 buffer and gives it a tag of "buffer"
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    // binds the buffer and gives it a type of array buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // give the buffer (the bound buffer) data, takes in the size(in bytes), 
    // pointer to the data (here vertPos array is already a pointer) and the method to draw
    glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), &vertPos, GL_STATIC_DRAW);


    // defines the layout of vertex attribute
    // index - location of attrib
    // size - number of component, 2 here (coords) NOT SIZE AS IN BYTES
    // stride - no. of bytes between two vertices 
    // pointer - Offset (in bytes) to the first attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);

    // enable vertex attributes
    glEnableVertexAttribArray(0);


    // layout(location = 0) specifies the index is at 0 in attribPointer
    char vertexShader[] = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) in vec4 position;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   gl_Position = position;\n"
        "}\n";
    

    char fragmentShader[] = 
        "#version 330 core\n"
        "\n"
        "layout(location = 0) out vec4 color;\n"
        "\n"
        "void main()\n"
        "{\n"
        "   color = vec4(0.0, 0.0, 1.0f, 1.0f);\n"
        "}\n";


    unsigned int shaderProgram = CreateShader(vertexShader, fragmentShader);
    glUseProgram(shaderProgram);

    // GLenum err;
    // while ((err = glGetError()) != GL_NO_ERROR) {
    //     printf("OpenGL error: %d\n", err);
    // }

 
    while (!glfwWindowShouldClose(window)) {
        
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw the buffer on screen
        glDrawArrays(GL_TRIANGLES, 0, 3); //glDrawArrays bcos we dont have index buffers yet

 
        //swap front and back buffers
        glfwSwapBuffers(window);

        //poll events
        glfwPollEvents();
    }
 
    glfwTerminate();
    return 0;
}