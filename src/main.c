#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"


// TODO: Free layout (MEM LEAK!)
// TODO: Cleanup all buffers



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
    glfwSwapInterval(1);
 
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        printf("Failed to initialize GLAD\n");
        return -1;
    }
    

    float vertPos[] = {
          0.5f, 0.0f,   // right 0
         0.25f, 0.3f,   // right top 1
          0.0f, 0.0f,   // mid 2
        -0.25f, 0.3f,   // left top 3
         -0.5f, 0.0f,   // left 4
          0.0f, -0.8f   // bottom 5
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 4,
        0, 4, 5
    };
    
    VertexArray va = CreateVertexArray();
    VertexBuffer vb = CreateVertexBuffer(&vertPos, 6 * 2 * sizeof(float));

    VertexBufferLayout layout = CreateLayout(); 
    layout.Pushf(&layout, 2);
    
    va.AddBuffer(&va, &vb, &layout);

    IndexBuffer ib = CreateIndexBuffer(indices, 3 * 3);
    


    // FREE: free the components of shaderProgramSource struct
    struct ShaderProgramSource shaderSource = ParseFile("res/vertex.shader", "res/frag.shader");

    unsigned int shaderProgram = CreateShader(shaderSource.vertexShader, shaderSource.fragShader);
    glUseProgram(shaderProgram);

    // uniforms
    int uniformLocation = glGetUniformLocation(shaderProgram, "u_color");
    glUniform4f(uniformLocation, 1.0f, 0.5f, 0.5f, 1.0f);
    
//*--------------------------------------------------------------------------------------------- 
    
    float red = 0.00f;
    float increment = 0.03f;

//*--------------------------------------------------------------------------------------------- 

    while (!glfwWindowShouldClose(window)) 
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        

        // color test
        glUniform4f(uniformLocation, red, 0.2f, 0.4f, 1.0f);
        red += increment;
        if (red >= 1.0f) increment = -increment;
        else if (red <= 0.0f) increment = -increment;


        // draw the buffer on screen
        GLCall(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, NULL));

        //swap front and back buffers
        glfwSwapBuffers(window);

        //poll events
        glfwPollEvents();
    }
    
    glDeleteProgram(shaderProgram); 

 
    glfwTerminate();
    return 0;
}