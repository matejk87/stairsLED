SET proj=LED20_STM32F030
SET dir=E:\projekti\smarhouse\stairsLED\SW\LED20_STM32F030\Release\
SET dirproj=%dir%%proj%
rm %dirproj%.bin %dirproj%.hex
arm-none-eabi-objcopy -O binary %dirproj%.elf %dirproj%.bin
arm-none-eabi-objcopy -O ihex %dirproj%.elf %dirproj%.hex
RS485_loader.exe 4 0x1201 LED20_STM32F030.bin

