import socket
import struct
import argparse

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8088
BUF_SIZE = 1024
STR_SIZE = 2


MAX_PAYLOAD_SIZE = 2
key_value_pair_t = struct.Struct('!2s2s')
message_t = struct.Struct('!HH' + f'{4 * MAX_PAYLOAD_SIZE}s')


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("ip", type=str)
    parser.add_argument("port", type=int)
    return parser


def parse_arguments(parser):
    return parser.parse_args()


def main():


    try:
        sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

        size = 1
        key = 'A'
        value = '1'

        id = 1
        count = 1
        pairs = [('ef', '12'), ('rt', '34')]

        few_pairs_packed = []

        for k, v in pairs:
            one_pair_packed = key_value_pair_t.pack(k.encode(), v.encode())
            few_pairs_packed.append(one_pair_packed)

        pairs_packed = b''.join(few_pairs_packed)

        message_t_packed = message_t.pack(id, count, pairs_packed)


        arguments = parse_arguments(get_parser())

        sockfd.sendto(message_t_packed, (arguments.ip, arguments.port))
        print("Data sent to server:")
        print(f'Size: {size}, Key: "{key}", Value: "{value}"')

        message, address = sockfd.recvfrom(BUF_SIZE)
        id, status_code = struct.unpack('!HB', message)
        message_to_print = f'Received from server  Id: {id}, status_code: {status_code}'
        print(message_to_print)

    except struct.error as e:
        print(f"An error occurred with the struct: {e}")

    except socket.error as e:
        print(f"An error occurred with the socket: {e}")

    finally:
        sockfd.close()


if __name__ == "__main__":
    main()