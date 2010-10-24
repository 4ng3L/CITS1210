#!/bin/bash
echo Running testing script
echo Removing old TestDirectory
rm -r ./TD1 ./TD2
echo Creating first new copy of TestDirectory
cp -r ./TestDirectory ./TD1
echo Creating second new copy of TestDirectory
cp -r ./TestDirectory ./TD2
echo Building program
make clean
make
echo Running mysync
./mysync
