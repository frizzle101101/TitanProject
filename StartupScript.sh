#!/bin/sh

echo "Adding read write permissions to RS232 adapter...\n";
sudo chmod o+rw /dev/ttyUSB0;

echo "Moving to project folder...\n";
cd /TitanProject/Client;

echo "Starting camera broadcast...\n"
sudo motion start;

echo "Starting client program...\n"
./Client.o
