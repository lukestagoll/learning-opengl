#version 330 core
out vec4 FragColor;

in vec2 TexCoord;
in vec3 color;

uniform sampler2D tex0;

void main()
{
    FragColor = texture(tex0, TexCoord) * vec4(color, 1.0);
}