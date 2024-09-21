cmake_minimum_required(VERSION 3.12)

# Change your executable name to something creative!
set(NAME OnOrbitPico) # <-- Name your project/executable here!

if (DEFINED ENV{PICO_SDK_PATH} AND (NOT PICO_SDK_PATH))
    set(PICO_SDK_PATH $ENV{PICO_SDK_PATH})
    message("Using PICO_SDK_PATH from environment ('${PICO_SDK_PATH}')")
endif ()

include(pico_sdk_import.cmake)

# Gooey boilerplate
project(${NAME} C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialize the SDK
pico_sdk_init()

# Add your source files
add_executable(${NAME}
        Side_LEDs.cpp
        main.cpp
)

add_subdirectory(pico_neopixels)

target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# Don't forget to link the libraries you need!
target_link_libraries(${NAME}
    pico_stdlib
    pico_malloc
    pico_mem_ops
    pico_neopixel
)

# create map/bin/hex file etc.
pico_add_extra_outputs(${NAME})

pico_enable_stdio_usb(${NAME} 1)
pico_enable_stdio_uart(${NAME} 0)

# Set up files for the release packages
install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.uf2
    ${CMAKE_CURRENT_LIST_DIR}/README.md
    DESTINATION .
)

set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
set(CPACK_GENERATOR "ZIP" "TGZ")
include(CPack)
