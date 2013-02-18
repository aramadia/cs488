varying float albedo, specular;

void main() {
    vec4 ambientp = gl_LightSource[0].ambient * gl_FrontMaterial.ambient;
    vec4 diffusep = gl_LightSource[0].diffuse * gl_FrontMaterial.diffuse;
    vec4 specularp = gl_LightSource[0].specular * gl_FrontMaterial.specular;

    gl_FragColor = ambientp + albedo * diffusep + specular*specularp;
}
