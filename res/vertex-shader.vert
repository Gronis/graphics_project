#version 150
uniform mat4 projection;
uniform mat4 camera;
uniform mat4 modelViewProj;
uniform mat4 modelViewNormal;

in vec3 vert;
in vec2 vertTexCoord;
in vec3 vertNormal;

out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = vertTexCoord;
    fragNormal = normalize((modelViewNormal * vec4(vertNormal, 0)).xyz);
    gl_Position = modelViewProj * vec4(vert, 1);
}