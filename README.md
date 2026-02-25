# llm-tokenizer
Este repositorio contiene un tokenizer realizado en python. El tokenizer en cuestion sera desarrollado por el metodo BPE (Byte Pair Encoding) que es una forma simple de compresión de datos en la que el par más común de bytes consecutivos se reemplaza con un byte que no ocurre dentro de esos datos. Se requiere una tabla de reemplazos para reconstruir los datos originales.

### Un ejemplo de como funciona BPE
Si queremos codificar los datos "aaabdaaabac" pordemos hacer el reemplazo "Z=aa" por ser el par de bytes de mayor frecuencia.

Entoncees los datos pasan a ser "ZabdZabac". Ahora se repite el proceso para el par "Y=ab", para obtener "ZYbZYac".

El proceso se puede continuar hasta el extremo en el que no existan mas pares de bytes que se produzcan mas de una vez.
