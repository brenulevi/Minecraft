#version 330 core

layout(location = 0) in vec3 pos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

void main()
{
    vec4 p = projection * view * model * vec4(pos, 1.0);
    p.z -= 0.00025;
    gl_Position = p;
}