varying float albedo, specular;

void main() {
  vec3 pos_EC = vec3(gl_ModelViewMatrix * gl_Vertex);
  vec3 normal_EC = normalize(gl_NormalMatrix * gl_Normal);
  vec3 light_EC = gl_LightSource[0].position.xyz - pos_EC;
  vec3 half_EC = normalize(normalize(light_EC) - normalize(pos_EC));

  light_EC = normalize(light_EC);

  albedo = max(dot(normal_EC, light_EC), 0.0);
  specular = pow(max(dot(normal_EC, half_EC),0.0),  gl_FrontMaterial.shininess);
  gl_Position= ftransform();
}

