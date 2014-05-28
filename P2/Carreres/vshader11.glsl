#if __VERSION__<130
#define IN attribute
#define OUT varying
#else
#define IN in
#define OUT out
#endif

uniform mat4 model_view;
uniform mat4 projection;
IN vec4 vPosition;
IN vec4 vColor;

OUT vec4 color;



void main() 
{
  gl_Position = projection*model_view*vPosition;
  color = vColor;
} 
