cmake_minimum_required(VERSION 3.10)

enable_language(C CXX ASM)
# Set project name
project(YourProjectName)

# Set the C standard to use (C11 in this case)
set(CMAKE_C_STANDARD 11)

# Set the C++ standard to use (C++11 in this case)
set(CMAKE_CXX_STANDARD 11)

# Specify the cross-compiler toolchain
set(CMAKE_SYSTEM_NAME Generic)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_CXX_COMPILER arm-none-eabi-g++)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)

# Set flags for the compiler
set(CMAKE_C_FLAGS "-mcpu=cortex-m3 -mthumb -fmessage-length=0 -ffunction-sections -fdata-sections -Wall -Wextra")
set(CMAKE_CXX_FLAGS "${CMAKE_C_FLAGS} -fno-exceptions -fno-rtti")

# Add additional defines
add_definitions(
    -DSTM32PLUS_F1_MD
    -DCPP_FREERTOS_NO_CPP_STRINGS
    -DCPP_FREERTOS_NO_EXCEPTIONS
    -DHSE_VALUE=8000000
)

# Specify the source directories
set(SOURCE_DIRS
    7003b
    buttonpoti
    config
    freertos
    game-of-life
    potmeter
    pwm
    stemwin
    stepper
    system
    systemview
    ws2812
    timer
)

# Collect source files
foreach(dir ${SOURCE_DIRS})
    file(GLOB_RECURSE sources "${dir}/*.c" "${dir}/*.cpp" "${dir}/*.s" "${dir}/*.S")
    list(APPEND project_sources ${sources})
endforeach()

# Add main.cpp to the project
list(APPEND project_sources main.cpp)

# Include directories
include_directories(${SOURCE_DIRS} . ../stm32plus/lib/include/ ../stm32plus/lib/ ../stm32plus/lib/include/stl)

# Add executable target
add_executable(${PROJECT_NAME}.elf ${project_sources})



# Set linker flags
set(LINKER_SCRIPT_PATH ${CMAKE_SOURCE_DIR}/system/f1mdvl_24_8/Linker.ld)
set_target_properties(${PROJECT_NAME}.elf PROPERTIES
    LINK_FLAGS "-T${LINKER_SCRIPT_PATH} -Wl,-Map=${PROJECT_NAME}.map,--cref -Wl,--gc-sections"
)

# Link against libstm32plus.a
target_link_libraries(${PROJECT_NAME}.elf
    ${CMAKE_SOURCE_DIR}/../stm32plus/lib/build/fast-f1mdvl-8000000e/libstm32plus-fast-f1mdvl-8000000e.a
    ${CMAKE_SOURCE_DIR}/stemwin/STemWinLibrary532/Lib/STemWin532_CM3_GCC.a
)

# Include FreeRTOS specific configuration
add_subdirectory(freertos)
add_subdirectory(systemview)
add_subdirectory(stemwin)

# Create binary file
add_custom_command(TARGET ${PROJECT_NAME}.elf POST_BUILD
    COMMAND arm-none-eabi-objcopy -O binary $<TARGET_FILE:${PROJECT_NAME}.elf> ${PROJECT_NAME}.bin
    COMMENT "Generating binary file"
)