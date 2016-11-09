import qbs

CppApplication {
    consoleApplication: true
    files: [
        "main.cpp",
        "statemachine.cpp",
        "statemachine.hpp",
    ]

    cpp.warningLevel: "all"
    cpp.treatWarningsAsErrors: false
    cpp.cxxLanguageVersion: "c++14"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
