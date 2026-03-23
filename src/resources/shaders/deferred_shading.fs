#version 330 core
out vec4 finalColor;

in vec2 texCoord;
in vec2 texCoord2;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;

struct Light {
    int enabled;
    int type; // Unused in this demo.
    vec3 position;
    vec3 target; // Unused in this demo.
    vec4 color;
};
//Functions
float Vec3Distance(vec3 a,vec3 b){
    float c=pow(a.r-b.r,2)+pow(a.g-b.g,2)+pow(a.b-b.b,2);
    c=sqrt(c);
    return c;
}
vec3 Linearize(vec3 a){
    float zNear = 0.01; // camera z near
    float zFar = 10.0;  // camera z far
    float depth = (a.r*(zFar - zNear)+(zFar + zNear))/2.0f;
    return vec3(depth);   
}

//Vars
const int NR_LIGHTS = 4;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPosition;
uniform vec2 resolution;
const float QUADRATIC = 0.032;
const float LINEAR = 0.09;
float threshold=sqrt(3)/4;
bool isSilhouette=false;
bool useOutline=false;

float depthThreshold = 0.0009; // tweak this
vec4 crease_edge=vec4(0.8,0.0,0.0,1.0f);
vec4 silhouette =vec4(0.0,1.0,1.0,1.0);
   
void main() {
    //What i will be using
    vec3 fragPosition = texture(gPosition, texCoord).rgb;
    vec3 normal = texture(gNormal, texCoord).rgb;
    vec3 albedo = texture(gAlbedoSpec, texCoord).rgb;
    float specular = texture(gAlbedoSpec, texCoord).a;

    
    //this will be for outlining
    vec2 texelSize=vec2(1/resolution.r,1/resolution.g);
    vec4 normal_center = texture(gNormal, texCoord);
    vec3 normal_left   = texture(gNormal, texCoord - vec2(texelSize.x, 0.0)).rgb;
    vec3 normal_right  = texture(gNormal, texCoord + vec2(texelSize.x, 0.0)).rgb;
    vec3 normal_up     = texture(gNormal, texCoord - vec2(0.0, texelSize.y)).rgb;
    vec3 normal_down   = texture(gNormal, texCoord + vec2(0.0, texelSize.y)).rgb;

    vec3 depth_center = Linearize(texture(gDepth, texCoord).rgb);
    vec3 depth_left   = Linearize(texture(gDepth, texCoord - vec2(texelSize.x, 0.0)).rgb);
    vec3 depth_right  = Linearize(texture(gDepth, texCoord + vec2(texelSize.x, 0.0)).rgb);
    vec3 depth_up     = Linearize(texture(gDepth, texCoord - vec2(0.0, texelSize.y)).rgb);
    vec3 depth_down   = Linearize(texture(gDepth, texCoord + vec2(0.0, texelSize.y)).rgb);

   
    //this part will be for lighting calculations
    vec3 ambient = albedo * vec3(0.1f);
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        if(lights[i].enabled == 0) continue;
        vec3 lightDirection = lights[i].position - fragPosition;
        vec3 diffuse = max(dot(normal, lightDirection), 0.0) * albedo * lights[i].color.xyz;

        vec3 halfwayDirection = normalize(lightDirection + viewDirection);
        float spec = pow(max(dot(normal, halfwayDirection), 0.0), 32.0);
        vec3 specular = specular * spec * lights[i].color.xyz;

        // Attenuation
        float distance = length(lights[i].position - fragPosition);
        float attenuation = 1.0 / (1.0 + LINEAR * distance + QUADRATIC * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        ambient += diffuse + specular;
    }
    //I dont want this to be affected by lighting for now
     if(normal_center.w>0.5){
        useOutline=true;
    }
    if(useOutline && (abs(depth_center - depth_left).r > depthThreshold || abs(depth_center - depth_right).r > depthThreshold ||
    abs(depth_center - depth_up).r > depthThreshold || abs(depth_center - depth_down).r > depthThreshold)) {
        ambient *= silhouette.rgb;
        isSilhouette=true;
    }
    if(!isSilhouette&&useOutline&&(Vec3Distance(normal_center.rgb,normal_left)>threshold ||Vec3Distance(normal_center.rgb,normal_right)>threshold)){
        //Check if silhouette
        ambient*=crease_edge.rgb;
    }
    if(!isSilhouette&&useOutline&&(Vec3Distance(normal_center.rgb,normal_up)>threshold ||Vec3Distance(normal_center.rgb,normal_down)>threshold)){
        ambient*=crease_edge.rgb;
    }
    finalColor = vec4(ambient, 1.0);
}

