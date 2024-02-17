#version 440 core
out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
in vec2 TexCoord;

uniform vec3 lightPos; 
uniform vec3 viewPos; 
uniform vec3 lightColor;
uniform sampler2D texture1;

void main()
{
    vec3 color = texture(texture1, TexCoord).rgb;

    // Increased ambient light
    vec3 ambient = 1.2 * color; // Increase ambient light for softer shadows

    // Light attenuation factors
    float constant = 1.0;
    float linear = 0.09;
    float quadratic = 0.032;
    float distance    = length(lightPos - FragPos);
    float attenuation = 1.0 / (constant + linear * distance + quadratic * (distance * distance));
    ambient *= attenuation;


    // diffuse
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 normal = normalize(Normal);
    float diff = max(dot(lightDir, normal), 0.0);
    diff = pow(diff, 1.2); // Experiment with the exponent for softer transitions
    vec3 diffuse = diff * color * lightColor;
    diffuse *= attenuation;

    // specular
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;

    // blinn
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0); // Increase the shininess factor for tighter highlights
    vec3 specular = vec3(0.1) * spec * lightColor; // Adjust specular intensity
    specular *= attenuation;

    // Combine components with gamma correction
    vec3 finalColor = ambient + diffuse + specular;
    finalColor = pow(finalColor, vec3(1.0/2.2)); // Gamma correction

    vec3 gammaCorrectedColor = pow(ambient + diffuse + specular, vec3(1.0/2.2)); // Gamma correction
    FragColor = vec4(gammaCorrectedColor, 1.0);
}
