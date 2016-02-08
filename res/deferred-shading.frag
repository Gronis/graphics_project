#version 330

// Note: this is core in OpenGL 3.1 (glsl 1.40) and later, we use OpenGL 3.0 for the tutorials
//#extension GL_ARB_texture_rectangle : enable

// required by GLSL spec Sect 4.5.3 (though nvidia does not, amd does)
//precision highp float;

float radius = 0.01f;

in vec2 fragCoord_0_1; //from 0 to 1
in vec2 fragCoord_m1_1; //from -1 to 1 use later to recreate position with inverse matrix

uniform mat4 projInverse;

uniform sampler2D frameBufferBaseColor;
uniform sampler2D frameBufferPBS;
uniform sampler2D frameBufferNormal;
uniform sampler2D frameBufferDepth;
out vec4 fragmentColor;

vec3 get_position(float depth);

float ambient_occlution(vec3 rvec, mat3 tbn, vec3 position, float depth);

void main() {
    vec3 pbs_components = texture(frameBufferPBS, fragCoord_0_1).xyz;
    float metallic = pbs_components.x;
    float specular = pbs_components.y;
    float roughness = pbs_components.z;
    float depth = texture(frameBufferDepth, fragCoord_0_1).x;
    float occluded_inverse = 1;
    vec3 position = get_position(depth);
    vec3 fls = texture(frameBufferNormal, fragCoord_0_1).rgb * 0.5f;
    vec3 fake_lighting = vec3(fls.x + fls.y, fls.x + fls.y, fls.x + fls.y);
    vec3 normal = texture(frameBufferNormal, fragCoord_0_1).xyz;

    depth -= 0.0001f;
    //depth *= 0.99995f;


    //rotate hemispear around normal
    vec3 rvec = vec3(1,0,0);
    vec3 tangent = normalize(rvec - normal * dot(rvec, normal));
    vec3 bitangent = cross(normal, tangent);
    mat3 tbn = mat3(tangent, bitangent, normal);

    occluded_inverse -= ambient_occlution(vec3( 0.5f,  0.6f, 0.6f), tbn, position, depth);
    occluded_inverse -= ambient_occlution(vec3( 0.6f, -0.7f, 0.3f), tbn, position, depth);
    occluded_inverse -= ambient_occlution(vec3(-0.6f, -0.4f, 0.2f), tbn, position, depth);
    occluded_inverse -= ambient_occlution(vec3(-0.5f,  0.4f, 0.2f), tbn, position, depth);
    occluded_inverse -= ambient_occlution(vec3( 0.1f,  0.2f, 0.0f), tbn, position, depth);
    occluded_inverse -= ambient_occlution(vec3( 0.3f, -0.3f, 0.2f), tbn, position, depth);
    occluded_inverse -= ambient_occlution(vec3(-0.2f, -0.3f, 0.6f), tbn, position, depth);
    occluded_inverse -= ambient_occlution(vec3(-0.3f,  0.2f, 0.2f), tbn, position, depth);



	fragmentColor = vec4(texture(frameBufferNormal, fragCoord_0_1).xyz, 1.0);
    fragmentColor = vec4(pbs_components, 1.0);
	fragmentColor = vec4(texture(frameBufferBaseColor, fragCoord_0_1).xyz, 1.0);
	fragmentColor = vec4(vec3(occluded_inverse, occluded_inverse, occluded_inverse)
	                    //* fake_lighting
	                    * texture(frameBufferBaseColor, fragCoord_0_1).xyz
	                    //* pbs_components
	                    , 1.0);
}

vec3 get_position(float depth)
{
    vec4 projected_pos = vec4(fragCoord_m1_1.x, fragCoord_m1_1.y, depth, 1.0f);
    // Transform by the inverse projection matrix
    vec4 screen_space_pos = projected_pos * projInverse;
    // Divide by w to get the view-space position
    return screen_space_pos.xyz / screen_space_pos.w;
}

float ambient_occlution(vec3 rvec, mat3 tbn, vec3 position, float depth){

//texture(frameBufferDepth, fragCoord_0_1 + vec2(0.005f, 0.006f) * radius).x > depth? 0.0625 : 0;

   // get sample position:
   rvec = tbn * rvec * radius;

    // get sample depth:
   float sampleDepth = texture(frameBufferDepth, rvec.xy + fragCoord_0_1).x;

    // range check & accumulate:
   float rangeCheck= abs(depth - sampleDepth) < (1 - depth) * 0.08f ? 1.0 : 0.0;
   return (sampleDepth < depth ? 0.125 /*0.0625*/ : 0.0) * rangeCheck;

/*
    // get sample position:
   rvec = tbn * rvec;
   rvec = rvec * radius + position;

    // project sample position:
   vec4 offset = vec4(rvec, 1.0);
   offset = proj * offset;
   offset.xy /= offset.w;
   offset.xy = offset.xy * 0.5 + 0.5;

    // get sample depth:
   float sampleDepth = texture(frameBufferDepth, offset.xy).x;

    // range check & accumulate:
   float rangeCheck= abs(position.z - sampleDepth) < radius ? 1.0 : 0.0;
   return (sampleDepth < rvec.z ? 0.0625 : 0.0) * rangeCheck;
   */
}