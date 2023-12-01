import socket
import argparse

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8088
BUF_SIZE = 1024


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("ip", type=str)
    parser.add_argument("port", type=int)
    return parser


def parse_arguments(parser):
    return parser.parse_args()


def main():
    arguments = parse_arguments(get_parser())

    with socket.socket(socket.AF_INET, socket.SOCK_DGRAM) as sockfd:
        datagram_size = 1
        while True:
            datagram = b'A' * datagram_size
            try:
                sockfd.sendto(datagram, (arguments.ip, arguments.port))
                print(f"INFO: Data sent: size = {datagram_size} bytes")
                sockfd.recvfrom(BUF_SIZE)
                datagram_size += 1
            except socket.error as e:
                print(f"INFO: Cannot send data of size {datagram_size}: {e}")
                break


if __name__ == "__main__":
    main()
