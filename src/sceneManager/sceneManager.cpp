#include "sceneManager/sceneManager.h"

SceneManager::SceneManager(EngineContext* ctx):context(ctx){
    context->resourceManager->LoadShader("GbufferShader","src/resources/shaders/gbuffer.vs","src/resources/shaders/gbuffer.fs");
    context->resourceManager->LoadShader("Deferred_Shader","src/resources/shaders/deferred_shading.vs","src/resources/shaders/deferred_shading.fs");
    context->resourceManager->LoadShader("Depth_Shader","src/resources/shaders/depth_render.vs","src/resources/shaders/depth_render.fs");

    context->resourceManager->GetShader("Deferred_Shader")->locs[SHADER_LOC_VECTOR_VIEW] =
        GetShaderLocation(*context->resourceManager->GetShader("Deferred_Shader"), "viewPosition");

    cam.position = {10.0,10.0,10.0};
    cam.target   = {0.0,0.0,0.0};
    cam.fovy     = 10.0f;
    cam.up       = {0.0,1.0,0.0};
    cam.projection = CAMERA_PERSPECTIVE;

    cube = LoadModelFromMesh(GenMeshCube(1.0f,1.0f,1.0f));
    gun  = LoadModel("src/resources/models/Y.glb");

    cube.materials[0].shader = *context->resourceManager->GetShader("GbufferShader");
    for (int i = 0; i < gun.materialCount; i++) {
        gun.materials[i].shader = *context->resourceManager->GetShader("GbufferShader");
    }

    gBuffer.frameBufferId = rlLoadFramebuffer();
    rlEnableFramebuffer(gBuffer.frameBufferId);

    gBuffer.positionTextureId    = rlLoadTexture(NULL,downWidth,downHeight,RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16,1);
    gBuffer.normalTextureId      = rlLoadTexture(NULL,downWidth,downHeight,RL_PIXELFORMAT_UNCOMPRESSED_R16G16B16A16,1);
    gBuffer.albedoSpecTextureId  = rlLoadTexture(NULL,downWidth,downHeight,RL_PIXELFORMAT_UNCOMPRESSED_R8G8B8A8,1);

    rlActiveDrawBuffers(3);

    rlFramebufferAttach(gBuffer.frameBufferId,gBuffer.positionTextureId,RL_ATTACHMENT_COLOR_CHANNEL0,RL_ATTACHMENT_TEXTURE2D,0);
    rlFramebufferAttach(gBuffer.frameBufferId,gBuffer.normalTextureId,RL_ATTACHMENT_COLOR_CHANNEL1,RL_ATTACHMENT_TEXTURE2D,0);
    rlFramebufferAttach(gBuffer.frameBufferId,gBuffer.albedoSpecTextureId,RL_ATTACHMENT_COLOR_CHANNEL2,RL_ATTACHMENT_TEXTURE2D,0);

    gBuffer.depthRenderBufferId = rlLoadTextureDepth(downWidth,downHeight,false);
    rlFramebufferAttach(gBuffer.frameBufferId,gBuffer.depthRenderBufferId,RL_ATTACHMENT_DEPTH,RL_ATTACHMENT_TEXTURE2D,0);

    if(rlFramebufferComplete(gBuffer.frameBufferId)){
        std::cout<<"FrameBuffer Completed Successfully"<<std::endl;
    }

    texUnitPosition = 0;
    texUnitNormal = 1;
    texUnitAlbedoSpec = 2;
    texUnitDepth = 3;

    rlEnableShader(context->resourceManager->GetShader("Deferred_Shader")->id);

    Vector2 renderSize = {(float)downWidth,(float)downHeight};

    SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
        rlGetLocationUniform(context->resourceManager->GetShader("Deferred_Shader")->id, "gPosition"),
        &texUnitPosition, RL_SHADER_UNIFORM_SAMPLER2D);

    SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
        rlGetLocationUniform(context->resourceManager->GetShader("Deferred_Shader")->id, "gNormal"),
        &texUnitNormal, RL_SHADER_UNIFORM_SAMPLER2D);

    SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
        rlGetLocationUniform(context->resourceManager->GetShader("Deferred_Shader")->id, "gAlbedoSpec"),
        &texUnitAlbedoSpec, RL_SHADER_UNIFORM_SAMPLER2D);

    SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
        rlGetLocationUniform(context->resourceManager->GetShader("Deferred_Shader")->id, "gDepth"),
        &texUnitDepth, RL_SHADER_UNIFORM_SAMPLER2D);

    SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
        rlGetLocationUniform(context->resourceManager->GetShader("Deferred_Shader")->id,"resolution"),
        &renderSize, RL_SHADER_UNIFORM_VEC2);

    rlDisableShader();

    lights[0] = CreateLight(LIGHT_POINT,{ -2, 1, -2 }, Vector3Zero(), YELLOW,
        *context->resourceManager->GetShader("Deferred_Shader"));

    rlEnableDepthTest();
}

