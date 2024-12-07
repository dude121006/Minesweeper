#include "VertexBufferLayout.h"


VertexBufferLayout CreateLayout()
{
    VertexBufferLayout layout;

    layout.m_elements = NULL;
    layout.m_elementCount = 0;
    layout.m_stride = 0;

    layout.Pushf = Pushf;
    layout.Pushui = Pushui;
    layout.Pushuc = Pushuc;
    layout.FreeLayout = FreeLayout;

    layout.GetElements = GetElements;
    layout.GetStride = GetStride;
    layout.GetSizeFromType = GetSizeFromType;
    
    return layout;
}

bool ReallocElementList(VertexBufferLayout* layout, unsigned int count)
{
    layout->m_elements = (VertexBufferElements *) realloc(layout->m_elements,
                             (layout->m_elementCount + 1) * sizeof(VertexBufferElements));

    if (layout->m_elements == NULL)
    {
        printf("Error while reallocating memeory for VertexBufferLayout");
        layout->FreeLayout(layout);
        return false;
    }
    
    return true;
}


// for type : float
void Pushf(VertexBufferLayout* layout, unsigned int count)
{
    if(!ReallocElementList(layout, count))
    {
        printf("Error in reallocation!");
        return;
    }
    
    VertexBufferElements newElement = { GL_FLOAT, count, GL_FALSE };
    layout->m_elements[layout->m_elementCount] = newElement;
    layout->m_stride += count * sizeof(float);
    
    layout->m_elementCount++;

}

// for type: Unsigned int
void Pushui(VertexBufferLayout* layout, unsigned int count)
{
    if(!ReallocElementList(layout, count))
        return;
    
    VertexBufferElements newElement = { GL_UNSIGNED_INT, count, GL_FALSE };
    layout->m_elements[layout->m_elementCount] = newElement;
    layout->m_stride += count * sizeof(GLuint);
    
    layout->m_elementCount++;

};

// for type: unsigned char
void Pushuc(VertexBufferLayout* layout, unsigned int count)
{
    if (!ReallocElementList(layout, count))
        return;

    VertexBufferElements newElement = { GL_UNSIGNED_BYTE, count, GL_TRUE };
    layout->m_elements[layout->m_elementCount] = newElement;
    layout->m_stride += count * sizeof(GLubyte);
    
    layout->m_elementCount++;

};

void FreeLayout(VertexBufferLayout* layout)
{
    free(layout->m_elements);
    layout->m_elements = NULL;
    layout->m_elementCount = 0;
    layout->m_stride = 0;
    return;
}

VertexBufferElements* GetElements(VertexBufferLayout* layout)
{
    return layout->m_elements;
}

unsigned int GetStride(VertexBufferLayout* layout)
{
    return layout->m_stride;
}

unsigned int GetSizeFromType(unsigned int type)
{
    switch (type)
    {
    case GL_FLOAT:
        return sizeof(GLfloat);
        break;
    
    case GL_UNSIGNED_BYTE:
        return sizeof(GLubyte);
        break;

    case GL_UNSIGNED_INT:
        return sizeof(GLuint);
        break;

    default:
        printf("Error in GetSizeFromType() !!");
        return 0;
        break;
    }
}