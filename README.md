# Maqueta de Control de Iluminación con ESP32

Este proyecto surge de la necesidad de programar un ESP32 para una maqueta de arquitectura a escala que simula la iluminación de distintos sectores de una vivienda. Se documenta aquí todo el trabajo para que cualquier estudiante o aficionado pueda aprovecharlo y adaptarlo a sus propios proyectos de domótica básica.

---

## Descripción General

- **Microcontrolador**: ESP32
- **Número de LEDs**: 17
- **Comunicación**: Portal cautivo WiFi (sin necesidad de Internet)
- **Interfaz**: Página web responsiva embebida en el ESP32
- **Alimentación**: USB o Powerbank

### Sectores Controlados

| Planta Baja                   | Planta Alta           |
| ----------------------------- | --------------------- |
| 1. Quincho                    | 6. Sala de estar      |
| 2. Patio                      | 7. Escalera PA        |
| 3. Comedor                    | 8. Habitación 1       |
| 4. Entrada                    | 9. Habitación 2       |
| 5. Garage                     | 10. Espacio           |

Controles generales:
- **Encender Todo**
- **Apagar Todo**
- **Enlace a GitHub** para descargar o clonar el proyecto

---

## Lista de Materiales

| Cantidad | Elemento             | Descripción                                             |
|---------:|----------------------|---------------------------------------------------------|
| 1        | ESP32 NodeMCU        | Microcontrolador con WiFi                              |
| 9        | LEDs rojos           | Indicadores visuales por sector                        |
| 9        | Resistencias 220 Ω   | Limitadoras de corriente para LEDs                     |
| 1        | Protoboard grande    | Base de montaje para conexión de LEDs                  |
| 2        | Cables de red RJ45   | Simulación de cableado doméstico (T568B)               |
| 1        | Fuente USB / Powerbank | Alimentación del ESP32                                |
| Varios   | Jumpers macho-macho  | Conexiones entre ESP32 y protoboard                    |

---

## Conexiones y Colores (Norma T568B)

### Planta Baja

| Sector     | GPIO ESP32 | Color Positivo | Color Negativo |
|-----------:|-----------:|---------------:|---------------:|
| Quincho    | 2          | Naranja        | Blanco/Naranja |
| Patio      | 4          | Verde          | Blanco/Verde   |
| Comedor    | 5          | Azul           | Blanco/Azul    |
| Entrada    | 18         | Marrón         | Blanco/Marrón  |
| Garage     | 19         | Naranja (2do)  | Blanco/Naranja |

### Planta Alta

| Sector          | GPIO ESP32 | Color Positivo      | Color Negativo |
|----------------:|-----------:|--------------------:|---------------:|
| Sala de estar   | 21         | Verde (2do cable)   | Blanco/Verde   |
| Escalera PA     | 22         | Azul (2do cable)    | Blanco/Azul    |
| Habitación 1    | 23         | Marrón (2do cable)  | Blanco/Marrón  |
| Habitación 2    | 26         | Naranja (2do cable) | Blanco/Naranja |
| Espacio         | 25         | Verde (2do cable)   | Blanco/Verde   |

> **Nota:** Cada par T568B utiliza 8 hilos (4 positivos + 4 neutros). Para cubrir todos los sectores se emplean dos cables RJ45 en paralelo. Los colores fuertes llevan la señal (positivo) y los blancos el GND común.

---

## Esquema de Uso

1. Conecta el ESP32 y todos los LEDs con sus resistencias en la protoboard siguiendo las tablas de conexión.
2. Alimenta el ESP32 (USB o Powerbank).
3. Conéctate por WiFi a la red **AP-Candela-Maqueta** (contraseña: _candelapereira_).
4. Abre cualquier navegador en tu dispositivo: serás redirigido automáticamente al portal cautivo en `http://192.168.4.1`.
5. Controla sector por sector o utiliza los botones **Encender Todo** / **Apagar Todo**.
6. Desde la misma página, haz clic en **Ver en GitHub** para ver el código fuente y la documentación completa.

---

## Diagrama de Conexiones

> *(Adjuntar aquí imagen o enlace al diagrama Fritzing del montaje en protoboard)*

---

## Conclusión

Este proyecto demuestra conceptos básicos de domótica y redes locales aplicados a una maqueta arquitectónica. Permite a estudiantes de arquitectura o ingeniería:
- Entender control de GPIOs desde una interfaz web.
- Aprender sobre portales cautivos y servidores embebidos.
- Experimentar con electrónica básica y cableado estructurado.

¡Siéntete libre de clonar o forkar este repositorio y adaptarlo a tus necesidades!

---

**Repositorio en GitHub:**  
[github.com/lordbasex/control-de-Iluminacion](https://github.com/lordbasex/control-de-Iluminacion)
