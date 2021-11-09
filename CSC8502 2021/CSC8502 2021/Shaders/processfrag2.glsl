#version 330 core

uniform sampler2D sceneTex ;
uniform sampler2D sceneTex2 ;



in Vertex {
	vec2 texCoord ;
} IN ;

out vec4 fragColor ;


void main ( void ) {

	
	fragColor = mix(texture2D( sceneTex , IN.texCoord.xy), texture2D( sceneTex2 , IN.texCoord.xy),0.5);
}