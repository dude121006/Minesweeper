#include "IndexBuffer.h"


IndexBuffer CreateIndexBuffer(const void* data, unsigned int count)
{
    unsigned int id;
    glGenBuffers(1, &id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW);
    
    IndexBuffer ib;

    ib.m_ID = id; 
    ib.m_count = count; 

    ib.Bind = BindIndexBuffer;
    ib.Unbind = UnbindIndexBuffer;
    ib.Delete = DeleteIndexBuffer;

    return ib; 
}

void BindIndexBuffer(IndexBuffer* ib)
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ib->m_ID);
}

void UnbindIndexBuffer()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void DeleteIndexBuffer(IndexBuffer* ib)
{
    glDeleteBuffers(1, &(ib->m_ID));
}