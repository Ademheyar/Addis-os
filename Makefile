cc = gcc
ld = ld
as = as
nm = nm
objdump = objdump
objcopy = objcopy
nasm = nasm
qemu = qemu-system-x86_64
bochs = bochs

arch ?= x86_64
kernel := build/Addis-$(arch).bin
isoname := Addis-os-$(arch).iso
iso := build/$(isoname)
grub_cfg := others/grub/grub.cfg
newbin := build/iso/boot/Addis.bin

EMULATOR_FLAGS = -kernel

linker_script := linkers/kernel.ld
ldflags :=  -m elf_x86_64 -nostdlib -n -T $(linker_script) --no-warn-rwx-segment

#nasm_source_files := $(shell find nasms/ -name *.asm)
-include ./assembly_source_files.list
#assembly_object_files := $(patsubst nasms/%.asm, build/%.o, $(assembly_source_files))
nasm_object_files := $(patsubst nasms/%.asm, build/%.o, $(assembly_source_files))
nasm_flags := -w-number-overflow -f elf64
#-f elf32

#c_source_files := $(shell find src/ -name *.c)
-include ./c_source_files.list
#c_object_files := $(patsubst src/impl/x86_64/%.c, build/x86_64/%.o, $(x86_64_c_source_files))
c_object_files := $(patsubst src/%.c, build/%.o, $(c_source_files))
c_include := 'include'
#cflags := -m32 -c -ffreestanding -I $(c_include)
cflags := -fno-pic  -m64 -nostdlib -nostdinc -fno-builtin -fno-stack-protector \
					-ffreestanding -mno-red-zone -mno-mmx -mno-sse -mno-sse2 \
					-I $(c_include) -nostartfiles -nodefaultlibs -fno-exceptions \
					-Wall -Wextra -Werror -c -mcmodel=large -Wno-implicit-fallthrough -O2 \
					-Wno-parentheses

object_files := $(c_object_files) $(nasm_object_files)

quemu_mem := 128
hd_image = build/disk.img

all: $(iso)
	mkdir -p disk/Sys/Wallp disk/Sys/Shell/Win/Tittelbar
	cp Shell/Wallp/Assets/*.* disk/Sys/Wallp
	cp Shell/Win/Tittelbar/*.* disk/Sys/Shell/Win/
	dd if=/dev/zero of=build/disk.img count=81920 bs=512
	mformat -F -v Addis -i build/disk.img
	mcopy -svn -i build/disk.img disk/* ::.
	clear
	#$(qemu) $(EMULATOR_FLAGS) disk/boot/Addis.bin
	$(qemu) -cdrom $(iso) -m 1024 -drive file=$(hd_image),format=raw,index=0,media=disk  -boot order=d -serial stdio

run:
	$(qemu) -cdrom $(iso) -m 1024 -drive file=$(hd_image),format=raw,index=0,media=disk  -boot order=d -serial stdio

run-bocsh: $(iso)
		$(objcopy) --only-keep-debug $(kernel) $(kernel).sym
		$(nm) $(kernel).sym | grep " T " | awk '{ print $$1" "$$3 }' > $(kernel).bochs.sym
		$(bochs) -rc bochs.rc -f bochs.cfg -q

run-qemu: $(iso)
	$(qemu) -cdrom $(iso) -m 1024 -drive file=$(hd_image) build/disk0.img,format=raw,index=0,media=disk -boot order=d -serial stdio

iso: $(iso)

debug: nasm_flags += -g -F dwarf
debug: cflags += -g
debug: all

$(iso): $(kernel) $(grub_cfg)
	@mkdir -p disk/boot/grub
	@cp $(kernel) disk/boot/Addis.bin
	@cp $(grub_cfg) disk/boot/grub
	@grub-mkrescue -o $(iso) disk/

$(kernel): $(nasm_object_files) $(c_object_files) $(linker_script)
		$(ld) $(ldflags) -o $(kernel) $(nasm_object_files) $(c_object_files)
		#$(objdump) -D $(kernel) > build/Addis.dump.asm
		#$(objdump) -x $(kernel) >> build/Addis.headers.txt

# compile assembly files
build/%.o: nasms/%.asm
	mkdir -p $(shell dirname $@)
	$(nasm) $(nasm_flags) $< -o $@

# compile c files
build/%.o: src/%.c
		@mkdir -p $(shell dirname $@)
		$(cc) $(cflags) $< -o $@

clean:
		@rm -rfv build
		@rm -rfv disk/Sys/Apps
	rm -rfv disk/boot/grub
	rm -rfv disk/boot/Addis.bin
	clear & clear

.PHONY: $(apps) all clean run iso install
