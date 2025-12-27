mkdir -p build
cd build || exit 1

cmake .. || { cd ..; exit 1; }
make || { cd ..; exit 1; }

cd ..
sudo openocd -f interface/cmsis-dap.cfg -f target/rp2040.cfg -c "adapter speed 5000; program build/src/firmware.elf verify reset exit"
