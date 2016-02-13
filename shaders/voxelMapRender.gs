#version 400 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in VS_OUT
{
    vec2 uvPos;
    uint voxelId;
    ivec3 xyzIndex;
} colorUV [];

out GS_OUT
{
    vec2 uvPos;
    vec3 color;
} colorUVOut;

uniform uint currentVoxelId;
uniform ivec3 selectedIndex;

void main(void)
{
    // Only draw the current voxel ID.
    if (colorUV[0].voxelId == currentVoxelId)
    {
        vec3 selectionFactor = vec3(0.0f);
        if (selectedIndex == colorUV[0].xyzIndex)
        {
            selectionFactor = vec3(0.40f);
        }

        for (int i = 0; i < gl_in.length(); i++)
        {
            gl_Position = gl_in[i].gl_Position;

            colorUVOut.color = selectionFactor;
            colorUVOut.uvPos = colorUV[i].uvPos;
            EmitVertex();
        }

        EndPrimitive();
    }
}
