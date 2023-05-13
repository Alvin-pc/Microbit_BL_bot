# Path to tool-chain
ARMGCC=/opt/arm-gcc

# Path to BSP library
BSPDIR=.

# Tool-chain programs
CC=$ARMGCC/bin/arm-none-eabi-gcc
LD=$ARMGCC/bin/arm-none-eabi-ld
AR=$ARMGCC/bin/arm-none-eabi-ar
SIZE=$ARMGCC/bin/arm-none-eabi-size
OBJCOPY=$ARMGCC/bin/arm-none-eabi-objcopy

# ARM architecture specific compiler flags
ARCH="-mcpu=cortex-m4 -mthumb -mabi=aapcs -mfloat-abi=hard -mfpu=fpv4-sp-d16"

# Keep every function in a separate section, this allows linker to discard unused ones
OPT="-ffunction-sections -fdata-sections -fno-strict-aliasing -fno-builtin"

# Compiler flags
CFLAGS="-O3 -g3 -Wall -Werror $ARCH $OPT"

# Compile the source files
echo Compiling startup.c
$CC -c $CFLAGS startup.c
echo Compiling system.c
$CC -c $CFLAGS system.c
echo Compiling board.c
$CC -c $CFLAGS board.c
echo Compiling serial.c
$CC -c $CFLAGS serial.c
echo Compiling gpio.c
$CC -c $CFLAGS gpio.c
echo Compiling pwm.c
$CC -c $CFLAGS pwm.c
echo Compiling pwm_audio.c
$CC -c $CFLAGS pwm_audio.c
echo Compiling adc.c
$CC -c $CFLAGS adc.c
echo Compiling lib.c
$CC -c $CFLAGS lib.c
echo Compiling audio.c
$CC -c $CFLAGS audio.c
echo Compiling buggy_controller.c
$CC -c $CFLAGS buggy_controller.c
echo Compiling i2c.c
$CC -c $CFLAGS i2c.c
echo Compiling lsm303agr.c
$CC -c $CFLAGS lsm303agr.c
echo Compiling utility.c
$CC -c $CFLAGS utility.c
echo Compiling motor_control.c
$CC -c $CFLAGS motor_control.c

# Link the object code to form exectuable program
echo Generating libbsp.a
/bin/rm -f libbsp.a
$AR -r libbsp.a startup.o system.o board.o \
        serial.o gpio.o pwm.o pwm_audio.o adc.o lib.o audio.o buggy_controller.o lsm303agr.o i2c.o utility.o motor_control.o
/bin/rm -f system.o board.o utility.o\
        serial.o gpio.o pwm.o pwm_audio.o adc.o lib.o audio.o buggy_controller.o lsm303agr.o i2c.o motor_control.o