#version 330

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

out vec3 vPosition;
out vec3 vNormal;
out vec2 vTexCoords;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;

void main()
{

    vPosition=vec4(aPosition, 1);
    vNormal= vec4(aNormal, 0);

    vPosition = vec3(uModelViewMatrix * vPosition);
	  vNormal = vec3(uNormalMatrix *vNormal);
	  vTexCoords = aTexCoords;

    gl_Position =  uModelViewProjMatrix * vPosition;
}
