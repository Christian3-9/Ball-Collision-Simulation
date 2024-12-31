#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord; // Texture coordinates attribute

out vec2 TexCoords; // Pass texture coordinates to fragment shader

uniform mat4 model;
uniform mat4 projection;

void main()
{
    gl_Position = projection * model * vec4(aPos, 1.0);
    TexCoords = aTexCoord; // Pass texture coordinates
}
