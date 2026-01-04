#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D hdrTexture;
uniform sampler2D bloomTexture;
uniform float bloomStrength;

void main()
{
    // 采样 HDR 和 Bloom 纹理
    vec3 hdrColor = texture(hdrTexture, TexCoords).rgb;
    vec3 bloomColor = texture(bloomTexture, TexCoords).rgb;
    
    // 合并 HDR 和 Bloom
    hdrColor += bloomColor * bloomStrength;
    
    // Reinhard Tone Mapping
    vec3 result = hdrColor / (hdrColor + vec3(1.0));
    
    // Gamma 校正
    result = pow(result, vec3(1.0 / 2.2));
    
    FragColor = vec4(result, 1.0);
}
