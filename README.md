# PBRTv4_Tests
## Introduction
A collection of tests that i am doing on the awesome pbrt v4 library. I expect the reader to be comfortable in compiling and running the pbrt v4 from source. I am also assuming that you have enabled GPU and interactive mode for speed of output. Otherwise you will have to wait a few seconds to see the output.

## SimpleScene
The first test i did is to render a simple pbrt scene file but by using my own C++ code in Windows in VisualStudio 2022 in Win32 console application. That is I am calling the pbrt library functions from the libpbrt.lib file that is generated from the pbrt library. The source code is shared in SimpleScene folder. Open the VisualStudio2022 solution and adjust the pbrt library root folders appropriately.
## Output
![SimpleScene](/SimpleScene/SimpleScene.png)


