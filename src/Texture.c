#include "Texture.h"


Texture CreateTexture(const char* filePath)
{
    unsigned int id;
    GLCall(glGenTextures(1, &id));
    GLCall(glBindTexture(GL_TEXTURE_2D, id));
    
    Texture tex;
    tex.m_id = id;
    tex.m_filePath = filePath;
    tex.m_localBuffer = NULL;
    tex.m_width = 0;
    tex.m_height = 0;
    tex.m_BPP = 0;
    
    // function pointers
    tex.Bind = BindTexture;
    tex.Unbind = UnbindTexture;
    tex.Delete = DeleteTexture;
    

    //Texture stuff:
    stbi_set_flip_vertically_on_load(1);

    tex.m_localBuffer = stbi_load(filePath, &tex.m_width, &tex.m_height, &tex.m_BPP, 4);

    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
    GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
    
    GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, tex.m_width, tex.m_height, 0, GL_RGBA,
                        GL_UNSIGNED_BYTE, tex.m_localBuffer)); 
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));


    if (tex.m_localBuffer)
        stbi_image_free(tex.m_localBuffer); 


    return tex;
}

void BindTexture(Texture* tex, unsigned int slot)
{
    GLCall(glActiveTexture(GL_TEXTURE0 + slot));
    GLCall(glBindTexture(GL_TEXTURE_2D, tex->m_id));
}


void UnbindTexture()
{
    GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

void DeleteTexture(Texture* tex)
{
    (glDeleteTextures(GL_TEXTURE_2D, &tex->m_id));
}