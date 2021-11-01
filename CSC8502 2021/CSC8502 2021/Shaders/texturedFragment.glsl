#version 330 core
uniform sampler2D diffuseTex ;

in Vertex {
	vec2 texCoord ;
} IN ;

out vec4 fragColour ;
void main ( void ) {
	fragColour = texture ( diffuseTex , IN.texCoord );
	if(fragColour.r < 0.5)
	{
		discard;
	}
}