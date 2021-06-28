#!/bin/bash
if [ -z $1 ];then
    echo Error: no se ingreso un archivo. Ingrese el del archivo de entrada como primer argumento. Uso: ./compile.sh miarchivo.cg;
else
    ./CG < $1 > ${1:0: -3}.c
    mv ${1:0: -3}.c generatedCode/
    gcc generatedCode/${1:0: -3}.c -o ${1:0: -3}
    mv ${1:0: -3} executables/
fi
