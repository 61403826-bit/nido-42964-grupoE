# Pruebas

| ID | Requisito | Orden o secuencia | Resultado esperado | Resultado obtenido | OK |
|---|---|---|---|---|---|
| P01 | R1 | `registrar @kiwi` (dos veces) | La segunda vez: `E: No se pudo registrar @kiwi` | Coincide exactamente | Sí |
| P02 | R1 | `registrar kiwi` (sin @) | `E: No se pudo registrar kiwi` | Coincide exactamente | Sí |
| P03 | R2 | `ver` y `arbol` (con las 10 cuentas iniciales) | Lista alfabética (`@beat` a `@zenit`), altura 5 y 3 hojas | Coincide exactamente | Sí |
| P04 | R3 | `seguir @a @a` | `E: Una cuenta no puede seguirse a si misma` | Coincide exactamente | Sí |
| P05 | R4 | `mutuos` y `popular` | 3 pares mutuos (`@beat-@koala`, `@cactus-@delta`, `@misti-@zenit`) y `@misti` (4 seguidores) | Coincide exactamente | Sí |
| P06 | R5 | `alcance @koala` y `llega @pixel @koala` | Alcance: 8 cuentas; `NO hay camino de @pixel a @koala` | Coincide exactamente | Sí |
| P07 | R6 | `guardar`, `salir`, abrir, `cargar`, `arbol` | El mismo preorden, la misma altura y los mismos seguimientos | Coincide exactamente | Sí |
| P08 | R7 | `hola`, `seguir`, `perfil @nadie` | Mensajes de error; el programa sigue | Coincide exactamente | Sí |
| P09 | R7 | `reporte` (tras registrar `@mango`) | `Cuentas: 11 \| seguimientos: 16 \| pares mutuos: 3` y `Arbol: altura 5 \| hojas 4` | Coincide exactamente | Sí |
| P10 | R1 | Registrar cuenta 13 (superando el límite de 12) | Mensaje de error por límite máximo de cuentas alcanzado | Coincide exactamente | Sí |
