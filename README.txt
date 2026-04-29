#PRÁCTICA 3 - SISTEMA DE BÚSQUEDA SSOOIIGLE#

#Descripción:
En esta práctica se utiliza los conceptos de sincronización de C++ para crear
un programa que se encargue de hacer una búsqueda de palabras multihilo por 
parte de varios clientes conectados a un sistema de pago. Habra tres tipos de
usuario con categorias distintas que afectarán en la manera en la que se buscan
las palabras o la prioridad. 

#Archivos:
- Buscador.cpp: Contiene el main. Crea los usuarios y realiza las busquedas

- SistemaBusqueda.hpp: Contiene las cabeceras con la estructura de las peticiones
de los usuarios

- Usuario.cpp/hpp: Programa que define la manera en la que el usuario crea la 
petición y espera los resultados

- BusquedaArchivos.cpp/hpp: Se encarga de definir la manera en la que se va contando
el numero de veces que se encuentra la palabra solicitada por el usuario

- SistemaPago.cpp/hpp: Sistema que se encarga de aumentar el saldo de los clientes
cuando estos llegan a 0. Permanece en espera hasta que le llega una petición de
incremento de saldo

#Estructura:

- Carpeta src: Contiene los archivos .cpp de codigo fuente
- Carpeta include: Contiene los archivos .hpp con las cabeceras
- Carpeta exec: Contiene los ejecutables
- Carpeta obj: Contiene los archivos objeto .o
- Carpeta lib: Contiene los libros en formato .txt para la búsqueda
- Makefile: fichero para la automatización de compilación y ejecución
- README.txt: fichero con información sobre el programa

#Compilación y ejecución:
1) Compilar el programa mediante el makefile:
	$make
2) Ejecutar el programa:
	$make run
3) Limpieza:
	$make clean

#Autores
Ainhoa Sánchez Esteban - 3ºIC
Laura Sánchez Urda - 3ºIC

#Fecha de creación:
29-Abril-2026 
