#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
 

struct ShaderProgramSource
{
    char* vertexShader;
    char* fragShader;
};


static struct ShaderProgramSource ParseFile(const char* vertexShaderPath, const char* fragShaderPath)
{
    FILE *vertexShaderFile = fopen(vertexShaderPath, "r");    
    FILE *fragShaderFile = fopen(fragShaderPath, "r");    
    
    if (!vertexShaderFile || !fragShaderFile)
    {
        printf("Error while opening shader files!");
        if (vertexShaderFile) fclose(vertexShaderFile);
        if (fragShaderFile) fclose(fragShaderFile);
        return (struct ShaderProgramSource){NULL, NULL};
    }
    
    char buffer[256];
    char *vertexShader = (char*)malloc(1);
    char *fragShader = (char*)malloc(1);
    vertexShader[0] = '\0';
    fragShader[0] = '\0';
    
    // check if malloc was successful
    if (!vertexShader || !fragShader)
    {
        printf("Error: Memory allocation failed.\n");
        if (vertexShader) free(vertexShader);
        if (fragShader) free(fragShader);
        fclose(vertexShaderFile);
        fclose(fragShaderFile);
        return (struct ShaderProgramSource){NULL, NULL};
    }

    while (fgets(buffer, sizeof(buffer), vertexShaderFile))
    {
        size_t shaderLen = strlen(vertexShader);
        size_t bufferLen = strlen(buffer);
        
        // allocate new memory for vertexShader on the heap
        vertexShader = (char*)realloc(vertexShader, shaderLen + bufferLen + 1);
        if (vertexShader == NULL)
        {
            printf("Error while reallocating memory for vertex shader!");
            free(fragShader);
            fclose(vertexShaderFile);
            fclose(fragShaderFile);
            return (struct ShaderProgramSource){NULL, NULL};
        }
        strcat(vertexShader, buffer);
    }             

    while (fgets(buffer, sizeof(buffer), fragShaderFile))
    {
        size_t shaderLen = strlen(fragShader);
        size_t bufferLen = strlen(buffer);
        
        // allocate new memory for fragShader on the heap
        fragShader = (char*)realloc(fragShader, shaderLen + bufferLen + 1);
        if (fragShader == NULL)
        {
            printf("Error while reallocating memory for fragment shader!");
            free(vertexShader);
            fclose(vertexShaderFile);
            fclose(fragShaderFile);
            return (struct ShaderProgramSource){NULL, NULL};
        }
        strcat(fragShader, buffer);
    }             
    fclose(vertexShaderFile);
    fclose(fragShaderFile);
    
    struct ShaderProgramSource result = {vertexShader, fragShader};

    return result;
}


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
       int length;
       glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
       char message[length];
       glGetShaderInfoLog(id, length, NULL, message);

        printf("The compilation for type %s was unsuccessful!\n", 
                    (type == GL_VERTEX_SHADER) ? "vertex shader" : "fragment shader");
        printf("Log message: %s", message);

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
    
    free(vertShaderSource);
    free(fragShaderSource);
    
    return program;
}


int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
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
    

    float vertPos[] = {
        0.5f, 0.0f,   // right 0
        0.0f, 0.5f,   // top 1
        -0.5f, 0.0f,  // left 2
        0.0f, -0.5f,  // bottom 3
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3
    };
    

    // creating an vertex array
    unsigned int vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);
    

    // creates 1 buffer and gives it a tag of "buffer"
    unsigned int buffer;
    glGenBuffers(1, &buffer);
    
    // binds the buffer and gives it a type of array buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffer);

    // give the buffer (the bound buffer) data, takes in the size(in bytes), 
    // pointer to the data (here vertPos array is already a pointer) and the method to draw
    glBufferData(GL_ARRAY_BUFFER, 4 * 2 * sizeof(float), &vertPos, GL_STATIC_DRAW);
    
    // defines the layout of vertex attribute
    // index - location of attrib
    // size - number of component, 2 here (coords) NOT SIZE AS IN BYTES
    // stride - no. of bytes between two vertices 
    // pointer - Offset (in bytes) to the first attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (const void*)0);
    
    // enable vertex attributes
    glEnableVertexAttribArray(0);
    

    // creating an index buffer
    unsigned int ibo;
    glGenBuffers(1, &ibo); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * 3 * sizeof(unsigned int), indices, GL_STATIC_DRAW);


    // FREE: free the components of shaderProgramSource struct
    struct ShaderProgramSource shaderSource = ParseFile("res/vertex.shader", "res/frag.shader");

    unsigned int shaderProgram = CreateShader(shaderSource.vertexShader, shaderSource.fragShader);
    glUseProgram(shaderProgram);

    // GLenum err;
    // while ((err = glGetError()) != GL_NO_ERROR) {
    //     printf("OpenGL error: %d\n", err);
    // }

    const GLubyte* version = glGetString(GL_VERSION);
    if (version) {
        printf("OpenGL Version: %s\n", version);
    } else {
        printf("Failed to retrieve OpenGL version\n");
    }
 
    while (!glfwWindowShouldClose(window)) 
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // draw the buffer on screen
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL); 

        //swap front and back buffers
        glfwSwapBuffers(window);

        //poll events
        glfwPollEvents();
    }
    
    glDeleteProgram(shaderProgram); 
 
    glfwTerminate();
    return 0;
}