#version 330 core

layout(location = 0) out vec4 color;

uniform sampler2D u_Texture;
in vec2 v_TexCoord;
uniform vec4 u_color;

void main()
{
   //test
   vec4 texColor = texture(u_Texture, v_TexCoord);
   color = texColor;
};