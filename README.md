## Proyecto: Sistema de Visualización 3D de Objetos Espaciales

### Descripción
Este proyecto es un sistema de visualización 3D para representar objetos espaciales. Utiliza OpenGL y GLM para renderizar modelos 3D de cuerpos celestes y naves espaciales en tiempo real. El sistema permite cargar modelos desde archivos OBJ, aplicar diferentes shaders y manipular la visualización a través de transformaciones geométricas.

Se realizó este proyecto para la clase de Gráficas por Computador de Universidad del Valle de Guatemala, como Proyecto1.

### Demostracion
![demostracion](noCodefiles/vid.mp4)

Al iniciar el programa, los planetas se encuentran alienados a la derecha de la estrella. Los planetas (5) orbitan al rededor de la estrella y tienen cada uno velocidades diferentes. La nave espacial se encuentra en el centro de la estrella y se puede mover con las teclas WASD.

Los controles son los siguientes:
- `W` - Acercarse.
- `S` - Alejarse.
- `A` - Moverse a la izquierda.
- `D` - Moverse a la derecha.
- `P` - Pausar la rotación de los planetas.
- `Left Arrow` - Aumentar la velocidad de rotación de los planetas. 
- `Right Arrow` - Disminuir la velocidad de rotación de los planetas.
- `Esc` - Salir del programa.

Adicionalmente, se agregó que al mover la camara (y por tanto la nave), la nave se calienta y se vuelve roja. Al dejar de moverse, la nave se enfría y vuelve a su color original.

La velocidad de rotación de los planetas se estableció en baja, para simular la rotación real; sin embargo puede ser aumentada sin perder rendimiento. 
### Características
- Renderizado de modelos 3D en tiempo real.
- Soporte para cargar modelos desde archivos OBJ.
- Uso de shaders personalizados para diferentes objetos (por ejemplo, Tierra, Sol, Júpiter).
- Transformaciones geométricas para manipular los modelos en el espacio.

### Requisitos
- Sistema operativo compatible: Windows, Linux o macOS.
- Dependencias:
    - OpenGL
    - GLM (OpenGL Mathematics)
    - C++ Standard Library
