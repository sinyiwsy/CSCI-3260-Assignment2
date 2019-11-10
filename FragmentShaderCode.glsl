#version 430

in vec2 UV;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform sampler2D myTextureSampler0;
uniform vec4 ambientLight;
uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;

uniform vec4 ambientLight0;
uniform vec3 lightPositionWorld0;
uniform vec3 eyePositionWorld0;

out vec4 color;


void main()
{
	vec3 LightColor = vec3(1,1,1);

	vec4 MaterialAmbientColor = texture( myTextureSampler0, UV ).rgba;
	vec4 MaterialDiffuseColor = texture( myTextureSampler0, UV ).rgba;

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

	//cat light
	//Diffuse
	vec3 lightVectorWorld0 = normalize(lightPositionWorld0 - vertexPositionWorld);
	float brightness0 = dot(lightVectorWorld0, normalize(normalWorld));
	vec4 diffuseLight0 = vec4(brightness0, brightness0, brightness0, 1.0);

	//Specular
	vec3 reflectedLightVectorWorld0 = reflect(-lightVectorWorld0, normalWorld);
	vec3 eyeVectorWorld0 = normalize(eyePositionWorld0 - vertexPositionWorld);
	float s1 =clamp(dot(reflectedLightVectorWorld0, eyeVectorWorld0), 0, 1);
	s1 = pow(s1, 50);
	vec4 specularLight0 = vec4(s1 , s1, s1, 1);

	 color = 
		MaterialDiffuseColor * clamp(diffuseLight, 0, 1)+
		specularLight +
		MaterialAmbientColor * ambientLight0 + 
		MaterialDiffuseColor * clamp(diffuseLight0, 0, 1)+
		specularLight0;
}
