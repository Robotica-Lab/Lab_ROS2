from setuptools import find_packages, setup

package_name = 'mi_paquete_python'


setup(
    # Nombre del paquete
    name=package_name,

    # Versión del paquete
    version='0.0.0',

    # Busca automáticamente los módulos Python del paquete
    packages=find_packages(
        exclude=['test']
    ),

    # Archivos adicionales que deben instalarse con el paquete
    data_files=[
        # Registra el paquete dentro del índice de ament
        (
            'share/ament_index/resource_index/packages',
            ['resource/' + package_name]
        ),

        # Instala el archivo package.xml
        (
            'share/' + package_name,
            ['package.xml']
        ),
    ],

    # Dependencias de Python instaladas mediante setuptools
    install_requires=['setuptools'],

    # Indica que el paquete puede instalarse de forma segura como archivo zip
    zip_safe=True,

    # Información del mantenedor
    maintainer='Tu Nombre',
    maintainer_email='tu_correo@ejemplo.com',

    # Descripción del paquete
    description='Paquete ROS 2 de ejemplo en Python.',

    # Licencia del paquete
    license='Apache-2.0',

    # Dependencias utilizadas para pruebas
    tests_require=['pytest'],

    # Define los ejecutables ROS 2 disponibles mediante ros2 run
    entry_points={
        'console_scripts': [
            # nombre_ejecutable = paquete.modulo:funcion
            'mi_nodo = mi_paquete_python.mi_nodo:main',
        ],
    },
)