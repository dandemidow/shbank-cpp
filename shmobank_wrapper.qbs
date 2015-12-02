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
                "src/bank.h",
                "src/memory.h",
                "src/user_types.h",
                "src/msgpolicy.h",
                "src/message.h",
                "src/producer.h",
                "src/consumer.h"
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
        cpp.staticLibraries: [ "shmobank", "sharedmem" ]
        cpp.dynamicLibraries: ["pthread", "boost_unit_test_framework","boost_system", "rt"]
        cpp.includePaths: [ "src/" ]
        files: [ "src/tests/tst.cpp" ]
        Group {
            name: "shmobank"
            prefix: "src/"
            files: [ "*.h", "*.cpp" ]
        }
    }
}
