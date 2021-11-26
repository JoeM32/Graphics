#version 330 core
uniform sampler2D DiffuseTex ;
uniform sampler2D BumpTex ;

uniform sampler2D shadowTex ; // NEW !

uniform float size;

uniform vec4 lightColour ;
uniform vec3 lightPos ;
uniform vec3 cameraPos ;
uniform float lightRadius ;
in Vertex {
	vec3 colour ;
	vec2 texCoord ;
	vec3 normal ;
	vec3 tangent ;
	vec3 binormal ;
	vec3 worldPos ;
	vec4 shadowProj ; // New !
} IN ;

out vec4 fragColour[2] ;

void main ( void ) {
	
	vec3 blending = abs( IN.normal );
	blending = normalize(max(blending, 0.00001)); // Force weights to sum to 1.0
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);
	
	vec3 incident = normalize ( lightPos - IN . worldPos );
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );
	
	mat3 TBN = mat3 ( normalize ( IN . tangent ) ,
	normalize ( IN . binormal ) , normalize ( IN . normal ));
	
	//vec4 diffuse = texture ( DiffuseTex , IN.texCoord );
	//vec3 bumpNormal = texture ( BumpTex , IN.texCoord ).rgb;
	
	vec4 diffusex = texture ( DiffuseTex , IN . worldPos.yz * size );
	vec4 diffusey = texture ( DiffuseTex , IN . worldPos.xz * size );
	vec4 diffusez = texture ( DiffuseTex , IN . worldPos.xy * size );
	vec4 diffuse = (diffusex * blending.x) + (diffusey * blending.y) + (diffusez * blending.z);
	
	vec4 bumpx = texture ( BumpTex , IN . worldPos.yz * size );
	vec4 bumpy = texture ( BumpTex , IN . worldPos.xz * size );
	vec4 bumpz = texture ( BumpTex , IN . worldPos.xy * size );
	vec3 bumpNormal = (bumpx * blending.x).rgb + (bumpy * blending.y).rgb + (bumpz * blending.z).rgb;
	
	

	vec3 normal = normalize ( TBN * bumpNormal * 2.0 - 1.0);
	float lambert = max ( dot ( incident , normal ) , 0.0f );
	float distance = length ( lightPos - IN . worldPos );
	float attenuation = 1.0f - clamp ( distance / lightRadius , 0.0 , 1.0);

	float specFactor = clamp ( dot ( halfDir , normal ) ,0.0 ,1.0);
	specFactor = pow ( specFactor , 60.0 );
	float shadow = 1.0; // New !

	vec3 shadowNDC = IN . shadowProj . xyz / IN . shadowProj . w ;
	if( abs ( shadowNDC . x ) < 1.0f &&
	abs ( shadowNDC . y ) < 1.0f &&
	abs ( shadowNDC . z ) < 1.0f ) {
		vec3 biasCoord = shadowNDC *0.5f + 0.5f ;
		float shadowZ = texture ( shadowTex , biasCoord . xy ). x;
		if( shadowZ < biasCoord . z ) {
			shadow = 0.0f ;
		}
	}
	vec3 surface = ( diffuse . rgb * lightColour . rgb ); // Base colour
	diffuse . rgb = surface * attenuation * lambert ; // diffuse
	diffuse . rgb +=( lightColour . rgb * attenuation * specFactor )*0.33;
	diffuse . rgb *= shadow ; // shadowing factor
	diffuse . rgb += surface * 0.1f ; // ambient
	diffuse . rgb = mix (vec3(0,0,0) ,diffuse . rgb,shadow) ; 
	fragColour[0] = diffuse;
	fragColour [1] = vec4 ( normal . xyz * 0.5 + 0.5 ,1.0);
}