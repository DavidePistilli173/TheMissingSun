#version 330

in vec2 texturePosOutput;
in mat4 matrix;

out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePosOutput);
    float r = fragmentColour.x;
    float g = fragmentColour.y;
    float b = fragmentColour.z;
    r = matrix[0][0];
    g = matrix[0][0];
    b = matrix[0][0];
    fragmentColour = vec4(r, g, b, 1.0);
}