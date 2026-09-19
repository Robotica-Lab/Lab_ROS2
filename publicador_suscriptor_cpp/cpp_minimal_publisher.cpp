/**
 * @file cpp_minimal_publisher.cpp
 * @brief Demuestra la publicación de mensajes de tipo string en un tópico de ROS2.
 *
 * Descripción: Demuestra los fundamentos de la publicación de mensajes dentro
 * del framework ROS2.
 *
 * La funcionalidad principal de este publisher consiste en enviar repetidamente
 * mensajes de tipo string a una frecuencia fija.
 *
 * -------
 * Tópicos de suscripción:
 *   Ninguno
 * -------
 * Tópicos de publicación:
 *   Mensaje de tipo String
 *   /cpp_example_topic - std_msgs/String
 * -------
 **/

#include "rclcpp/rclcpp.hpp" // Biblioteca cliente de ROS2 para C++
#include "std_msgs/msg/string.hpp" // Tipo de mensaje estándar para cadenas

using namespace std::chrono_literals; // Elimina la necesidad de usar el prefijo.
									  // Maneja duración del tiempo
/**
 * @class MinimalCppPublisher
 * @brief Define un nodo publisher mínimo de ROS2.
 *
 * Esta clase hereda de rclcpp::Node y muestra cómo crear un
 * publicador, publicar mensajes y usar una función de temporizador en ROS2.
 **/
class MinimalCppPublisher : public rclcpp::Node
{
public:
    /**
     * @brief Construye un nodo MinimalCppPublisher.
     *
     * Configura un publicador para mensaje de tipo 'std_msgs::msg::String'
     * en el tópico "/cpp_example_topic" e inicializa un temporizador para llamar
     * al método timerCallback.
     **/
    MinimalCppPublisher() : Node("minimal_cpp_publisher"), count_(0)
    {
        // Crea un objeto publicador para enviar mensajes de tipo string
        // con una cola de tamaño 10.
        publisher_ = create_publisher<std_msgs::msg::String>(
          "/cpp_example_topic", 10);
 
        // Configura un temporizador para llamar a la función timerCallback
        timer_ = create_wall_timer(500ms,
          std::bind(&MinimalCppPublisher::timerCallback, this));
 
        RCLCPP_INFO(get_logger(), "Publishing at 2 Hz");
    }

    /**
     * @brief Función callback del timer.
     *
     * Este método se llama a intervalos fijos.
     * Publica un mensaje de texto
     * que contiene "Hello World" seguido de un número de secuencia.
     * @return void
     **/
    void timerCallback()
    {
        // Crea un objeto de mensaje String.
        auto message = std_msgs::msg::String();
        message.data = "Hello World! " +  std::to_string(count_++);
 
        // Publica el mensaje
        publisher_->publish(message);
    }

private:
  // Variables miembro
  size_t count_;  // Lleva el conteo de la cantidad de mensajes publicados
  rclcpp::Publisher<std_msgs::msg::String>::SharedPtr
    publisher_;  // El objeto publicador
  rclcpp::TimerBase::SharedPtr timer_;  // Temporizador
};

/**
 * @brief Función principal.
 *
 * Inicializa el sistema ROS2 y ejecuta el nodo minimal_cpp_publisher.
 * Mantiene el nodo activo hasta que se termine manualmente.
 **/
int main(int argc, char * argv[])
{
  // Inicializa ROS2.
  rclcpp::init(argc, argv);
 
  // Crea una instancia del nodo MinimalCppPublisher y lo mantiene en ejecución.
  auto minimal_cpp_publisher_node = std::make_shared<MinimalCppPublisher>();
  rclcpp::spin(minimal_cpp_publisher_node);
 
  // Apaga ROS2 cuando el nodo se termina.
  rclcpp::shutdown();
 
  // Fin del programa.
  return 0;
}