SET proj_app=LED20_STM32F030
SET dir_app=E:\projekti\smarhouse\stairsLED\SW\LED20_STM32F030\Release\
SET dirproj_app=%dir_app%%proj_app%

rm %dirproj_app%.bin %dirproj_app%.hex
arm-none-eabi-objcopy -O binary %dirproj_app%.elf %dirproj_app%.bin
arm-none-eabi-objcopy -O ihex %dirproj_app%.elf %dirproj_app%.hex

SET proj=boot_STM32f030
SET dir=E:\projekti\smarhouse\bootloader\BOOT_STM32F030\Release\
SET dirproj=%dir%%proj%

ST-LINK_CLI.exe -ME
ST-LINK_CLI.exe -P "%dirproj_app%.bin" 0x08000000
ST-LINK_CLI.exe -SE 0 19
ST-LINK_CLI.exe -P "%dirproj%.bin" 0x08000000 -Rst
ST-LINK_CLI.exe -HardRst
sleep 2

