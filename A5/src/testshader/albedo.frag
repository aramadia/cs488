varying float albedo;

void main()
{
	gl_FragColor = vec4(albedo * gl_FrontMaterial.diffuse.rgb, 1.0);
}
