#version 330 core

out vec4 color;

in vec3 fColor;
in vec2 fTexCoord;

uniform sampler2D uTexture1;
uniform sampler2D uTexture2;

void main()
{
    // color = texture(uTexture, fTexCoord) * vec4(fColor, 1.0f);
    color = mix(texture(uTexture1, fTexCoord), texture(uTexture2, fTexCoord), 0.2);
}