import socket
import argparse

BUFFER_SIZE = 1024


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("port", type=int)
    return parser


def parse_arguments(parser):
    return parser.parse_args()


def run_server(server_ip, server_port):
    with socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM) as server_socket:
        server_socket.bind((server_ip, server_port))
        print(f'INFO: Server is listening on IP: {server_ip}, PORT: {server_port}')

        while True:
            try:
                datagram_message, address = server_socket.recvfrom(BUFFER_SIZE)
                if not datagram_message:
                    print("INFO: No datagram was received")
                    continue
                print(f"INFO: Received datagram of size {len(datagram_message)} bytes from {address}")
                server_socket.sendto(b'OK', address)

            except socket.error as error:
                print(f"ERROR: Socket error: {error}")
                break


def main():
    arguments = parse_arguments(get_parser())
    run_server('localhost', arguments.port)
    return 0


if __name__ == "__main__":
    main()
