 # version 330 core
2
3 in vec3 position ;
4 in vec2 texCoord ;
5
6 out Vertex {
7 vec2 texCoord ;
8 } OUT ;
9
10 void main ( void ) {
11 gl_Position = vec4 ( position , 1.0);
12 OUT . texCoord = texCoord ;
13 }
