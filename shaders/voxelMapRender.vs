#version 400

layout (location = 0) in vec3 position;
layout (location = 3) in vec2 uvPosition;

uniform sampler1D voxelTopTexture;

out VS_OUT
{
    vec2 uvPos;
    uint voxelId;
    ivec3 xyzIndex;
} vs_out;

uniform mat4 projMatrix;

uniform ivec2 xyLengths;

// Performs rotation from 0 deg to 270 for 0-3, and the same for 4-7, but inverted.
mat4 calculateRotationMatrix(in float rotationAmount)
{
    float rotation = (3.14159265358f / 2.0f) * rotationAmount;

    // Z-axis rotation
    float c = cos(rotation);
    float s = sin(rotation);

    mat4 rotMatrix = mat4(
		c,  s, 0.0f, 0.0f,
		-s, c, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);

    if (rotation > 5.0f) // > 270 deg, < 360 deg.
    {
        // Rotate around the Y axis to flip, with a 180 deg flip, cos(180) = 0, sin(180) = -1
        return rotMatrix * mat4(
            1.0f, 0.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 1.0f);
    }

    return rotMatrix;
}

mat4 calculateTranslationMatrix()
{
    // Offset based on the voxel index.
    float spacing = 2.0f;
    int xyLength = (xyLengths.x * xyLengths.y);
    int zIndex = (gl_InstanceID / xyLength);
    int yIndex = (gl_InstanceID - zIndex * xyLength) / xyLengths.x;
    int xIndex = gl_InstanceID - (zIndex * xyLength + yIndex * xyLengths.x);
    vs_out.xyzIndex = ivec3(xIndex, yIndex, zIndex);

    return mat4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        spacing * float(xIndex) + spacing / 2.0f, spacing * float(yIndex) + spacing / 2.0f, spacing * float(zIndex) + spacing / 2.0f, 1.0f);
}

// Perform our position and projection transformations, and pass-through the color / texture data
void main(void)
{
    vec4 voxelInfo = texelFetch(voxelTopTexture, gl_InstanceID, 0);
    vs_out.voxelId = uint(voxelInfo.x * 255.0f);
    vs_out.uvPos = uvPosition;

    // Rotation is stored in voxelInfo.y, in quarters.
    mat4 viewRotationMatrix = calculateRotationMatrix(voxelInfo.y * 255.0f);
    mat4 viewTranslationMatrix = calculateTranslationMatrix();

    // Add the vertex position to all our transformations to get the final result.
    gl_Position = projMatrix * viewTranslationMatrix * viewRotationMatrix * vec4(position, 1);
}
