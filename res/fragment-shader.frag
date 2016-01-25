#version 150

uniform sampler2D tex; //this is the texture

in vec2 fragTexCoord; //this is the texture coord
in vec3 fragNormal;
out vec4 finalColor; //this is the output color of the pixel

void main() {
    //finalColor = texture(tex, fragTexCoord);
    finalColor = vec4(fragNormal / 2 + vec3(0.5f, 0.5f, 0.5f),1);
}