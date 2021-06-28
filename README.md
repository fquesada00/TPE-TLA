# C-Graph
¿Cuántas veces quisimos nosotros programadores realizar un programa que maneje grafos? Si hubiera un contador de la cantidad de veces que un programador implementó una librería de grafos ya estaría en overflow. ¡Ahora con **C-Graph** vas a poder realizar operaciones básicas con grafos de manera muy sencilla!
Este es un lenguaje diseñado para facilitarnos a nosotros programadores trabajar con estructuras sin tener que preocuparnos por tener que implementarlas! Simplemente escribimos nuestro programa en un archivo .cg como por arte de magia el compilador nos devolverá un archivo .C con codigo equivalente.
Manejar grafos nunca fue tan sencillo ;)

## Instalación

Para compilar el compilador de C-Graph hay que ejecutar  `make all` . Para ello hay que tener previamente instalado instalado `yacc` ,`flex` y `gcc` en nuestra computadora.

## Compilar un archivo

Para compilar un archivo `.cg` simplemente ejecutamos el script de bash `compile.sh` pasandole como unico argumento nuestro archivo.
Ejemplo:
`./compile.sh helloWorld.cg`

El archivo compilado a C se encontrará en la carpeta `generatedCode` y el ejecutable compilado con `gcc` en la carpeta `executables`.

## Casos de testeo

Para correr los casos de testeo que se encuetran en la carpeta `examplePrograms` ejecutamos el script de bash `test.sh`.
Ejemplo:
`./test.sh`
Al igual que como el otro script en la carpeta`generatedCode` se encontrarán los archivos `.c` y en la carpeta  `executables` los ejecutables.
