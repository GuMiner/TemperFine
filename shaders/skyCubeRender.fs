#version 400 core

uniform samplerCube skyCubeMap;

out vec4 color;

in VS_OUT
{
    vec3 uvwPos;
} fs_in;

void main(void)
{
    // Simply apply the color taking it from the cube map.
    color = texture(skyCubeMap, fs_in.uvwPos);
}