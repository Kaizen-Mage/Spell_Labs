#include "sceneManager/sceneManager.h"
void SceneManager::LoadPallete(std::string palletteName,int size,std::string location){
    context->resourceManager->LoadTexture(palletteName.c_str(),location.c_str());
    Pallete p;
    p.palleteTex=context->resourceManager->GetTexture(palletteName.c_str());
    p.numColors=size;
    palletes[palletteName.c_str()]=p;
    palleteNames.push_back(palletteName.c_str());
}
SceneManager::SceneManager(EngineContext* ctx):context(ctx){
    context->resourceManager->LoadShader("GbufferShader","src/resources/shaders/gbuffer.vs","src/resources/shaders/gbuffer.fs");
    context->resourceManager->LoadShader("Deferred_Shader","src/resources/shaders/deferred_shading.vs","src/resources/shaders/deferred_shading.fs");
    context->resourceManager->LoadShader("Depth_Shader","src/resources/shaders/depth_render.vs","src/resources/shaders/depth_render.fs");
    
    //Pallete loading
    LoadPallete("Ephemera",12,"src/resources/textures/ephemera.png");
    LoadPallete("Mother32",32,"src/resources/textures/mother32.png");
    LoadPallete("Bloodthirst",9,"src/resources/textures/bloodthirst.png");
    LoadPallete("Frozen",14,"src/resources/textures/frozen.png");
    LoadPallete("Nopal12",12,"src/resources/textures/nopal-12.png");
    LoadPallete("Pola",5,"src/resources/textures/pola.png");
    LoadPallete("Daydream",20,"src/resources/textures/daydream.png");
    LoadPallete("Crystal Flames",24,"src/resources/textures/crystal-flames.png");
    LoadPallete("Midnight Ablaze",7,"src/resources/textures/midnight-ablaze.png");
    LoadPallete("AKC12",12,"src/resources/textures/akc12.png");
    LoadPallete("sotc12",12,"src/resources/textures/sotc12.png");
    LoadPallete("Itten64",64,"src/resources/textures/itten64.png");
    LoadPallete("Piratte64",64,"src/resources/textures/pirate.png");
    LoadPallete("Lovecraft",24,"src/resources/textures/lovecraft.png");
    LoadPallete("Horror",4,"src/resources/textures/look-of-horror.png");
    LoadPallete("Maggot24",24,"src/resources/textures/maggot24.png");
    LoadPallete("Enchanted Purple",12,"src/resources/textures/enchanted-purple.png");

    std::sort(palleteNames.begin(), palleteNames.end());
    context->resourceManager->GetShader("Deferred_Shader")->locs[SHADER_LOC_VECTOR_VIEW] =
        GetShaderLocation(*context->resourceManager->GetShader("Deferred_Shader"), "viewPosition");

    cam.position = {7.5,7.5,-7.5};
    cam.target   = {0.0,0.0,0.0};
    cam.fovy     = 7.5f;
    cam.up       = {0.0,1.0,0.0};
    cam.projection = CAMERA_ORTHOGRAPHIC;

    cube = LoadModelFromMesh(GenMeshCube(1.0f,1.0f,1.0f));
    gun  = LoadModel("src/resources/models/isometric_cafe.glb");

    cube.materials[0].shader = *context->resourceManager->GetShader("GbufferShader");
    for (int i = 0; i < gun.materialCount; i++) {
        gun.materials[i].shader = *context->resourceManager->GetShader("GbufferShader");
    }

    gBuffer.frameBufferId = rlLoadFramebuffer();
    rlEnableFramebuffer(gBuffer.frameBufferId);

    //Textures to store pos,normal and albedo
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

    lights[0] = CreateLight(LIGHT_POINT,0.0f,{ -2, 1, -2 }, Vector3Zero(), WHITE,
        *context->resourceManager->GetShader("Deferred_Shader"));

    rlEnableDepthTest();
    rlImGuiSetup(true);
}

void SceneManager::Update(float dt){
    SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
        context->resourceManager->GetShader("Deferred_Shader")->locs[SHADER_LOC_VECTOR_VIEW],
        &cam.position, SHADER_UNIFORM_VEC3);
    UpdateCamera(&cam,CAMERA_ORBITAL);
    if (IsKeyPressed(KEY_Y)) { lights[0].enabled = !lights[0].enabled; }
    for (int i = 0; i < MAX_LIGHTS; i++)
        UpdateLightValues(*context->resourceManager->GetShader("Deferred_Shader"), lights[i]);
}

