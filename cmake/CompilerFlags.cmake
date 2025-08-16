# Compiler-specific flags for cryptographic security
function(apply_crypto_compiler_flags TARGET)
    target_compile_options(${TARGET} PRIVATE
        # Security flags - Enable comprehensive warnings
        $<$:-Wall -Wextra -Wpedantic -Werror -pthread>
        #   -Wall: Enable all common warning flags
        #   -Wextra: Enable extra warnings not covered by -Wall
        #   -Wpedantic: Strictly follow C++ standard, reject non-standard extensions
        
        $<$:/W4>
        #   /W4: Enable all warnings (MSVC equivalent to -Wall -Wextra)
        
        # Cryptographic security - Prevent undefined behavior
        $<$:-fno-strict-overflow>
        #   -fno-strict-overflow: Don't assume signed overflow is undefined
        #   Critical for cryptographic operations where overflow might be intentional
        #   Prevents compiler from optimizing away overflow checks
        
        $<$:-fno-delete-null-pointer-checks>
        #   -fno-delete-null-pointer-checks: Don't optimize away null pointer checks
        #   Essential for secure memory management in cryptographic code
        #   Prevents compiler from removing safety checks
        
        # Optimization flags - Build configuration specific
        $<$:-O3>
        #   -O3: Maximum optimization level for release builds
        #   Enables aggressive optimizations for performance-critical crypto operations
        #   Includes loop unrolling, vectorization, and interprocedural optimizations
        
        $<$:-O0 -g>
        #   -O0: No optimization for easier debugging
        #   -g: Include debug symbols for stack traces and debugging
        #   Essential for development and security auditing
    )
endfunction()