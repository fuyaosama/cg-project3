#version 330 core

out vec4 fragColor;

uniform samplerCube cubeTex;

in vec3 reflectN;

void main()
{
  // fragColor = vec4(1.0, 0.0, 0.0,1.0);
  fragColor = texture(cubeTex, reflectN);
}
