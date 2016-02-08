#version 330

// Note: this is core in OpenGL 3.1 (glsl 1.40) and later, we use OpenGL 3.0 for the tutorials
//#extension GL_ARB_texture_rectangle : enable

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
//precision highp float;

in vec2 fragCoord_0_1; //from 0 to 1
in vec2 fragCoord_m1_1; //from -1 to 1 use later to recreate position with inverse matrix

uniform sampler2D frameBufferBaseColor;
uniform sampler2D frameBufferPBS;
uniform sampler2D frameBufferNormal;
uniform sampler2D frameBufferDepth;
out vec4 fragmentColor;

void main() {
    vec3 pbs_components = texture(frameBufferPBS, fragCoord_0_1).xyz;
    float metallic = pbs_components.x;
    float specular = pbs_components.y;
    float roughness = pbs_components.z;
    float depth = 1 - (pow(texture(frameBufferDepth, fragCoord_0_1).x, 15)) * 0;
	fragmentColor = vec4(texture(frameBufferNormal, fragCoord_0_1).xyz, 1.0);
    fragmentColor = vec4(pbs_components, 1.0);
	fragmentColor = vec4(texture(frameBufferBaseColor, fragCoord_0_1).xyz, 1.0);
	fragmentColor = vec4(vec3(depth, depth, depth)
	                    //* texture(frameBufferNormal, fragCoord_0_1).xyz
	                    * texture(frameBufferBaseColor, fragCoord_0_1).xyz
	                    //* pbs_components
	                    , 1.0);
}