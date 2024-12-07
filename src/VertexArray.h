#pragma once

#include <stdio.h>

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

struct VertexArray;

typedef struct VertexArray
{
    
    unsigned int m_ID;
    
    VertexArray (*CreateVertexArray)();
    void (*AddBuffer)(VertexArray* va, VertexBuffer* vb, VertexBufferLayout* layout);
    void (*BindVertexArray)(VertexArray* va);
    void (*UnbindVertexArray)();
    

} VertexArray;

VertexArray CreateVertexArray();
void AddBuffer(VertexArray* va, VertexBuffer* vb, VertexBufferLayout* layout);

void BindVertexArray(VertexArray* va);
void UnbindVertexArray();