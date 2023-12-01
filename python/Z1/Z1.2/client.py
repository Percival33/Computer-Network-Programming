import socket
import struct
import argparse

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8088
BUF_SIZE = 1024

MAX_PAYLOAD_SIZE = 2
key_value_pair_t = struct.Struct('!2s2s')
message_t = struct.Struct('!HH' + f'{4 * MAX_PAYLOAD_SIZE}s')
response_t = struct.Struct('!HB')  # Definicja formatu odpowiedzi

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
        print(message_t_packed)
        print("Data sent to server.")

        # Odbieranie odpowiedzi od serwera
        message, address = sockfd.recvfrom(BUF_SIZE)
        response_id, status_code = response_t.unpack(message)
        print(f"Received response from server: Id: {response_id}, Status Code: {status_code}")

        # Wysyłanie odpowiedzi do serwera
        # response_id = 2  # Przykładowy identyfikator odpowiedzi
        # status_code = 0  # Przykładowy kod statusu
        # response = response_t.pack(response_id, status_code)
        # sockfd.sendto(response, (arguments.ip, arguments.port))
        # print("Response sent back to server.")

    except struct.error as e:
        print(f"An error occurred with the struct: {e}")
    except socket.error as e:
        print(f"An error occurred with the socket: {e}")
    finally:
        sockfd.close()

if __name__ == "__main__":
    main()
