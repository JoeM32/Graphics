#version 330 core
uniform sampler2D rockTex ;
uniform sampler2D rockBumpTex ;

uniform sampler2D grassTex ;
uniform sampler2D grassBumpTex ;

uniform sampler2D sandTex ;
uniform sampler2D sandBumpTex ;

uniform sampler2D mudTex ;
uniform sampler2D mudBumpTex ;

uniform sampler2D noise ;

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
	vec3 localPos;
} IN ;

out vec4 fragColour ;

void main ( void ) {

	vec4 col = vec4(0,0,0,1);
	vec4 diffuse = vec4(0,0,0,1);
	vec4 bump = vec4(0,0,0,1);
	vec3 blending = abs( IN.normal );
	blending = normalize(max(blending, 0.00001)); // Force weights to sum to 1.0
	float b = (blending.x + blending.y + blending.z);
	blending /= vec3(b, b, b);
	
	float rockScale = 0.005;
	float grassScale = 0.005;
	float sandScale = 0.005;
	float mudScale = 0.008;

	float beach = clamp(((IN.worldPos.y) / (700)) * 5,0,1);
	float steepness = clamp(((length(IN.normal - vec3(0,1,0))) - 0.7) * 4,0,1);
	float mud = clamp(texture(noise, IN.worldPos.xz * 0.0002).r * 8 - 4,0,1);
	
	vec4 rockTexTriplanar = (texture ( rockTex , IN . worldPos.yz * rockScale ) * blending.x) + (texture ( rockTex , IN . worldPos.xz * rockScale ) * blending.y) + (texture ( rockTex , IN . worldPos.xy * rockScale ) * blending.z);
	vec4 rockBumpTexTriplanar = (texture ( rockBumpTex , IN . worldPos.yz * rockScale ) * blending.x) + (texture ( rockBumpTex , IN . worldPos.xz * rockScale ) * blending.y) + (texture ( rockBumpTex , IN . worldPos.xy * rockScale ) * blending.z);
	vec4 grassTexTriplanar = (texture ( grassTex , IN . worldPos.yz * grassScale ) * blending.x) + (texture ( grassTex , IN . worldPos.xz * grassScale ) * blending.y) + (texture ( grassTex , IN . worldPos.xy * grassScale ) * blending.z);
	vec4 grassBumpTexTriplanar = (texture ( grassBumpTex , IN . worldPos.yz * grassScale ) * blending.x) + (texture ( grassBumpTex , IN . worldPos.xz * grassScale ) * blending.y) + (texture ( grassBumpTex , IN . worldPos.xy * grassScale ) * blending.z);
	vec4 sandTexTriplanar = (texture ( sandTex , IN . worldPos.yz * sandScale ) * blending.x) + (texture ( sandTex , IN . worldPos.xz * sandScale ) * blending.y) + (texture ( sandTex , IN . worldPos.xy * sandScale ) * blending.z);
	vec4 sandBumpTexTriplanar = (texture ( sandBumpTex , IN . worldPos.yz * sandScale ) * blending.x) + (texture ( sandBumpTex , IN . worldPos.xz * sandScale ) * blending.y) + (texture ( sandBumpTex , IN . worldPos.xy * sandScale ) * blending.z);
	vec4 mudTexTriplanar = (texture ( mudTex , IN . worldPos.yz * mudScale ) * blending.x) + (texture ( mudTex , IN . worldPos.xz * mudScale ) * blending.y) + (texture ( mudTex , IN . worldPos.xy * mudScale ) * blending.z);
	vec4 mudBumpTexTriplanar = (texture ( mudBumpTex , IN . worldPos.yz * mudScale ) * blending.x) + (texture ( mudBumpTex , IN . worldPos.xz * mudScale ) * blending.y) + (texture ( mudBumpTex , IN . worldPos.xy * mudScale ) * blending.z);
	
	
	diffuse = mix(grassTexTriplanar, mudTexTriplanar, mud);
	diffuse = mix(diffuse, rockTexTriplanar, steepness);
	diffuse = mix(sandTexTriplanar, diffuse, beach);
	
	bump = mix(grassBumpTexTriplanar, mudBumpTexTriplanar, mud);
	bump = mix(bump, rockBumpTexTriplanar, steepness);
	bump = mix(sandBumpTexTriplanar, bump, beach);
	
	vec3 normal = bump.rgb ;

	vec3 incident = normalize ( lightPos - IN . worldPos );
	vec3 viewDir = normalize ( cameraPos - IN . worldPos );
	vec3 halfDir = normalize ( incident + viewDir );

	mat3 TBN = mat3 ( normalize ( IN . tangent ) ,
	normalize ( IN . binormal ) , normalize ( IN . normal ));
	normal = normalize ( TBN * normal * 2.0 - 1.0);
	
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
	fragColour . rgb = surface * attenuation * lambert ; // diffuse
	fragColour . rgb +=( lightColour . rgb * attenuation * specFactor )*0.33;
	fragColour . rgb *= shadow ; // shadowing factor
	fragColour . rgb += surface * 0.1f ; // ambient
	fragColour . a = diffuse . a ; // alpha
	
	fragColour = col;
}

