#include <cmath>       // Funciones matem√°ticas: sqrt(), atan2(), sin(), cos()
#include <memory>      // Permite usar std::make_shared()
#include <functional>  // Permite usar std::bind() y placeholders

#include "rclcpp/rclcpp.hpp"           // Biblioteca cliente de ROS 2 para C++
#include "geometry_msgs/msg/twist.hpp" // Mensaje Twist para velocidad
#include "turtlesim/msg/pose.hpp"      // Mensaje Pose de turtlesim

// ============================================================
// CLASE DEL NODO
// ============================================================

// Creamos una clase llamada TurtleGoToGoal.

// Esta clase hereda de rclcpp::Node,
// lo que significa que se comportara como un nodo ROS 2.

// En programacion tradicional podriamos tener:
// - variables globales
// - funciones separadas

// agrupamos todo dentro de una misma clase.

class TurtleGoToGoal : public rclcpp::Node
{
public:
    // ========================================================
    // CONSTRUCTOR
    // ========================================================

    // El constructor se ejecuta automaticamente
    // cuando se crea una instancia de la clase.

    // En este caso se utiliza para:
    // - asignar nombre al nodo
    // - crear el publisher
    // - crear el subscriber

    TurtleGoToGoal()
    : Node("turtle_go_to_goal")
    {
        // ----------------------------------------------------
        // PUBLISHER
        // ----------------------------------------------------

        // Crea un publisher que enviara mensajes Twist
        // al topico /turtle1/cmd_vel.

        // Este topico controla la velocidad de la tortuga.

        publisher_ =
            this->create_publisher<geometry_msgs::msg::Twist>(
                "/turtle1/cmd_vel",
                10
            );

        // ----------------------------------------------------
        // SUBSCRIBER
        // ----------------------------------------------------

        // Crea un subscriber al topico /turtle1/pose.

        // Este topico entrega:
        // - posicion x
        // - posicion y
        // - orientacion theta

        // Cada vez que llega un mensaje,
        // se ejecutara la funcion poseCallback().

        subscription_ =
            this->create_subscription<turtlesim::msg::Pose>(
                "/turtle1/pose",
                10,

                std::bind(
                    &TurtleGoToGoal::poseCallback,
                    this,
                    std::placeholders::_1
                )
            );

        // Mensaje informativo al iniciar el nodo
        RCLCPP_INFO(
            this->get_logger(),
            "Nodo turtle_go_to_goal iniciado"
        );
    }

private:
    // ========================================================
    // VARIABLES DEL OBJETIVO
    // ========================================================

    // Coordenadas a las que queremos llevar la tortuga.

    const double target_x_ = 8.0;
    const double target_y_ = 8.0;

    // ========================================================
    // GANANCIAS
    // ========================================================

    // Estas constantes determinan cuanto afecta el error
    // a la velocidad del robot.

    // k_linear_:
    // controla cuanto aumenta la velocidad lineal
    // cuando estamos lejos del objetivo.

    // k_angular_:
    // controla cuanto aumenta la velocidad angular
    // cuando la tortuga esta mal orientada.

    const double k_linear_ = 1.0;
    const double k_angular_ = 4.0;

    // ========================================================
    // TOLERANCIA
    // ========================================================
    //
    // Consideramos que la tortuga llega al objetivo
    // cuando la distancia es menor que este valor.
    //
    const double tolerance_ = 0.1;

