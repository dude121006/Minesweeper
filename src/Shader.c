#include "Shader.h"


Shader CreateShader(const char* vertexShaderPath, const char* fragShaderPath)
{
    unsigned int id;

    ShaderSource source = ParseShader(vertexShaderPath, fragShaderPath);
    id = CreateShaderFromSource(source.vertexShader, source.fragShader);

    Shader shader;
    shader.m_id = id;
    
    shader.Bind = BindShaderProgram;
    shader.Unbind = UnbindShaderProgram;
    shader.DeleteShaderProgram = DeleteShaderProgram;

    shader.GetUniformLocation = GetUniformLocation;
    shader.SetUniform4f = SetUniform4f;
    shader.SetUniform1i = SetUniform1i;

    return shader;
}


void BindShaderProgram(Shader* shader)
{
    GLCall(glUseProgram(shader->m_id));
}

void UnbindShaderProgram()
{
    GLCall(glUseProgram(0));
}

void DeleteShaderProgram(Shader* shader)
{
    GLCall(glDeleteProgram(shader->m_id)); 
}

static int GetUniformLocation(Shader* shader, const char* name)
{
    GLCall( int location = glGetUniformLocation(shader->m_id, name));
    
    if (location == -1)
    {
        printf("No valid location for uniform: %s!\n", name);
    }
    return location;
}

static void SetUniform4f(Shader* shader, const char* name, float v0, float v1, float v2, float v3)
{
    GLCall(glUniform4f(GetUniformLocation(shader, name), v0, v1, v2, v3));
}

static void SetUniform1i(Shader* shader, const char* name, int value)
{
    GLCall(glUniform1i(GetUniformLocation(shader, name), value));
}

static unsigned int CreateShaderFromSource(char* vertShaderSource, char* fragShaderSource)
{
    GLCall(unsigned int program = glCreateProgram());
    
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertShaderSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragShaderSource);

    GLCall(glAttachShader(program, vs));
    GLCall(glAttachShader(program, fs));
    
    GLCall(glLinkProgram(program));
    GLCall(glValidateProgram(program));
    
    int validateResult;
    GLCall(glGetProgramiv(program, GL_VALIDATE_STATUS, &validateResult));
    if (validateResult == GL_FALSE)
    {
        printf("Error in validation!\n");
    }
    
    GLCall(glDeleteShader(vs));
    GLCall(glDeleteShader(fs));
    
    free(vertShaderSource);
    free(fragShaderSource);
   
    return program;
}


static ShaderSource ParseShader(const char* vertexShaderPath, const char* fragShaderPath)
{
    FILE *vertexShaderFile = fopen(vertexShaderPath, "r");    
    FILE *fragShaderFile = fopen(fragShaderPath, "r");    
    
    if (!vertexShaderFile || !fragShaderFile)
    {
        printf("Error while opening shader files!");
        if (vertexShaderFile) fclose(vertexShaderFile);
        if (fragShaderFile) fclose(fragShaderFile);
        return (ShaderSource){NULL, NULL};
    }
    
    // temp buffer to store a single line
    char buffer[256];

    // initial allocation
    char *vertexShader = (char*)malloc(1);
    char *fragShader = (char*)malloc(1);
    vertexShader[0] = '\0';
    fragShader[0] = '\0';
    
    // check if malloc was successful
    if (!vertexShader || !fragShader)
    {
        printf("Error: Memory allocation failed.\n");
        if (vertexShader) free(vertexShader);
        if (fragShader) free(fragShader);
        fclose(vertexShaderFile);
        fclose(fragShaderFile);
        return (ShaderSource){NULL, NULL};
    }

    while (fgets(buffer, sizeof(buffer), vertexShaderFile))
    {
        unsigned int shaderLen = strlen(vertexShader);
        unsigned int bufferLen = strlen(buffer);
        
        // allocate new memory for vertexShader on the heap
        vertexShader = (char*)realloc(vertexShader, shaderLen + bufferLen + 1);
        if (vertexShader == NULL)
        {
            printf("Error while reallocating memory for vertex shader!");
            free(fragShader);
            fclose(vertexShaderFile);
            fclose(fragShaderFile);
            return (ShaderSource){NULL, NULL};
        }
        strcat(vertexShader, buffer);
    }             

    while (fgets(buffer, sizeof(buffer), fragShaderFile))
    {
        size_t shaderLen = strlen(fragShader);
        size_t bufferLen = strlen(buffer);
        
        // allocate new memory for fragShader on the heap
        fragShader = (char*)realloc(fragShader, shaderLen + bufferLen + 1);
        if (fragShader == NULL)
        {
            printf("Error while reallocating memory for fragment shader!");
            free(vertexShader);
            fclose(vertexShaderFile);
            fclose(fragShaderFile);
            return (ShaderSource){NULL, NULL};
        }
        strcat(fragShader, buffer);
    }             

    fclose(vertexShaderFile);
    fclose(fragShaderFile);
    
    ShaderSource result = {vertexShader, fragShader};

    return result;
}

static unsigned int CompileShader(unsigned int type, char* source)
{
    unsigned int id = glCreateShader(type);
    glShaderSource(id, 1, &source, NULL);
    glCompileShader(id);
    
    // querying the success of compilation
    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);

    if (result == GL_FALSE)
    {
       int length;
       glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
       char message[length];
       glGetShaderInfoLog(id, length, NULL, message);

        printf("The compilation for type %s was unsuccessful!\n", 
                    (type == GL_VERTEX_SHADER) ? "vertex shader" : "fragment shader");
        printf("Log message: %s", message);

        // delete the shader bcos compilation was unsuccessful
        glDeleteShader(id);
        return 0;
    }

    return id;
}

