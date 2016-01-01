#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 3) in vec2 texPos;

out VS_OUT
{
    vec4 color;
    vec2 texPos;
} vs_out;

uniform mat4 mv_matrix;
uniform mat4 proj_matrix;

// Perform our position and projection transformations, and pass-through the color / texture data
void main(void)
{
    vs_out.color = vec4(color.x, color.y, color.z, 1);
    vs_out.texPos = texPos;
    
    gl_Position = proj_matrix * mv_matrix * vec4(position, 1);
}