import os

from conan import ConanFile
from conan.tools.cmake import CMake, CMakeDeps, CMakeToolchain, cmake_layout
from conan.tools.files import copy


class MyLibraryConan(ConanFile):
    name = "libcontrol"
    version = "1.0.0"

    settings = "os", "compiler", "build_type", "arch"
    options = {"shared": [True, False], "fPIC": [True, False]}
    default_options = {"shared": True, "fPIC": True}

    exports_sources = "CMakeLists.txt", "src/*", "include/*"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.variables["BUILD_SHARED_LIBS"] = self.options.shared
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["control"]

    def requirements(self):
        self.requires("flatbuffers/24.12.23")
        self.requires("librpc/1.1.6")
        self.requires("eigen/3.4.1")
        self.requires("spdlog/1.16.0")
        self.requires("sentry-native/0.12.2")
        self.requires("sentry-crashpad/0.6.5")

    def configure(self):
        if self.settings.os == "Linux":
            self.options.fPIC = True
