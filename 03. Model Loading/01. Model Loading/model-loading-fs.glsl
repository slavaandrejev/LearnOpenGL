#version 450 core

out vec4 FragColor;

in vec2 TexCoords;

struct Material {
    sampler2D diffuse[1];
};
uniform Material material;

void main() {
    FragColor = texture(material.diffuse[0], TexCoords);
}
