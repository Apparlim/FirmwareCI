cmake_minimum_required(VERSION 3.16)
project(FirmwareCI C ASM)

option(ENABLE_HOST_TESTS "Build native host unit tests" OFF)

if(ENABLE_HOST_TESTS)
    enable_testing()
    add_executable(test_buffer tests/unit/test_buffer.c)
    add_test(NAME test_buffer COMMAND test_buffer)

    add_executable(test_parser tests/unit/test_parser.c)
    add_test(NAME test_parser COMMAND test_parser)
else()
    include_directories(src/include src/drivers)

    set(SOURCES
        src/startup.s
        src/main.c
        src/drivers/uart.c
    )

    add_executable(firmware.elf ${SOURCES})

    set_target_properties(firmware.elf PROPERTIES
        LINK_FLAGS "-T${CMAKE_CURRENT_SOURCE_DIR}/src/linker.ld -nostdlib -Wl,--gc-sections"
    )

    add_custom_command(TARGET firmware.elf POST_BUILD
        COMMAND ${CMAKE_OBJCOPY} -O binary firmware.elf firmware.bin
        COMMENT "Generating raw binary firmware.bin"
    )
endif()
