# Versión mínima de CMake requerida
cmake_minimum_required(VERSION 3.8)


# Nombre del paquete ROS 2
project(smartphone_cmd_vel)


# Busca el sistema de construcción de ROS 2
find_package(ament_cmake REQUIRED)


# Busca la biblioteca cliente ROS 2 para C++
find_package(rclcpp REQUIRED)


# Busca los mensajes geometry_msgs
# Aquí se encuentra geometry_msgs/msg/Twist
find_package(geometry_msgs REQUIRED)


# Busca la librería libcurl
# Se utiliza para realizar solicitudes HTTP hacia phyphox
find_package(CURL REQUIRED)


# Busca la librería nlohmann_json
# Se utiliza para interpretar la respuesta JSON de phyphox
find_package(nlohmann_json REQUIRED)


# Crea el ejecutable del nodo
# a partir del archivo smartphone_cmd_vel.cpp
add_executable(
  smartphone_cmd_vel_node
  src/smartphone_cmd_vel.cpp
)


# Declara las dependencias ROS 2 utilizadas por el ejecutable
ament_target_dependencies(
  smartphone_cmd_vel_node
  rclcpp
  geometry_msgs
)


# Enlaza las librerías externas utilizadas por el programa
target_link_libraries(
  smartphone_cmd_vel_node
  CURL::libcurl
  nlohmann_json::nlohmann_json
)


# Instala el ejecutable dentro del paquete
# Esto permite utilizar posteriormente ros2 run
install(
  TARGETS
  smartphone_cmd_vel_node
  DESTINATION lib/${PROJECT_NAME}
)


# Registra el paquete dentro del sistema ament
ament_package()