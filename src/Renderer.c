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