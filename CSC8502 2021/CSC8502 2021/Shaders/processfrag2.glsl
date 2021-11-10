#version 330 core

uniform sampler2D sceneTex ;
uniform sampler2D sceneTex2 ;

in vec2 resolution;

in vec4 gl_FragCoord;

in Vertex {
	vec2 texCoord ;
} IN ;

out vec4 fragColor ;


void main ( void ) {
	float a = 0;
	fragColor = vec4(0,0,0,1);
	if((gl_FragCoord.x/1280) > 0.5)
	{
		a = 1;
	}
	fragColor = mix(texture2D( sceneTex , IN.texCoord.xy), texture2D( sceneTex2 , IN.texCoord.xy), a);
}