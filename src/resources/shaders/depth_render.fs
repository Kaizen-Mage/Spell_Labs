#version 330

// Input vertex attributes (from vertex shader)
in vec2 texCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D shadowMap;     // Depth texture
uniform vec4 colDiffuse;

// Output fragment color
out vec4 finalColor;
vec3 Linearize(vec3 a){
    float zNear = 0.1; // camera z near
    float zFar = 100.0;  // camera z far
    float linearDepth = (2.0 * zNear * zFar) / (zFar + zNear - a.r * (zFar - zNear));
    return vec3(linearDepth);   
}
// NOTE: Add here your custom variables

void main()
{
    float zNear = 0.1; // camera z near
    float zFar = 100.0;  // camera z far
    vec3 depth = Linearize(texture(shadowMap, texCoord).rgb);
    // Calculate final fragment color
    finalColor = vec4(depth, 1.0f);
}