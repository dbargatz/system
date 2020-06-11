DRIVER_SRC_DIR       = $(SRC_DIR)/driver/cpu/x64
DRIVER_BUILD_DIR     = $(BUILD_DIR)/driver/cpu/x64
DRIVER_ASM_SRC       = $(shell find $(DRIVER_SRC_DIR) -name *.asm)
DRIVER_CPP_SRC       = $(shell find $(DRIVER_SRC_DIR) -name *.cpp)
DRIVER_LD_SRC        = $(DRIVER_SRC_DIR)/linker.ld
DRIVER_OBJ           = $(DRIVER_BUILD_DIR)/core_x64.o
DRIVER_BIN           = $(DRIVER_BUILD_DIR)/core_x64.bin

$(DRIVER_BIN) : $(DRIVER_ASM_SRC) $(DRIVER_CPP_SRC)
	@mkdir -p $(DRIVER_BUILD_DIR)
	@for s in $(DRIVER_CPP_SRC); do \
		$(CXX) -c $(CPPFLAGS) $(CXXFLAGS) -o $(DRIVER_BUILD_DIR)/$$(basename $$s .cpp).o $$s; \
		done
	@for s in $(DRIVER_ASM_SRC); do \
		$(AS) $(ASFLAGS) -o $(DRIVER_BUILD_DIR)/$$(basename $$s .asm).o $$s; \
		done
	@rm -rf $(DRIVER_OBJ) # Prevents 'File truncated' error when linking
	@$(LD) -r -o $(DRIVER_OBJ) $(DRIVER_BUILD_DIR)/*.o
	@$(LD) -n --gc-sections -T $(DRIVER_LD_SRC) $(DRIVER_OBJ) -o $@
