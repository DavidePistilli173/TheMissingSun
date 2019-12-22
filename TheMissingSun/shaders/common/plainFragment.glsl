#version 330

in vec2 texturePos;

out vec4 fragmentColour;

uniform sampler2D baseTexture;

void main()
{
    fragmentColour = texture(baseTexture, texturePos);
}