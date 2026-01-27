#!/usr/bin/env bash
set -e

mkdir -p build
cd build

if [ -z "$1" ]; then
    cargo build --manifest-path "../examples/firmware/Cargo.toml" --target-dir rust/target --target thumbv6m-none-eabi --release --lib
    cp "rust/target/thumbv6m-none-eabi/release/libfirmware.a" firmware.a

    OPENSPRIG_APPLICATION=0 cmake ..
    make

    sudo openocd \
        -f interface/cmsis-dap.cfg \
        -f target/rp2040.cfg \
        -c "adapter speed 5000; program firmware.elf verify reset exit"
else
    mkdir -p "$1"
    cd $1

    cargo build --manifest-path "../../examples/$1/Cargo.toml" --target-dir rust/target --target thumbv6m-none-eabi --release --lib
    cp "rust/target/thumbv6m-none-eabi/release/lib$1.a" firmware.a

    OPENSPRIG_APPLICATION=1 cmake ../..
    make
fi
