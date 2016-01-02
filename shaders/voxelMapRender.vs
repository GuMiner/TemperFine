#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 3) in vec2 uvPosition;

uniform usampler1D voxelTopTexture;

out VS_OUT
{
    vec4 color;
    vec2 uvPos;
} vs_out;


uniform mat4 projMatrix;

// Perform our position and projection transformations, and pass-through the color / texture data
void main(void)
{
    uvec4 voxelInfo = texelFetch(voxelTopTexture, gl_InstanceID, 0);
    float amount = 0.0f;
    if (voxelInfo.x != 0)
    {
        amount += 2.0f;
    }

    vs_out.uvPos = uvPosition;
    vs_out.color = vec4(voxelInfo.x, voxelInfo.y, 1.0f, 1);

    gl_Position = projMatrix  * (vec4(position, 1) + vec4(4.0f * gl_InstanceID, amount, 0.0f, 0.0f));
}
