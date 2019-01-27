#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

out vec3 fColor;

uniform vec3 uDirectionalLightDir;
uniform vec3 uDirectionalLightIntensity;

uniform vec3 uPointLightPosition;
uniform vec3 uPointLightIntensity;

uniform vec3 uKd;
uniform sampler2D uKdSampler;


void main()
{
    float distToPointLight = length(uPointLightPosition - vPosition);
    vec3 dirToPointLight = (uPointLightPosition - vPosition) / distToPointLight;
    vec3 kd = uKd * vec3(texture(uKdSampler, vTexCoords));

    fColor = kd * (uDirectionalLightIntensity * max(0.f, dot(vNormal, uDirectionalLightDir)) + uPointLightIntensity * max(0., dot(vNormal, dirToPointLight)) / (distToPointLight * distToPointLight));
}
