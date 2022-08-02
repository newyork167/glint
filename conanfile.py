import sys
import dependencies
import os

try:
    from conans import ConanFile, CMake
except ImportError:
    print("Conan is not installed! Go to https://conan.io/downloads.html to download or use `pip install conan`", file=sys.stderr)


class GlintConan(ConanFile):
    license = "MIT"
    description = "Glint conan recipe"
    url = "https://github.com/newyork167/glint"
    settings = "os", "compiler", "build_type", "arch"

    requires = dependencies.requires
    generators = ["cmake", "cmake_find_package"]
    default_options = dependencies.options

    def build(self):
        cmake = CMake(self)
        cmake.configure(source_folder='{}'.format(self.source_folder))
        cmake.build()
        cmake.install()

    def imports(self):
        # Path to external src provided as env var from cmake or shell call
        external_src = os.getenv("CONAN_EXTERNAL_PATH", f"{os.getcwd()}/../external/")

        # Copy main bits to bin dir
        self.copy("*.dll", dst="bin", src="bin")
        self.copy("*.so*", dst="bin", src="lib")
        self.copy("*.dylib*", dst="bin", src="lib")
        self.copy("*.bin", dst="bin", src="bin")
        self.copy("*.dat", dst="bin", src="bin")
        self.copy("*.pak", dst="bin", src="bin")

        # Handle putting IMGui in correct place for compiler
        imgui_dst = f"{external_src}/imgui"
        self.copy("imgui_impl_glfw.cpp", dst=f"{imgui_dst}", src="./res/bindings")
        self.copy("imgui_impl_opengl3.cpp", dst=f"{imgui_dst}", src="./res/bindings")
        self.copy("imgui_impl_glfw.h", dst=f"{imgui_dst}", src="./res/bindings")
        self.copy("imgui_impl_opengl3.h", dst=f"{imgui_dst}", src="./res/bindings")
