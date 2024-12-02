#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <stdio.h>
 
static unsigned int CompileShader(unsigned int type, char* source)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    
    //TODO: error handling

    return id;
}

static unsigned int CreateShader(char* vertShaderSource, char* fragShaderSource)
{
    unsigned int program = glCreateProgram();
    
    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    
    glLinkProgram(program);
    glValidateProgram(program);
    
    glDeleteShader(vs);
    glDeleteShader(fs);
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