import rclpy                  # Biblioteca cliente de ROS 2 para Python
from rclpy.node import Node   # Clase base para crear nodos ROS 2
from std_msgs.msg import String  # Tipo de mensaje String estándar de ROS 2


class MiNodo(Node):

    def __init__(self):
        # Inicializa la clase base Node y asigna un nombre al nodo
        super().__init__('mi_nodo')

        # Crea un publisher que enviará mensajes String al tópico 'mi_topico'
        self.publisher_ = self.create_publisher(
            String,
            'mi_topico',
            10
        )

        # Crea un timer que ejecutará timer_callback cada 0.5 segundos
        self.timer = self.create_timer(
            0.5,
            self.timer_callback
        )


    def timer_callback(self):
        # Crea un nuevo mensaje de tipo String
        msg = String()

        # Asigna información al mensaje
        msg.data = 'Hola desde ROS 2 en Python'

        # Publica el mensaje
        self.publisher_.publish(msg)

        # Muestra información en la consola
        self.get_logger().info(
            'Mensaje publicado'
        )


def main(args=None):

    # Inicializa las comunicaciones de ROS 2
    rclpy.init(args=args)

    # Crea una instancia del nodo
    nodo = MiNodo()

    # Mantiene el nodo en ejecución y procesa sus callbacks
    rclpy.spin(nodo)

    # Libera los recursos asociados al nodo
    nodo.destroy_node()

    # Finaliza las comunicaciones de ROS 2
    rclpy.shutdown()


if __name__ == '__main__':
    # Ejecuta la función principal
    main()