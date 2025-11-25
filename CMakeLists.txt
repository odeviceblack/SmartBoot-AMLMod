cmake_minimum_required(VERSION 3.10)

# Configurações Android
set(ANDROID_PLATFORM android-24) # Android 7.0+
set(CMAKE_BUILD_TYPE Release)

# Nome do projeto
project(${PROJECT_TITLE})

# Define C++ padrão
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable file logging
option(LOG_IN_FILE "Enable logging to file" OFF)

if(LOG_IN_FILE)
	add_compile_definitions(LOG_IN_FILE)
endif()

# Inclui subdiretórios
add_subdirectory(src)
add_subdirectory(libs)

# Includes da pasta include
include_directories(
    ${CMAKE_SOURCE_DIR}/include
)

# Cria a lib final
add_library(${PROJECT_NAME} SHARED ${SRC_ALL_FILES})

# Flags de compilação
if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
    target_compile_options(${PROJECT_NAME} PRIVATE -O2 -mfloat-abi=softfp -DNDEBUG)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -O2 -DNDEBUG)
endif()

# Linka libs Android + libs vindas da pasta libs
target_link_libraries(${PROJECT_NAME}
    log
    ${LIBS_IMPORTED}
)

# Constantes
target_compile_definitions(${PROJECT_NAME} PRIVATE
    PROJECT_NAME_STR="${PROJECT_NAME}"
)
