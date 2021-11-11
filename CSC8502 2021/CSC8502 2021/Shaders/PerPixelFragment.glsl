#version 330 core

uniform sampler2D diffuseTex ;
uniform vec3 cameraPos ;

uniform vec4 lightColour ;
uniform vec4 lightColourSpecular ;
uniform vec3 lightPos ;
uniform float height ;
uniform float baseRadius ;
uniform vec3 lightDirection ;

in Vertex {
	vec3 colour ;
	vec2 texCoord ;
	vec3 normal ;
	vec3 worldPos ;
} IN ;

out vec4 fragColour ;

void main ( void ) {


	fragColour . rgb = vec3(0,0,0);
	
	/*
	
	//Normal
	
	float constantAttenuation=1.0;
    float linearAttenuation=0.22;
    float quadraticAttenuation=0.20;

	vec3 incident = normalize ( lightPos - IN.worldPos );
	vec3 viewDir = normalize ( cameraPos - IN.worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	vec4 diffuse = texture ( diffuseTex , IN.texCoord );
	float lambert = max ( dot ( incident , IN.normal ) , 0.0f );
	float distance = length ( lightPos - IN.worldPos );
	distance = distance - lightRadius;
	if(distance < 0)
	{
		distance = 0;
	}
	//float attenuation = 1.0 - clamp ( distance / lightRadius , 0.0 , 1.0);
	//float attenuation = 1.0 / clamp((distance * distance),0.001,20000);
	float attenuation = constantAttenuation / ((1+linearAttenuation*distance)*(1+quadraticAttenuation*distance*distance));
	float specFactor = clamp ( dot ( halfDir , IN.normal ) ,0.0 ,1.0);
	specFactor = pow ( specFactor , 60.0 );
	vec3 surface = ( diffuse.rgb * lightColour.rgb );
	fragColour . rgb = surface * lambert * attenuation * lightColour.a;
	fragColour . rgb += ( lightColourSpecular.rgb * specFactor )* attenuation * lightColourSpecular.a *0.33;
	fragColour . rgb += surface * 0.1f ; // ambient !
	
	*/
	
	
	
	//Attempt 1
	
	float cone_dist = dot(IN.worldPos - lightPos, lightDirection);
	float cone_radius = (cone_dist / height) * baseRadius;
	float orth_distance = length((IN.worldPos - lightPos) - cone_dist * lightDirection);
	if(orth_distance < cone_radius)
	{
		fragColour . rgb = lightColourSpecular.rgb ;
	}
	
	
	
	/*vec3 incident = normalize ( lightPos - IN.worldPos );
	fragColour.r = (dot(incident, lightDirection));*/
}
