#version 400

out VS_OUT
{
    vec3 uvwPos;
} vs_out;

uniform mat4 viewMatrix;

// Multiply are fixed vertices by the view matrix to render the full-screen sky cube.
void main(void)
{
    vec3[4] vertices = vec3[4](
        vec3(-1.0, -1.0, 1.0),
        vec3(1.0, -1.0, 1.0),
        vec3(-1.0, 1.0, 1.0),
        vec3(1.0, 1.0, 1.0));

    vs_out.uvwPos = transpose(mat3(viewMatrix)) * vertices[gl_VertexID];
    gl_Position = vec4(vertices[gl_VertexID], 1.0);
}
