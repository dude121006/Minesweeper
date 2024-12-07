#include <glad/glad.h>
#include <GLFW/glfw3.h>
 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"


// TODO: Free layout (MEM LEAK!)
// TODO: Cleanup all buffers


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
    
    
    //! FREE: free the components of shaderProgramSource struct
    Shader shader = CreateShader("res/vertex.shader", "res/frag.shader");
    shader.Bind(&shader);
    

    va.Unbind();
    shader.Unbind();
    vb.Unbind();
    ib.Unbind();

    
//*--------------------------------------------------------------------------------------------- 
    
    float red = 0.00f;
    float increment = 0.03f;

//*--------------------------------------------------------------------------------------------- 

    while (!glfwWindowShouldClose(window)) 
    {
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        

        shader.Bind(&shader);
        shader.SetUniform4f(&shader, "u_color", red, 0.2f, 0.4f, 1.0f);
        red += increment;
        if (red >= 1.0f) increment = -increment;
        else if (red <= 0.0f) increment = -increment;

        va.Bind(&va);
        ib.Bind(&ib);

        // draw the buffer on screen
        GLCall(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, NULL));

        //swap front and back buffers
        glfwSwapBuffers(window);

        //poll events
        glfwPollEvents();
    }
    
    shader.DeleteShaderProgram(&shader);

 
    glfwTerminate();
    return 0;
}