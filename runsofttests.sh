echo "Running minnumber"
./build/AIProject ./demoTests/minnumber.txt 1 0 0 0 100 100 0 10
echo "minnumber done"
read -p "Press any key to continue... " -n 1 -s

echo "Running pairing"
./build/AIProject ./demoTests/pairing.txt 0 0 1 0 0 0 11 0
echo "pairing done"
read -p "Press any key to continue... " -n 1 -s

echo "Running parallelpen"
./build/AIProject ./demoTests/parallelpen.txt 0 0 0 1 1 1 0 5
echo "parallelpen done"
read -p "Press any key to continue... " -n 1 -s

echo "Running prefexamp"
./build/AIProject ./demoTests/prefexamp.txt 0 1 0 0 100 100 0 100
echo "prefexamp done"