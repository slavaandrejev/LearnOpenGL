#version 450 core

out vec4 FragColor;
in vec3 gouraudColor;

void main()
{
    FragColor = vec4(gouraudColor, 1.0);
}
