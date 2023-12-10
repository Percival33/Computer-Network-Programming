#ifndef SERIALIZE_H
#define SERIALIZE_H

#include "common.h"
#include "node.h"

uint16_t pack(uint8_t* buf, Node* node);

Node* unpack(uint8_t* buf);


#endif /* !SERIALIZE_H */
