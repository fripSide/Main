# encoding: utf-8
# https://pymotw.com/3/socket/binary.html

import socket
import struct

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("127.0.0.1", 9991))
print(str(sock.recv(4096)))
s = b"hello server"
l = len(s)
packer = struct.Struct("i")
sock.send(packer.pack(l))
sock.send(s)
sock.close()