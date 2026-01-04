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
layout (location = 2) out vec4 gAlbedo;
layout (location = 3) out vec4 gMetallicRoughness; // 金属度 + 粗糙度

uniform sampler2D texture1;

void main()
{
	gPosition = fs_in.i_pos;
	gNormal = normalize(fs_in.i_normal);
	gAlbedo.rgb = texture(texture1, fs_in.i_texCoord).rgb * fs_in.i_color.rgb;
	gAlbedo.a = 1.0; // Alpha
	gMetallicRoughness.r = 0.0; // 金属度（简化）
	gMetallicRoughness.g = 0.5; // 粗糙度（简化）
	gMetallicRoughness.ba = vec2(0.0, 1.0);
}; 