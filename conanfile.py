from conans import ConanFile, CMake


class MadlabConan(ConanFile):
    name = "madlab"
    version = "0.1.0"

    scm = {
        "type": "git",
        "url": "https://github.com/JiveHelix/symbolic.git",
        "revision": "auto",
        "submodule": "recursive"}

    license = "MIT"
    author = "Jive Helix (jivehelix@gmail.com)"
    url = "https://github.com/JiveHelix/symbolic"
    description = "Symbolic algebral library"
    topics = ("C++", "Algebra", "Linear Algebra")

    settings = "os", "compiler", "build_type", "arch"

    generators = ["cmake"]

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()

    def package(self):
        cmake = CMake(self)
        cmake.install()

    def package_info(self):
        self.cpp_info.libs = ["symbolic"]

    def requirements(self):
        self.requires("jive/[~1.1]")
        self.requires("tau/[~1.10]")
        self.requires("fmt/[~10]")
