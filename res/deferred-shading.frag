#version 330

// Note: this is core in OpenGL 3.1 (glsl 1.40) and later, we use OpenGL 3.0 for the tutorials
//#extension GL_ARB_texture_rectangle : enable

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
//precision highp float;

float radius = 1.0f;

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
    float depth = texture(frameBufferDepth, fragCoord_0_1).x - 0.0007f;
    float ssao_contribution = 0;
    vec3 v = texture(frameBufferNormal, fragCoord_0_1).xyz;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(0.005f, 0.006f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(0.006f, -0.007f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(-0.006f, -0.004f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(-0.005f, 0.004f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(0.001f, 0.002f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(0.003f, -0.003f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(-0.002f, -0.003f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(-0.003f, 0.002f) * radius).x > depth? 0.0625 : 0;

    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(0.010f, 0.008f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(0.009f, -0.008f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(-0.010f, -0.007f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(-0.007f, 0.007f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(0.004f, 0.002f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(0.006f, -0.003f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(-0.002f, -0.008f) * radius).x > depth? 0.0625 : 0;
    ssao_contribution += texture(frameBufferDepth, fragCoord_0_1 + vec2(-0.006f, 0.002f) * radius).x > depth? 0.0625 : 0;

	fragmentColor = vec4(texture(frameBufferNormal, fragCoord_0_1).xyz, 1.0);
    fragmentColor = vec4(pbs_components, 1.0);
	fragmentColor = vec4(texture(frameBufferBaseColor, fragCoord_0_1).xyz, 1.0);
	fragmentColor = vec4(vec3(ssao_contribution, ssao_contribution, ssao_contribution)
	                    * texture(frameBufferNormal, fragCoord_0_1).xyz
	                    * texture(frameBufferBaseColor, fragCoord_0_1).xyz
	                    //* pbs_components
	                    , 1.0);
}