import qbs
import "CxxStaticLibrary.qbs" as CxxStaticLibrary
import "ConsoleApplication.qbs" as ConsoleApplication

Project {
    CxxStaticLibrary {
        cpp.cxxFlags: "-std=c++11"
        name: "shmobank++"
        cpp.staticLibraries: ["sharedmem","shmobank"]
        files: [

        ]
        Group {
            name: "headers"
            qbs.install: true
            qbs.installDir: "/usr/include/shmobank++"
            files: [
                "src/message_bank.h",
                "src/shared_memory.h",
                "src/user_types.h",
                "src/message.h",
            ]
        }
        Group {
            fileTagsFilter: product.type
            qbs.install: true
            qbs.installDir: "/usr/lib"
        }
    }
    ConsoleApplication {
        cpp.cxxFlags: "-std=c++11"
        name: "tst_shmobank++"
        type: ["application"]
        cpp.staticLibraries: [ "shmobank", "sharedmem", "gpio" ]
        cpp.dynamicLibraries: ["pthread", "asound", "check", "rt", "boost_unit_test_framework","boost_system"]
        cpp.includePaths: [ "src/" ]
        files: [ "src/tests/tst.cpp" ]
        Group {
            name: "shmobank"
            prefix: "src/"
            files: [ "*.h", "*.cpp" ]
        }
    }
}
