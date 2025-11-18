cmake_minimum_required(VERSION 3.10)

# Caminho do toolchain do NDK
set(CMAKE_TOOLCHAIN_FILE "/data/data/com.termux/files/home/android-ndk-r27b/build/cmake/android.toolchain.cmake")

# Configurações de build
set(ANDROID_PLATFORM android-24)
set(CMAKE_BUILD_TYPE Release)

# Nome do projeto
project(${PROJECT_TITLE})

# Define o padrão do C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Adiciona os diretórios de include
include_directories(
	${CMAKE_SOURCE_DIR}/include
	${CMAKE_SOURCE_DIR}/src
)

# Caminho das bibliotecas estáticas (.a)
set(MYLIB_DIR "${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}")

# --- Auto-importar todas as .a na pasta ${MYLIB_DIR} ---
# Gera uma lista com todos os arquivos .a
file(GLOB LIB_ARCHIVES "${MYLIB_DIR}/*.a")

# Lista para armazenar os targets importados
set(AUTO_IMPORTED_LIB_TARGETS "")

foreach(libfile IN LISTS LIB_ARCHIVES)
	# libfile = /path/.../libNome.a
	get_filename_component(libname "${libfile}" NAME)		# libNome.a
	# remove prefix "lib" se existir e suffix ".a"
	string(REGEX REPLACE "^lib" "" libname_noprefix "${libname}")
	string(REGEX REPLACE "\\.a$" "" libname_base "${libname_noprefix}")

	# sanitize name -> allow only alnum and underscore
	string(REGEX REPLACE "[^0-9A-Za-z_]" "_" libname_sane "${libname_base}")

	# define um target name único
	set(target_name "external_${libname_sane}")

	# cria target IMPORTED
	add_library(${target_name} STATIC IMPORTED GLOBAL)
	set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION "${libfile}")

	# optional: print info at configure time
	message(STATUS "Auto-imported static lib: ${libfile} -> target: ${target_name}")

	list(APPEND AUTO_IMPORTED_LIB_TARGETS ${target_name})
endforeach()

if (LIB_ARCHIVES STREQUAL "")
	message(STATUS "No .a files found in ${MYLIB_DIR}")
endif()
# --- fim auto-import ---

# Coleta os arquivos-fonte
file(GLOB_RECURSE SOURCE_FILES
	${CMAKE_SOURCE_DIR}/src/*.cpp
)

# Cria a biblioteca compartilhada principal
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

# Adiciona flags de compilação
if (${ANDROID_ABI} STREQUAL "armeabi-v7a")
	target_compile_options(${PROJECT_NAME} PRIVATE -O2 -mfloat-abi=softfp -DNDEBUG)
else()
	target_compile_options(${PROJECT_NAME} PRIVATE -O2 -DNDEBUG)
endif()

# Faz o link das libs auto-importadas + libs do Android
target_link_libraries(${PROJECT_NAME}
	log
	${AUTO_IMPORTED_LIB_TARGETS}
)

# Adiciona definições de constantes
target_compile_definitions(${PROJECT_NAME} PRIVATE
	PROJECT_NAME_STR="${PROJECT_NAME}"
)
