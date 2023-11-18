#define ERROR_INVALID_ARGC 1

int main(int argc, char *argv[]) {
    if (argc != 2) {
        exit(ERROR_INVALID_ARGC);
    }
    exit(0);
}