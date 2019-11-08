#version 430

in vec2 UV;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform sampler2D myTextureSampler0;
uniform vec4 ambientLight;
uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;

out vec4 color;


void main()
{
	vec3 LightColor = vec3(1,1,1);

	vec4 MaterialAmbientColor = texture( myTextureSampler0, UV ).rgba;
	vec4 MaterialDiffuseColor = texture( myTextureSampler0, UV ).rgba;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3);

	//Diffuse
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec4 diffuseLight = vec4(brightness, brightness, brightness, 1.0);

	//Specular
	vec3 reflectedLightVectorWorld = reflect(-lightVectorWorld, normalWorld);
	vec3 eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s =clamp(dot(reflectedLightVectorWorld, eyeVectorWorld), 0, 1);
	s = pow(s, 50);
	vec4 specularLight = vec4(s , s, s, 1);

	 color = 
		MaterialAmbientColor * ambientLight + 
		MaterialDiffuseColor * clamp(diffuseLight, 0, 1)+
		specularLight;
}
