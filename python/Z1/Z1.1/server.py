import socket
import argparse
import struct

BUFFER_SIZE = 1024
MAX_PAYLOAD_SIZE = 2
key_value_pair_t = struct.Struct("!2s2s")
message_t = struct.Struct("!HH" + f"{4 * MAX_PAYLOAD_SIZE}s")


def send_to_client(server_socket, address):
    id = 1
    status_code = 0
    message_t_packed = struct.pack("!HB", id, status_code)
    server_socket.sendto(message_t_packed, address)


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("port", type=int)
    return parser


def parse_arguments(parser):
    return parser.parse_args()


def run_server(server_ip, server_port):
    server_socket = socket.socket(family=socket.AF_INET, type=socket.SOCK_DGRAM)

    try:
        server_socket.bind((server_ip, server_port))
        print(f"Server listening on ip: {server_ip} port: {server_port}")

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
                message_unpacked = message_t.unpack(message[: message_t.size])
                id, count = message_unpacked[:2]
                pairs = message_unpacked[2:][0]

                print(pairs)
                for i in range(count):

                    key, value = key_value_pair_t.unpack(pairs[i * 4 : (i + 1) * 4])
                    print(f"Key: {key.decode()}, Value: {value.decode()}")

                send_to_client(server_socket, address)

            except struct.error as e:
                print(f"Error while unpacking message: {e}")

    except socket.error as e:
        print(f"Socket error: {e}")

    finally:
        server_socket.close()


def main():
    arguments = parse_arguments(get_parser())
    run_server("0.0.0.0", arguments.port)
    return 0


if __name__ == "__main__":
    main()
