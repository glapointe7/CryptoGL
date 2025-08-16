# Chercher tous les fichiers se terminant par Test.hpp
file(GLOB UNIT_TEST_HEADERS "*Test.hpp")

# Ajouter tous ces headers dans l'exécutable cryptoGL_tests
target_sources(cryptoGL_tests PRIVATE ${UNIT_TEST_HEADERS})