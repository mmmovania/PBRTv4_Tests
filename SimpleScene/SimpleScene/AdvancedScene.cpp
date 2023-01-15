// SimpleScene.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>

#define PBRT_HAS_INTRIN_H
#define PBRT_IS_WINDOWS
#define PBRT_BUILD_GPU_RENDERER
#define PBRT_IS_MSVC
#define _ENABLE_EXTENDED_ALIGNED_STORAGE
#define NOMINMAX
#define PBRT_NOINLINE = __declspec(noinline)
#define PBRT_RESTRICT = __restrict
#define PBRT_HAVE__ALIGNED_MALLOC
#define PTEX_STATIC
#if _DEBUG
#define PBRT_DEBUG_BUILD
#endif
#define UTF8PROC_STATIC

#define PBRT_BUILD_NATIVE_EXECUTABLE
#define PBRT_DBG_LOGGING

#include <pbrt/pbrt.h>
#include <pbrt/cpu/render.h>
#include <pbrt/scene.h> 
#include <pbrt/gpu/memory.h>
#include <pbrt/gpu/util.h>
#include <pbrt/wavefront/wavefront.h>

using namespace pbrt;

int main()
{
    pbrt::FileLoc loc; 

    PBRTOptions options;
    options.useGPU = true;
    options.interactive = true;
    options.logFile = "log.txt";

    InitPBRT(options); 

    BasicScene scene; 
    BasicSceneBuilder builder(&scene);
       
    //Camera parameters
    pbrt::ParsedParameterVector pv1;
    pbrt::ParsedParameter* params1 = new pbrt::ParsedParameter(loc);
    params1->name = "fov";
    params1->type = "float"; 
    params1->AddFloat(45);
    pv1.push_back(params1);

    builder.LookAt(3, 4, 1.5, 0.5, 0.5, 0, 0, 0, 1, loc);
    builder.Camera("perspective", pv1, loc);         

    //Sampler parameters
    pbrt::ParsedParameterVector pv2;
    pbrt::ParsedParameter* params2 = new pbrt::ParsedParameter(loc);
    params2->name = "pixelsamples";
    params2->type = "integer";
    params2->AddInt(128);
    pv2.push_back(params2);

    builder.Sampler("halton", pv2, loc);    

    //Integrator parameters
    pbrt::ParsedParameterVector pv3;  
    builder.Integrator("path", pv3, loc);
    
    //Film parameters
    pbrt::ParsedParameterVector pv4;
    pbrt::ParsedParameter* params41 = new pbrt::ParsedParameter(loc);
    params41->name = "yresolution";
    params41->type = "integer";
    params41->AddInt(400);
    pv4.push_back(params41);
        
    pbrt::ParsedParameter* params42 = new pbrt::ParsedParameter(loc);
    params42->name = "xresolution";
    params42->type = "integer";
    params42->AddInt(400);
    pv4.push_back(params42);
        
    pbrt::ParsedParameter* params43 = new pbrt::ParsedParameter(loc);
    params43->name = "filename";
    params43->type = "string";
    params43->AddString("simple.png");
    pv4.push_back(params43);
         
    builder.Film("rgb", pv4, loc);
    
    builder.WorldBegin(loc); 

    //Light source infinite
    pbrt::ParsedParameterVector pv5;
    pbrt::ParsedParameter* params5 = new pbrt::ParsedParameter(loc);
    params5->name = "L";
    params5->type = "rgb";

    params5->AddFloat(0.4);
    params5->AddFloat(0.45);
    params5->AddFloat(0.5);
         
    pv5.push_back(params5);

    builder.LightSource("infinite", pv5, loc);
   
    //Light source distant
    pbrt::ParsedParameterVector pv6;
    pbrt::ParsedParameter* params61 = new pbrt::ParsedParameter(loc);
    params61->name = "scale";
    params61->type = "float";
    params61->AddFloat(1.5f); 
    pv6.push_back(params61);

    pbrt::ParsedParameter* params62 = new pbrt::ParsedParameter(loc);
    params62->name = "L";
    params62->type = "blackbody";
    params62->AddFloat(3000);
    pv6.push_back(params62);

    pbrt::ParsedParameter* params63 = new pbrt::ParsedParameter(loc);
    params63->name = "from";
    params63->type = "point3";
    params63->AddFloat(-30.0f); 
    params63->AddFloat(40.0f);  
    params63->AddFloat(100.0f); 
    pv6.push_back(params63);

    builder.LightSource("distant", pv6, loc);
    
    builder.AttributeBegin(loc);
        pbrt::ParsedParameterVector pv7; 
        builder.Material("dielectric", pv7, loc);
         
        pbrt::ParsedParameterVector pv8;
        pbrt::ParsedParameter* params8 = new pbrt::ParsedParameter(loc);
        params8->name = "radius";
        params8->type = "float";
        params8->AddFloat(1.0f);
        pv8.push_back(params8);

        builder.Shape("sphere", pv8, loc);        
    builder.AttributeEnd(loc);
    
    builder.AttributeBegin(loc);
        pbrt::ParsedParameterVector pv9;
        pbrt::ParsedParameter* t2 = new pbrt::ParsedParameter(loc);
        t2->name = "tex2";
        t2->type = "rgb";
        t2->AddFloat(0.8);
        t2->AddFloat(0.8);
        t2->AddFloat(0.8);
        pv9.push_back(t2);

        pbrt::ParsedParameter* t1 = new pbrt::ParsedParameter(loc);
        t1->name = "tex1";
        t1->type = "rgb";
        t1->AddFloat(0.1);
        t1->AddFloat(0.1);
        t1->AddFloat(0.1);
        pv9.push_back(t1);
        
        pbrt::ParsedParameter* vscale = new pbrt::ParsedParameter(loc);
        vscale->name = "vscale";
        vscale->type = "float";
        vscale->AddFloat(8);
        pv9.push_back(vscale);

        pbrt::ParsedParameter* uscale = new pbrt::ParsedParameter(loc);
        uscale->name = "uscale";
        uscale->type = "float";
        uscale->AddFloat(8);
        pv9.push_back(uscale);

        builder.Texture("checks", "spectrum", "checkerboard", pv9, loc);

        pbrt::ParsedParameterVector pv10;
        pbrt::ParsedParameter* params10 = new pbrt::ParsedParameter(loc);
        params10->name = "reflectance";
        params10->type = "texture";
        params10->AddString("checks");
        pv10.push_back(params10);
        builder.Material("diffuse", pv10, loc);

        pbrt::ParsedParameterVector pv11;
        builder.Translate(0, 0, -1, loc);

        pbrt::ParsedParameterVector pv12;
        pbrt::ParsedParameter* uv = new pbrt::ParsedParameter(loc);
        uv->name = "uv";
        uv->type = "point2";
        uv->AddFloat(0.0f);        uv->AddFloat(0.0f);
        uv->AddFloat(1.0f);        uv->AddFloat(0.0f);
        uv->AddFloat(1.0f);        uv->AddFloat(1.0f);
        uv->AddFloat(0.0f);        uv->AddFloat(1.0f);

        pv12.push_back(uv);

        pbrt::ParsedParameter* P = new pbrt::ParsedParameter(loc);
        P->name = "P";
        P->type = "point3";
        P->AddFloat(-20.0f);        P->AddFloat(-20.0f);        P->AddFloat(0.0f);
        P->AddFloat(20.0f);         P->AddFloat(-20.0f);        P->AddFloat(0.0f);
        P->AddFloat(20.0f);         P->AddFloat(20.0f);         P->AddFloat(0.0f);
        P->AddFloat(-20.0f);        P->AddFloat(20.0f);         P->AddFloat(0.0f);

        pv12.push_back(P);

        pbrt::ParsedParameter* indices = new pbrt::ParsedParameter(loc);
        indices->name = "indices";
        indices->type = "integer";
        indices->AddInt(0);         indices->AddInt(1);         indices->AddInt(2);
        indices->AddInt(0);         indices->AddInt(2);         indices->AddInt(3);
        pv12.push_back(indices);

        builder.Shape("trianglemesh", pv12, loc);
    builder.AttributeEnd(loc);

    builder.EndOfFiles();

    if(options.useGPU)
        RenderWavefront(scene);
    else
        RenderCPU(scene);
     
    CleanupPBRT();

    delete params1;

    delete params2;

    delete params41;
    delete params42;
    delete params43;

    delete params5;

    delete params61;
    delete params62;
    delete params63;

    delete params8;

    delete params10; 

    delete t1; 
    delete t2;
    delete uscale;
    delete vscale;
    delete uv;
    delete P;
    delete indices;

    return 0;
}