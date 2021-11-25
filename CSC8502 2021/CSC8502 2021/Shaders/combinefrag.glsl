#version 330 core
uniform sampler2D diffuseTex ;
uniform sampler2D diffuseLight ;
uniform sampler2D specularLight ;
in Vertex {
vec2 texCoord ;
} IN ;
out vec4 fragColour ;

void main ( void ) {
vec3 diffuse = texture ( diffuseTex , IN . texCoord ). xyz ;
vec3 light = texture ( diffuseLight , IN . texCoord ). xyz ;
vec3 specular = texture ( specularLight , IN . texCoord ). xyz ;

fragColour . xyz = diffuse * 0.2; // ambient
fragColour . xyz += diffuse * light ; // lambert
fragColour . xyz += specular / 2; // Specular
fragColour . xyz += light / 2 ; // Specular
fragColour . a = 1.0;
float gamma = 2.2;
fragColour.rgb = pow(fragColour.rgb, vec3(1.0/gamma));
}