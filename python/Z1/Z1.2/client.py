import socket
import struct
import argparse

SERVER_IP = '127.0.0.1'
SERVER_PORT = 8088
BUF_SIZE = 1024

MAX_PAYLOAD_SIZE = 2
key_value_pair_t = struct.Struct('!2s2s')
message_t = struct.Struct('!HH' + f'{4 * MAX_PAYLOAD_SIZE}s')
response_t = struct.Struct('!HB')

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
        sockfd.settimeout(5.0)

        while True:
            try:

                sockfd.sendto(message_t_packed, (arguments.ip,arguments.port))


                response, server = sockfd.recvfrom(8088)
                print(f"INFO: Response received from server: {response}")
                id = 1
                status_code = 0
                message_t_packed = struct.pack('!HB', id, status_code)
                print("INFO: Sending a reply to the server")
                sockfd.sendto(message_t_packed, (arguments.ip,arguments.port))
                break
            except socket.timeout:
                print("INFO: No response from the server, sending again....")


        print(f"INFO: Data sent to server: {message_t_packed}")


    except struct.error as e:
        print(f"ERROR: An error occurred with the struct: {e}")
    except socket.error as e:
        print(f"ERROR: An error occurred with the socket: {e}")
    finally:
        sockfd.close()

if __name__ == "__main__":
    main()
