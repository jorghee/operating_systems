
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
        <td colspan="5" style="padding: 10px;">Exclusión mutua para Threads - Mutex</td>
      </tr>
      <tr>
        <td style="padding: 10px; font-weight: bold;">FECHA DE PRESENTACIÓN:</td>
        <td style="padding: 10px;">27 de Octubre, 2025</td>
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

Exclusión mutua y sincronización con mutex en C utilizando la 
biblioteca Pthreads.

## Prerrequisitos

Para compilar y ejecutar los programas, necesitarás:
*   Un compilador de C (se recomienda `gcc`).
*   Un sistema operativo tipo UNIX (Linux, macOS) con la biblioteca
Pthreads instalada.

> [!NOTE]
> Todos los programas utilizan la biblioteca Pthreads, por lo que 
> deben ser enlazados con la opción `-pthread` durante la compilación.

---

## Ejercicio 1: Suma de Vector con Mutex (`ejercicio01.c`)

Este programa calcula la suma de los elementos de un vector de gran
tamaño de forma paralela. Utiliza un mutex para proteger la variable
global donde se acumula la suma total, evitando condiciones de carrera.

### Compilación
```bash
gcc ejercicio01.c -o ejercicio01 -pthread
```

### Ejecución
El programa requiere un argumento numérico que especifica la
cantidad de hilos a utilizar (entre 1 y 8).

```bash
./ejercicio01 <numero_de_hilos>
```

**Ejemplo con 4 hilos:**
```bash
./ejercicio01 4
```

---

## Ejercicio 2: Problema del Productor-Consumidor (`ejercicio02.c`)

Implementación del clásico problema del productor-consumidor. Un hilo
productor genera datos y un hilo consumidor los procesa, utilizando
un buffer compartido protegido por un mutex.

### Compilación
```bash
gcc ejercicio02.c -o ejercicio02 -pthread
```

### Ejecución
Este programa no requiere argumentos.

```bash
./ejercicio02
```

---

## Ejercicio 3: Multiplicación de Matrices (`ejercicio03.c`)

Programa que multiplica dos matrices de gran tamaño de forma
concurrente. Emplea un pool de hilos que toman tareas (filas a
calcular) de forma segura gracias a un mutex que protege el contador
de tareas.

### Compilación
```bash
gcc ejercicio03.c -o ejercicio03 -pthread
```

### Ejecución
Este programa no requiere argumentos.

```bash
./ejercicio03
```
