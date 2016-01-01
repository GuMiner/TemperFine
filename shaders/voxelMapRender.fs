#version 400 core

out vec4 color;

in VS_OUT
{
    vec4 color;
} fs_in;

void main(void)
{
    // Scale each color of the provided object by the given color.
    color = fs_in.color;
}