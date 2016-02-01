#version 400

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 uvPosition;

uniform usampler1D voxelTopTexture;

out VS_OUT
{
    vec2 uvPos;
    uint voxelId;
    uint currentId;
} vs_out;

uniform mat4 projMatrix;

uniform ivec2 xyLengths;
uniform uint currentVoxelId;

// Perform our position and projection transformations, and pass-through the color / texture data
void main(void)
{
    uvec4 voxelInfo = texelFetch(voxelTopTexture, gl_InstanceID, 0);
    vs_out.voxelId = voxelInfo.x;
    vs_out.currentId = currentVoxelId;

    float spacing = 2.0f;
    int xyLength = (xyLengths.x * xyLengths.y);
    int zIndex = (gl_InstanceID / xyLength);
    int yIndex = (gl_InstanceID - zIndex * xyLength) / xyLengths.x;
    int xIndex = gl_InstanceID - (zIndex * xyLength + yIndex * xyLengths.x);

    float zPos = spacing * float(zIndex);
    float yPos = spacing * float(yIndex);
    float xPos = spacing * float(xIndex);

    vs_out.uvPos = uvPosition;

    // Adding vertex position plus the position of the instance itself.
    gl_Position = projMatrix  * (vec4(position, 1) + vec4(xPos, yPos, zPos, 0.0f));
}
