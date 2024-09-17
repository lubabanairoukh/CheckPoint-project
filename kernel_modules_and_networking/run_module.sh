#!/bin/bash

MODULE_NAME="netfilter_module"
PING_TARGET="8.8.8.8"

echo "Cleaning dmesg before starting"
sudo dmesg -C

echo "Using 'make' to build the kernel module..."
if ! make; then
    echo "Error: Failed to build the kernel module."
    exit 1
fi

echo "Inserting module using insmod..."
if ! sudo insmod ${MODULE_NAME}.ko; then
    echo "Error: Failed to insert module."
    exit 1
fi

echo "Initial dmesg output:"
dmesg | tail -10

echo "Testing the module, by pinging $PING_TARGET..."
ping -c 4 $PING_TARGET

echo "Final dmesg output after pinging:"
dmesg | tail -10

echo "Removing the module using rmmod..."
if ! sudo rmmod ${MODULE_NAME}; then
    echo "Error: Failed to remove module."
    exit 1
fi

echo "Cleaning up build artifacts using 'make clean'..."
make clean

echo "Finished"
