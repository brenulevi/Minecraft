#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in float light;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

out vec2 vtexCoord;
out float vlight;
out float vfogDepth;

void main()
{
    vec4 worldPos = model * vec4(pos, 1.0);
    vec4 viewPos  = view * worldPos;

    gl_Position = projection * viewPos;

    vtexCoord = texCoord;
    vlight = light;
    vfogDepth = -viewPos.z;
}