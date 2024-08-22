#version 330 core
in VS_OUT
{
	vec4 i_pos;
    vec4 i_color;
    vec4 i_normal;
	vec2 i_texCoord;
}fs_in;


out vec4 FragColor;
uniform highp vec3 cameraPos;

struct Light
{
	vec4 position;
	vec4 intensity;
	vec4 color;
};
layout (std140) uniform LightUBO
{
	Light lightInfo[8];
	ivec4 nCount;
};
void main()
{
	//position = vec3(0, 0, 20);
	//intensity = vec3(500, 500, 500);
	vec3 amb_light_intensity = vec3(10, 10, 10);
	float p = 150;
	vec3 result_color = vec3( 0, 0, 0);
	vec3 ka = vec3(0.01, 0.01, 0.01);

	for(int i=0; i<nCount.x; i++)
	{
		Light light = lightInfo[i];

		vec3 kd = vec3( 0.5f,0.5f,0.5f ).x * light.color.xyz;
		vec3 ks = vec3(0.7937, 0.7937, 0.7937).x * light.color.xyz;
		vec3 ld = light.position.xyz - fs_in.i_pos.xyz;
		float r2 = dot(ld,ld);
		ld = normalize(ld);
		vec3 diffuse = kd*(light.intensity.xyz * (1 / r2)) * max(0.0f, dot(fs_in.i_normal.xyz,ld));
		vec3 h = normalize(normalize(cameraPos - fs_in.i_pos.xyz) + ld);
		vec3 specular = ks*(light.intensity.xyz * (1 / r2)) * max(0.0f, pow(dot(fs_in.i_normal.xyz,h), p));
		vec3 ambient = ka*(amb_light_intensity);
		result_color += diffuse + specular + ambient;
	}

	//result_color *= fs_in.i_color;
	FragColor = vec4(result_color, 1.0f);
}; 