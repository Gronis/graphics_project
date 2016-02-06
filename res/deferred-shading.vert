#version 330
// This vertex shader simply outputs the input coordinates to the rasterizer. It only uses 2D coordinates.
layout (location = 0) in vec2 position;

out vec2 fragCoord_0_1;
out vec2 fragCoord_m1_1;

void main()
{
	gl_Position = vec4(position, 0, 1.0);
	fragCoord_0_1 = (position + 1.0) / 2.0;
	fragCoord_m1_1 = position;
}