#version 330 core

in vec2 vtexCoord;
in float vlight;
in float vfogDepth;

uniform sampler2D atlas;
uniform vec3 ambientColor;

uniform float fogStart;
uniform vec3 fogColor;
uniform float fogDensity;

out vec4 fragColor;

void main()
{
    vec4 texColor = texture(atlas, vtexCoord);

    vec3 litColor = texColor.rgb * ambientColor * vlight;

    float fogDistance = max(vfogDepth - fogStart, 0.0);
    float fogFactor = exp(-fogDensity * fogDistance);
    fogFactor = clamp(fogFactor, 0.0, 1.0);
    vec3 finalColor = mix(fogColor, litColor, fogFactor);

    fragColor = vec4(finalColor, texColor.a);
}