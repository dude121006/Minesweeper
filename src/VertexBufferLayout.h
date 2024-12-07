#pragma once

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

#include <glad/glad.h>


typedef struct
{
    unsigned int type;
    unsigned int count;    
    unsigned int normalized;

} VertexBufferElements;


struct VertexBufferLayout;

typedef struct VertexBufferLayout
{
    unsigned int m_stride;    
    unsigned int m_count;    
    unsigned int m_elementCount;    
    
    // an array of vertexBufferElements
    VertexBufferElements* m_elements;
    
    // function pointers
    VertexBufferLayout (*CreateLayout)();

    void (*Pushf)(VertexBufferLayout* layout, unsigned int count);
    void (*Pushui)(VertexBufferLayout* layout, unsigned int count);
    void (*Pushuc)(VertexBufferLayout* layout, unsigned int count);
    void (*FreeLayout)(VertexBufferLayout* layout);

    VertexBufferElements* (*GetElements)(VertexBufferLayout* layout);
    unsigned int (*GetStride)(VertexBufferLayout* layout);
    
    unsigned int (*GetSizeFromType)(unsigned int type);

} VertexBufferLayout;


VertexBufferLayout CreateLayout();
void Pushf(VertexBufferLayout* layout, unsigned int count);
void Pushui(VertexBufferLayout* layout, unsigned int count);
void Pushuc(VertexBufferLayout* layout, unsigned int count);

bool ReallocElementList(VertexBufferLayout* layout, unsigned int count);
void FreeLayout(VertexBufferLayout* layout);

VertexBufferElements* GetElements(VertexBufferLayout* layout);
unsigned int GetStride(VertexBufferLayout* layout);

unsigned int GetSizeFromType(unsigned int type);