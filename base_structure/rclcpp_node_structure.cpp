#include <chrono>      // Manejo de tiempos y duraciones
#include <functional>  // Permite usar std::bind para asociar callbacks
#include <memory>      // Permite usar punteros inteligentes como std::shared_ptr

#include "rclcpp/rclcpp.hpp"          // Biblioteca cliente de ROS 2 para C++
#include "std_msgs/msg/string.hpp"    // Tipo de mensaje String estándar de ROS 2


class MiNodo : public rclcpp::Node
{
public:

    MiNodo()
    : Node("mi_nodo")
    {
        // Crea un publisher que enviará mensajes String al tópico 'mi_topico'
        publisher_ =
            this->create_publisher<std_msgs::msg::String>(
                "mi_topico",
                10
            );

        // Crea un timer que ejecutará timer_callback cada 500 ms
        timer_ =
            this->create_wall_timer(
                std::chrono::milliseconds(500),
                std::bind(
                    &MiNodo::timer_callback,
                    this
                )
            );
    }


private:

    void timer_callback()
    {
        // Crea un nuevo mensaje de tipo String
        auto msg = std_msgs::msg::String();

        // Asigna información al mensaje
        msg.data = "Hola desde ROS 2 en C++";

        // Publica el mensaje
        publisher_->publish(msg);

        // Muestra información en la consola
        RCLCPP_INFO(
            this->get_logger(),
            "Mensaje publicado"
        );
    }


    // Variable que almacena el timer
    rclcpp::TimerBase::SharedPtr timer_;

    // Variable que almacena el publisher
    rclcpp::Publisher<std_msgs::msg::String>::SharedPtr publisher_;
};


int main(int argc, char * argv[])
{
    // Inicializa las comunicaciones de ROS 2
    rclcpp::init(argc, argv);

    // Crea el nodo y mantiene activos sus callbacks
    rclcpp::spin(
        std::make_shared<MiNodo>()
    );

    // Finaliza las comunicaciones de ROS 2
    rclcpp::shutdown();

    return 0;
}