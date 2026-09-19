# Versión mínima requerida de CMake
# Nombre del proyecto
cmake_minimum_required(VERSION 3.8)
project(ros2_fundamentals_examples)
 
# Verifica si el compilador que se esta utilizando es el.
# compilador C++ de GNU (g++) o Clang.
# Agrega banderas del compilador para todos los objetivos
# que se definirán más adelante en el archivo CMakeLists.
# Estas banderas habilitan advertencias adicionales para
# ayudara detectar posibles problemas en el código.
# Agrega opciones al proceso de compilación.
if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()
 
# Ubica y configura los paquetes requeridos por el proyecto.
find_package(ament_cmake REQUIRED)
find_package(ament_cmake_python REQUIRED)
find_package(rclcpp REQUIRED)
find_package(rclpy REQUIRED)
find_package(std_msgs REQUIRED)
 
# Define una variable de CMake llamada dependencia
# que enumera todos los paquetes de ROS2 y otras
# dependecias que el proyecto requiere.
set(dependencies
  rclcpp
  std_msgs
)
 
# Agrega los directorios especificados a la lista de rutas
# que el compilador usa para buscar archivos de encabezado.
# Esto es importante para proyectos en C++ donde se tienen
# archivos de encabezador personalizado que no están
# ubicados en las rutas estándar del sistema.
include_directories(
  include
)
 
# Indica a CMake que cree un objeto ejecutable llamado
# cpp_minimal_publisher a partir del archivo fuente
# src/cpp_minimal_publisher.cpp. Tambien asegúrate de que
# CMake conozca las dependencias del programa.
add_executable(cpp_minimal_publisher src/cpp_minimal_publisher.cpp)
ament_target_dependencies(cpp_minimal_publisher ${dependencies})

# Descomentar las siguientes líneas al momento de utilizar el nodo cpp_minimal_subscriber
#add_executable(cpp_minimal_subscriber src/cpp_minimal_subscriber.cpp)
#ament_target_dependencies(cpp_minimal_subscriber ${dependencies})

install(
  DIRECTORY include/
  DESTINATION include
)
 
# Instala los ejecutable cpp
install(
  TARGETS
  cpp_minimal_publisher
#  cpp_minimal_subscriber
  DESTINATION lib/${PROJECT_NAME}
)
 
# Automatiza el proceso de configuración del análisis
# estático (linting) para el paquete, lo cual
# consiste en ejecutar herramientas que analizan el
# código para detectar posibles errores, problemas de
# estilo y otras discrepancias que no cumplen con los
# estándares de codificación o buenas prácticas especificadas.
if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # La siguiente linea omite el analizador que verifica
  # derechos de autor, comenta la linea cuando se agregue
  # un aviso de copyright y licencia a todos los archivos fuente.
  set(ament_cmake_copyright_FOUND TRUE)
  # La siguiente línea omite cpplint (solo funciona en un repositorio git),
  # comenta la línea cuando este paquete esté en un repositorio git y cuando
  # se agregue un aviso de copyright y licencia a todos los archivos fuente
  set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()
 
# Se usa para exportar directorios de inclusión de un paquete de modo que puedan
# ser facilmente incluidos por otros paquetes de dependan de este.
ament_export_include_directories(include)
 
# Genera e instala todos los hooks de entorno y CMake necesarios que
# permiten que otros paquetes encuentren y utilicen este paquete.
ament_package()