# coding: utf-8

import socket, time

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(('', 8888))
server_socket.listen(5)

while True:
    pass
