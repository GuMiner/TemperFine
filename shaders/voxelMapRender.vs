#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out VS_OUT
{
    vec4 color;
} vs_out;

uniform mat4 projMatrix;

// Perform our position and projection transformations, and pass-through the color / texture data
void main(void)
{
    vs_out.color = vec4(color.x, color.y, color.z, 1);
    
    gl_Position = projMatrix  * vec4(position, 1);
}