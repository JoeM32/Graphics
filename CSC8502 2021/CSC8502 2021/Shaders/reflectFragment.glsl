#version 330 core

uniform sampler2D diffuseTex ;
uniform sampler2D bumpTex ;
uniform samplerCube cubeTex ;
uniform vec3 cameraPos ;
uniform float roughness ;

in Vertex {
	vec4 colour ;
	vec2 texCoord ;
	vec3 normal ;
	vec3 tangent ; // New !
	vec3 binormal ; // New !
	vec3 worldPos ;
} IN ;

out vec4 fragColour ;
 
void main ( void ) {
	
	vec4 rvec = vec4(roughness,roughness,roughness,1);

	mat3 TBN = mat3 ( normalize ( IN . tangent ) , normalize ( IN . binormal ) , normalize ( IN . normal ));

	vec3 bumpNormal = texture ( bumpTex , IN . texCoord ). rgb ;
	bumpNormal = normalize ( TBN * normalize ( bumpNormal * 2.0 - 1.0));	

	vec4 diffuse = texture ( diffuseTex , IN . texCoord );

	if(roughness < 0.5)
	{
		bumpNormal = IN. normal;
		diffuse = vec4(1,1,1,1);
	}


	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 reflectDir = reflect ( - viewDir , normalize ( bumpNormal ));
	vec4 reflectTex = texture ( cubeTex , reflectDir );
	fragColour = reflectTex + ( diffuse * 0.25f );
 }
