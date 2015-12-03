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
                "src/bank.hpp",
                "src/memory.hpp",
                "src/user_types.hpp",
                "src/msgpolicy.hpp",
                "src/producer.hpp",
                "src/consumer.hpp"
            ]
        }
        Group {
            name: "def_headers"
            qbs.install: true
            qbs.installDir: "/usr/include/shmobank++/default"
            prefix: "src/default/"
            files: [
                "producer.hpp",
                "consumer.hpp",
                "bank.hpp"
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
            files: [ "*.hpp" ]
        }
    }
}
