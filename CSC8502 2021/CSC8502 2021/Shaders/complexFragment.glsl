#version 330 core
uniform sampler2D diffuseTex ;
uniform sampler2D diffuseTex2 ;
uniform float blending;

in Vertex {
	noperspective vec2 texCoord ;
	noperspective vec4 colour;
} IN ;

out vec4 fragColour ;
void main ( void ) {
	fragColour = mix(texture ( diffuseTex2 , IN.texCoord ),texture ( diffuseTex , IN.texCoord ),0.5);
	fragColour = mix(IN.colour,fragColour,blending);
}