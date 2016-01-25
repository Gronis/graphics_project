#version 150
uniform mat4 projection;
uniform mat4 camera;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;

out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = vertTexCoord;
    fragNormal = vertNormal;
    
    gl_Position = projection * camera * vec4(vert, 1);
}