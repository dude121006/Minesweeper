#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>


// forward declaration
struct VertexBuffer;

typedef struct VertexBuffer
{
    unsigned int m_ID;
    
    void (*BindBuffer)(VertexBuffer* vb);
    void (*UnbindBuffer)();
    void (*DeleteBuffer)(VertexBuffer* vb);
    
} VertexBuffer;

VertexBuffer CreateVertexBuffer(const void* data, unsigned int size);
void BindVertexBuffer(VertexBuffer* vb);
void UnbindVertexBuffer();
void DeleteVertexBuffer(VertexBuffer* vb);