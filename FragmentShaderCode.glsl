#version 430

in vec2 UV;


uniform sampler2D myTextureSampler0;
out vec3 color;


void main()
{
	vec3 MaterialAmbientColor = texture( myTextureSampler0, UV ).rgb;
	vec3 MaterialDiffuseColor = texture( myTextureSampler0, UV ).rgb;
	vec3 MaterialSpecularColor = vec3(0.3,0.3,0.3); // or a specular texture
	
	vec3 LightColor = vec3(1,1,1);
	float AmbientBrightness = 0.5f;
	float DiffuseBrightness = 1.0f;
	float SpecularBrightness = 1.0f;

	 color = 
		// Ambient : simulates indirect lighting
		MaterialAmbientColor * (LightColor * AmbientBrightness);
		//+
		// Diffuse : "color" of the object
		//MaterialDiffuseColor * LightColor * DiffuseBrightness+
		// Specular : reflective highlight, like a mirror
		//MaterialSpecularColor * LightColor * pow(SpecularBrightness, 50);
}
