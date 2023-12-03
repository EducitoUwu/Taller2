# Taller2

Con la poda alfa-beta, el tiempo de ejecución del algoritmo debería ser significativamente menor en comparación con la versión sin poda. Esto es especialmente notable en árboles de búsqueda grandes.

Analisis con poda y sin poda:

sin poda alfa-beta, la maquina debe analizar todas las ramas del árbol de casos, dependiendo de la profundidad dada por la dificultad, este tipo de lógica tiene una complejidad algorítmica de O(7^p) donde p es la profundidad segun el nivel.

Con poda alfa-beta, el análisis de la maquina se optimiza y se ocupa menos espacio en memoria, la complejidad algorítmica de este es O(b^(d/2)) donde b es la cantidad de hijos que puede tener el árbol, este va variando, dependiendo del estado de la columna y d representa la profundidad del árbol. la característica principal de este algoritmo es que los cosas donde la maquina gana estas cortan el proceso de análisis de las ramas que seguían y tienen un mayor puntaje, lo que ayuda a la maquina a elegir la mejor opción de manera más optima.