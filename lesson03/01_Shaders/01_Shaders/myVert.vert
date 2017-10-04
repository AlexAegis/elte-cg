#version 130

// lokális változók: két tömb
vec4 positions[4] = vec4[4](
	vec4(-1, -1, 0, 1),
	vec4( 1,  -1, 0, 1),
	vec4(-1,  1, 0, 1),
	vec4( 1,  1, 0, 1)
);

vec4 colors[4] = vec4[4](
	vec4(100, 34, 23, 1),
	vec4(-40, 1, -0.1, 1),
	vec4(20, -72, 0.5, 1),
	vec4(10, 65, 1, 41)
);

// kimeneti változó: a csúcspont színe
out vec4 vs_out_col;

void main()
{
	// gl_VertexID: https://www.khronos.org/registry/OpenGL-Refpages/gl4/html/gl_VertexID.xhtml
	gl_Position = positions[gl_VertexID];
	vs_out_col	= colors[gl_VertexID];
}