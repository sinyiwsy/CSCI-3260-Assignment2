#version 430

in vec2 UV;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform sampler2D myTextureSampler0;
uniform vec3 lightPositionWorld;
uniform vec3 eyePositionWorld;

out vec3 color;


void main()
{
	vec3 lightVectorWorld = normalize(lightPositionWorld - vertexPositionWorld);
	float brightness = dot(lightVectorWorld, normalize(normalWorld));
	vec3 diffuseLight = vec3(brightness, brightness, brightness);

	vec3 MaterialAmbientColor = texture( myTextureSampler0, UV ).rgb;
	vec3 MaterialDiffuseColor = texture( myTextureSampler0, UV ).rgb;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3); // or a specular texture
	
	vec3 LightColor = vec3(1,1,1);
	float AmbientBrightness = 0.1f;
	float DiffuseBrightness = 0.8f;
	float SpecularBrightness = 1.0f;

	 color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor * (LightColor * AmbientBrightness) + clamp(diffuseLight, 0, 1) * DiffuseBrightness;
		// Diffuse : "color" of the object
		//MaterialDiffuseColor * LightColor * DiffuseBrightness+
		// Specular : reflective highlight, like a mirror
		//MaterialSpecularColor * LightColor * pow(SpecularBrightness, 50);
}
