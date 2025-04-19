#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

void main()
{
    // Basit ışık hesaplaması
    vec3 lightPos = vec3(2.0, 2.0, 2.0);
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 objectColor = vec3(0.9, 0.6, 0.3); // TNT rengi
    
    // Ambient
    float ambientStrength = 0.3;
    vec3 ambient = ambientStrength * lightColor;
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Sonuç
    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}