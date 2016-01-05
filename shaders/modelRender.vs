#version 400

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 uvPos;

out VS_OUT
{
    vec2 uvPos;
} vs_out;

uniform mat4 mvMatrix;
uniform mat4 projMatrix;

// Perform our position and projection transformations, and pass-through the color / texture data
void main(void)
{
    vs_out.uvPos = uvPos;
    gl_Position = projMatrix * mvMatrix * vec4(position, 1);
}