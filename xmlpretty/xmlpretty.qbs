import qbs

CppApplication {
    consoleApplication: true
    files: [
        "../hfsm.cpp",
        "../hfsm.hpp",
        "../simple.hpp",
        "../types.hpp",
        "main.cpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }
}
