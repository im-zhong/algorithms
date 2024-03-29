from conan import ConanFile
from conan.tools.cmake import CMakeToolchain, CMake, cmake_layout, CMakeDeps
import os


class mycppRecipe(ConanFile):
    name = "Algorithms"
    version = "0.0.1"
    package_type = "application"

    # Optional metadata
    license = "<Put the package license here>"
    author = "<Put your name here> <And your email here>"
    url = "<Package recipe repository url here, for issues about the package>"
    description = "<Description of zcc package here>"
    topics = ("<Put some tag here>", "<here>", "<and here>")

    # Binary configuration
    settings = "os", "compiler", "build_type", "arch"

    # Sources are located in the same place as this recipe, copy them to the recipe
    exports_sources = "CMakeLists.txt", "src/*"

    def layout(self):
        cmake_layout(self)

    def generate(self):
        deps = CMakeDeps(self)
        deps.generate()
        tc = CMakeToolchain(self)
        tc.generate()

    def build(self):
        cmake = CMake(self)
        cmake.configure()
        cmake.build()
        # 我们希望你可以自动的生成 db 文件
        # print(self.settings.build_type)
        # 它在执行build的时候 pwd是在build文件夹里面的
        db_path = os.path.join(
            "build", str(self.settings.build_type), "compile_commands.json")
        # 当被链接的文件不存在的时候 这个判断的结果也是错的
        # 所以正确的方法应该是在conan clean中把db符号链接文件也删掉
        if os.path.exists("../../compile_commands.json"):
            os.unlink("../../compile_commands.json")
        os.symlink(db_path, "../../compile_commands.json")

    def package(self):
        cmake = CMake(self)
        cmake.install()
