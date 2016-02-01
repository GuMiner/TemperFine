#version 400 core

uniform sampler2D voxelTextures;

out vec4 color;

in GS_OUT
{
    vec2 uvPos;
} fs_in;

void main(void)
{
    color = texture2D(voxelTextures, fs_in.uvPos);
}
