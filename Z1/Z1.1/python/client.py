import socket
import struct

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8088
BUF_SIZE = 1024
STR_SIZE = 2

message_t = struct.Struct('!H2s2s')

def main():
    try:
        sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        size = 1
        key = 'A'
        value = '1'
        message_t_packed = message_t.pack(size, key.encode(), value.encode())

        sockfd.sendto(message_t_packed, (SERVER_IP, SERVER_PORT))
        print("Data sent to server:")
        print(f'Size: {size}, Key: "{key}", Value: "{value}"')

        message, address = sockfd.recvfrom(BUF_SIZE)
        size, key, value = struct.unpack('!H2s2s', message)
        message_to_print = f'Received from server  Size: {size}, Key: {key.decode("utf-8")}, Value: {value.decode("utf-8")}'
        print(message_to_print)

    except struct.error as e:
        print(f"An error occurred with the struct: {e}")

    except socket.error as e:
        print(f"An error occurred with the socket: {e}")

    finally:
        sockfd.close()

if __name__ == "__main__":
    main()
