# $@ = target file
# $< = first dependency
# $^ = all dependencies

# detect all .o files based on their .c source
C_SOURCES = $(wildcard kernel/*.c drivers/*.c cpu/*.c)
HEADERS = $(wildcard kernel/*.h  drivers/*.h cpu/*.h)
OBJ_FILES = ${C_SOURCES:.c=.o cpu/interrupt.o kernel/clean.o}

# First rule is the one executed when no parameters are fed to the Makefile
all: run

# Notice how dependencies are built as needed
kernel.bin: boot/kernel_entry.o ${OBJ_FILES}
	i686-elf-ld -no-pie  -m elf_i386 -o $@ -Ttext 0x1000 $^ --oformat binary

os-image.bin: boot/loader.bin kernel.bin
	cat $^ > $@

run: os-image.bin
	qemu-system-i386 -machine accel=kvm -smp 1,cores=4,maxcpus=4 -m 4096 -vga std     -netdev user,id=mynet0 -device rtl8139,netdev=mynet0 --enable-kvm -fda $<

echo: os-image.bin
	xxd $<

# only for debug
kernel.elf: boot/kernel_entry.o ${OBJ_FILES}
	i686-elf-ld -no-PIE   -o $@ -Ttext 0x1000 $^

debug: os-image.bin kernel.elf
	qemu-system-i386   -machine accel=kvm -smp 1,cores=4,maxcpus=4 -m 1024 -vga std  -soundhw all   -netdev user,id=mynet0 -device rtl8139,netdev=mynet0  --enable-kvm -fda os-image.bin -d guest_errors,int &
%.o: %.c ${HEADERS}
	gcc -fno-pic -m32  -g  -ffreestanding -c $< -o $@ # -g for debugging

%.o: %.asm
	nasm $< -f elf -o $@

%.bin: %.asm
	nasm $< -f bin -o $@

%.dis: %.bin
	ndisasm -b 32 $< > $@

clean:
	$(RM) *.bin *.o *.dis *.elf
	$(RM) kernel/*.o
	$(RM) boot/*.o boot/*.bin
	$(RM) drivers/*.o
	$(RM) cpu/*.o
