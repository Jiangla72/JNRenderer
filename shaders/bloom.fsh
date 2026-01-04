#version 330 core
in vec2 TexCoords;
out vec4 FragColor;

uniform sampler2D hdrTexture;
uniform float threshold;
uniform float strength;

void main()
{
    vec3 color = texture(hdrTexture, TexCoords).rgb;
    
    // 计算亮度
    float brightness = dot(color, vec3(0.2126, 0.7152, 0.0722));
    
    // 如果亮度超过阈值，则提取为 Bloom
    if (brightness > threshold) {
        FragColor = vec4(color * strength, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 1.0);
    }
}
