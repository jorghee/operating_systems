<div align="center">
<table>
    <theader>
        <tr>
            <td><img src="https://github.com/rescobedoq/pw2/blob/main/epis.png?raw=true" alt="EPIS" style="width:50%; height:auto;"/></td>
            <th>
                <span>UNIVERSIDAD NACIONAL DE SAN AGUSTIN</span><br />
                <span>FACULTAD DE INGENIERÍA DE PRODUCCIÓN Y SERVICIOS</span><br />
                <span>DEPARTAMENTO ACADÉMICO DE INGENIERÍA DE SISTEMAS E INFORMÁTICA</span><br />
                <span>ESCUELA PROFESIONAL DE INGENIERÍA DE SISTEMAS</span>
            </th>
            <td><img src="https://github.com/rescobedoq/pw2/blob/main/abet.png?raw=true" alt="ABET" style="width:50%; height:auto"/></td>
        </tr>
    </theader>
</table>
</div>

<div style="text-align: center; margin: 20px;">
  <table style="width: 80%; border-collapse: collapse; margin: 0 auto;">
    <tbody>
      <tr>
        <td style="padding: 10px; font-weight: bold;">ASIGNATURA:</td>
        <td colspan="5" style="padding: 10px;">Sistemas operativos</td>
      </tr>
      <tr>
        <td style="padding: 10px; font-weight: bold;">LABORATORIO:</td>
        <td colspan="5" style="padding: 10px;">Programación de colas de mensajes en C para Linux</td>
      </tr>
      <tr>
        <td style="padding: 10px; font-weight: bold;">FECHA DE PRESENTACIÓN:</td>
        <td style="padding: 10px;">13 de Octubre, 2025</td>
        <td style="padding: 10px; font-weight: bold;">AÑO LECTIVO:</td>
        <td style="padding: 10px;">2025 B</td>
        <td style="padding: 10px; font-weight: bold;">NRO. SEMESTRE:</td>
        <td style="padding: 10px;">VI</td>
      </tr>
      <tr>
        <td colspan="6" style="padding: 10px; font-weight: bold;">DOCENTE:</td>
      </tr>
      <tr>
        <td colspan="6" style="padding: 10px;">Arroyo Paz, Antonio</td>
      </tr>
      <tr>
        <td colspan="6" style="padding: 10px; font-weight: bold;">ESTUDIANTE:</td>
      </tr>
      <tr>
        <td colspan="6" style="padding: 10px;"> 20230488 - Mamani Huarsaya, Jorge Luis</td>
      </tr>
    </tbody>
  </table>
</div>

Estos ejercicios contiene están enfocados en la comunicación entre
procesos (IPC) mediante colas de mensajes y memoria compartida.

**Estructura del Directorio:**

```
.
├── ejercicio1/
│   ├── sender.c
│   ├── receiver.c
│   └── common.h
├── ejercicio2/
│   └── nowait_example.c
├── ejercicio3/
│   ├── player_x.c
│   ├── player_o.c
│   └── tictactoe.h
└── Makefile
```

**Compilación:**

Para compilar todos los ejecutables, simplemente ejecuta el
comando `make` en la raíz del proyecto.

```bash
make
```

También puedes compilar los ejercicios individualmente:
- `make ex1`
- `make ex2`
- `make ex3`

Para limpiar los archivos compilados:
```bash
make clean
```

**Ejecución:**

Las instrucciones detalladas para cada ejercicio se encuentran en
sus respectivas secciones a continuación.

### Ejercicio 1 — Separar en dos procesos


El objetivo de este ejercicio es dividir una aplicación monolítica
en dos procesos independientes: un emisor (`sender`) y un receptor
(`receiver`), que se comunican a través de una cola de mensajes 
System V.

**Compilación:**
```bash
gcc -Wall -Wextra -std=c11 -o receiver ejercicio1/receiver.c
gcc -Wall -Wextra -std=c11 -o sender ejercicio1/sender.c
```

**Ejecución:**
1.  Abre una terminal y ejecuta el receptor. Se quedará esperando un
mensaje.
    ```bash
    ./receiver
    ```
2.  Abre una segunda terminal y ejecuta el emisor.
    ```bash
    ./sender
    ```


### Ejercicio 2 — Investigar y ejemplificar `IPC_NOWAIT`

El objetivo es investigar y demostrar el funcionamiento de la
bandera `IPC_NOWAIT` en la llamada al sistema `msgrcv`. Se creará un
ejemplo práctico que muestre cómo esta bandera previene el bloqueo
del proceso cuando no hay mensajes disponibles en la cola.

**Compilación:**
```bash
gcc -Wall -Wextra -std=c11 -o nowait_example ejercicio2/nowait_example.c
```

**Ejecución:**
```bash
./nowait_example
```


### Ejercicio 3 — Tres en raya (Tic-tac-toe) con comunicación síncrona

Implementar un juego de "tres en raya" entre dos procesos. El estado
del tablero se comparte a través de memoria compartida (SHM), y la
sincronización de turnos se gestiona mediante una cola de mensajes,
implementando un protocolo de rendezvous.

**Compilación:**
```bash
gcc -Wall -Wextra -std=c11 -o player_x ejercicio3/player_x.c
gcc -Wall -Wextra -std=c11 -o player_o ejercicio3/player_o.c
```

**Ejecución:**
1.  Abre una terminal y ejecuta al jugador X. Creará los recursos y
    esperará.
    ```bash
    ./player_x
    ```
2.  Abre una segunda terminal y ejecuta al jugador O.
    ```bash
    ./player_o
    ```
3.  Juega por turnos, introduciendo las coordenadas (fila y columna)
    en la terminal correspondiente.
