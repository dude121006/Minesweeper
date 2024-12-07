#version 330 core

layout(location = 0) in vec4 position;

void main()
{
   gl_Position = position + vec4(0.0f, 0.3f, 0.0f, 0.0f);
};