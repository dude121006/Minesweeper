#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "VertexArray.h"
#include "Renderer.h"


VertexArray CreateVertexArray()
{
    unsigned int id;
    GLCall(glGenVertexArrays(1, &id));
    GLCall(glBindVertexArray(id));
    
    VertexArray va;
    va.m_ID = id;

    va.AddBuffer = AddBuffer;
    va.BindVertexArray = BindVertexArray;
    va.UnbindVertexArray = UnbindVertexArray;

    return va;
}

void BindVertexArray(VertexArray* va)
{
    if (va == NULL)
    {
        printf("Vertex array is NULL!\n");
        return;
    }
        GLCall(glBindVertexArray(va->m_ID));
}

void UnbindVertexArray()
{
    GLCall(glBindVertexArray(0));
}

void AddBuffer(VertexArray* va, VertexBuffer* vb, VertexBufferLayout* layout)
{
    va->BindVertexArray(va);

    //bind the vertex buffer
    vb->BindBuffer(vb);
    
    //define they layout of the buffer
    const VertexBufferElements* elements = layout->GetElements(layout);

    int offset = 0;
    
    for (int i = 0; i < layout->m_elementCount; i++)
    {
        const VertexBufferElements element = elements[i];        

        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized,
                        layout->GetStride(layout), (const void*) offset ));

        offset += element.count * GetSizeFromType(element.type);
    }
}
