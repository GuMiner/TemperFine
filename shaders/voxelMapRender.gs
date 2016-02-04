#version 400 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in VS_OUT
{
    vec2 uvPos;
    uint voxelId;
} colorUV [];

out GS_OUT
{
    vec2 uvPos;
} colorUVOut;

uniform uint currentVoxelId;

void main(void)
{
    // Only draw the current voxel ID.
    if (colorUV[0].voxelId == currentVoxelId)
    {
        for (int i = 0; i < gl_in.length(); i++)
        {
            gl_Position = gl_in[i].gl_Position;
            colorUVOut.uvPos = colorUV[i].uvPos;
            EmitVertex();
        }

        EndPrimitive();
    }
}
