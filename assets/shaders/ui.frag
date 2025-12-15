#version 330 core

in vec2 vtexCoord;

uniform sampler2D uiTexture;

out vec4 fragColor;

void main()
{
    fragColor = texture(uiTexture, vtexCoord);
}