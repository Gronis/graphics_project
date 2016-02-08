#version 330
uniform mat4 modelViewProj;
uniform mat4 modelViewNormal;
uniform vec2 uv_scale;

layout (location = 0) in vec3 vert;
layout (location = 1) in vec2 vertTexCoord;
layout (location = 2) in vec3 vertNormal;

out vec3 fragNormal;
out vec2 fragTexCoord;

void main() {
    // Pass the tex coord straight through to the fragment shader
    fragTexCoord = vertTexCoord * uv_scale;
    fragNormal = normalize((modelViewNormal * vec4(vertNormal, 0)).xyz);
    gl_Position = modelViewProj * vec4(vert, 1);
}


