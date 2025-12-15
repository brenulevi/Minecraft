#version 330 core

uniform vec3 outlineColor;

out vec4 fragColor;

void main()
{
    fragColor = vec4(outlineColor, 1.0);
}