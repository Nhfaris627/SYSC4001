mkdir -p bin

clang++ -std=c++17 interrupts.cpp -o bin/interrupts

echo "build complete! Executable created at bin/interrupts"