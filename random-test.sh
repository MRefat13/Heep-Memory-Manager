#!/bin/bash


# before using it move the free_and_sbrk.c from test_debug folder to main the working Directory

# Function to generate random integer within a range
function randomInt() {
  local min=$1
  local max=$2
  echo $(( min + RANDOM % (max - min + 1) ))
}

# Number of times to run the program
n=100

# Loop to run the program 'n' times
for ((i=1; i<=n; i++)); do
  echo "Running program iteration $i"

  # Generate random values for arguments
  numAllocs=$(randomInt 1 100000)
  blockSize=$(randomInt 1 100000)
  freeStep=1#$(randomInt 1 10)
  freeMin=$(randomInt 1 $((numAllocs - 1)))
  freeMax=$(randomInt $freeMin $numAllocs)

  # Run the program with random values
  ./a.exe $numAllocs $blockSize $freeStep $freeMin $freeMax

  echo "-------------------------------------"
done