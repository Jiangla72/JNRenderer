#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 color;
layout (location = 2) in vec2 texCoord;
layout (location = 3) in vec3 normal;

uniform highp mat4 mvpMatrix;
out VS_OUT
{
    vec4 i_pos;
    vec4 i_color;
    vec4 i_normal;
    vec2 i_texCoord;
}vs_out;
void main()
{
    vs_out.i_pos = vec4(aPos,1);
    vs_out.i_normal = vec4(normal,1); 
    vs_out.i_color = color;
    vs_out.i_texCoord = texCoord;
    gl_Position = mvpMatrix * vec4(aPos.x, aPos.y, aPos.z, 1.0);
};