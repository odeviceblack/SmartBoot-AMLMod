# Coletar todos os .cpp da pasta src e suas subpastas
file(GLOB_RECURSE SRC_ALL_FILES
    ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp
)

# Expor variável para o CMakeLists pai
set(SRC_ALL_FILES "${SRC_ALL_FILES}" PARENT_SCOPE)
