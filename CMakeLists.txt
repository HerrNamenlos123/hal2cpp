cmake_minimum_required(VERSION 3.13)
project(hal2cpp VERSION 0.1.1 LANGUAGES C CXX)

if (NOT DEFINED HAL2CPP_MAIN_CONFIG_HAL_FILE)
    message(FATAL_ERROR "HAL2CPP_MAIN_CONFIG_HAL_FILE is not defined. Please use -DHAL2CPP_MAIN_CONFIG_HAL_FILE=... to set it to the <configname>.hal file of your LinuxCNC StepConf configuration.")
endif ()

if (NOT EXISTS ${HAL2CPP_MAIN_CONFIG_HAL_FILE})
    message(FATAL_ERROR "The file ${HAL2CPP_MAIN_CONFIG_HAL_FILE} does not exist. Please specify an existing file")
endif ()

# =============================
#      C++ shared library
# =============================

add_library(hal2cpp SHARED)
target_compile_features(hal2cpp PUBLIC cxx_std_17)
set_target_properties(hal2cpp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(hal2cpp PUBLIC "hal")
target_include_directories(hal2cpp PUBLIC "src")

target_sources(hal2cpp PRIVATE 
    src/hal2cpp.cpp
)

add_custom_target(hal2cpp_generate
    COMMAND python3 hal/generate.py ${HAL2CPP_MAIN_CONFIG_HAL_FILE}
    COMMENT "Generating HAL file"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)
add_dependencies(hal2cpp hal2cpp_generate)

set(SOCKPP_BUILD_SHARED OFF CACHE BOOL "Build sockpp as a shared library" FORCE)
set(SOCKPP_BUILD_STATIC ON CACHE BOOL "Build sockpp as a shared library" FORCE)
add_subdirectory(sockpp)
add_library(sockpp::sockpp ALIAS sockpp-static)
target_link_libraries(hal2cpp PRIVATE sockpp::sockpp)

# =====================================================
#      C-based HAL driver loading the C++ library
# =====================================================

add_custom_target(hal2cpp_hal
    COMMAND sudo halcompile --install hal/hal2cpp.comp
    COMMENT "Building HAL driver"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)

# ===========================
#      Install targets
# ===========================

install(TARGETS hal2cpp
    LIBRARY DESTINATION "/usr/lib/linuxcnc/hal2cpp"
)
add_dependencies(hal2cpp hal2cpp_hal)
