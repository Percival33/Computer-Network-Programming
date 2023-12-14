from dataclasses import dataclass
import socket
import struct

@dataclass
class Node:
    a: int
    b: int
    len: int
    text: str
    next: 'Node' = None

def unpack_data(data):
    print('data:', data)
    offset = 0
    node_count, = struct.unpack_from('!H', data, offset)
    offset += 2

    head = None
    current = None

    for _ in range(node_count):
        a, b, text_len = struct.unpack_from('!HIH', data, offset)
        offset += 8
        text, = struct.unpack_from(f'!{text_len}s', data, offset)
        offset += text_len
        new_node = Node(a, b, text_len, text.decode('utf-8'))

        if not head:
            head = new_node
            current = head
        else:
            current.next = new_node
            current = new_node

    return head

def print_linked_list(head):
    current = head
    while current:
        print(f"Node(a={current.a}, b={current.b}, len={current.len}, text='{current.text}')")
        current = current.next

def start_server(host='::', port=8888):
    with socket.socket(socket.AF_INET6, socket.SOCK_STREAM) as s:
        s.setsockopt(socket.IPPROTO_IPV6, socket.IPV6_V6ONLY, 0)
        s.bind((host, port, 0, 0))
        s.listen()
        print(f"Server started, listening on [{host}]:{port}")

        while True:
            conn, addr = s.accept()
            with conn:
                print(f"Connected by {addr}")
                data = conn.recv(1024)
                if not data:
                    continue

                head_node = unpack_data(data)
                print_linked_list(head_node)

if __name__ == "__main__":
    print('Z2.2 Python server')
    start_server()
