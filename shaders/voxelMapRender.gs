#version 400 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in VS_OUT
{
    vec2 uvPos;
    uint voxelId;
    uint currentId;
} colorUV [];

out GS_OUT
{
    vec2 uvPos;
} colorUVOut;

void main(void)
{
    // Only draw the current voxel ID.
    if (colorUV[0].voxelId != 0)// colorUV[0].currentId)
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
