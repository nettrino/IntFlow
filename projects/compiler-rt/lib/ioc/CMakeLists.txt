# Build for the undefined behavior sanitizer runtime support library.

set(IOC_SOURCES
  ioc_report.c
  )

include_directories(..)

set(IOC_CFLAGS ${SANITIZER_COMMON_CFLAGS})

set(IOC_RUNTIME_LIBRARIES)

if(CAN_TARGET_X86_64)
  add_library(clang_rt.ioc-x86_64 STATIC ${IOC_SOURCES})
  set_target_compile_flags(clang_rt.ioc-x86_64
    ${UBSAN_CFLAGS} ${TARGET_X86_64_CFLAGS}
    )
  list(APPEND IOC_RUNTIME_LIBRARIES clang_rt.ioc-x86_64)
endif()

if(CAN_TARGET_I386)
  add_library(clang_rt.ioc-i386 STATIC ${IOC_SOURCES})
  set_target_compile_flags(clang_rt.ioc-i386
    ${IOC_CFLAGS} ${TARGET_I386_CFLAGS}
    )
  list(APPEND UBSAN_RUNTIME_LIBRARIES clang_rt.ioc-i386)
endif()

set_property(TARGET ${IOC_RUNTIME_LIBRARIES} APPEND PROPERTY
  COMPILE_DEFINITIONS ${IOC_COMMON_DEFINITIONS})
add_clang_compiler_rt_libraries(${IOC_RUNTIME_LIBRARIES})

