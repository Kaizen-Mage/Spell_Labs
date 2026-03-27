#version 330 core
out vec4 finalColor;

in vec2 texCoord;
in vec2 texCoord2;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;
uniform sampler2D gDepth;
uniform sampler2D shadowMap;
uniform sampler2D pallete;
//Lighting variables
struct Light {
    int enabled;
    int type; // Unused in this demo.
    vec3 position;
    vec3 target; // Unused in this demo.
    vec4 color;
    float intensity;
};
const int NR_LIGHTS = 4;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPosition;
uniform vec2 resolution;
uniform mat4 lightVP; // Light source view-projection matrix
const float QUADRATIC = 0.032;
const float LINEAR = 0.09;
float threshold=sqrt(3)/4;
bool isSilhouette=false;
float ambient_lighting=0.3f;
// outline variables
float depthThreshold = 0.03; // tweak this
vec4 crease_edge=vec4(0.8,0.0,0.0,1.0f);
vec4 silhouette =vec4(1.0,1.0,1.0,1.0);
//Quantizaton variables
uniform int useOutline;
uniform int colorSpace;
uniform int colorMap;
uniform int showEdges;
uniform int  palleteSize;


//Functions
float Vec3Distance(vec3 a,vec3 b){
    float c=pow(a.r-b.r,2)+pow(a.g-b.g,2)+pow(a.b-b.b,2);
    c=sqrt(c);
    return c;
}
vec3 Linearize(vec3 a){
    float zNear = 0.1; // camera z near
    float zFar = 100.0;  // camera z far
    float linearDepth = (2.0 * zNear * zFar) / (zFar + zNear - a.r * (zFar - zNear));
    return vec3(linearDepth);   
}
vec3 Vec3Pow(vec3 a,float b){
    return vec3(pow(a.r,b),pow(a.g,b),pow(a.b,b));
}
vec3 rgb2xyz(vec3 rgb){
    rgb=Vec3Pow(rgb,2.2);
    float x= 0.4124564*rgb.r + 0.3575761*rgb.g + 0.1804375*rgb.b;
    float y = 0.2126729*rgb.r + 0.7151522*rgb.g + 0.0721750*rgb.b;
    float z = 0.0193339*rgb.r + 0.1191920*rgb.g + 0.9503041*rgb.b;
    return vec3(x,y,z);
}
vec3 xyz2lab(vec3 xyz) {
    vec3 ref = vec3(0.95047, 1.0, 1.08883); // D65 reference
    xyz /= ref;

    vec3 f = pow(xyz, vec3(1.0/3.0));
    vec3 cond = step(xyz, vec3(0.008856)); // 1.0 if xyz <= 0.008856, else 0.0
    f = mix(f, 7.787 * xyz + 16.0/116.0, cond);

    float L = 116.0 * f.y - 16.0;
    float a = 500.0 * (f.x - f.y);
    float b = 200.0 * (f.y - f.z);

    return vec3(L, a, b);
}
vec3 ColorMapTextureXYZ(vec3 color) {
    color = rgb2xyz(color);
    color = xyz2lab(color);

    vec3 nearestColor = vec3(0.0);
    float nearestDist = 50000.0;

    for (int i = 0; i < palleteSize; i++) {
        float u = float(i) / float(palleteSize - 1); // normalized coordinate
        vec3 palColor = texture(pallete, vec2(u, 0.5)).rgb; // sample the palette
        palColor = rgb2xyz(palColor);
        palColor = xyz2lab(palColor);

        float dist = Vec3Distance(color, palColor);
        if (dist < nearestDist) {
            nearestDist = dist;
            nearestColor = texture(pallete, vec2(u, 0.5)).rgb; // return original RGB
        }
    }

    return nearestColor;
}
vec3 ColorMapTextureRGB(vec3 color) {

    vec3 nearestColor = vec3(0.0);
    float nearestDist = 50000.0;

    for (int i = 0; i < palleteSize; i++) {
        float u = float(i) / float(palleteSize - 1); // normalized coordinate
        vec3 palColor = texture(pallete, vec2(u, 0.5)).rgb; // sample the palette
        float dist = Vec3Distance(color, palColor);
        if (dist < nearestDist) {
            nearestDist = dist;
            nearestColor = texture(pallete, vec2(u, 0.5)).rgb; // return original RGB
        }
    }

    return nearestColor;
}
void main() {
    //What i will be using
    vec3 fragPosition = texture(gPosition, texCoord).rgb;
    vec3 normal = texture(gNormal, texCoord).rgb;
    vec3 albedo = texture(gAlbedoSpec, texCoord).rgb;
    float specular = texture(gAlbedoSpec, texCoord).a;
    vec3 gShadow=Linearize(texture(shadowMap,texCoord).rgb);

    
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
    vec3 ambient = albedo * ambient_lighting;
    vec3 viewDirection = normalize(viewPosition - fragPosition);
    for(int i = 0; i < NR_LIGHTS; ++i)
    {
        if(lights[i].enabled == 0) continue;
        vec3 lightDirection = normalize(lights[i].position - fragPosition);
        vec3 diffuse = max(dot(normal, lightDirection), 0.0) * albedo * lights[i].color.xyz;

        vec3 halfwayDirection = normalize(lightDirection + viewDirection);
        float spec = pow(max(dot(normal, halfwayDirection), 0.0), 32.0);
        vec3 specular = specular * spec * lights[i].color.xyz;

        // Attenuation
        float distance = length(lights[i].position - fragPosition);
        float attenuation = lights[i].intensity / (1.0 + LINEAR * distance + QUADRATIC * distance * distance);
        diffuse *= attenuation;
        specular *= attenuation;
        ambient += diffuse + specular;
    }
  
   vec3 lightDot = vec3(0.0); // declare once, before the loop
    for (int i = 0; i < NR_LIGHTS; ++i) {
        vec3 lightDir(lights[i].position-lights[i].target)/Vec3Distance(lights[i].position,lights[i].target);
        vec4 fragPosLightSpace = lightVP * vec4(fragPosition, 1);
        fragPosLightSpace.xyz /= fragPosLightSpace.w; // Perform the perspective division
        fragPosLightSpace.xyz = (fragPosLightSpace.xyz + 1.0f) / 2.0f; // Transform from [-1, 1] range to [0, 1] range
        vec2 sampleCoords = fragPosLightSpace.xy;
        float curDepth = fragPosLightSpace.z;
        float bias = max(0.0002 * (1.0 - dot(normal_center.rgb, lightDir)), 0.00002) + 0.00001;
        if(curDepth-bias<gShadow.r){

        }
  
    }
    //Anything here is not affected by lighting
    //I dont want this to be affected by lighting for now
    if(useOutline==1&& (abs(depth_center - depth_left).r > depthThreshold || abs(depth_center - depth_right).r > depthThreshold ||
    abs(depth_center - depth_up).r > depthThreshold || abs(depth_center - depth_down).r > depthThreshold)) {
        ambient = silhouette.rgb;
        isSilhouette=true;
    }
    if(!isSilhouette&&showEdges==1&&(Vec3Distance(normal_center.rgb,normal_left)>threshold ||Vec3Distance(normal_center.rgb,normal_right)>threshold)){
        //Check if silhouette
        ambient=crease_edge.rgb;
    }
    if(!isSilhouette&&showEdges==1&&(Vec3Distance(normal_center.rgb,normal_up)>threshold ||Vec3Distance(normal_center.rgb,normal_down)>threshold)){
        ambient=crease_edge.rgb;
    }

    //Decide which colorSpace for mapping to use
    if(colorMap==1){
        if(colorSpace==1){
        ambient=ColorMapTextureXYZ(ambient);
        }
        else{
            ambient=ColorMapTextureRGB(ambient);
        }
    }
    else{

    }
    
    finalColor = vec4(gShadow, 1.0);
}