void SceneManager::Update(float dt){
    SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
        context->resourceManager->GetShader("Deferred_Shader")->locs[SHADER_LOC_VECTOR_VIEW],
        &cam.position, SHADER_UNIFORM_VEC3);

    UpdateCamera(&cam,CAMERA_ORBITAL);

    if (IsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }

    if (IsKeyPressed(KEY_ONE))   { mode=0; }
    if (IsKeyPressed(KEY_TWO))   { mode=1; }
    if (IsKeyPressed(KEY_THREE)) { mode=2; }
    if (IsKeyPressed(KEY_FOUR))  { mode=3; }
    if (IsKeyPressed(KEY_FIVE))  { mode=4; }

    for (int i = 0; i < MAX_LIGHTS; i++)
        UpdateLightValues(*context->resourceManager->GetShader("Deferred_Shader"), lights[i]);
}

void SceneManager::Draw(){
    rlEnableFramebuffer(gBuffer.frameBufferId);
    rlViewport(0,0,downWidth,downHeight);
    rlClearColor(0, 0, 0, 0);
    rlClearScreenBuffers();
    rlDisableColorBlend();

    BeginMode3D(cam);
        float Outline=1.0f;
        SetShaderValue(*context->resourceManager->GetShader("GbufferShader"),
            GetShaderLocation(*context->resourceManager->GetShader("GbufferShader"),"outline"),
            &Outline,SHADER_UNIFORM_FLOAT);

        DrawModel(cube,Vector3Zero(),0.25,WHITE);
        DrawModelEx(cube,{0.0,-0.25f,0.0f},{0.0f,0.0f,0.0f},0.0f,{5.0f,0.20f,5.0f},WHITE);

        Outline=0.0f;
        SetShaderValue(*context->resourceManager->GetShader("GbufferShader"),
            GetShaderLocation(*context->resourceManager->GetShader("GbufferShader"),"outline"),
            &Outline,SHADER_UNIFORM_FLOAT);
        DrawModelEx(gun,{0.0,1.0,0.0},{1.0,0.0,0.0},-90,{80,80,80},WHITE);
    EndMode3D();

    rlEnableColorBlend();
    rlDisableFramebuffer();

    rlClearScreenBuffers();
    rlViewport(0,0,screenWidth,screenHeight);

    switch (mode)
    {
    case 0:
        rlDisableColorBlend();
        rlEnableShader(context->resourceManager->GetShader("Deferred_Shader")->id);

        rlActiveTextureSlot(texUnitPosition);
        rlEnableTexture(gBuffer.positionTextureId);

        rlActiveTextureSlot(texUnitNormal);
        rlEnableTexture(gBuffer.normalTextureId);

        rlActiveTextureSlot(texUnitAlbedoSpec);
        rlEnableTexture(gBuffer.albedoSpecTextureId);

        rlActiveTextureSlot(texUnitDepth);
        rlEnableTexture(gBuffer.depthRenderBufferId);

        rlLoadDrawQuad();

        rlDisableShader();
        rlEnableColorBlend();

        rlBindFramebuffer(RL_READ_FRAMEBUFFER, gBuffer.frameBufferId);
        rlBindFramebuffer(RL_DRAW_FRAMEBUFFER, 0);
        rlBlitFramebuffer(0, 0, downWidth, downHeight, 0, 0, screenWidth, screenHeight, 0x00000100);
        rlDisableFramebuffer();

        rlViewport(0,0,screenWidth,screenHeight);

        BeginMode3D(cam);
            for (int i = 0; i < MAX_LIGHTS; i++)
            {
                if (lights[i].enabled)
                    DrawSphereEx(lights[i].position, 0.2f, 8, 8, lights[i].color);
                else
                    DrawSphereWires(lights[i].position, 0.2f, 8, 8, ColorAlpha(lights[i].color, 0.3f));
            }
        EndMode3D();
        break;

    case 1:
        DrawTextureRec((Texture2D){
                .id = gBuffer.positionTextureId,
                .width = screenWidth,
                .height = screenHeight,
            }, (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight }, Vector2Zero(), RAYWHITE);
        break;

    case 2:
        DrawTextureRec((Texture2D){
                .id = gBuffer.normalTextureId,
                .width = screenWidth,
                .height = screenHeight,
            }, (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight }, Vector2Zero(), RAYWHITE);
        break;

    case 3:
        DrawTextureRec((Texture2D){
                .id = gBuffer.albedoSpecTextureId,
                .width = screenWidth,
                .height = screenHeight,
            }, (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight }, Vector2Zero(), RAYWHITE);
        break;

    case 4:
        BeginShaderMode(*context->resourceManager->GetShader("Depth_Shader"));
        rlActiveTextureSlot(0);
        rlEnableTexture(gBuffer.depthRenderBufferId);
        DrawTextureRec((Texture2D){
                .id = gBuffer.depthRenderBufferId,
                .width = screenWidth,
                .height = screenHeight,
            }, (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight }, Vector2Zero(), RAYWHITE);
        EndShaderMode();
        break;

    default:
        break;
    }
}