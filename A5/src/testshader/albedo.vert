varying float albedo ;

void main ( )
{
	vec3 pos_EC = vec3 ( gl_ModelViewMatrix * gl_Vertex );
	vec3 normal_EC = normalize ( gl_NormalMatrix * gl_Normal ) ;
	vec3 light_EC = normalize ( gl_LightSource[0].position.xyz - pos_EC );

	albedo = dot ( normal_EC , light_EC ) ;
	gl_Position = ftransform( ) ;
}
