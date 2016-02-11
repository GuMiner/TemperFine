#version 400

layout (location = 0) in vec3 position;

uniform mat4 projMatrix;

// Perform our position and projection transformations
void main(void)
{
    gl_Position = projMatrix * vec4(position, 1);
}