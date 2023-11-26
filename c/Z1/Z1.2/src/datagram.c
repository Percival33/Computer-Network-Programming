#include "datagram.h"
#include "string.h"


bool text_is_only_zeroes(char *text, int text_length) {
    for (int i = 0; i < text_length; i++) {
        if (text[i] != '\0') {
            return false;
        }
    }
    return true;
}


bool datagram_is_valid(data_t *datagram) {
    // Datagram is valid, if:
    // - After the declared pairs there are only zeroes,
    // - The number of pairs does not exceed the max number of
    // pairs that can fit in a packet
    
    // TODO parametrize

    int count = datagram->count;
    // Size
    if (count > MAX_PAIR_COUNT) {
        printf(LOG_ERROR "Datagram is not valid. MAX_PAIR_COUNT(%d) exceeded got: %d\n", MAX_PAIR_COUNT, count);
        return false;
    }

    // Check if the remaining are equal to 0.
    // IMPORTANT - THERE MUST ONLY BE FULL PAIRS
    char *key;
    char *value;
    for (int i = count; i < MAX_PAIR_COUNT; i++) {
        key = datagram->pairs[i].key;
        if (!text_is_only_zeroes(key, KEY_SIZE)) {
            printf(LOG_ERROR"Datagram is not valid. Error parsing key[%d]\n", i);
            return false;
        }
        value = datagram->pairs[i].value;
        if (!text_is_only_zeroes(value, VALUE_SIZE)) {
            printf(LOG_ERROR"Datagram is not valid. Error parsing value[%d]=(%s)\n", i, value);
            return false;
        }
    }
    return true;
}


void print_datagram_data(data_t *datagram) {
    printf("Number of pairs: %d\n", datagram->count);
    printf("Packet id: %d\n", datagram->id);
    printf("Pairs: \n");
    char printable_key[KEY_SIZE + 1];
    char printable_value[VALUE_SIZE + 1]; 
    printable_key[KEY_SIZE] = '\0';
    printable_value[VALUE_SIZE] = '\0';
    for (int i = 0; i < datagram->count; i++) {
        memcpy(printable_key, datagram->pairs[i].key, KEY_SIZE);
        memcpy(printable_value, datagram->pairs[i].value, VALUE_SIZE);

        printf("%s:%s\n", printable_key, printable_value);
    }
    printf("\n");
}


void ntoh_on_datagram(data_t *datagram) {
    datagram->count = ntohs(datagram->count);
    datagram->id = ntohs(datagram->id);
}