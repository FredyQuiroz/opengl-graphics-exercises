# OpenGL Graphics Exercises
[English](#english) · [Español](#español)

## English


Windows C++ exercises for drawing and transforming shapes with OpenGL

> [!WARNING]
> **Status: Legacy.** Windows exercises from 2011. Converted to MSVC v145 in 2026.

### Overview

These Windows exercises show how a C++ program turns coordinates into pixels with OpenGL. They form a progression from drawing lines and filled shapes to comparing transformations and animation.

### Features

- **`primero`** · Radial lines and a triangle on a black background.
- **`taller4`** · A grid comparing points, line strips and filled polygons.
- **`taller5a`** · Colored primitives viewed under perspective projection.
- **`taller5b`** · Coordinate axes and a radial drawing translated from the center.
- **`taller6`** · A triangle demonstration that animates translation, rotation and scaling.

### Requirements

- On Windows, install [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) with Desktop development with C++ and a Windows SDK. Use the Win32 target.
- The archived Win32 OpenGL, GLU and GLAUX headers and libraries are included under include/ and lib/. The companion file lists their origin and hashes.

### Getting started

Build the solution in a Developer PowerShell for Visual Studio at the project root.

```powershell
MSBuild OpenGLExercises.sln /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /p:PreferredToolArchitecture=x64
```

### Usage

Press Enter in the console for `primero` or any key in the console for `taller4`. Close the other exercise windows using the close button.

```powershell
.\bin\primero\Release\primero.exe
.\bin\taller4\Release\taller4.exe
.\bin\taller5a\Release\taller5a.exe
.\bin\taller5b\Release\taller5b.exe
.\bin\taller6\Release\taller6.exe
```

### Project structure

- `include/` and `lib/` · The Win32 SDK headers and import libraries from the course backup.
- `primero/` · Radial lines and a triangle on a black background. Start with `primero/main.cpp`.
- `taller4/` · A grid comparing points, line strips and filled polygons. Start with `taller4/main.cpp`.
- `taller5a/` · Colored primitives viewed under perspective projection. Start with `taller5a/main.cpp`.
- `taller5b/` · Coordinate axes and a radial drawing translated from the center. Start with `taller5b/main.cpp`.
- `taller6/` · A triangle demonstration that animates translation, rotation and scaling. Start with `taller6/main.cpp`.

### Contributing

Describe the exercise and expected drawing in the change. Keep the historic project files for comparison and verify the Win32 solution before sending the change.

### Support

Use the [issue tracker](https://github.com/FredyQuiroz/opengl-graphics-exercises/issues) to report a build or rendering issue. Include the exercise folder, toolset version and first error line.

### History

The 2011 revisions preserve the recovered C++ and Visual C++ projects. The dates use the calendar day of each archived `main.cpp`, with noon UTC as a reference because the archive has no timezone. The 2026 reconstruction adds the v145 projects, the root solution and per-exercise output directories. It adds the legacy CRT import definitions and uses x86 linking without SafeSEH metadata because the archived GLAUX objects predate it.

### License

The MIT license applies to the original exercise code and the 2026 build files and demo assets. See [LICENSE](LICENSE).

---

## Español


Ejercicios C++ para dibujar y transformar figuras con OpenGL en Windows

> [!WARNING]
> **Estado: Legacy (histórico).** Ejercicios para Windows de 2011, convertidos a MSVC v145 en 2026.

### Descripción general

Estos ejercicios de Windows muestran cómo un programa de C++ convierte coordenadas en píxeles con OpenGL. Van del dibujo de líneas y formas rellenas a la comparación de transformaciones y la animación.

### Funciones

- **`primero`** · Líneas radiales y un triángulo sobre fondo negro.
- **`taller4`** · Una cuadrícula para comparar puntos, tiras de líneas y polígonos rellenos.
- **`taller5a`** · Figuras de colores dibujadas con perspectiva.
- **`taller5b`** · Ejes de coordenadas y un dibujo radial desplazado del centro.
- **`taller6`** · Una demostración que anima el desplazamiento, el giro y la escala de un triángulo.

### Requisitos

- En Windows, instala [Visual Studio Community](https://visualstudio.microsoft.com/vs/community/) con Desarrollo para el escritorio con C++ y un SDK de Windows. Selecciona Win32.
- Las cabeceras y bibliotecas Win32 archivadas de OpenGL, GLU y GLAUX están en include/ y lib/. Los avisos registran su origen y hashes.

### Cómo empezar

Compila la solución desde la raíz en Developer PowerShell para Visual Studio.

```powershell
MSBuild OpenGLExercises.sln /m:1 /t:Rebuild /p:Configuration=Release /p:Platform=Win32 /p:PreferredToolArchitecture=x64
```

### Uso

Presiona Enter en la consola para `primero` o cualquier tecla en la consola para `taller4`. Usa el botón de cerrar en las demás ventanas.

```powershell
.\bin\primero\Release\primero.exe
.\bin\taller4\Release\taller4.exe
.\bin\taller5a\Release\taller5a.exe
.\bin\taller5b\Release\taller5b.exe
.\bin\taller6\Release\taller6.exe
```

### Estructura del proyecto

- `include/` y `lib/` · Cabeceras y bibliotecas de enlace Win32 del respaldo del curso.
- `primero/` · Líneas radiales y un triángulo sobre fondo negro. Empieza en `primero/main.cpp`.
- `taller4/` · Una cuadrícula para comparar puntos, tiras de líneas y polígonos rellenos. Empieza en `taller4/main.cpp`.
- `taller5a/` · Figuras de colores dibujadas con perspectiva. Empieza en `taller5a/main.cpp`.
- `taller5b/` · Ejes de coordenadas y un dibujo radial desplazado del centro. Empieza en `taller5b/main.cpp`.
- `taller6/` · Una demostración que anima el desplazamiento, el giro y la escala de un triángulo. Empieza en `taller6/main.cpp`.

### Contribuir

Describe el ejercicio y lo que debería dibujar. Conserva los proyectos históricos para comparar y verifica la solución Win32 antes de enviar el cambio.

### Soporte

Usa el [registro de problemas](https://github.com/FredyQuiroz/opengl-graphics-exercises/issues) para reportar un fallo de compilación o dibujo. Incluye la carpeta, la versión del compilador y el primer error.

### Historial

Las revisiones de 2011 conservan el C++ y los proyectos de Visual C++ recuperados. Cada fecha usa el día de su `main.cpp` archivado y las 12:00 UTC como referencia porque el respaldo carece de zona horaria. La reconstrucción de 2026 agrega los proyectos v145, la solución en la raíz y salidas por ejercicio. Agrega las definiciones antiguas de la biblioteca de C y enlaza en x86 sin los metadatos SafeSEH porque los objetos archivados de GLAUX son anteriores.

### Licencia

La licencia MIT cubre el código propio de los ejercicios y los archivos de compilación y demo añadidos en 2026. Consulta [LICENSE](LICENSE).
