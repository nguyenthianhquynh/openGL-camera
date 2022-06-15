#version 330 core
in vec3 FragPos;
in vec3 Normal;

uniform vec3 objColor;
uniform vec3 lightColor;

uniform vec3 lightPos;
uniform vec3 viewPos;


out vec4 FragColor;

void main()
{
    vec3 vecLight = normalize(lightPos - FragPos);
    vec3 vecNormal = normalize(Normal);
    
    float goc = max(dot(vecLight,vecNormal),0.0);
    
    vec3 amColor = objColor * lightColor * 0.1;
    vec3 diffColor = goc * objColor;
    
    //specular
    //vector pháp túyến vs vector light
    vec3 vecNormalLight = reflect(-vecLight,vecNormal);
    vec3 vecView = normalize(viewPos - FragPos);
    float gocView = pow(max(dot(vecView, vecNormalLight), 0.0), 32);

    vec3 spec = gocView * lightColor * 0.5; //*0.5 để bớt đi độ quá sáng
    

    FragColor = vec4(diffColor + amColor + spec, 1.0);
} 
