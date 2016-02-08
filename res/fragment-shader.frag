#version 330

uniform sampler2D diffuse_tex; //this is the texture

in vec2 fragTexCoord; //this is the texture coord
in vec3 fragNormal;
in vec3 fragColor;
//out vec4 finalColor; //this is the output color of the pixel

layout (location = 0) out vec3 base_color;
layout (location = 1) out vec3 pbs;
layout (location = 2) out vec3 normal;

void main() {
    //finalColor = vec4(fragNormal,1);
    vec4 sample = texture(diffuse_tex, fragTexCoord);
    //if(sample.a < 0.1f){
    //    discard;
    //}
    float metallic = 0.5f;
    float specular = 0.5f;
    float roughness = 1.0f;

    base_color = sample.rgb * fragColor;
    pbs = vec3(metallic, specular, roughness);
    normal = fragNormal;

    //float blend = dot(vec3(0,0,1), fragNormal);
    //finalColor = sample * blend + sample * 0.7f * (1 - blend);
}