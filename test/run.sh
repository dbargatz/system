#!/usr/bin/env bash

USAGE="Usage: $0 [-d/--debug] [-g/--gdb] [additional QEMU args]"

ROOT_DIR="$(realpath $(dirname $(readlink -f $0))/../)"
DEBUG=
GDB=
ISO="${ROOT_DIR}/build/artifacts/system.iso"
LOG="-D ${ROOT_DIR}/test/artifacts/qemu.log"
MEMORY=1G
SMP=3
QEMU_ARGS=()

while [[ $# -gt 0 ]]
do
key="$1"

case $key in
    -d|--debug)
    DEBUG="-d int"
    shift
    ;;
    -g|--gdb)
    GDB="-s -S"
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

# Create test/artifacts if it doesn't exist.
mkdir -p ${ROOT_DIR}/test/artifacts

# Note: ${QEMU_ARGS[@]} expands to all the members of QEMU_ARGS.
qemu-system-x86_64 -m ${MEMORY} -smp ${SMP} -nographic ${DEBUG} ${GDB} ${LOG} \
    -cdrom ${ISO} ${QEMU_ARGS[@]}
