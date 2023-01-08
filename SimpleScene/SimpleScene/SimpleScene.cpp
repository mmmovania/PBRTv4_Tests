// SimpleScene.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#define _CRT_SECURE_NO_WARNINGS 

#include <iostream>

#define PBRT_HAS_INTRIN_H
#define PBRT_IS_WINDOWS
#define PBRT_BUILD_GPU_RENDERER

#include <pbrt/pbrt.h>
#include <pbrt/scene.h> 

#include <pbrt/wavefront/wavefront.h>

using namespace pbrt;

int main()
{
    std::vector<std::string> filenames;
    filenames.push_back("./scenes/simple_scene.pbrt");

    PBRTOptions options;
    options.useGPU = true;
    options.interactive = true;

    InitPBRT(options);

    BasicScene scene;
    BasicSceneBuilder builder(&scene);
    ParseFiles(&builder, filenames);

    RenderWavefront(scene);

    CleanupPBRT();

    return 0;
} 