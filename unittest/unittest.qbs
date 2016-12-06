import qbs

CppApplication {
    consoleApplication: true
    files: [
        "../hfsm.cpp",
        "../hfsm.hpp",
        "../hfsm.test.cpp",
        "../simple.hpp",
        "../types.hpp",
    ]

    Group {     // Properties for the produced executable
        fileTagsFilter: product.type
        qbs.install: true
    }

    cpp.staticLibraries: [
        "gtest_main",
        "gtest"
    ]
}
