
# Compiler
CC = gcc

# All W Flags
CFLAGS = -Wall -Werror -Wextra -fanalyzer

# All .h Includes
INCLUDES = -IdeviceManager -IfileOperations -IcJSON -I.

# All source files
source_files = $(wildcard *.c deviceManager/*.c fileOperations/*.c cJSON/*.c)

# All directory paths are removed from source file names
file_names = $(notdir $(source_files))

# All object file names are created with path to device folder
object_files = $(patsubst %.c, device/%.o, $(file_names))

# All assembly file names are created with path to device folder
assembly_files = $(patsubst %.c, device/%.s, $(file_names))

# All object file names are created with path to debug folder
debug_files = $(patsubst %.c, debug/%.o, $(file_names))

VPATH = .:deviceManager:fileOperations:cJSON

# Generate executable, object and assembly file and store them in device folder
# Generate object file for debugging and store them in debug
device_list: Create_device Create_debug Executable Object Debug Assembly

# Create device folder
Create_device:
	mkdir -p device

# Create debug folder
Create_debug:
	mkdir -p debug

# Generate executable and store it in device folder
Executable: $(source_files)
	$(CC) $(CFLAGS) $(INCLUDES) $(source_files) -o device/deviceList.exe

# Generate object file in device folder
Object: $(object_files)
device/%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $^ -c -o $@

# Generate object file for debugging in debug folder
Debug: $(debug_files)
debug/%.o: %.c
	$(CC) -g -O0 $(CFLAGS) $(INCLUDES) $^ -c -o $@

# Generate assembly file
Assembly: $(assembly_files)
device/%.s: %.c
	$(CC) $(CFLAGS) $(INCLUDES) $^ -S -o $@

# Remove device, debug and Build directories

clean: $(source_files)
	rm -rf device
	rm -rf debug
	rm -rf build