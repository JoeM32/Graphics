# version 330 core
2
3 uniform sampler2D depthTex ;
4 uniform sampler2D normTex ;
5
6 uniform vec2 pixelSize ; // reciprocal of resolution
7 uniform vec3 cameraPos ;
8
9 uniform float lightRadius ;
10 uniform vec3 lightPos ;
11 uniform vec4 lightColour ;
12 uniform mat4 inverseProjView ;
13
14 out vec4 diffuseOutput ;
15 out vec4 specularOutput ;
void main ( void ) {
17 vec2 texCoord = vec2 ( gl_FragCoord . xy * pixelSize );
18 float depth = texture ( depthTex , texCoord . xy ). r ;
19 vec3 ndcPos = vec3 ( texCoord , depth ) * 2.0 - 1.0;
20 vec4 invClipPos = inverseProjView * vec4 ( ndcPos , 1.0);
21 vec3 worldPos = invClipPos . xyz / invClipPos . w ;
22
23 float dist = length ( lightPos - worldPos );
24 float atten = 1.0 - clamp ( dist / lightRadius , 0.0 , 1.0);
25
26 if( atten == 0.0) {
27 discard ;
28 }
29
30 vec3 normal = normalize ( texture ( normTex , texCoord . xy ). xyz *2.0 -1.0);
31 vec3 incident = normalize ( lightPos - worldPos );
32 vec3 viewDir = normalize ( cameraPos - worldPos );
33 vec3 halfDir = normalize ( incident + viewDir );
34
35 float lambert = clamp ( dot ( incident , normal ) ,0.0 ,1.0);
36 float rFactor = clamp ( dot ( halfDir , normal ) ,0.0 ,1.0);
37 float specFactor = clamp ( dot ( halfDir , normal ) ,0.0 ,1.0);
38 specFactor = pow ( specFactor , 60.0 );
39 vec3 attenuated = lightColour . xyz * atten ;
40 diffuseOutput = vec4 ( attenuated * lambert , 1.0);
41 specularOutput = vec4 ( attenuated * specFactor * 0.33 , 1.0);
42 }
