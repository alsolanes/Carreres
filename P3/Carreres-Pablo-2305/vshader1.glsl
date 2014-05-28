#version 150

uniform mat4 model_view;
uniform mat4 projection;
in vec4 vPosition;
in vec4 vColor;

out vec4 color;

struct tipusLlum
{
    vec4 LightPosition;
    vec3 Idifusa;
    vec3 Iambient;
    vec3 Iespecular;
    vec3 atenuacio;
    vec3 direccio;
    float coef_a;
};

uniform tipusLlum llum1;
uniform tipusLlum llum2;
uniform tipusLlum llum3;

void main() 
{
  gl_Position = projection*model_view*vPosition;
  //gl_Position = es passa a coordenades homogenies
  color = vColor;
} 