    // ========================================================
    // CALLBACK
    // ========================================================
    //
    // Esta funcion se ejecuta automaticamente
    // cada vez que llega un nuevo mensaje
    // desde /turtle1/pose.
    //
    void poseCallback(
        const turtlesim::msg::Pose::SharedPtr msg
    )
    {
        // ----------------------------------------------------
        // 1. LEER LA POSICION ACTUAL
        // ----------------------------------------------------

        // Posicion actual en X
        double x =
            msg->x;

        // Posicion actual en Y
        double y =
            msg->y;

        // Orientacion actual de la tortuga
        double theta =
            msg->theta;

        // ----------------------------------------------------
        // 2. CALCULAR ERROR DE POSICION
        // ----------------------------------------------------
        
        // Calculamos cuanto falta para llegar
        // desde la posicion actual hasta el objetivo.

        double error_x =
            target_x_ - x;

        double error_y =
            target_y_ - y;

        // ----------------------------------------------------
        // 3. CALCULAR DISTANCIA AL OBJETIVO
        // ----------------------------------------------------

        // Aplicamos la distancia euclidiana:
        // d = sqrt(dx≤ + dy≤)

        double distance =
            std::sqrt(
                error_x * error_x +
                error_y * error_y
            );

        // ----------------------------------------------------
        // 4. CALCULAR ANGULO HACIA EL OBJETIVO
        // ----------------------------------------------------

        // atan2() calcula el angulo desde la posicion actual
        // hacia el objetivo.

        double target_angle =
            std::atan2(
                error_y,
                error_x
            );

        // ----------------------------------------------------
        // 5. CALCULAR ERROR ANGULAR
        // ----------------------------------------------------

        // Diferencia entre:
        // - hacia donde deberia mirar la tortuga
        // - hacia donde esta mirando actualmente

        double angle_error =
            target_angle - theta;

        // ----------------------------------------------------
        // 6. NORMALIZAR EL ANGULO
        // ----------------------------------------------------
        
        // Esta operacion mantiene el error angular
        // dentro del intervalo:
        // [-pi, pi]
        
        // Evita que la tortuga intente girar por el camino largo.

        angle_error =
            std::atan2(
                std::sin(angle_error),
                std::cos(angle_error)
            );

        // ----------------------------------------------------
        // 7. CREAR MENSAJE DE VELOCIDAD
        // ----------------------------------------------------

        // Twist contiene velocidades lineales y angulares.

        geometry_msgs::msg::Twist cmd_vel;

        // ----------------------------------------------------
        // 8. DECIDIR SI MOVERSE O DETENERSE
        // ----------------------------------------------------

        if (distance > tolerance_)
        {
            // ------------------------------------------------
            // VELOCIDAD LINEAL
            // ------------------------------------------------

            // Mientras mas lejos esta la tortuga,
            // mayor sera la velocidad lineal.

            cmd_vel.linear.x =
                k_linear_ * distance;

            // ------------------------------------------------
            // VELOCIDAD ANGULAR
            // ------------------------------------------------

            // Mientras mayor sea el error de orientacion,
            // mayor sera la velocidad de giro.

            cmd_vel.angular.z =
                k_angular_ * angle_error;

            // ------------------------------------------------
            // MOSTRAR INFORMACION EN TERMINAL
            // ------------------------------------------------

            RCLCPP_INFO(
                this->get_logger(),

                "Posicion: (%.2f, %.2f) | Distancia: %.2f | Error angular: %.2f",

                x,
                y,
                distance,
                angle_error
            );
        }

        else
        {
            // ------------------------------------------------
            // OBJETIVO ALCANZADO
            // ------------------------------------------------

            // Si estamos suficientemente cerca,
            // enviamos velocidad cero.

            cmd_vel.linear.x =
                0.0;

            cmd_vel.angular.z =
                0.0;

            RCLCPP_INFO(
                this->get_logger(),
                "Objetivo alcanzado"
            );
        }

        // ----------------------------------------------------
        // 9. PUBLICAR EL COMANDO
        // ----------------------------------------------------

        // Enviamos el mensaje Twist a /turtle1/cmd_vel.

        publisher_->publish(cmd_vel);
    }

    // ========================================================
    // VARIABLES ROS 2 DE LA CLASE
    // ========================================================

    // Publisher que enviara comandos de velocidad
    rclcpp::Publisher<
        geometry_msgs::msg::Twist
    >::SharedPtr publisher_;

    // Subscriber que recibira la posicion de la tortuga
    rclcpp::Subscription<
        turtlesim::msg::Pose
    >::SharedPtr subscription_;
};

// ============================================================
// FUNCION PRINCIPAL
// ============================================================

// Igual que en un programa C++ tradicional,
// la ejecucion comienza en main().

int main(
    int argc,
    char * argv[]
)
{
    // --------------------------------------------------------
    // 1. INICIALIZAR ROS 2
    // --------------------------------------------------------

    rclcpp::init(
        argc,
        argv
    );

    // --------------------------------------------------------
    // 2. CREAR EL NODO
    // --------------------------------------------------------

    // std::make_shared crea una instancia de TurtleGoToGoal
    // utilizando un puntero inteligente.

    // Luego spin() mantiene el nodo en ejecucion
    // y permite procesar los callbacks.

    rclcpp::spin(
        std::make_shared<TurtleGoToGoal>()
    );

    // --------------------------------------------------------
    // 3. FINALIZAR ROS 2
    // --------------------------------------------------------

    rclcpp::shutdown();

    // Fin correcto del programa
    return 0;
}

