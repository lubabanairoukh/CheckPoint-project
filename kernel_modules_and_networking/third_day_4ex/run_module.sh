#!/bin/bash

MODULE_NAME="netfilter_module"
PING_TARGET="127.0.0.1"

echo "//////////////////////////"
echo "Cleaning dmesg before starting"
echo "//////////////////////////"
sudo dmesg -C

echo "//////////////////////////"
echo "Using 'make' to build the kernel module..."
echo "//////////////////////////"
if ! make; then
    echo "Error: Failed to build the kernel module."
    exit 1
fi

echo "//////////////////////////"
echo "Inserting module using insmod..."
echo "//////////////////////////"
if ! sudo insmod ${MODULE_NAME}.ko; then
    echo "Error: Failed to insert module."
    exit 1
fi

echo "//////////////////////////"
echo "Initial dmesg output:"
echo "//////////////////////////"
dmesg | tail -10

echo "Testing the module, by pinging $PING_TARGET..."
echo "//////////////////////////"
ping -c 4 $PING_TARGET

echo "Final dmesg output after pinging:"
echo "//////////////////////////"
dmesg

echo "Removing the module using rmmod..."
echo "//////////////////////////"
if ! sudo rmmod ${MODULE_NAME}; then
    echo "Error: Failed to remove module."
    exit 1
fi

echo "Cleaning up build artifacts using 'make clean'..."
echo "//////////////////////////"
make clean


echo "/////////////Finished/////////////"
