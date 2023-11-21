import socket
import struct

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8888
BUF_SIZE = 1024
STR_SIZE = 2

message_t = struct.Struct('H2s2s')

if __name__ == "__main__":
    sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    size = 1
    key = 'A'
    value = '1'
    message_t_packed = message_t.pack(socket.htons(size), key.encode(), value.encode())

    sockfd.sendto(message_t_packed, (SERVER_IP, SERVER_PORT))
    print("Data sent to server")

    data, addr = sockfd.recvfrom(BUF_SIZE)
    print("Received from server:", data)

    sockfd.close()
