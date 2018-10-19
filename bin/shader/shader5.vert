#version 330 core

layout(location = 0) in vec3 vPos;
layout(location = 2) in vec2 vTexCoord;

out vec2 fTexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(vPos, 1.0f);
    fTexCoord = vTexCoord;
}