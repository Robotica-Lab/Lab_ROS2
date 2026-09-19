/**
 * @file cpp_minimal_subscriber.cpp
 * @brief Demonstrates subscribing to string messages on a ROS 2 topic.
 *
 * Description: Demonstrates the basics of subscribing to messages within
 *   the ROS 2 framework. The core functionality of this subscriber is to
 *   display output to the terminal window when a message is received over
 *   a topic.
 *
 * -------
 * Subscription Topics:
 *   String message
 *   /cpp_example_topic - std_msgs/String
 * -------
 * Publishing Topics:
 *   None
 * -------
 **/
 
#include "rclcpp/rclcpp.hpp" // Biblioteca cliente de ROS2 para C++
#include "std_msgs/msg/string.hpp" // Manejar mensajes de tipo String
using std::placeholders::_1; // Marcador de posición para el
							 // argumento de la función de callback
/**
 * @class MinimalCppSubscriber
 * @brief Define un nodo suscriptor mínimo en ROS2.
 *
 * Esta clase hereda de rclcpp::Node
 * demuestra cómo crear un suscriptor y
 * suscribirse a mensajes.
 */
class MinimalCppSubscriber : public rclcpp::Node
{
public:
    /**
     * @brief Construye un nodo MinimalCppSubscriber.
     *
     * Configura un suscriptor para mensajes de tipo 'std_msgs::msg::String'
     * en el tópico "/cpp_example_topic".
     */
    MinimalCppSubscriber() : Node("minimal_cpp_subscriber")
    {
        // Crea un objeto suscriptor para escuchar mensajes de tipo string
        // con un tamaño de cola de 10.
        subscriber_ = create_subscription<std_msgs::msg::String>
        (
            "/cpp_example_topic",
            10,
            std::bind(
                &MinimalCppSubscriber::topicCallback,
                this,
                _1
            )
        );
    }
 
    /**
     * @brief Esta función se ejecuta cada vez que se escriba un mensaje en el tópico.
     *
     * Esta es la función de callback del suscriptor. Publica un
     * mensaje del tipo String cada vez que se recibe un mensaje del tópico.
     *
     * @param msg: El mensaje string recibido en el tópico
     * @return void
     */
    void topicCallback(const std_msgs::msg::String &msg) const
    {
        // Escribe un mensaje cada vez que se recibe un nuevo mensaje en el tópico.
        RCLCPP_INFO_STREAM(get_logger(), "I heard: " << msg.data.c_str());
 
    }
 
private:
    // Variables miembro.
    rclcpp::Subscription<std_msgs::msg::String>::SharedPtr subscriber_;
};
 
/**
 * @brief Función principal.
 *
 * Inicializa el sistema ROS2 y ejecuta el nodo minimal_cpp_subscriber.
 * Mantiene el nodo activo hasta que sea terminado manualmente.
 */
int main(int argc, char * argv[])
{
 
  // Inicializar ROS2.
  rclcpp::init(argc, argv);
 
  // Crea una instancia del nodo MinimalCppSubscriber y mantenerlo en ejecución.
  auto minimal_cpp_subscriber_node = std::make_shared<MinimalCppSubscriber>();
  rclcpp::spin(minimal_cpp_subscriber_node);
 
  // Finalizar ROS2 al terminar el nodo.
  rclcpp::shutdown();
 
  // Fin del programa.
  return 0;
}