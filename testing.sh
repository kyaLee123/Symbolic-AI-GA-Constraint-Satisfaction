#!/bin/bash

# rm -rf ./build/*
cmake -B ../AI-2/build/ .
make -C ../AI-2/build/
# Array of commands and expected outputs
commands=(
    "./build/AIProject ./demoTests/hc1.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc2.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc3.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc4.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc5.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc6.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc7.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc8.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc9.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc10.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc11.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/hc12.txt 1 0 1 0 10 10 10 10"
    "./build/AIProject ./demoTests/pairing.txt 0 0 1 0 0 0 11 0"
    "./build/AIProject ./demoTests/parallelpen.txt 0 0 0 1 1 1 0 5"
    "./build/AIProject ./demoTests/prefexamp.txt 0 1 0 0 100 100 0 100"
)

expected_outputs=(
    "Expected output: Unsolveable!"
    "Expected output: Unsolveable!"
    "Expected output: Unsolveable!"
    "Expected output: Unsolveable!"
    "Expected output: Unsolveable!\n"
    "Expected output: Unsolveable!\n"
    "Expected output: Unsolveable!\n"
    "Expected output: Unsolveable!\n"
    "Expected output: Unsolveable!\n"
    "Expected output: Unsolveable!\n"
    "Expected output: Unsolveable!\n"
    "Expected output: Unsolveable!\n"
    "Expected output: Eval value: 55\n"
    "Expected output: Eval value: 5\n"
    "Expected output: Eval value 30\n"
)

# Loop through commands and expected outputs
for i in "${!commands[@]}"; do
    echo "Running: ${commands[$i]}"
    eval ${commands[$i]}
    echo -e "${expected_outputs[$i]}"
    echo "-----------------------------------"
done