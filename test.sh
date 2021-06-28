#!/bin/bash
for fileName in examplePrograms/*.cg;
do
    file=${fileName##*/}
    CG=./CG
    if [ -f $CG ];
    then
        echo testing $file
        ./CG < $fileName > ${file:0: -3}.c
        if [ -s ${file:0: -3}.c ];
        then
            mv ${file:0: -3}.c generatedCode/
            gcc generatedCode/${file:0: -3}.c -o ${file:0: -3}
            if [ -s ${file:0: -3} ];
                then
                mv ${file:0: -3} executables/
            fi
        fi
    else
        echo Please run make all before running the tests
        break
    fi
done
