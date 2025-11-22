# Diretório das libs específicas da ABI
set(MYLIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/${ANDROID_ABI}")

# LISTA MANUAL -> você adiciona aqui somente o que quiser linkar
# Exemplo:
set(MANUAL_LIBS
    libLogger.a
)

set(LIBS_IMPORTED "")

foreach(libname IN LISTS MANUAL_LIBS)
    set(fullpath "${MYLIB_DIR}/${libname}")

    if(NOT EXISTS "${fullpath}")
        message(FATAL_ERROR "Library not found: ${fullpath}")
    endif()

    # gera nome sanitizado
    string(REGEX REPLACE "^lib" "" base "${libname}")
    string(REGEX REPLACE "\\.a$" "" base "${base}")
    string(REGEX REPLACE "[^0-9A-Za-z_]" "_" base "${base}")
    set(target_name "external_${base}")

    # cria biblioteca importada
    add_library(${target_name} STATIC IMPORTED GLOBAL)
    set_target_properties(${target_name} PROPERTIES IMPORTED_LOCATION "${fullpath}")

    message(STATUS "Imported manual static lib: ${fullpath} -> target ${target_name}")

    list(APPEND LIBS_IMPORTED ${target_name})
endforeach()

# Exporta para o CMake principal
set(LIBS_IMPORTED "${LIBS_IMPORTED}" PARENT_SCOPE)
