#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 3) in vec2 uvPosition;

out VS_OUT
{
    vec4 color;
    vec2 uvPos;
} vs_out;

uniform mat4 projMatrix;

// Perform our position and projection transformations, and pass-through the color / texture data
void main(void)
{
    vs_out.uvPos = uvPosition;
    vs_out.color = vec4(color.x, color.y, color.z, 1);

    gl_Position = projMatrix  * vec4(position, 1);
}
