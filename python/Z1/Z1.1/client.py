import socket
import struct
import argparse

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8088
BUF_SIZE = 1024
STR_SIZE = 2

MAX_PAYLOAD_SIZE = 127
key_value_pair_t = struct.Struct('!2s2s')
message_t = struct.Struct('!HH' + f'{4 * MAX_PAYLOAD_SIZE}s')


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("ip", type=str)
    parser.add_argument("port", type=int)
    return parser


def parse_arguments(parser):
    return parser.parse_args()


def fill_payload_with_zeros(count):
    zero_pair = b'\0\0'
    return zero_pair * (MAX_PAYLOAD_SIZE - count)


def main():
    try:
        sockfd = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        idx = 1
        count = 5
        pairs = [('ef', '12'), ('ef', '34'), ('ef', '34'), ('ef', '34'), ('ef', '34')]

        few_pairs_packed = [key_value_pair_t.pack(k.encode(), v.encode()) for k, v in pairs]
        pairs_packed = b''.join(few_pairs_packed)
        
        if len(few_pairs_packed) < MAX_PAYLOAD_SIZE:
            zero_payload = fill_payload_with_zeros(len(few_pairs_packed))
            pairs_packed += zero_payload

        message_t_packed = message_t.pack(idx, count, pairs_packed)

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
