import socket

IP_ADDRESS = "192.168.1.123"
PORT = 8888

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

s.bind((IP_ADDRESS, PORT));

while True:
    print(s.recvfrom(1024))