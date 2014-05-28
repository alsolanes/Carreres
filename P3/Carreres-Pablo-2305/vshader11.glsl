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
IN vec4 vNormal;

OUT vec4 color;

struct tipusLlum
{
    int activa;
    vec4 LightPosition;
    vec3 Idifusa;
    vec3 Iambient;
    vec3 Iespecular;
    vec3 atenuacio;
    vec3 direccio;

};

struct tipusMaterial
{
    vec3 ks;
    vec3 ka;
    vec3 kd;
    float coef_a;

};

uniform tipusMaterial m;
uniform tipusLlum llum1;
uniform tipusLlum llum2;
uniform tipusLlum llum3;
uniform vec3 ambientGlobal;

void main()
{
     // Harcoded ks, ka, kd
    vec3 ks = m.ks;
    vec3 ka = m.ka;
    vec3 kd = m.kd;
    float coef_a = m.coef_a;
  gl_Position = projection*model_view*vPosition;
  color = vec4(0,0,0,0);


  float att;
  float d;
  vec3 h;
  vec4 llum;

  if (llum1.activa!=0){
      // Calculem els coeficients
      d = length(llum1.LightPosition.xyz-vPosition.xyz);
      att = 1.0 / ( llum1.atenuacio.x*pow(d,2) + llum1.atenuacio.y*d + llum1.atenuacio.z );
      h = (llum1.direccio+gl_Position.xyz)/length(llum1.direccio+gl_Position.xyz);

      llum = vec4(llum1.Idifusa*kd*max(dot(llum1.direccio,vNormal.xyz),0),0);
      llum += vec4(llum1.Iespecular*ks*max(pow(dot(vNormal.xyz,h),coef_a),0),0);
      llum += vec4(llum1.Iambient*ka,0);
      llum *= att;

      color += llum;
  }
  if (llum2.activa!=0){
      // Calculem els coeficients
      d = distance(llum2.LightPosition.xyz,vPosition.xyz);
      att = 1.0 / ( llum2.atenuacio.x*pow(d,2) + llum2.atenuacio.y*d + llum2.atenuacio.z );
      h = (llum2.direccio+gl_Position.xyz)/length(llum2.direccio+gl_Position.xyz);

      // Calculem la intensitat de llum2
      llum = vec4(llum2.Idifusa*kd*max(dot(llum2.direccio,vNormal.xyz),0),0);
      llum += vec4(llum2.Iespecular*ks*max(pow(dot(vNormal.xyz,h),coef_a),0),0);
      llum += vec4(llum2.Iambient*ka,0);
      //llum *= att;

      color += llum;
  }
  if (llum3.activa!=0){
      // Calculem els coeficients
      d = length(abs(llum3.LightPosition.xyz-vPosition.xyz));
      att = 1.0 / ( llum3.atenuacio.x*pow(d,2) + llum3.atenuacio.y*d + llum3.atenuacio.z );
      h = (llum3.direccio+gl_Position.xyz)/length(llum3.direccio+gl_Position.xyz);

      // Calculem la intensitat de llum3
      llum = vec4(llum3.Idifusa*kd*max(dot(llum3.direccio,vNormal.xyz),0),0);
      llum += vec4(llum3.Iespecular*ks*max(pow(dot(vNormal.xyz,h),coef_a),0),0);
      llum += vec4(llum3.Iambient*ka,0);
      //llum *= att;

      color += llum;
  }

  color += vec4(ambientGlobal * ka,0);
  // Set it to 0.
  if (color.x > 1.0) color.x = 1.0;
  if (color.y > 1.0) color.y = 1.0;
  if (color.z > 1.0) color.z = 1.0;

}
