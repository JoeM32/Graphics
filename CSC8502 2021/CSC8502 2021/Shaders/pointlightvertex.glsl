# version 330 core
2
3 uniform mat4 modelMatrix ;
4 uniform mat4 viewMatrix ;
5 uniform mat4 projMatrix ;
6
7 in vec3 position ;
8
9 uniform float lightRadius ;
10 uniform vec3 lightPos ;
11 uniform vec4 lightColour ;
12
13 void main ( void ) {
14 vec3 scale = vec3 ( lightRadius );
15 vec3 worldPos = ( position * scale ) + lightPos ;
16 gl_Position = ( projMatrix * viewMatrix ) * vec4 ( worldPos , 1.0);
17 }