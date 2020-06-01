KERNEL_SRC_DIR       = $(SRC_DIR)/kernel
KERNEL_ASM_SRC       = $(shell find $(KERNEL_SRC_DIR) -name *.asm)
KERNEL_CPP_SRC       = $(shell find $(KERNEL_SRC_DIR) -name *.cpp)
KERNEL_LD_SRC 	     = $(KERNEL_SRC_DIR)/linker.ld
KERNEL_BUILD_DIR     = $(BUILD_DIR)/kernel
KERNEL_OBJ           = $(KERNEL_BUILD_DIR)/kernel.o
KERNEL               = $(KERNEL_BUILD_DIR)/kernel.bin

$(KERNEL) : $(KERNEL_ASM_SRC) $(KERNEL_CPP_SRC)
	@mkdir -p $(KERNEL_BUILD_DIR)
	@for s in $(KERNEL_CPP_SRC); do \
		$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $(KERNEL_BUILD_DIR)/$$(basename $$s .cpp).o $$s; \
		done
	@for s in $(KERNEL_ASM_SRC); do \
		$(AS) $(ASFLAGS) -o $(KERNEL_BUILD_DIR)/$$(basename $$s .asm).o $$s; \
		done
	@rm -rf $(KERNEL_OBJ) # Prevents 'File truncated' error when linking
	@$(LD) -r -o $(KERNEL_OBJ) $(KERNEL_BUILD_DIR)/*.o
	@$(LD) -n --gc-sections -T $(KERNEL_LD_SRC) $(KERNEL_OBJ) -o $@
