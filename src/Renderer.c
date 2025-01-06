#include "Renderer.h"
#include "stdio.h"


void GLClearErrors()
{
    while (glGetError() != GL_NO_ERROR);
}

void GLLogCall(const char* funcName, const char* fileName, int lineNum)
{
    GLenum error;
    while (error = glGetError())
    {
        printf("[GLError]: %d %s  %s : %d\n", error, funcName, fileName, lineNum);
    }
}

//*-------------------------------------------------------------------------------------------------------*\\

Renderer CreateRenderer()
{
    Renderer renderer;

    renderer.Draw = Draw;
    renderer.Clear = ClearScreen;
    
    return renderer;
}


static void Draw(VertexArray* va, IndexBuffer* ib, Shader* shader)
{
    va->Bind(va);
    ib->Bind(ib);
    shader->Bind(shader);
    
    GLCall(glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, NULL));

    va->Unbind();
    ib->Unbind();
    shader->Unbind();
}


static void ClearScreen()
{
    GLCall(glClear(GL_COLOR_BUFFER_BIT));
}