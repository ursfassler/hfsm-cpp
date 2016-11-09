import qbs

CppApplication {
    consoleApplication: true
    files: [
        "fullstatemachine.cpp",
        "fullstatemachine.hpp",
        "main.cpp",
    ]

    cpp.warningLevel: "all"
    cpp.treatWarningsAsErrors: false
    cpp.cxxLanguageVersion: "c++14"

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
