# Temperature Log

A simple C program designed for logging the temperature of a thermal zone in linux.

Usage: `templog [-o outfile|-t interval|-z thermalzone]`

The outfile is a CSV file of the recorded temperatures. The interval specifies how often, in seconds, the temperature is recorded to the file. The thermal zone specifies what linux thermal zone is to be recorded. A list of thermalzones can be obtained by running `ls /sys/class/thermal/`.

# Building

Use `make` to build the program. Requires `gcc`. Both can be installed through `build-essentials`.

The output will be `bin/templog`.
