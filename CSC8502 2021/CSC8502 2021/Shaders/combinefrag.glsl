# version 330 core
2 uniform sampler2D diffuseTex ;
3 uniform sampler2D diffuseLight ;
4 uniform sampler2D specularLight ;
5
6 in Vertex {
7 vec2 texCoord ;
8 } IN ;
9
10 out vec4 fragColour ;
11
12 void main ( void ) {
13 vec3 diffuse = texture ( diffuseTex , IN . texCoord ). xyz ;
14 vec3 light = texture ( diffuseLight , IN . texCoord ). xyz ;
15 vec3 specular = texture ( specularLight , IN . texCoord ). xyz ;
16
17 fragColour . xyz = diffuse * 0.1; // ambient
18 fragColour . xyz += diffuse * light ; // lambert
19 fragColour . xyz += specular ; // Specular
20 fragColour . a = 1.0;
21 }