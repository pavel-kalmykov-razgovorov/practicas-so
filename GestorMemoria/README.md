# Práctica 3 - Gestor de Memoria
Se pretende realizar una simulación de gestión de memoria con particiones dinámicas.
Se ofrecerá un menú con la opción de realizar la asignación de particiones mediante los
algoritmos de primer hueco y mejor hueco.

El programa recibirá como entrada un archivo que tendrá una línea por cada proceso a
cargar con el formato siguiente: <Proceso> <Instante_llegada> <Memoria_requerida>
<Tiempo_de_ejecución>. La cantidad de memoria total será de 2000. El resultado será
guardado en un fichero particiones.txt. Se representarán, en líneas de texto, el estado de
la memoria con cada entrada o salida de procesos, con el formato:

Instante de tiempo [Dirección_inicio_1 Estado_1 tamaño_1] ...
[Dirección_inicio_n Estado_n tamaño_n] salto de línea

Por ejemplo:
1 [0 P1 300] [300 P2 200] [500 hueco 1500] salto de línea

Como alternativa, el resultado se puede mostrar gráficamente, aspecto que se valorará
especialmente.

El programa se llamará gestormemoria y se deberá implementar las estructuras
necesarias para la gestión de las particiones. Se debe de realizar al menos 2 trazas de
programa para tener un ejemplo de la ejecución de cada uno de los algoritmos de
asignación de particiones.
