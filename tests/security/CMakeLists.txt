# Chercher tous les fichiers se terminant par SecurityTest.hpp
file(GLOB SECURITY_TEST_HEADERS "*SecurityTest.hpp")

# Ajouter tous ces headers dans l'exécutable cryptoGL_tests
target_sources(cryptoGL_tests PRIVATE ${SECURITY_TEST_HEADERS})