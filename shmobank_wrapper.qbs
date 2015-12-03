import qbs
import "CxxStaticLibrary.qbs" as CxxStaticLibrary
import "ConsoleApplication.qbs" as ConsoleApplication

Project {
    CxxStaticLibrary {
        cpp.cxxFlags: "-std=c++11"
        name: "shmobank++"
        cpp.staticLibraries: ["sharedmem","shmobank"]
        Group {
            name: "headers"
            qbs.install: true
            qbs.installDir: "/usr/include/shmobank++"
            prefix: "src/"
            files: [
                "bank.hpp",
                "memory.hpp",
                "user_types.hpp",
                "msgpolicy.hpp",
                "msgerrors.hpp",
                "producer.hpp",
                "consumer.hpp"
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
