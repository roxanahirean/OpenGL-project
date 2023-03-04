#version 410 core

in vec3 fNormal;
in vec4 fPosEye;
in vec2 fTexCoords;
in vec3 fragPos;
in vec4 fragPosLightSpace;
out vec4 fColor;

//lighting
uniform	vec3 lightDir;
uniform	vec3 lightColor;
uniform mat3 lightDirMatrix;
uniform	mat3 normalMatrix;

//texture
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D shadowMap;

//spotlight

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

};
PointLight pointLight;

uniform int bool_spotlight; 
float constant = 0.8f;
float linear = 0.8f;
float quadratic = 0.8f;

//fog
uniform int bool_fog;

vec3 ambient;
float ambientStrength = 0.5f;
vec3 diffuse;
vec3 specular;
float specularStrength = 0.7f;
float shininess = 64.0f;

float computeShadow(){


	// transformăm poziția fragmentului în coordonatele normalizate ale spațiului luminii
	// obtinem coord normalizate
	vec3 normalizedCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;

	// transformam coord din int [-1, 1] in int [0, 1] pt ca valorile adancimii
	// fragmentului sunt in int [0, 1]
	normalizedCoords = normalizedCoords * 0.5 + 0.5;

	// putem eșantiona adâncimea existentă în harta de adâncime folosind aceste coordonate,
	// deoarece acestea corespund acum cu coordonatele NDC din prima trecere:
	// get closest depth value from light's perspective
	float closestDepth = texture(shadowMap, normalizedCoords.xy).r;

	// luam adancimea fracmentului curent
	float currentDepth = normalizedCoords.z;

	float bias = 0.005f;

	// comparam adancimea frag curent cu val din harta adancimii. daca e >, frag curent e in umbra
	// altfel e iluminat
	float shadow = currentDepth - bias > closestDepth ? 1.0 : 0.0;

	//modific frag din afara frustumului hartii de adancime care au o valoare a adancimii>1
	if (normalizedCoords.z > 1.0f)
		return 0.0f;
	return shadow;
}

float computeFog()
{
 float fogDensity = 0.05f;
 float fragmentDistance = length(fPosEye);
 float fogFactor = exp(-pow(fragmentDistance * fogDensity, 2));

 return clamp(fogFactor, 0.0f, 1.0f);
}


void CalcPointLight(PointLight light, vec3 fNormal, vec3 fragPos, vec3 viewDir)
{
    	vec3 lightDir = normalize(light.position - fragPos);
	vec3 cameraPosEye = vec3(0.0f);
	vec3 lighrDir=normalize(light.position-fragPos);
	vec3 lightDirN = normalize(lightDirMatrix * lightDir);	
	vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
	vec3 halfVector = normalize(lightDirN + viewDirN);
	vec3 normalEye = normalize(normalMatrix * fNormal);	
    float distance    = length(light.position - fragPos);
    float att = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));   
	ambient = att * ambientStrength * lightColor; 
	diffuse = att * max(dot(normalEye, lightDirN), 0.0f) * lightColor; 

        //compute specular light
        
	float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
	specular = att * specularStrength * specCoeff * lightColor;
} 

void computeLightComponents()
{	
	vec3 cameraPosEye = vec3(0.0f);//in eye coordinates, the viewer is situated at the origin
	
	//transform normal
	vec3 normalEye = normalize(fNormal);	
	
	//compute light direction
	vec3 lightDirN;
	if(bool_spotlight == 0)
	{
		lightDirN = normalize(lightDir);
	
		//compute view direction 
		vec3 viewDirN = normalize(cameraPosEye - fPosEye.xyz);
		
		//compute half vector
		vec3 halfVector = normalize(lightDirN + viewDirN);
			
		//compute ambient light
		// lumina amibientala nu vine dintr o anumita directie, ea e aproximarea luminii globale imprastiate in jurul scenei
		ambient = ambientStrength * lightColor;
		
		//compute diffuse light
		//ea e cea imprastiata in toate directiile de o sursa de lumina
		diffuse = max(dot(normalEye, lightDirN), 0.0f) * lightColor;
		
		//compute specular light
		// tine cont de pozitia camerei
		float specCoeff = pow(max(dot(halfVector, normalEye), 0.0f), shininess);
		specular = specularStrength * specCoeff * lightColor;
	}
	else
	{
		vec3 viewD=vec3(0.0f)-fPosEye.xyz;
		CalcPointLight(pointLight,fNormal,fragPos,viewD);
	}
	
	
	
}

void main() 
{
	

	pointLight.position=vec3(30.87f,5.47f,-1.93f);
	pointLight.constant=1.0f;
	pointLight.linear=0.09f;
	pointLight.quadratic=0.032f;
	computeLightComponents();
	
	vec3 baseColor = vec3(0.9f, 0.35f, 0.0f);//orange
	
	ambient *= texture(diffuseTexture, fTexCoords).rgb;
	diffuse *= texture(diffuseTexture, fTexCoords).rgb;
	specular *= texture(specularTexture, fTexCoords).rgb;
	float shadow;
	if(bool_spotlight == 0)
		shadow = computeShadow();
	else
		shadow = 0.0f;
		
	vec3 color = min((ambient + (1.0f - shadow) * diffuse) + (1.0f - shadow) * specular, 1.0f);
	
	float fogFactor;
	vec4 fogColor;
	vec4 lastColor;
	if(bool_fog == 1)
	{
		fogFactor = computeFog();
		fogColor = vec4(0.5f, 0.5f, 0.5f, 1.0f);
		lastColor = fogColor * (1 - fogFactor) + vec4(color, 1.0f) * fogFactor;
	}
	else
	{
		lastColor = vec4(color, 1.0f);
	}
	
	fColor = lastColor;

	
    
    
}
