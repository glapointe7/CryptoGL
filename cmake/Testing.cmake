# Comprehensive testing configuration for header-only tests
function(add_crypto_test_runner TEST_RUNNER_NAME SRC)
    find_package(GTest REQUIRED)
    
    # Create test runner executable
    add_executable(${TEST_RUNNER_NAME} ${SRC})
    
    # Include tests/unit directory for header-only test files
    target_include_directories(${TEST_RUNNER_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/tests/unit
    )
    
    # Link with required libraries
    target_link_libraries(${TEST_RUNNER_NAME} PRIVATE
        gtest
        gtest_main
        cryptoGL::Core
        cryptoGL::Algorithms
        cryptoGL::BigIntegers
    )
    
    # Apply security flags
    apply_crypto_security_flags(${TEST_RUNNER_NAME})
    
    # Apply performance flags
    apply_performance_flags(${TEST_RUNNER_NAME})
    
    # Add to CTest
    add_test(NAME ${TEST_RUNNER_NAME} COMMAND ${TEST_RUNNER_NAME})
    
    # Set test properties
    set_tests_properties(${TEST_RUNNER_NAME} PROPERTIES
        TIMEOUT 300
        ENVIRONMENT "CRYPTO_SECURITY_LEVEL=MAXIMUM"
    )
endfunction()

# Alternative: Individual test target creation
function(add_crypto_individual_test TEST_NAME TEST_SOURCES)
    find_package(GTest REQUIRED)
    
    add_executable(${TEST_NAME} ${TEST_SOURCES})
    
    # Include UnitTests directory
    target_include_directories(${TEST_NAME} PRIVATE
        ${CMAKE_SOURCE_DIR}/UnitTests
    )
    
    # Link with required libraries
    target_link_libraries(${TEST_NAME} PRIVATE
        gtest
        gtest_main
        cryptoGL::Core
        cryptoGL::Algorithms
        cryptoGL::BigIntegers
    )
    
    # Apply security flags
    apply_crypto_security_flags(${TEST_NAME})
    
    # Apply performance flags
    apply_performance_flags(${TEST_NAME})
    
    # Add to CTest
    add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})
endfunction()

# Recommended: Hybrid approach for comprehensive testing
function(add_crypto_test_suite SUITE_NAME)
    find_package(GTest REQUIRED)
    
    # Create main test suite executable (runs ALL tests)
    add_executable(${SUITE_NAME}_all ${ARGN})
    
    # Include UnitTests directory
    target_include_directories(${SUITE_NAME}_all PRIVATE
        ${CMAKE_SOURCE_DIR}/UnitTests
    )
    
    # Link with required libraries
    target_link_libraries(${SUITE_NAME}_all PRIVATE
        gtest
        gtest_main
        cryptoGL::Core
        cryptoGL::Algorithms
        cryptoGL::BigIntegers
    )
    
    # Apply security flags
    apply_crypto_security_flags(${SUITE_NAME}_all)
    
    # Apply performance flags
    apply_performance_flags(${SUITE_NAME}_all)
    
    # Add to CTest as main test suite
    add_test(NAME ${SUITE_NAME}_all_tests COMMAND ${SUITE_NAME}_all)
    
    # Create individual algorithm test targets (optional)
    # These can run specific algorithm tests independently
    foreach(ALGO aes des sha256 serpent blowfish)
        add_executable(${SUITE_NAME}_${ALGO} 
            tests/${ALGO}TestRunner.cpp
        )
        
        target_include_directories(${SUITE_NAME}_${ALGO} PRIVATE
            ${CMAKE_SOURCE_DIR}/UnitTests
        )
        
        target_link_libraries(${SUITE_NAME}_${ALGO} PRIVATE
            gtest
            gtest_main
            cryptoGL::Core
            cryptoGL::Algorithms
            cryptoGL::BigIntegers
        )
        
        apply_crypto_security_flags(${SUITE_NAME}_${ALGO})
        add_test(NAME ${SUITE_NAME}_${ALGO} COMMAND ${SUITE_NAME}_${ALGO})
    endforeach()
endfunction()

# Usage Example for Header-Only Tests
# In your tests_run/CMakeLists.txt:
# 
# # Option 1: Main test runner (includes all HPP test files)
# add_crypto_test_runner(unit_tests
#     TestsRun.cpp
# )
# 
# # Option 2: Hybrid approach - BEST RECOMMENDATION
# # This gives you BOTH "run all tests" AND individual algorithm testing
# add_crypto_test_suite(crypto_tests
#     TestsRun.cpp  # Your main runner that includes all .hpp files
# )
# 
# # Option 3: Individual test targets (if you need specific algorithm testing)
# add_crypto_individual_test(aes_tests
#     tests/AESTestRunner.cpp  # Only if you have individual runners
# )
# 
# # Enable testing
# enable_testing()