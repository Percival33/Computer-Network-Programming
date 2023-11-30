import socket
import argparse
import struct

BUFFER_SIZE = 1024
message_t = struct.Struct('!H2s2s')


def send_to_client(server_socket, address):
    size = 1
    key = 'A'
    value = 'ok'
    message_t_packed = message_t.pack(size, key.encode(), value.encode())
    server_socket.sendto(message_t_packed, address)


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("port", type=int)
    return parser


def parse_arguments(parser):
    return parser.parse_args()


def run_server(server_ip, server_port):
    server_socket = socket.socket(
        family=socket.AF_INET,
        type=socket.SOCK_DGRAM
    )

    try:
        server_socket.bind((server_ip, server_port))
        print(
            f'Server listening on ip: {server_ip} port: {server_port}'
        )

        while True:
            try:
                message, address = server_socket.recvfrom(BUFFER_SIZE)
            except socket.error as e:
                print(f"Socket error: {e}")
                break

            if not message:
                print("Error in datagram")
                break

            try:
                size, key, value = struct.unpack('!H2s2s', message)
                message_to_print = f'Size: {size}, Key: {key.decode("utf-8")}, Value: {value.decode("utf-8")}'

                print(f'Client ip address   : {address}')
                print(f'Message received    : {message_to_print}')

                send_to_client(server_socket, address)

            except struct.error as e:
                print(f"Error while unpacking message: {e}")

    except socket.error as e:
        print(f"Socket error: {e}")

    finally:
        server_socket.close()


def main():
    arguments = parse_arguments(get_parser())
    run_server('0.0.0.0', arguments.port)
    return 0


if __name__ == "__main__":
    main()
