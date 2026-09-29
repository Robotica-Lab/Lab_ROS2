#include <algorithm>   // Permite usar std::clamp()
#include <chrono>      // Permite trabajar con tiempos y periodos
#include <cmath>       // Permite usar funciones matemáticas como std::abs()
#include <memory>      // Permite usar std::make_shared()
#include <string>      // Permite usar std::string

#include <curl/curl.h>         // Librería para realizar solicitudes HTTP
#include <nlohmann/json.hpp>   // Librería para interpretar datos en formato JSON

#include "rclcpp/rclcpp.hpp"             // Biblioteca cliente de ROS 2 para C++
#include "geometry_msgs/msg/twist.hpp"   // Tipo de mensaje Twist


using namespace std::chrono_literals;


class SmartphoneCmdVel : public rclcpp::Node
{
public:

    SmartphoneCmdVel()
    : Node("smartphone_cmd_vel")
    {
        // Crea un publisher de mensajes Twist
        // Los mensajes serán enviados al tópico /turtle1/cmd_vel
        publisher_ =
            this->create_publisher<geometry_msgs::msg::Twist>(
                "/turtle1/cmd_vel",
                10
            );

        // Crea un timer que ejecutará timerCallback cada 100 ms
        // Esto equivale aproximadamente a una frecuencia de 10 Hz
        timer_ =
            this->create_wall_timer(
                100ms,
                std::bind(
                    &SmartphoneCmdVel::timerCallback,
                    this
                )
            );

        // Mensaje informativo al iniciar el nodo
        RCLCPP_INFO(
            this->get_logger(),
            "Nodo smartphone_cmd_vel iniciado"
        );
    }


private:

    // Dirección IP del smartphone
    // Debe coincidir con la IP mostrada por phyphox
    const std::string phone_ip_ =
        "172.16.53.45";

    // Puerto utilizado por el servidor HTTP de phyphox
    const std::string phone_port_ =
        "8080";


    // Esta función recibe los datos enviados por libcurl
    // y los almacena dentro de un string
    static size_t writeCallback(
        void *contents,
        size_t size,
        size_t nmemb,
        void *userp
    )
    {
        // Calcula la cantidad total de bytes recibidos
        size_t total_size =
            size * nmemb;

        // Convierte el puntero genérico a un puntero de tipo string
        std::string *response =
            static_cast<std::string *>(userp);

        // Agrega los datos recibidos al string response
        response->append(
            static_cast<char *>(contents),
            total_size
        );

        // Informa a libcurl cuántos bytes fueron procesados
        return total_size;
    }


    void timerCallback()
    {
        // Crea una instancia de libcurl
        CURL *curl =
            curl_easy_init();

        // Si curl no pudo inicializarse, termina el callback
        if (!curl)
        {
            return;
        }


        // String donde se guardará la respuesta recibida desde phyphox
        std::string response;


        // Construye la URL para consultar los sensores del smartphone
        std::string url =
            "http://" +
            phone_ip_ +
            ":" +
            phone_port_ +
            "/get?accX&accY&accZ";


        // Indica a libcurl la URL que debe consultar
        curl_easy_setopt(
            curl,
            CURLOPT_URL,
            url.c_str()
        );

        // Indica qué función procesará los datos recibidos
        curl_easy_setopt(
            curl,
            CURLOPT_WRITEFUNCTION,
            writeCallback
        );

        // Indica dónde se guardará la respuesta
        curl_easy_setopt(
            curl,
            CURLOPT_WRITEDATA,
            &response
        );


        // Realiza la solicitud HTTP
        CURLcode result =
            curl_easy_perform(curl);


        // Si ocurre un error de comunicación
        if (result != CURLE_OK)
        {
            RCLCPP_WARN(
                this->get_logger(),
                "No se pudo comunicar con el smartphone"
            );

            // Libera los recursos utilizados por curl
            curl_easy_cleanup(curl);

            return;
        }


        // Libera los recursos utilizados por curl
        curl_easy_cleanup(curl);


        try
        {
            // Convierte la respuesta recibida desde texto JSON
            // a una estructura que pueda ser procesada en C++
            auto json =
                nlohmann::json::parse(response);


            // Obtiene el último valor disponible del eje X
            double acc_x =
                json["buffer"]["accX"]["buffer"].back();

            // Obtiene el último valor disponible del eje Y
            double acc_y =
                json["buffer"]["accY"]["buffer"].back();


            /*
             * Conversión de aceleración a comandos de velocidad:
             *
             * accY → movimiento hacia adelante/atrás
             * accX → giro izquierda/derecha
             */


            // Convierte la inclinación adelante/atrás
            // en velocidad lineal
            double linear_x =
                -acc_y / 4.0;

            // Convierte la inclinación lateral
            // en velocidad angular
            double angular_z =
                acc_x / 3.0;


            // Limita la velocidad lineal entre -2 y 2 m/s
            linear_x =
                std::clamp(
                    linear_x,
                    -2.0,
                    2.0
                );

            // Limita la velocidad angular entre -2 y 2 rad/s
            angular_z =
                std::clamp(
                    angular_z,
                    -2.0,
                    2.0
                );


            // Zona muerta para evitar pequeños movimientos
            // producidos por ruido del sensor
            if (std::abs(linear_x) < 0.15)
            {
                linear_x = 0.0;
            }

            if (std::abs(angular_z) < 0.15)
            {
                angular_z = 0.0;
            }


            // Crea un mensaje de tipo Twist
            geometry_msgs::msg::Twist msg;


            // Asigna la velocidad lineal
            msg.linear.x =
                linear_x;

            // Asigna la velocidad angular
            msg.angular.z =
                angular_z;


            // Publica el mensaje en /turtle1/cmd_vel
            publisher_->publish(msg);


            // Muestra los valores recibidos y calculados
            RCLCPP_INFO(
                this->get_logger(),

                "accX: %.2f  accY: %.2f  linear.x: %.2f  angular.z: %.2f",

                acc_x,
                acc_y,
                linear_x,
                angular_z
            );
        }

        // Captura cualquier error al interpretar el JSON
        catch (...)
        {
            RCLCPP_WARN(
                this->get_logger(),
                "Error interpretando datos de phyphox"
            );
        }
    }


    // Variable que almacena el publisher
    rclcpp::Publisher<
        geometry_msgs::msg::Twist
    >::SharedPtr publisher_;


    // Variable que almacena el timer
    rclcpp::TimerBase::SharedPtr timer_;
};


int main(
    int argc,
    char *argv[]
)
{
    // Inicializa las comunicaciones de ROS 2
    rclcpp::init(
        argc,
        argv
    );


    // Crea el nodo y mantiene activos sus callbacks
    rclcpp::spin(
        std::make_shared<
            SmartphoneCmdVel
        >()
    );


    // Finaliza las comunicaciones de ROS 2
    rclcpp::shutdown();

    return 0;
}