#pragma once
#include "raylib.h"
#define MAX_LIGHTS 1

typedef struct {   
    int type;
    bool enabled;
    Vector3 position;
    Vector3 target;
    Color color;
    float attenuation;
    float intensity;
    
    // Shader locations
    int enabledLoc;
    int typeLoc;
    int positionLoc;
    int targetLoc;
    int colorLoc;
    int attenuationLoc;
    float intensityLoc;
} Light;

// Light type
typedef enum {
    LIGHT_DIRECTIONAL = 0,
    LIGHT_POINT
} LightType;
static int lightsCount = 0;    


Light CreateLight(int type,float intensity, Vector3 position, Vector3 target, Color color, Shader shader);

void UpdateLightValues(Shader shader, Light light);
