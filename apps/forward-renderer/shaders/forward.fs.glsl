#version 330

in vec3 vPosition;
in vec3 vNormal;
in vec2 vTexCoords;

//uniform sampler2D tex;
//uniform sampler2D nuage;

out vec3 fFragColor;

void main()
{
  fFragColor = vec3(1, 0, 0);
      //  fFragColor = texture(tex, vTexCoords)+texture(nuage,vTexCoords);
}
