#include "response.h"
#include "common.h"


bool response_is_valid(response_t *response, int expected_id) {
    // TODO parametrize
    int packet_id = response->id;
    if (packet_id != expected_id) {
        printf(LOG_ERROR"parsing datagram id. Got (%d) expected(%d)\n", packet_id, expected_id);
        return false;
    }
    int status_code = response->status;
    if (status_code != 0) {
        printf(LOG_ERROR"status. Got (%d) expected(%d)\n", status_code, 0);
        return false;
    }
    return true;
}


void print_response_data(response_t *response) {
    printf("Response's packet id: %d\n", response->id);
    printf("Response status: %d\n", response->status);
}


void ntoh_on_response(response_t *response) {
    response->id = ntohs(response->id);
    response->status = ntohs(response->status);
}