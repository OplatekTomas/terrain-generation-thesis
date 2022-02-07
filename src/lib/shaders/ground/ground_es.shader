#version 420 core

layout(triangles, equal_spacing, ccw) in;

out vec3 WorldPos_FS_in;
out vec2 TexCoord_FS_in;
out vec3 Normal_FS_in;

uniform mat4 view;
uniform mat4 projection;

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

vec3 interpolate3D(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}



vec3 getH(int i, int j){
    vec3 A = patchData.Normal[i] + patchData.Normal[j];
    vec3 B = patchData.WorldPos[j] - patchData.WorldPos[i];
    float v = 2.0 * (dot(B, A) / dot(B, B));
    return (A/2) - ((v/2.0) * B);
}


void main()
{
    // Interpolate the attributes of the output vertex using the barycentric coordinates
    TexCoord_FS_in = interpolate2D(patchData.TexCoord[0], patchData.TexCoord[1], patchData.TexCoord[2]);

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

    vec3 Normal200 =  patchData.Normal[0];
    vec3 Normal020 =  patchData.Normal[1];
    vec3 Normal002 =  patchData.Normal[2];
    vec3 Normal110 = normalize(getH(0, 1));
    vec3 Normal011 = normalize(getH(1, 2));
    vec3 Normal101 = normalize(getH(2, 0));

    Normal_FS_in =
    uPow2 * Normal200 +
    vPow2 * Normal020 +
    wPow2 * Normal002 +
    u * v * Normal110 +
    u * w * Normal101 +
    v * w * Normal011;
    Normal_FS_in = normalize(Normal_FS_in);
    //Normal_FS_in = interpolate3D(patchData.Normal[0], patchData.Normal[1], patchData.Normal[2]);

    gl_Position = projection * view * vec4(WorldPos_FS_in, 1.0);
}