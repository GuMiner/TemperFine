#version 400 core

uniform sampler2D modelTexture;

out vec4 color;

in VS_OUT
{
    vec2 uvPos;
} fs_in;

void main(void)
{
    // Scale each color of the provided object by the given color.
    color = texture2D(modelTexture, fs_in.uvPos);
}