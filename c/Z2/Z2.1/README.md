
## Serialized structure
- uint16: number of nodes
- uint16: value a
- uint32: value b
- uint16: length of string
- uint8: string characters

Example (serialized to binary):
```shell
1
3
4
4
def\0
```

## Serialization test
```c
Node* B = create_node();
B->a = 3;
B->b = 4;
strncpy(B->text, "def\0", 4);
print_nodes(B);

uint8_t buf[1024];
uint16_t size = pack(buf, B);
printf("Serialized size: %d\n", size);
Node* deserialized_B = unpack(buf);
print_nodes(deserialized_B);
```

This should print:
```shell
3 4 def
INFO:   Packed size: 13
Serialized size: 13
INFO:   No of nodes to deserialize: 1
3 4 def
```
