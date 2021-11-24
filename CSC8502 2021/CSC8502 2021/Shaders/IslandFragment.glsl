#version 330 core
uniform sampler2D rockTex ;
uniform sampler2D rockBumpTex ;

uniform sampler2D grassTex ;
uniform sampler2D grassBumpTex ;

uniform sampler2D sandTex ;
uniform sampler2D sandBumpTex ;

uniform sampler2D mudTex ;
uniform sampler2D mudBumpTex ;

uniform sampler2D shadowTex ; // NEW !

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
	
	vec4 col = vec4(0,0,0,1);
	vec4 col2 = vec4(1,0,0,1);
	float steepness = (((length(IN.normal - vec3(0,1,0))) - 0.85) * 4);
	
	
	
	mat3 TBN = mat3 ( normalize ( IN . tangent ) ,
	normalize ( IN . binormal ) , normalize ( IN . normal ));
	
	vec4 diffuse = texture ( rockTex , IN . worldPos.xz * 0.005 );
	vec4 diffusex = texture ( rockTex , IN . worldPos.yz * 0.005 );
	vec4 diffusey = texture ( rockTex , IN . worldPos.xz * 0.005 );
	vec4 diffusez = texture ( rockTex , IN . worldPos.xy * 0.005 );
	
	vec3 bumpNormal = texture ( rockBumpTex , IN . worldPos.xz * 0.005 ).rgb;// //texture ( rockBumpTex , IN . texCoord ). rgb ;

	vec4 tex = (diffusex * blending.x) + (diffusey * blending.y) + (diffusez * blending.z);

	vec3 normal = normalize ( TBN * bumpNormal * 2.0 - 1.0);
	float lambert = max ( dot ( incident , bumpNormal ) , 0.0f );
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
	diffuse . a = diffuse . a ; // alpha
	fragColour[0].a = 1;
	fragColour[0].rgb = diffuse.rgb;
	//fragColour[0].rgb = IN.tangent.rgb;
	fragColour [1] = vec4 ( normal . xyz * 0.5 + 0.5 ,1.0);
}