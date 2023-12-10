import socket
import struct
from dataclasses import dataclass

@dataclass
class Node:
    """
    uint16: number of nodes
    uint16: value a
    uint32: value b
    uint16: length of string
    uint8: string characters
    """
    a: int
    b: int
    len: int
    text: str


def pack(nodes_list):
    packed_data = struct.pack('!H', len(nodes_list))
    for node in nodes_list:
        text_len = len(node.text)
        node_data = struct.pack(f'!HIH{text_len}s', node.a, node.b, text_len, bytes(node.text, 'ascii'))
        print(node_data)
        b''.join([packed_data, node_data])

    return packed_data


def unpack(data) -> list[Node]:
    print('data:', data)
    node_number, = struct.unpack('!H', data[:2])

    unpacked_data = []
    buf_ptr = 2

    for _ in range(node_number):
        a, b, text_len = struct.unpack('!HIH', data[buf_ptr:buf_ptr + 8])
        buf_ptr += 8

        text, = struct.unpack(f'!{text_len}s', data[buf_ptr:buf_ptr + text_len])
        buf_ptr += text_len

        new_node = Node(a, b, text_len, text.decode('ascii'))
        print(new_node)
        unpacked_data.append(new_node)

    return unpacked_data


def start_server(host='127.0.0.1', port=65432):
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print(f"Server started, listening on {host}:{port}")

        while True:
            conn, addr = s.accept()
            with conn:
                print(f"Connected by {addr}")
                while True:
                    data = conn.recv(1024)
                    if not data:
                        break

                    unpacked = unpack(data)
                    print(f"Received data: {unpacked}")
                    conn.sendall(pack(unpacked))  # Echo back the received data
                    print('ok')


if __name__ == "__main__":
    start_server()