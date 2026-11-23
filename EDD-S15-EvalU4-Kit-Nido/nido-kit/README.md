# Nido · comunidad `nido-grupoE`

Red social de consola hecha en C++ con un **árbol binario de búsqueda** (las cuentas, ordenadas por @usuario), un **grafo dirigido** en matriz de adyacencia (quién sigue a quién), **recursividad** (recorridos del árbol y DFS para el alcance de un post) y persistencia en `nido.txt`.

Estructura de Datos · Evaluación C2 · Unidad 4 · 2026-20 · NRC `42964` · Grupo `E`

## Equipo

| Integrante | Usuario de GitHub | Rol | Requisitos |
| :--- | :--- | :--- | :--- |
| Dayana Quispe Sucasaca | `@61578130-cloud` | Líder (scrum) / Control de calidad y documentación | R1, R2, R3, R5, R6, R7, pruebas |
| Andrea Celeste Limache Tinta | `@61120408-star` | Dev · árbol y grafo básico | R1, R2, R3 |
| Shany Anjhara Becerra Yari | `@61403826-bit` | Dev · comunidad | R4 |
| Marcos Abel Palomino Quispe | `@MarcosPalomino2` | Dev · alcance y archivo | R5, R6, R7a |

Tablero del equipo: `GitHub Projects (nido-42964-grupoE)` · Presentación: `GRUPOE_Nido.pdf`

## Como compilar y ejecutar

1. Abrir `nido.cpp` en Dev-C++ y compilar (F11). Si aparece un aviso, activar C++11 en
   Herramientas > Opciones del compilador.
2. Ejecutar y escribir `ayuda`.
3. Para guardar la salida de una secuencia: `nido.exe < pruebas\secuencia.txt > pruebas\salida.txt`

## Pruebas automaticas (GitHub Actions)

En cada push, GitHub compila el programa, ejecuta `pruebas/secuencia.txt` y revisa que cada linea
de `pruebas/esperado.txt` aparezca en la salida. El kit trae el ejemplo de la comunidad de prueba.
Cuando carguen su ficha, reemplacen:

- `pruebas/secuencia.txt` por la secuencia oficial de su ficha;
- `pruebas/esperado.txt` por las lineas de su traza a mano (arbol, alcance, llega, reporte).

Si la marca sale roja, abran la pestana **Actions** y vean que linea falta.

## Versiones

| Etiqueta | Entrega | Contenido |
|---|---|---|
| v0.1 | E1 | Equipo, repositorio, tablero y acuerdos |
| v0.5 | E2 | Diseno, traza a mano y los requisitos de la etapa 2 |
| v1.0 | E3 | R1 a R7, informe, pruebas, presentacion |
