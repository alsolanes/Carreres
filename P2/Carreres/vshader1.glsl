#version 150

uniform mat4 model_view;
uniform mat4 projection;
in vec4 vPosition;
in vec4 vColor;

out vec4 color;



void main() 
{
  gl_Position = projection*model_view*vPosition;
  //gl_Position = es passa a coordenades homogenies
  color = vColor;
} 
