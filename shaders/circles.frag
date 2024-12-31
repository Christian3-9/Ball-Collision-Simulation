#version 330 core
out vec4 FragColor;

in vec2 TexCoords; // Receive texture coordinates from vertex shader
uniform sampler2D ourTexture; // Texture sampler
uniform vec3 ballColor; // Ball color

void main()
{
    vec4 texColor = texture(ourTexture, TexCoords); // Sample the texture
    if (texColor.a < 0.1) // Discard transparent fragments (for circular shape)
        discard;

    // Mix the texture color with the ball color
    FragColor = vec4(ballColor, 1.0) * texColor;
}
