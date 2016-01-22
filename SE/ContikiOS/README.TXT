Commands to compile generate hex and upload binary:

cd contiki-2.7/examples/[my_example]

make clean TARGET=avr-atmega128rfa1 

make all TARGET=avr-atmega128rfa1

avr-objcopy -O ihex -R .eeprom -R .fuse -R .signature ./[my_example].avr-atmega128rfa1 [my_example].hex

avr-size -C --mcu=atmega128rfa1 ./[my_example].avr-atmega128rfa1

avrdude -p atmega128rfa1 -c jtag2 -P usb -Uflash:w:[my_example].hex
OR
atprogram -t jtagice3 -i jtag -d atmega128rfa1 program -f ./[my_example].hex

WITH FUSE ON LINUX:
sudo avrdude -p atmega128rfa1 -c jtag3 -P usb -Uflash:w:e-protect.hex -Ulfuse:w:0xe2:m -Uhfuse:w:0x99:m -Uefuse:w:0xff:m
