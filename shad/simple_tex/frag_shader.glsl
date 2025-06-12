#version 330 core
in vec2 TexCoord;
out vec4 FragColor;
uniform sampler2D ourTexture;
void main() {

    FragColor = texture(ourTexture, TexCoord) ;
    // FragColor = vec4(.5, .3, .3, 1.0); // Ensure alpha is set to 1.0 for full opacity
}