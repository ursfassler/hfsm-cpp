import qbs

CppApplication {
    consoleApplication: true
    files: [
        "debug.cpp",
        "debug.hpp",
        "hfsm.cpp",
        "hfsm.hpp",
        "hfsmprint.cpp",
        "hfsmprint.hpp",
        "lambda.hpp",
        "main.cpp",
        "simple.hpp",
        "types.hpp",
    ]

    cpp.warningLevel: "all"
    cpp.treatWarningsAsErrors: false
    cpp.cxxLanguageVersion: "c++14"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
