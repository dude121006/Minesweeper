#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "glad/glad.h"
#include "Renderer.h"


typedef struct
{
    char* vertexShader;
    char* fragShader;

} ShaderSource;

struct Shader;

typedef struct Shader
{
    unsigned int m_id;
    
    void (*Bind)(Shader* shader);
    void (*Unbind)();
    void (*DeleteShaderProgram)(Shader* shader);

    int (*GetUniformLocation)(Shader* shader, const char* name);
    void (*SetUniform4f)(Shader* shader, const char* name, float v0, float v1, float v2, float v3);

    // ShaderSource (*ParseShader)(const char* vertexShaderPath, const char* fragShaderPath);
    // unsigned int (*CompileShader)(unsigned int type, char* source);
    // unsigned int (*CreateShaderFromSource)(char* vertShaderSource, char* fragShaderSource);
    
} Shader;

Shader CreateShader(const char* vertexShaderPath, const char* fragShaderPath);

void BindShaderProgram(Shader* shader);
void UnbindShaderProgram();
void DeleteShaderProgram(Shader* shader);

// void SetUniform4f()
static int GetUniformLocation(Shader* shader, const char* name);
static void SetUniform4f(Shader* shader, const char* name, float v0, float v1, float v2, float v3);

static ShaderSource ParseShader(const char* vertexShaderPath, const char* fragShaderPath);
static unsigned int CompileShader(unsigned int type, char* source);
static unsigned int CreateShaderFromSource(char* vertShaderSource, char* fragShaderSource);

