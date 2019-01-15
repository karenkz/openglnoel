#version 330 core

layout(location = 0) in vec3 aPosition;
layout(location = 1) in vec3 aNormal;
layout(location = 2) in vec2 aTexCoords;

uniform mat4 uModelViewProjMatrix;
uniform mat4 uModelViewMatrix;
uniform mat4 uNormalMatrix;

out vec3 vViewSpacePosition;
out vec3 vViewSpaceNormal;
out vec2 vTexCoords;

 void main(){

    vec4 vertexPosition = vec4(aVertexPosition, 1);
    vec4 vertexNormal = vec4(aVertexNormal, 0);


    vViewSpacePosition = vec3(uModelViewMatrix * vertexPosition);
    vViewSpaceNormal = vec3(uNormalMatrix * vertexNormal);
    vTexCoords = aVertexTexCoords;

    gl_Position = uModelViewProjMatrix * vertexPosition;

}
