import socket
import argparse
import struct

BUFFER_SIZE = 1024
MAX_PAYLOAD_SIZE = 2
key_value_pair_t = struct.Struct('!2s2s')
message_t = struct.Struct('!HH' + f'{4 * MAX_PAYLOAD_SIZE}s')
response_t = struct.Struct('!HB')


def parse_key_value_from_data(data, index, size):
    beggin = index * size
    end = beggin + size
    return key_value_pair_t.unpack(data[beggin:end])

def send_to_client(server_socket, address):
    id = 1
    status_code = 0
    message_t_packed = struct.pack('!HB', id, status_code)
    server_socket.sendto(message_t_packed, address)


def get_parser():
    parser = argparse.ArgumentParser()
    parser.add_argument("port", type=int)
    return parser


def parse_arguments(parser):
    return parser.parse_args()


def send_to_client_and_wait_for_response(server_socket, address):
    id = 1
    status_code = 0
    message_t_packed = struct.pack('!HB', id, status_code)

    while True:
        try:
            print("INFO: Sending a reply to the client")

            server_socket.sendto(message_t_packed, address)


            server_socket.settimeout(5.0)


            try:
                response, client_address = server_socket.recvfrom(BUFFER_SIZE)
                if client_address == address:
                    print("INFO: Received new message from the client.")
                    return response
            except socket.timeout:
                print("INFO: No new message received, sending response again...")

        except socket.error as e:
            print(f"Error while sending to/receiving from client: {e}")
            break


def run_server(server_ip, server_port):
    server_socket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    try:
        server_socket.bind((server_ip, server_port))
        print(f'INFO: Server listening on ip: {server_ip} port: {server_port}')

        while True:
            try:
                message, address = server_socket.recvfrom(BUFFER_SIZE)
                print(message)

                message_unpacked = message_t.unpack(message[:message_t.size])
                id, count = message_unpacked[:2]
                pairs = message_unpacked[2:][0]

                print(pairs)
                for i in range(count):
                    key, value = key_value_pair_t.unpack(pairs[i * 4:(i + 1) * 4])
                    print(f'Key: {key.decode()}, Value: {value.decode()}')


                new_message = send_to_client_and_wait_for_response(server_socket, address)
                if new_message:
                    print(new_message)
                    break

            except socket.error as e:
                print(f"ERROR: Socket error: {e}")

    except socket.error as e:
        print(f"ERROR: Socket binding error: {e}")

    finally:
        server_socket.close()


def main():
    arguments = parse_arguments(get_parser())
    run_server('localhost', arguments.port)
    return 0


if __name__ == "__main__":
    main()