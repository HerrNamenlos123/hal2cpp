.PHONY: all

CONFIG ?=

all:
	@if [ "$$(id -u)" -eq 0 ]; then \
		echo "Please do not run as root."; \
		exit 1; \
	fi
	@if [ -z "$(CONFIG)" ]; then \
        echo "Usage: make CONFIG=<path-to-your-main-config.hal>"; \
        echo "Example: make CONFIG=~/linuxcnc/configs/CNC/CNC.hal"; \
        exit 1; \
    fi
	@command -v cmake > /dev/null 2>&1 || { \
		echo "CMake is not installed. Please install CMake using 'sudo apt update && sudo apt install -y cmake'"; \
		exit 1; \
	}
	@mkdir -p build
	@cmake -S . -B build -DHAL2CPP_MAIN_CONFIG_HAL_FILE="$(CONFIG)" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
	@cmake --build build
	@sudo cmake --build build --target install

.SILENT:
