#version 130

// bemeneti változó - most a vertex shader-bõl (vagyis ottani out)
in vec4 vs_out_col;

// kimeneti változó - a fragment színe
out vec4 fs_out_col;

void main()
{
	//				  R, G, B, A
	fs_out_col = vs_out_col;

	//				  R, G, B, A
	//fs_out_col = vec4(1, 1, 1, 1);
}