#version 330
out vec4 FragColor;


in vec3 vColor;
in vec2 vTex;
in vec3 vNormal;
in vec3 vPos;

void main()
{
	vec3 norm = normalize(vNormal); 
	vec3 lightDir = normalize(vec3(1, 0, 0)); 
	float diff = max(dot(norm, lightDir), 0.0);
	vec3 diffuse = diff * vec3(1, 1, 1);
	diffuse = max(vec3(0.3, 0.3, 0.3), diffuse);

    FragColor = vec4(vColor * diffuse, 1.0);
} 