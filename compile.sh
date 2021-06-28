#!/bin/bash
if [ -z $1 ];then
    echo Error: no se ingreso un archivo. Ingrese el del archivo de entrada como primer argumento. Uso: ./compile.sh miarchivo.cg;
else
    if [ -f $1 ];
    then
        CG=./CG
        if [ -f $CG ];
        then
            file=${1##*/}
            ./CG < $1 > ${file:0: -3}.c
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
        fi
    else
        echo File $1 does not exists
    fi
fi
