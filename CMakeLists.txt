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
target_compile_features(hal2cpp PUBLIC cxx_std_20)
set_target_properties(hal2cpp PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(hal2cpp PUBLIC "src")

target_sources(hal2cpp PRIVATE 
    src/hal2cpp.cpp
    src/TinyStd_impl.cpp
)
target_link_options(hal2cpp PRIVATE -Wl,--no-undefined)

# add_custom_target(hal2cpp_generate
#     COMMAND python3 generate.py ${HAL2CPP_MAIN_CONFIG_HAL_FILE}
#     COMMENT "Generating HAL file"
#     WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
# )
# add_dependencies(hal2cpp hal2cpp_generate)

find_package(sockpp REQUIRED)
target_link_libraries(hal2cpp PRIVATE Sockpp::sockpp-static)

# =====================================================
#      C-based HAL driver loading the C++ library
# =====================================================

execute_process(
    COMMAND sudo -v
    RESULT_VARIABLE SUDO_CHECK_RESULT
    OUTPUT_QUIET
    ERROR_QUIET
)
if (SUDO_CHECK_RESULT EQUAL 0)
add_custom_target(hal2cpp_hal
    COMMAND sudo halcompile --install src/hal/hal2cpp.comp
    COMMENT "Building HAL driver"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)
else()
message(halcompile --install src/hal/hal2cpp.comp)
add_custom_target(hal2cpp_hal
    COMMAND halcompile --install src/hal/hal2cpp.comp
    COMMENT "Building HAL driver"
    WORKING_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}"
)
endif()

# ===========================
#      Install targets
# ===========================

install(TARGETS hal2cpp
    LIBRARY DESTINATION "/usr/lib/linuxcnc/hal2cpp"
)
add_dependencies(hal2cpp hal2cpp_hal)
