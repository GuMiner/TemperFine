#version 400 core

layout (triangles) in;
layout (triangle_strip) out;
layout (max_vertices = 3) out;

in VS_OUT
{
    vec4 color;
    vec2 uvPos;
    uint voxelId;
} colorUV [];

out GS_OUT
{
    vec4 color;
    vec2 uvPos;
} colorUVOut;

void main(void)
{
    // Draw anything that isn't open space
    if (colorUV[0].voxelId != 0)
    {
        for (int i = 0; i < gl_in.length(); i++)
        {
            gl_Position = gl_in[i].gl_Position;
            colorUVOut.color = colorUV[i].color;
            colorUVOut.uvPos = colorUV[i].uvPos;
            EmitVertex();
        }

        EndPrimitive();
    }
}
