 # version 330 core
2
3 uniform sampler2D diffuseTex ; // Diffuse texture map
4 uniform sampler2D bumpTex ; // Bump map
5
6 in Vertex {
7 vec4 colour ;
8 vec2 texCoord ;
9 vec3 normal ;
10 vec3 tangent ;
11 vec3 binormal ;
12 vec3 worldPos ;
13 } IN ;
14
15 out vec4 fragColour [2]; // Our final outputted colours !
16
17 void main ( void ) {
18 mat3 TBN = mat3 ( normalize ( IN . tangent ) ,
19 normalize ( IN . binormal ) ,
20 normalize ( IN . normal ));
21
22 vec3 normal = texture2D ( bumpTex , IN . texCoord ). rgb * 2.0 - 1.0;
23 normal = normalize ( TBN * normalize ( normal ));
24
25 fragColour [0] = texture2D ( diffuseTex , IN . texCoord );
26 fragColour [1] = vec4 ( normal . xyz * 0.5 + 0.5 ,1.0);
27 }
