#!/bin/bash

# Compile and link server
echo "Compiling and linking server..."
gcc -o server server.c
# Compile and link client
echo "Compiling and linking client..."
gcc -o client client.c
