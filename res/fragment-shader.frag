#version 150

uniform sampler2D diffuse_tex; //this is the texture

in vec2 fragTexCoord; //this is the texture coord
in vec3 fragNormal;
out vec4 finalColor; //this is the output color of the pixel

void main() {
    //finalColor = vec4(fragNormal,1);
    vec4 sample = texture(diffuse_tex, fragTexCoord);
    if(sample.a < 0.1f){
        discard;
    }
    float blend = dot(vec3(0,0,1), fragNormal);

    finalColor = sample * blend + sample * 0.7f * (1 - blend);
}