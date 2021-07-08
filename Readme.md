# [Glint](https://github.com/newyork167/glint)
![Screenshot](https://upload.wikimedia.org/wikipedia/commons/a/ae/BallsRender.png)

## Summary
Glint is based on [Glitter](http://polytonic.github.io/Glitter/) using instead a package manager called [conan](https://conan.io/) and some basic setup for each third-party library that you can use as desired.

This also adds in ImGUI for adding gui configurations quickly and easily along with many other useful libraries!

### TODO
- Add in native project format generator for visual studio?
- Add in test scene with more complex geometry
- Add a branch for Vulkan (maybe another repo would be better)

## Getting Started
Download cmake: [cmake](http://www.cmake.org/download/)

Download conan: [conan](https://conan.io/downloads.html)
- Can also be installed with ```pip install conan```

The CMake script will handle calling all of the relevant conan install/update functions. 

```bash
git clone https://github.com/newyork167/glint

########################################################################
# If not using Visual Studio or another IDE with cmake or conan plugin #
########################################################################
cd glint
mkdir build && cd build

# UNIX Makefile
cmake ..

# Mac OSX
cmake -G "Xcode" ..

# Microsoft Windows
cmake -G "Visual Studio 15 2017 Win64" ..
cmake -G "Visual Studio 16 2019" -A "x64" ..
```

## Third Party Libraries
[Dependencies](https://conan.io/center/) can be added in the dependencies.py file. The following are installed at baseline.

Functionality           | Library
----------------------- | ------------------------------------------
Mesh Loading            | [assimp](https://github.com/assimp/assimp)
Physics                 | [bullet](https://github.com/bulletphysics/bullet3)
OpenGL Function Loader  | [glad](https://github.com/Dav1dde/glad)
Windowing and Input     | [glfw](https://github.com/glfw/glfw)
OpenGL Mathematics      | [glm](https://github.com/g-truc/glm)
Texture Loading         | [stb](https://github.com/nothings/stb)
Dead Simple Gui         | [imgui](https://github.com/ocornut/imgui)
Fast C++ Logging        | [spdlog](https://github.com/gabime/spdlog)

## Conan Package Manager Steps
If you would like to manually run: ```conan install ./conanfile.py```

## Issues
When upgrading to Big Sur, I needed to do the following to get the linkage to the new sdk to match JetBrains Darwin.cmake: [https://developer.apple.com/forums/thread/667561](https://developer.apple.com/forums/thread/667561)

## License
>The MIT License (MIT)

>Copyright (c) 2021 Cody Dietz

>Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

>The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

>THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
