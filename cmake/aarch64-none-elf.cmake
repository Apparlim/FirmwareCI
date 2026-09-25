set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_PROCESSOR aarch64)

if(NOT DEFINED CROSS_COMPILE)
    set(CROSS_COMPILE aarch64-linux-gnu-)
endif()

set(CMAKE_C_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_ASM_COMPILER ${CROSS_COMPILE}gcc)
set(CMAKE_OBJCOPY ${CROSS_COMPILE}objcopy)
set(CMAKE_OBJDUMP ${CROSS_COMPILE}objdump)

set(CMAKE_C_FLAGS_INIT "-Wall -Wextra -Werror -pedantic -std=c99 -ffreestanding -nostdlib -mcpu=cortex-a72")
set(CMAKE_ASM_FLAGS_INIT "-Wall -ffreestanding -mcpu=cortex-a72")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
