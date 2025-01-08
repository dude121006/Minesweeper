#include <glad/glad.h>
#include <GLFW/glfw3.h>
// #include "cglm/ivec2.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "Shader.h"
#include "Texture.h"


// TODO: error handling in texture file

// TODO: Free layout (MEM LEAK!)
// TODO: Cleanup all buffers

// TODO: Cache Uniform Locations


int main() 
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
 
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL", NULL, NULL);
    
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
    
    GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));
    GLCall(glEnable(GL_BLEND));


    float vertices[] = {
          0.5f, 0.0f,  // right 0
         0.25f, 0.3f,  // right top 1
          0.0f, 0.0f,   // mid 2
        -0.25f, 0.3f,  // left top 3
         -0.5f, 0.0f,  // left 4
          0.0f, -0.8f   // bottom 5
    };
     

    float verticesTexture[] = {
         0.25f, -0.25f,     1.0f, 0.0f,  // bottom right
         0.25f, 0.25f,      1.0f, 1.0f,  // top right  
        -0.25f, 0.25f,      0.0f, 1.0f,  // top left
        -0.25f, -0.25f,     0.0f, 0.0f,  // bottom left
    };
    
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 4,
        0, 4, 5
    };
    
    unsigned int indicesTexture[] = {
            0, 1, 2,
            0, 2, 3
    };

    
    VertexArray va = CreateVertexArray();
    VertexBuffer vb = CreateVertexBuffer(&vertices, 6 * 2 * sizeof(float));
    IndexBuffer ib = CreateIndexBuffer(&indices, 3 * 3);

    VertexBufferLayout layout = CreateLayout(); 
    layout.Pushf(&layout, 2);
    
    va.AddBuffer(&va, &vb, &layout);
    va.Unbind();
    vb.Unbind();
    ib.Unbind();

    // vertex stuff
    VertexArray vaTexture = CreateVertexArray();
    VertexBuffer vbTexture = CreateVertexBuffer(&verticesTexture, (4 * (2 + 2)) * sizeof(float));
    IndexBuffer ibTexture = CreateIndexBuffer(indicesTexture, 3 * 2);

    VertexBufferLayout layoutTexture = CreateLayout(); 
    layoutTexture.Pushf(&layoutTexture, 2);
    layoutTexture.Pushf(&layoutTexture, 2);

    vaTexture.AddBuffer(&vaTexture, &vbTexture, &layoutTexture);
    vaTexture.Unbind();
    vbTexture.Unbind();
    ibTexture.Unbind();

    // shader stuff
    
    // heart shader
    Shader shader = CreateShader("res/vertex.shader", "res/frag.shader");
    // doge shader
    Shader shaderTexture = CreateShader("res/vertex.shader", "res/fragTex.shader");
    shaderTexture.Bind(&shaderTexture);
    shaderTexture.SetUniform1i(&shaderTexture,"u_Texture", 0);
    shaderTexture.Unbind();
    
    // renderer stuff 
    Renderer renderer = CreateRenderer();
    
    // texture stuff
    Texture tex = CreateTexture("res/doge_transparent.png");
    tex.Bind(&tex, 0);

    
//*--------------------------------------------------------------------------------------------- 
    
    float red = 0.00f;
    float increment = 0.03f;

//*--------------------------------------------------------------------------------------------- 

    while (!glfwWindowShouldClose(window)) 
    {
        renderer.Clear();

        // heart rendering
        shader.Bind(&shader);
        shader.SetUniform4f(&shader, "u_color", red, 0.2f, 0.4f, 1.0f);
        renderer.Draw(&va, &ib, &shader);
        shader.Unbind();

        // doge rendering
        shaderTexture.Bind(&shaderTexture);
        // shaderTexture.SetUniform4f(&shaderTexture, "u_color", red, 0.2f, 0.4f, 1.0f);
        renderer.Draw(&vaTexture, &ibTexture, &shaderTexture);
        shaderTexture.Unbind();

        // color test
        red += increment;
        if (red >= 1.0f) increment = -increment;
        else if (red <= 0.0f) increment = -increment;

        

        glfwSwapBuffers(window);

        glfwPollEvents();
    }

    // tex.Delete(&tex);
    shader.DeleteShaderProgram(&shader);
    shader.DeleteShaderProgram(&shaderTexture);

    glfwTerminate();
    return 0;
}