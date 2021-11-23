#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 shadowMatrix;
uniform mat4 modelMatrix;

void main()
{
    gl_Position = shadowMatrix * modelMatrix * vec4(aPos, 1.0);
}  