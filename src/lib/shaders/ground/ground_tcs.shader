#version 420 core

// define the number of CPs in the output patch
layout (vertices = 1) out;


// attributes of the input CPs
in vec3 WorldPos_VS_Out[];
in vec2 TexCoord_VS_Out[];
in vec3 Normal_VS_Out[];

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


out patch OutputPatch patchData;

// attributes of the output CPs

vec3 ProjectToPlane(vec3 Point, vec3 PlanePoint, vec3 PlaneNormal){
    vec3 v = Point - PlanePoint;
    float Len = dot(v, PlaneNormal);
    vec3 d = Len * PlaneNormal;
    return (Point - d);
}




void CalcPositions(){
    // The original vertices stay the same
    patchData.WorldPos030 = WorldPos_VS_Out[0];
    patchData.WorldPos003 = WorldPos_VS_Out[1];
    patchData.WorldPos300 = WorldPos_VS_Out[2];

    // Edges are names according to the opposing vertex
    vec3 Edge300 = patchData.WorldPos003 - patchData.WorldPos030;
    vec3 Edge030 = patchData.WorldPos300 - patchData.WorldPos003;
    vec3 Edge003 = patchData.WorldPos030 - patchData.WorldPos300;

    // Generate two midpoints on each edge
    patchData.WorldPos021 = patchData.WorldPos030 + Edge300 / 3.0;
    patchData.WorldPos012 = patchData.WorldPos030 + Edge300 * 2.0 / 3.0;
    patchData.WorldPos102 = patchData.WorldPos003 + Edge030 / 3.0;
    patchData.WorldPos201 = patchData.WorldPos003 + Edge030 * 2.0 / 3.0;
    patchData.WorldPos210 = patchData.WorldPos300 + Edge003 / 3.0;
    patchData.WorldPos120 = patchData.WorldPos300 + Edge003 * 2.0 / 3.0;

    // Project each midpoint on the plane defined by the nearest vertex and its normal
    patchData.WorldPos021 = ProjectToPlane(patchData.WorldPos021, patchData.WorldPos030, Normal_VS_Out[0]);
    patchData.WorldPos012 = ProjectToPlane(patchData.WorldPos012, patchData.WorldPos003, Normal_VS_Out[1]);
    patchData.WorldPos102 = ProjectToPlane(patchData.WorldPos102, patchData.WorldPos003, Normal_VS_Out[1]);
    patchData.WorldPos201 = ProjectToPlane(patchData.WorldPos201, patchData.WorldPos300, Normal_VS_Out[2]);
    patchData.WorldPos210 = ProjectToPlane(patchData.WorldPos210, patchData.WorldPos300, Normal_VS_Out[2]);
    patchData.WorldPos120 = ProjectToPlane(patchData.WorldPos120, patchData.WorldPos030, Normal_VS_Out[0]);

    // Handle the center
    vec3 Center = (patchData.WorldPos003 + patchData.WorldPos030 + patchData.WorldPos300) / 3.0;
    patchData.WorldPos111 = (patchData.WorldPos021 + patchData.WorldPos012 + patchData.WorldPos102 +
    patchData.WorldPos201 + patchData.WorldPos210 + patchData.WorldPos120) / 6.0;
    patchData.WorldPos111 += (patchData.WorldPos111 - Center) / 2.0;

}


void main(){
    float tessLevel = 10.0;
    // Set the control points of the output patch
    for (int i = 0; i < 3; i++) {
        patchData.Normal[i] = Normal_VS_Out[i];
        patchData.WorldPos[i] = WorldPos_VS_Out[i];
        patchData.TexCoord[i] = TexCoord_VS_Out[i];
    }

    CalcPositions();
    //CalcNormals();
    // Calculate the tessellation levels
    gl_TessLevelOuter[0] = tessLevel;
    gl_TessLevelOuter[1] = tessLevel;
    gl_TessLevelOuter[2] = tessLevel;
    gl_TessLevelInner[0] = tessLevel;

}