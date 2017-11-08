#version 330 core

layout (location = 8) in vec3 vertices;
layout (location = 9) in vec3 normals;

out vec3 reflectN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 cameraPos;

void main()
{
  vec4 worldPos = model * vec4(vertices, 1.0);
  vec3 worldNorm = normalize(mat3(transpose(inverse(model))) * normals);
  reflectN = normalize(reflect(normalize(vec3(worldPos) - cameraPos), worldNorm));
  gl_Position = projection * view * worldPos;
  // gl_Position = vec4(vertices, 1.0);
}
