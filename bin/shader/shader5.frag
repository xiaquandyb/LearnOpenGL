#version 330 core

out vec4 color;

in vec2 fTexCoord;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main()
{
    color = mix(texture(uTexture1, fTexCoord), texture(uTexture2, fTexCoord), 0.2);
}