void SceneManager::Draw(){
    rlEnableFramebuffer(gBuffer.frameBufferId);
    rlViewport(0,0,downWidth,downHeight);
    rlClearColor(255, 255, 255, 255);
    rlClearScreenBuffers();
    rlDisableColorBlend();

    BeginMode3D(cam);
        float Outline=1.0f;
        /*SetShaderValue(*context->resourceManager->GetShader("GbufferShader"),
            GetShaderLocation(*context->resourceManager->GetShader("GbufferShader"),"outline"),
            &Outline,SHADER_UNIFORM_FLOAT);

        DrawModel(cube,Vector3Zero(),0.25,WHITE);
        DrawModelEx(cube,{0.0,-0.25f,0.0f},{0.0f,0.0f,0.0f},0.0f,{5.0f,0.20f,5.0f},WHITE);
        */
        Outline=1.0f;
        SetShaderValue(*context->resourceManager->GetShader("GbufferShader"),
            GetShaderLocation(*context->resourceManager->GetShader("GbufferShader"),"outline"),
            &Outline,SHADER_UNIFORM_FLOAT);
        DrawModelEx(gun,{0.5,0.0,-0.5},{0.0,1.0,0.0},90.0f,{0.4,0.4,0.4},WHITE);
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
    
         rlActiveTextureSlot(texUnitPalette);
        rlEnableTexture(palletes[palleteNames[curPallete]].palleteTex->id); 
        
        SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
            GetShaderLocation(*context->resourceManager->GetShader("Deferred_Shader"), "pallete"),
            &texUnitPalette, RL_SHADER_UNIFORM_SAMPLER2D);


        SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
            GetShaderLocation(*context->resourceManager->GetShader("Deferred_Shader"), "useOutline"),
            &useOutline,SHADER_UNIFORM_FLOAT);

        SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
            GetShaderLocation(*context->resourceManager->GetShader("Deferred_Shader"), "colorSpace"),
            &colorSpace,SHADER_UNIFORM_FLOAT);
         SetShaderValue(*context->resourceManager->GetShader("Deferred_Shader"),
            GetShaderLocation(*context->resourceManager->GetShader("Deferred_Shader"), "palleteSize"),
            &palletes[palleteNames[curPallete]].numColors,SHADER_UNIFORM_INT);

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
        rlViewport(0,0,screenWidth,screenHeight);
        DrawTextureRec((Texture2D){
                .id = gBuffer.positionTextureId,
                .width = screenWidth,
                .height = screenHeight,
            }, (Rectangle) { 0, 0, (float)screenWidth, (float)-screenHeight }, Vector2Zero(), RAYWHITE);
        break;

    case 2:    
        rlViewport(0,0,screenWidth,screenHeight);
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
    rlImGuiBegin();
    ImGui::Begin("Pallette Swap");
    ImGui::Checkbox("Use Outline:",&useOutline);
    ImGui::Checkbox("Use XYZ Mapping:",&colorSpace);
    if (ImGui::BeginCombo("Palette", palleteNames[curPallete].c_str())) {
    for (int i = 0; i < palleteNames.size(); i++) {
        bool isSelected = (curPallete == i);

        if (ImGui::Selectable(palleteNames[i].c_str(), isSelected)) {
            curPallete = i;
        }

        if (isSelected)
            ImGui::SetItemDefaultFocus();
    }
    ImGui::EndCombo();

    }
    ImGui::End();
    ImGui::Begin("Camera Controls");
    ImGui::SliderFloat("Camera Fovy",&cam.fovy,5.0,10.0);
    ImGui::SliderFloat3("Camera Position",&cam.position.x,5.0,15.0);
    ImGui::End();
    ImGui::Begin("Light Controls");
    ImGui::InputFloat3("Light Position",&lights[0].position.x);
    ImGui::SliderFloat("Light intensity",&lights[0].intensity,0.0f,2.0f);
    ImGui::End();

    ImGui::Begin("Texture Controls");
    ImGui::InputInt("Texture Mode ",&mode);
    ImGui::SliderFloat("Light intensity",&lights[0].intensity,0.0f,2.0f);
    ImGui::End();

    rlImGuiEnd();
}
void SceneManager::TakeScreenShot(){
    TakeScreenshot("RGB Space.png");
    Image image=LoadImage("RGB Space.png");
    ImageCrop(&image,{0,270,1920,1080});
    ExportImage(image,"CIE_LAB Space.png");
}