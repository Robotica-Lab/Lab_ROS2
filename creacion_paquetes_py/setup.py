
import os
from glob import glob
from setuptools import find_packages, setup

package_name = 'py_twist_publisher'

setup(
    name=package_name,
    version='0.0.0',
    packages=find_packages(exclude=['test']),
    data_files=[
        ('share/ament_index/resource_index/packages',
            ['resource/' + package_name]),
        ('share/' + package_name, ['package.xml']),
    ],
    install_requires=['setuptools'],
    zip_safe=True,
    maintainer='kuiquer',
    maintainer_email='kuiquer@todo.todo',
    description='TODO: Package description',
    license='TODO: License declaration',
    tests_require=['pytest'],
    entry_points={
        'console_scripts': [
                'twist_publisher_node = py_twist_publisher.twist_publisher_node:main',
        ],
    },
)

