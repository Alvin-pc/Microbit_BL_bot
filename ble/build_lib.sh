export PATH=$PATH:/opt/arm-gnu-toolchain-12.2.rel1-x86_64-arm-none-eabi/bin
make
rm -f libble_uart.a
arm-none-eabi-ar -r libble_uart.a _build/nrf52833_xxaa/*.o
arm-none-eabi-ar -d libble_uart.a gcc_startup_nrf52833.S.o
arm-none-eabi-ar -d libble_uart.a system_nrf52833.c.o