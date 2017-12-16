#version 330 core

out vec3 vs_out_pos;

uniform mat4 MVP;

const float axis_length = 3.0;

const vec3 data[6] = vec3[]
(
  vec3( 0.0, 0.0, 0.0),
  vec3( axis_length, 0.0, 0.0),
  vec3( 0.0, 0.0, 0.0),
  vec3( 0.0, axis_length, 0.0),
  vec3( 0.0, 0.0, 0.0),
  vec3( 0.0, 0.0, axis_length)
);
 
void main()
{
  gl_Position = MVP * vec4( data[ gl_VertexID ], 1.0);

  vs_out_pos = data[ gl_VertexID ];
}
