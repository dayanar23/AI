# Nombre: Daniel Leones 09-10977
# Función para el cálculo de la profundidad
from math import ceil,log10

def primeraProfu(b,c):
    return ceil(log10(c)/log10(b))