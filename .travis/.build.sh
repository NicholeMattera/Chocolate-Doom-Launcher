#!/bin/bash

# Make sure everything is up to date
dkp-pacman -Syu --noconfirm

# Build
cd Chocolate-Doom-Launcher && make
