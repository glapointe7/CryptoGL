# Cryptographic security hardening
function(apply_crypto_security_flags TARGET)
    if(UNIX OR APPLE)
        set(IS_UNIX_OR_APPLE 1)
    else()
        set(IS_UNIX_OR_APPLE 0)
    endif()

    # Linux / macOS (GCC et Clang)
    target_compile_options(${TARGET} PRIVATE
        # Stack protection - Prevent stack buffer overflows
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-fstack-protector-strong>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-fstack-protector-strong>
        #   -fstack-protector-strong: Insert stack canaries for all functions
        #   Protects against stack-based buffer overflow attacks
        #   Critical for cryptographic functions that handle sensitive data

        #$<$:/GS>
        #   /GS: MSVC equivalent to -fstack-protector-strong
        #   Enables stack buffer overrun detection

        # Position Independent Executable - Address Space Layout Randomization
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-fPIE>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-fPIE>
        #   -fPIE: Generate position-independent executable
        #   Enables ASLR (Address Space Layout Randomization) at runtime
        #   Makes it harder for attackers to predict memory addresses

        #$<$:/DYNAMICBASE>
        #   /DYNAMICBASE: MSVC equivalent to -fPIE
        #   Enables ASLR for Windows executables

        # Non-executable stack - Prevent code injection
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-fno-stack-check>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-fno-stack-check>
        #   -fno-stack-check: Disable stack checking (performance optimization)
        #   Note: This is safe when combined with -fstack-protector-strong
        #   Prevents stack-based code execution attacks

        #$<$:/NXCOMPAT>
        #   /NXCOMPAT: MSVC equivalent to -Wl,-z,noexecstack
        #   Marks executable as compatible with Data Execution Prevention (DEP)
        #   Prevents execution of code from non-executable memory pages

        # Additional Windows security flags
        #$<$:/HIGHENTROPYVA>
        #   /HIGHENTROPYVA: Enable 64-bit address space randomization
        #   Provides stronger ASLR by using full 64-bit address space
        #   Critical for 64-bit applications to prevent address prediction
    )

    target_link_options(${TARGET} PRIVATE
        # Read-only relocations - Memory protection
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-Wl,-z,relro>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-Wl,-z,relro>
        #   -Wl,-z,relro: Make relocation table read-only after loading
        #   Prevents modification of function pointers and GOT entries
        #   Critical for preventing GOT (Global Offset Table) hijacking attacks

        # Immediate binding - Prevent lazy binding attacks
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-Wl,-z,now>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-Wl,-z,now>
        #   -Wl,-z,now: Resolve all symbols at load time (no lazy binding)
        #   Prevents PLT (Procedure Linkage Table) hijacking attacks
        #   Slightly increases startup time but improves security

        # Non-executable stack - Prevent stack-based code execution
        $<$<AND:$<CXX_COMPILER_ID:GNU>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-Wl,-z,noexecstack>
        $<$<AND:$<CXX_COMPILER_ID:Clang>,$<BOOL:${IS_UNIX_OR_APPLE}>>:-Wl,-z,noexecstack>
        #   -Wl,-z,noexecstack: Mark stack as non-executable
        #   Prevents execution of shellcode injected via stack overflow
        #   Essential for preventing return-oriented programming (ROP) attacks
    )

    # Windows / MSVC
    target_compile_options(${TARGET} PRIVATE
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,WIN32>:/GS>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,WIN32>:/DYNAMICBASE>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,WIN32>:/NXCOMPAT>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,WIN32>:/HIGHENTROPYVA>
        $<$<AND:$<CXX_COMPILER_ID:MSVC>,WIN32>:/guard:cf>
    )
endfunction()