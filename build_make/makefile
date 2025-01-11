################################################################################
# TryKernel 2.0 makefile
################################################################################
# Shell指定
SHELL := sh

# 実行ファイル
EXE_FILE := tryknl

# コマンド
GCC := arm-none-eabi-gcc
AS := arm-none-eabi-gcc
LINK := arm-none-eabi-gcc
RM := rm -rf

# インクルードファイル・パス
INCPATH = -I"../include"

# コマンド・パラメタ
CFLAGS := -mcpu=cortex-m0plus -mthumb -ffreestanding -std=c99 -O0 -g3
ASFLAGS := -mcpu=cortex-m0plus -mthumb -ffreestanding -x assembler-with-cpp -O0 -g3
LFLAGS := -mcpu=cortex-m0plus -mthumb -ffreestanding -nostartfiles -O0 -g3 --specs=nano.specs

# リンカ定義ファイル
LNKFILE := "../linker/pico_memmap.ld"

all: $(EXE_FILE).elf

include sources.mk
OBJS = $(CSRCS:%.c=./%.o) $(ASRCS:%.S=./%.o)
DEPS = $(CSRCS:%.c=./%.d) $(ASRCS:%.S=./%.d)

# 依存情報ファイルの読み込み
ifneq ($(MAKECMDGOALS),clean)
-include $(DEPS)
endif

# ビルド・ルール
BUILD_C = \
    echo 'Building file: $<'; \
    $(GCC) $(CFLAGS) $(INCPATH) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"; \
    echo 'Finished building: $<'; \
    echo ' '

BUILD_ASM = \
	echo 'Building file: $<'; \
	$(AS) $(ASFLAGS) $(INCPATH) -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"; \
	echo 'Finished building: $<'; \
	echo ' '

$(EXE_FILE).elf: $(OBJS) makefile sources.mk
	@echo 'Linker: $@'
	$(LINK)  $(LFLAGS) -T $(LNKFILE) -Wl,-Map,"$(EXE_FILE).map" -o "$(EXE_FILE).elf" $(OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

application/%.o: ../application/%.c
	@$(BUILD_C)

device/devmgr/%.o: ../device/devmgr/%.c
	@$(BUILD_C)

device/adc/%.o: ../device/adc/%.c
	@$(BUILD_C)

device/i2c/%.o: ../device/i2c/%.c
	@$(BUILD_C)

boot/%.o: ../boot/%.c
	@$(BUILD_C)

kernel/%.o: ../kernel/%.c
	@$(BUILD_C)

kernel/%.o: ../kernel/%.S
	@$(BUILD_ASM)

# Other EXE_FILEs
clean:
	-$(RM) $(OBJS) $(DEPS) $(EXE_FILE).elf $(EXE_FILE).map
	-@echo ' '

.PHONY: all clean
