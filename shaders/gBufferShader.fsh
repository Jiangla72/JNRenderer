#version 330 core
in VS_OUT
{
	vec4 i_pos;
    vec4 i_color;
    vec4 i_normal;
	vec2 i_texCoord;
}fs_in;

layout (location = 0) out vec4 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec; //颜色

uniform sampler2D texture1;

void main()
{
	gPosition = fs_in.i_pos;
	gNormal = normalize(fs_in.i_normal);
	gAlbedoSpec.rgb = texture(texture1, fs_in.i_texCoord).rgb;
	gAlbedoSpec.a = 0.7937;
}; 