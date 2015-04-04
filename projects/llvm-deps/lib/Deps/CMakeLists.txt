set(SOURCES
        CallContext.cpp
        InfoflowSignature.cpp
        Slice.cpp
        Infoflow.cpp
        SignatureLibrary.cpp
        StdLibSignatures.cpp
)

add_llvm_loadable_module(Deps
        CallContext.cpp
        InfoflowSignature.cpp
        Slice.cpp
        Infoflow.cpp
        SignatureLibrary.cpp
        StdLibSignatures.cpp
)
