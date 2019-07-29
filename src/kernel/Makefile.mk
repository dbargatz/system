KERNEL_DIR = $(SRC_DIR)/kernel
KERNEL_ASM_SRC = $(shell find $(KERNEL_DIR) -name *.asm)
KERNEL_CPP_SRC = $(shell find $(KERNEL_DIR) -name *.cpp)
KERNEL_BUILD_DIR = $(BUILD_DIR)/kernel
KERNEL_ELF = $(BUILD_DIR)/kernel.elf
KERNEL_LINKER_SCRIPT = $(BUILD_DIR)/../linker.ld
KERNEL_OBJ = $(BUILD_DIR)/kernel.o

$(KERNEL_ELF) : $(KERNEL_ASM_SRC) $(KERNEL_CPP_SRC)
	for s in $(KERNEL_CPP_SRC); do \
		$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) $$s -o $(KERNEL_BUILD_DIR)/$$(basename $$s).o; \
		done
	for s in $(KERNEL_ASM_SRC); do \
		$(AS) $(ASFLAGS) $$s -o $(KERNEL_BUILD_DIR)/$$(basename $$s).o; \
		done
	$(LD) -r -o $(KERNEL_OBJ) $(KERNEL_BUILD_DIR)/*.o
	$(LD) -n --gc-sections -T $(KERNEL_LINKER_SCRIPT) $(KERNEL_OBJ) -o $@
