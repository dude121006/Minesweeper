#include "VertexBuffer.h"


VertexBuffer CreateVertexBuffer(const void* data, unsigned int size)
{
    unsigned int id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ARRAY_BUFFER, id);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    
    VertexBuffer vb;

    vb.m_ID = id; 
    vb.BindBuffer = BindVertexBuffer;
    vb.UnbindBuffer = UnbindVertexBuffer;
    vb.DeleteBuffer = DeleteVertexBuffer;

    return vb; 
}

void BindVertexBuffer(VertexBuffer* vb)
{
    glBindBuffer(GL_ARRAY_BUFFER, vb->m_ID);
}

void UnbindVertexBuffer()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DeleteVertexBuffer(VertexBuffer* vb)
{
    glDeleteBuffers(1, &(vb->m_ID));
}