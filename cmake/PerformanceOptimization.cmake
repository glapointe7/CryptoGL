# Performance optimization and SIMD detection
function(detect_cpu_features)
    include(CheckCXXCompilerFlag)
    
    # Check for AVX2 support - Advanced Vector Extensions 2
    check_cxx_compiler_flag("-mavx2" COMPILER_SUPPORTS_AVX2)
    if(COMPILER_SUPPORTS_AVX2)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mavx2")
        add_definitions(-DUSE_AVX2)
        #   -mavx2: Enable AVX2 instruction set (256-bit vector operations)
        #   Provides 2x performance improvement for cryptographic operations
        #   Critical for AES, SHA, and other crypto algorithms
        #   Supports 256-bit integer and floating-point vector operations
    endif()
    
    # Check for SSE4.2 support - Streaming SIMD Extensions 4.2
    check_cxx_compiler_flag("-msse4.2" COMPILER_SUPPORTS_SSE42)
    if(COMPILER_SUPPORTS_SSE42)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -msse4.2")
        add_definitions(-DUSE_SSE42)
        #   -msse4.2: Enable SSE4.2 instruction set (128-bit vector operations)
        #   Provides 4x performance improvement for cryptographic operations
        #   Essential for older CPUs that don't support AVX2
        #   Includes CRC32, POPCNT, and other crypto-friendly instructions
    endif()
endfunction()

function(apply_performance_flags TARGET)
    target_compile_options(${TARGET} PRIVATE
        # Link Time Optimization - Cross-module optimization
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-flto>
        #   -flto: Enable Link Time Optimization
        #   Performs optimizations across all source files during linking
        #   Can improve performance by 5-15% for cryptographic libraries
        #   Enables cross-module inlining and dead code elimination
        
        # Function inlining - Reduce function call overhead
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-finline-functions>
        #   -finline-functions: Inline functions that are called once
        #   Eliminates function call overhead for small, frequently-called functions
        #   Critical for cryptographic primitives (S-box lookups, bit operations)
        #   Can improve performance by 10-20% for crypto-heavy code
        
        # Interprocedural optimization - Cross-function analysis
        $<$<OR:$<CXX_COMPILER_ID:GNU>,$<CXX_COMPILER_ID:Clang>>:-fipa-pta>
        #   -fipa-pta: Interprocedural Pointer Analysis
        #   Analyzes pointer usage across function boundaries
        #   Enables better optimization of memory access patterns
        #   Important for cryptographic algorithms with complex data structures

        $<$<CXX_COMPILER_ID:GNU>:-fomit-frame-pointer -funroll-loops -ftree-vectorize>
        $<$<CXX_COMPILER_ID:Clang>:-fomit-frame-pointer -funroll-loops -fvectorize>
        $<$<CXX_COMPILER_ID:MSVC>:/O2 /GL /arch:AVX2>
    )
    
    # Add SIMD preprocessor definitions
    target_compile_definitions(${TARGET} PRIVATE
        $<$<BOOL:${CMAKE_CXX_COMPILER_ID}>:HAVE_AVX2>
        $<$<BOOL:${CMAKE_CXX_COMPILER_ID}>:HAVE_SSE4_2>
    )

    set_target_properties(${TARGET} PROPERTIES
        # CMake-level interprocedural optimization
        INTERPROCEDURAL_OPTIMIZATION TRUE
        #   INTERPROCEDURAL_OPTIMIZATION: CMake equivalent to -flto
        #   Ensures LTO is enabled regardless of compiler-specific flags
        #   Provides consistent optimization behavior across different compilers
        #   Without LTO: each .cpp is compiled separatly → the compiler won't see the other files.
        #   With LTO: the compiler will see the entire project when linking → It can merge, delete, 
        #   reorganize or inline through many files.
    )
endfunction()