#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// forward declaration
struct IndexBuffer;

typedef struct IndexBuffer
{
    unsigned int m_ID;
    unsigned int m_count;
    
    void (*BindBuffer)(IndexBuffer* ib);
    void (*UnbindBuffer)();
    void (*DeleteBuffer)(IndexBuffer* ib);
    
} IndexBuffer;

IndexBuffer CreateIndexBuffer(const void* data, unsigned int count);
void BindIndexBuffer(IndexBuffer* ib);
void UnbindIndexBuffer();
void DeleteIndexBuffer(IndexBuffer* ib);