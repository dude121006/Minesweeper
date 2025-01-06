#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Shader.h"
#include "VertexArray.h"
#include "IndexBuffer.h"


// used for error checking
#define GLCall(x) GLClearErrors();\
    x;\
    GLLogCall(#x, __FILE__, __LINE__)
 

// turn clear all the error flags
void GLClearErrors();
void GLLogCall(const char* funcName, const char* fileName, int lineNum);

//*-------------------------------------------------------------------------------------------------------*\\

//* forward declaration
typedef struct Shader Shader;


typedef struct
{
    // function pointers
    void (*Draw)(VertexArray *va, IndexBuffer *ib, Shader *shader);
    void (*Clear)();

} Renderer;

Renderer CreateRenderer();

static void Draw(VertexArray *va, IndexBuffer *ib, Shader *shader);
static void ClearScreen();