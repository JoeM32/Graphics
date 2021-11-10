#version 330 core

uniform sampler2D diffuseTex ;
uniform vec3 cameraPos ;

uniform vec4 lightColour[4] ;
uniform vec4 lightColourSpecular[4] ;
uniform vec3 lightPos[4] ;
uniform float lightRadius[4] ;

in Vertex {
	vec3 colour ;
	vec2 texCoord ;
	vec3 normal ;
	vec3 worldPos ;
} IN ;

out vec4 fragColour ;

void main ( void ) {
	vec3 incident = normalize ( lightPos[0] - IN.worldPos );
	vec3 viewDir = normalize ( cameraPos - IN.worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	vec4 diffuse = texture ( diffuseTex , IN.texCoord );
	float lambert = max ( dot ( incident , IN.normal ) , 0.0f );
	float distance = length ( lightPos[0] - IN.worldPos );
	distance = distance - lightRadius[0];
	distance = clamp(distance,0,10000);
	float attenuation = 1.0 - clamp ( distance / lightRadius[0] , 0.0 , 1.0);
	//float attenuation = 1.0 / clamp((distance * distance),0.001,20000);
	float specFactor = clamp ( dot ( halfDir , IN.normal ) ,0.0 ,1.0);
	specFactor = pow ( specFactor , 60.0 );
	vec3 surface = ( diffuse.rgb * lightColour[0].rgb );
	fragColour . rgb = surface * lambert * attenuation * lightColour[0].a;
	fragColour . rgb += ( lightColourSpecular[0].rgb * specFactor )* attenuation * lightColourSpecular[0].a *0.33;
	fragColour . rgb += surface * 0.1f ; // ambient !
	fragColour . a = diffuse.a ;
	fragColour = lightColourSpecular[2];
}
