#version 130

in vec2 vs_out_tex;

out vec4 fs_out_col;

uniform sampler2D frameTex;
uniform float t;
uniform mat3 color_matrix = mat3( 0.393f, 0.349f, 0.272f, 
								  0.769f, 0.686f, 0.534f,
								  0.189f, 0.168f, 0.131f );

//1.:
uniform sampler2D frameDepthTex;
uniform float near;
uniform float far;
//
float linearizedDepth(vec2 uv)
{
	float depth = texture(frameDepthTex, uv).x;
	return (2.0 * near) / (far + near - depth * (far - near));
}

//2.:
uniform float width;
uniform float height;
vec2 uv = vs_out_tex;
float xStep = 1.0 / width;
float yStep = 1.0 / height;
vec2 pixelCoordsAround[9] = {
	vec2(uv.x-xStep, uv.y+yStep),		vec2(uv.x, uv.y+yStep),		vec2(uv.x+xStep, uv.y+yStep),
	vec2(uv.x-xStep, uv.y      ),		vec2(uv.x, uv.y      ),		vec2(uv.x+xStep, uv.y      ),
	vec2(uv.x-xStep, uv.y-yStep),		vec2(uv.x, uv.y-yStep),		vec2(uv.x+xStep, uv.y-yStep)
};
vec4 blurredColor()
{
	vec4 color = vec4(0.0);
	for (int i=0; i<9; ++i) {
		color += texture(frameTex, pixelCoordsAround[i]);
	}
	color /= 9.0;
	return color;
}

//3.:
bool isEdge(float threshold)
{
	float myDepth = linearizedDepth(pixelCoordsAround[4]);
	
	for (int i=0; i<9; ++i) {
		float currentDepth = linearizedDepth(pixelCoordsAround[i]);
		float depthDifference = abs(myDepth - currentDepth);
		if (depthDifference >= threshold)
			return true;
	}
	return false;
}

void main()
{
	vec4 originalColor = texture(frameTex, vs_out_tex);
	vec4 sepiaColored = vec4(
								color_matrix*originalColor.xyz, 
								1
							);
	//
	fs_out_col = mix(originalColor, sepiaColored, t);

	//1.:
	float c = linearizedDepth(vs_out_tex);
	fs_out_col = vec4(c, c, c, 1);

	//2.:
	fs_out_col = blurredColor();

	//3.:
	if (isEdge(0.1))
		fs_out_col = vec4(vec3(1), 1);
	else
		fs_out_col = vec4(vec3(0), 1);

	//4.:
	if (isEdge(0.1))
		fs_out_col = blurredColor();
	else
		fs_out_col = originalColor;
}