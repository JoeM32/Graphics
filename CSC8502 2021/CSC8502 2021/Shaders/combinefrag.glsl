#version 330 core
uniform sampler2D diffuseTex ;
uniform sampler2D diffuseLight ;
uniform sampler2D specularLight ;
uniform sampler2D miniMap ;
uniform float height;
uniform float width;

in Vertex {
vec2 texCoord ;
} IN ;
out vec4 fragColour ;

void main ( void ) {
vec3 diffuse = texture ( diffuseTex , IN . texCoord ). xyz ;
vec3 light = texture ( diffuseLight , IN . texCoord ). xyz ;
vec3 specular = texture ( specularLight , IN . texCoord ). xyz ;
vec3 map = texture ( miniMap , vec2(IN . texCoord.x / (300/width + 50/width),IN . texCoord.y / (300/height + 50/height)) ). xyz ;

float zone = 0;

fragColour . xyz = diffuse * 0.2; // ambient
fragColour . xyz += diffuse * light ; // lambert
fragColour . xyz += specular / 2; // Specular
fragColour . xyz += light / 2 ; // Specular

if(length(vec2(IN.texCoord.x * width,IN.texCoord.y * height) - vec2(200,200)) < 150)//shouldnt be hardcoded but opengl was complaing about optimization. This who shader is obvioslu pretty inefficent, correct use of stencil buffer would be better.
{
zone = 1;
}

fragColour.rgb = mix(fragColour.rgb,map,zone);

fragColour . a = 1.0;
float gamma = 2.2;
fragColour.rgb = pow(fragColour.rgb, vec3(1.0/gamma));
}