#version 420 core
out float FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D gPosition;
layout(binding = 1) uniform sampler2D gNormal;
layout(binding = 2) uniform sampler2D texNoise;

uniform vec3 samples[64];
uniform mat4 projection;
uniform vec2 noiseScale;

void main(){
    float kernelSize = 64;
    float radius = 0.03;
    float bias = 0.005;
    vec3 fragPos   = texture(gPosition, TexCoords).xzy;
    vec3 normal    = texture(gNormal, TexCoords).rgb;
    vec3 randomVec = texture(texNoise, TexCoords * noiseScale).xyz;
    vec3 tangent   = normalize(randomVec - normal * dot(randomVec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 TBN       = mat3(tangent, bitangent, normal);
    float occlusion = 0.0;
    for (int i = 0; i < kernelSize; ++i){
        // get sample position
        vec3 samplePos = TBN * samples[i];// from tangent to view-space
        samplePos = fragPos + samplePos * radius;
        vec4 offset = vec4(samplePos, 1.0);
        offset      = projection * offset;// from view to clip-space
        offset.xyz /= offset.w;// perspective divide
        offset.xyz  = offset.xyz * 0.5 + 0.5;// transform to range 0.0 - 1.0
        float sampleDepth = texture(gPosition, offset.xz).y;
        float rangeCheck = smoothstep(0.0, 1.0, radius / abs(fragPos.y - sampleDepth));
        occlusion += (sampleDepth >= samplePos.y + bias ? 1.0 : 0.0) * rangeCheck;
    }
    occlusion = 1.0 - (occlusion / kernelSize);
    FragColor = pow(occlusion, 4);
}