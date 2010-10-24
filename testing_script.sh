#!/bin/bash
clear
echo Running testing script
echo Removing old TestDirectory
rm -r ./TD
echo Creating new copy of TestDirectory
cp -r ./TestDirectory ./TD
echo Building program
make clean
make
echo Running mysync
./mysync ./TD/Dir1 ./TD/Dir2
echo Finished running testing script
