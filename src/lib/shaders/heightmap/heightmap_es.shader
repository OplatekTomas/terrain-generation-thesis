#version 420 core

layout(triangles, equal_spacing, ccw) in;

out vec3 WorldPos_FS_in;

struct OutputPatch{
    vec3 WorldPos030;
    vec3 WorldPos021;
    vec3 WorldPos012;
    vec3 WorldPos003;
    vec3 WorldPos102;
    vec3 WorldPos201;
    vec3 WorldPos300;
    vec3 WorldPos210;
    vec3 WorldPos120;
    vec3 WorldPos111;

    vec3 WorldPos[3];
    vec3 Normal[3];
    vec2 TexCoord[3];
};

in patch OutputPatch patchData;

vec2 interpolate2D(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

mat4 lookAt(){
    vec3 eye = vec3(0.5, 1.0, 0.5);
    vec3 center = vec3(0.5, 0.0, 0.5);
    vec3 up = vec3(0.0, 0, 1.0);
    vec3 f = normalize(center - eye);
    vec3 s = normalize(cross(f, up));
    vec3 u = cross(s, f);
    return mat4(
    s.x, u.x, -f.x, 0.0,
    s.y, u.y, -f.y, 0.0,
    s.z, u.z, -f.z, 0.0,
    -dot(s, eye), -dot(u, eye), dot(f, eye), 1.0
    );
}
void main(){

    float u = gl_TessCoord.x;
    float v = gl_TessCoord.y;
    float w = gl_TessCoord.z;

    float uPow3 = pow(u, 3);
    float vPow3 = pow(v, 3);
    float wPow3 = pow(w, 3);
    float uPow2 = pow(u, 2);
    float vPow2 = pow(v, 2);
    float wPow2 = pow(w, 2);

    WorldPos_FS_in = patchData.WorldPos300 * wPow3 +
    patchData.WorldPos030 * uPow3 +
    patchData.WorldPos003 * vPow3 +
    patchData.WorldPos210 * 3.0 * wPow2 * u +
    patchData.WorldPos120 * 3.0 * w * uPow2 +
    patchData.WorldPos201 * 3.0 * wPow2 * v +
    patchData.WorldPos021 * 3.0 * uPow2 * v +
    patchData.WorldPos102 * 3.0 * w * vPow2 +
    patchData.WorldPos012 * 3.0 * u * vPow2 +
    patchData.WorldPos111 * 6.0 * w * u * v;

    mat4 vw = lookAt();
    gl_Position = vw * (vec4((WorldPos_FS_in) - vec3(0.25, 0, 0.25), 0.5));
}