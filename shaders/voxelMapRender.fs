#version 400 core

uniform sampler2D voxelTextures;

out vec4 color;

in GS_OUT
{
    vec2 uvPos;
    vec3 color;
} fs_in;

void main(void)
{
    color = texture2D(voxelTextures, fs_in.uvPos) + vec4(fs_in.color, 0.0f);
}
