# C言語のソースファイル
CSRCS = \
boot/boot2.c \
boot/reset_hdr.c \
boot/vector_tbl.c \
kernel/context.c \
kernel/eventflag.c \
kernel/gpio.c \
kernel/inittsk.c \
kernel/interrupt.c \
kernel/messagebuf.c \
kernel/scheduler.c \
kernel/semaphore.c \
kernel/syslib.c \
kernel/systimer.c \
kernel/task_mange.c \
kernel/task_queue.c \
kernel/task_sync.c \
device/devmgr/device.c \
device/devmgr/device_tbl.c \
device/adc/adc_rp2040.c \
device/i2c/i2c_rp2040.c \
application/usermain.c 

# アセンブラ言語のソースファイル
ASRCS = \
kernel/dispatch.S
