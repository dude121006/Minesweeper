#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

void main()
{
   gl_Position = position + vec4(0.0f, 0.3f, 0.0f, 0.0f);
   v_TexCoord = texCoord;
};