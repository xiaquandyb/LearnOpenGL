#version 330 core
layout(location = 0) in vec3 vPos;
layout(location = 3) in vec3 vNormal;

out vec3 fNormal;
out vec3 fPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    fNormal = vNormal;
    fPos = vec3(model * vec4(vPos, 1.0f));

    gl_Position = projection * view * model * vec4(vPos, 1.0f);
}