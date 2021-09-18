#!/usr/bin/env bash

USAGE="Usage: $0 [-d/--debug] [-g/--gdb] [additional QEMU args]"

ROOT_DIR="$(realpath $(dirname $(readlink -f $0))/../)"
DEBUG=
GDB=
ISO="${ROOT_DIR}/build/system.iso"
QEMU_LOG="-D ${HOME}/qemu.log"
SERIAL_LOG="-serial file:${HOME}/os.log"
MEMORY=5G
SMP=3
QEMU_ARGS=()

while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -d|--debug)
    DEBUG="-d int -no-reboot"
    shift
    ;;
    -g|--gdb)
    GDB="-d int -no-reboot -s -S"
    shift
    ;;
    -h|--help)
    echo ${USAGE}
    exit
    ;;
    *)
    # Skip over any unrecognized arguments; presume they're args to be passed
    # directly to QEMU.
    QEMU_ARGS+=("$1")
    shift
    ;;
esac
done

# Note: ${QEMU_ARGS[@]} expands to all the members of QEMU_ARGS.
qemu-system-x86_64 -m ${MEMORY} -smp ${SMP} ${SERIAL_LOG} \
    ${DEBUG} ${GDB} ${QEMU_LOG} -cdrom ${ISO} ${QEMU_ARGS[@]}
