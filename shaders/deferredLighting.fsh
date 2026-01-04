#version 330 core
out vec4 FragColor;

in vec2 TexCoords;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedo;
uniform sampler2D gMetallicRoughness;
uniform sampler2DShadow shadowMap;

uniform int numLights;
uniform vec3 lightPositions[10];
uniform vec3 lightColors[10];
uniform vec3 viewPos;
uniform mat4 lightSpaceMatrix;

const float PI = 3.14159265359;

float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}

float ShadowCalculation(vec4 fragPosLightSpace, float NdotL)
{
    vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;
    if (projCoords.z > 1.0) return 0.0;

    float currentDepth = projCoords.z;
    // 法线相关偏移：接触更锐利，斜角更保守
    // float bias = mix(0.0005, 0.0020, clamp(1.0 - NdotL, 0.0, 1.0));
    float bias = 0.005
    currentDepth -= bias;
    // Poisson盘采样：8点 + 硬件PCF（线性比较）
    vec2 texelSize = 1.0 / vec2(textureSize(shadowMap, 0));
    vec2 offsets[8] = vec2[](
        vec2(-0.5, -0.5), vec2( 0.5, -0.5),
        vec2(-0.5,  0.5), vec2( 0.5,  0.5),
        vec2(-0.8,  0.0), vec2( 0.8,  0.0),
        vec2( 0.0, -0.8), vec2( 0.0,  0.8)
    );
    float shadow = 0.0;
    float radius = 1.0; // 可调半径
    for (int i = 0; i < 8; ++i) {
        float cmp = texture(shadowMap, vec3(projCoords.xy + offsets[i] * texelSize * radius, currentDepth));
        shadow += 1.0 - cmp; // 比较结果：1=可见，0=阴影
    }
    shadow /= 8.0;

    return shadow;
}

vec3 ShadeAt(vec2 uv)
{
    vec3 FragPos = texture(gPosition, uv).rgb;
    vec3 Normal = texture(gNormal, uv).rgb;
    vec3 Albedo = texture(gAlbedo, uv).rgb;
    float Metallic = texture(gMetallicRoughness, uv).r;
    float Roughness = texture(gMetallicRoughness, uv).g;

    vec3 N = normalize(Normal);
    vec3 V = normalize(viewPos - FragPos);

    vec3 F0 = vec3(0.04);
    F0 = mix(F0, Albedo, Metallic);

    vec3 Lo = vec3(0.0);
    for(int i = 0; i < numLights; ++i)
    {
        vec3 L = normalize(lightPositions[i] - FragPos);
        vec3 H = normalize(V + L);
        float distance = length(lightPositions[i] - FragPos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = lightColors[i] * attenuation;

        float NDF = DistributionGGX(N, H, Roughness);
        float G = GeometrySmith(N, V, L, Roughness);
        vec3 F = fresnelSchlick(max(dot(H, V), 0.0), F0);

        vec3 numerator = NDF * G * F;
        float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
        vec3 specular = numerator / denominator;

        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - Metallic;

        float NdotL = max(dot(N, L), 0.0);
        float shadow = ShadowCalculation(lightSpaceMatrix * vec4(FragPos, 1.0), NdotL);
        float visibility = 1.0 - shadow;
        Lo += (kD * Albedo / PI + specular) * radiance * NdotL * visibility;
    }

    vec3 ambient = vec3(0.03) * Albedo;
    vec3 color = ambient + Lo;

    return color;
}

void main()
{
    vec3 color = ShadeAt(TexCoords);

    // 局部屏幕空间抗锯齿（近似FXAA）：检测法线/深度边缘并做邻域均值混合
    vec2 texel = 1.0 / vec2(textureSize(gAlbedo, 0));
    vec3 Nc = texture(gNormal, TexCoords).rgb;
    float Dc = texture(gPosition, TexCoords).z;
    vec3 Nl = texture(gNormal, TexCoords + vec2(-texel.x, 0)).rgb;
    vec3 Nr = texture(gNormal, TexCoords + vec2(texel.x, 0)).rgb;
    vec3 Nt = texture(gNormal, TexCoords + vec2(0, texel.y)).rgb;
    vec3 Nb = texture(gNormal, TexCoords + vec2(0, -texel.y)).rgb;
    float Dl = texture(gPosition, TexCoords + vec2(-texel.x, 0)).z;
    float Dr = texture(gPosition, TexCoords + vec2(texel.x, 0)).z;
    float Dt = texture(gPosition, TexCoords + vec2(0, texel.y)).z;
    float Db = texture(gPosition, TexCoords + vec2(0, -texel.y)).z;

    float edgeN = max(max(length(Nc - Nl), length(Nc - Nr)), max(length(Nc - Nt), length(Nc - Nb)));
    float edgeD = max(max(abs(Dc - Dl), abs(Dc - Dr)), max(abs(Dc - Dt), abs(Dc - Db)));
    float edge = clamp(edgeN * 0.6 + edgeD * 0.4, 0.0, 1.0);

    vec3 colorL = ShadeAt(TexCoords + vec2(-texel.x, 0));
    vec3 colorR = ShadeAt(TexCoords + vec2(texel.x, 0));
    vec3 colorT = ShadeAt(TexCoords + vec2(0, texel.y));
    vec3 colorB = ShadeAt(TexCoords + vec2(0, -texel.y));
    vec3 avgColor = (color + colorL + colorR + colorT + colorB) / 5.0;

    color = color / (color + vec3(1.0));
    color = pow(color, vec3(1.0/2.2));
    vec3 aaColor = mix(color, avgColor, edge);
    FragColor = vec4(aaColor, 1.0);
}

//     vec3 ambient = vec3(0.03) * Albedo;
//     vec3 color = ambient + Lo;

//     // HDR tonemapping
//     color = color / (color + vec3(1.0));
//     // Gamma correction
//     color = pow(color, vec3(1.0/2.2));

//     // 阴影（简化）
//     vec4 fragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
//     float shadow = ShadowCalculation(fragPosLightSpace);
//     color *= (1.0 - shadow * 0.5);

//     FragColor = vec4(color, 1.0);
// }
