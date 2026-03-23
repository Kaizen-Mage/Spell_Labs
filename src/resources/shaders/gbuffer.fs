#version 330 core
layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec4 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;
layout (location = 3) out vec4 gDepth;


in vec3 fragPosition;
in vec2 fragTexCoord;
in vec3 fragNormal;
uniform float      outline;
uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D depthTexture;
void main() {
    // store the fragment position vector in the first gbuffer texture
    gPosition = fragPosition;
    // also store the per-fragment normals into the gbuffer
    gNormal.xyz= normalize(fragNormal);
    if(outline>0.5){
        gNormal.w=1.0f;
    }
    else{
        gNormal.w=0.0f;
    }
    // and the diffuse per-fragment color
    gAlbedoSpec.rgb = texture(diffuseTexture, fragTexCoord).rgb;
    // store specular intensity in gAlbedoSpec's alpha component
    gAlbedoSpec.a = texture(specularTexture, fragTexCoord).r;
    gDepth.rgb=texture(depthTexture,fragTexCoord).rgb;
}
