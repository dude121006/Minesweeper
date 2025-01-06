#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "stb_image.h"
#include "Renderer.h"


struct Texture;

typedef struct Texture
{
    unsigned int m_id;
    const char* m_filePath;
    unsigned char* m_localBuffer;
    int m_width;
    int m_height;
    int m_BPP;

    void (*Bind)(Texture* tex, unsigned int slot);
    void (*Unbind)();
    void (*Delete)(Texture* tex);

} Texture;


Texture CreateTexture(const char* filePath);

void BindTexture(Texture* tex, unsigned int slot);
void UnbindTexture();
void DeleteTexture(Texture* tex);