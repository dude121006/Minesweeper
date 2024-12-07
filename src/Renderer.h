#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// used for error checking
#define GLCall(x) GLClearErrors();\
    x;\
    GLLogCall(#x, __FILE__, __LINE__)
 

// turn clear all the error flags
void GLClearErrors();

void GLLogCall(const char* funcName, const char* fileName, int lineNum);