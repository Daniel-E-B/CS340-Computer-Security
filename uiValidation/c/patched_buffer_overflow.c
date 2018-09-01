// from http://stackoverflow.com/questions/34247068/buffer-overflow-does-not-work-on-mac-osx-el-capitan
#include <stdio.h>
#include <string.h>

int check_authentication(char *password) {
    int auth_flag = 0;
    char password_buffer[16];
    
    strncpy(password_buffer, password, sizeof(password_buffer));

    if (strcmp(password_buffer, "password") == 0) {
        auth_flag = 1;
    }

    return auth_flag;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <password>\n", argv[0]);
    }

    if (check_authentication(argv[1])) {
        printf("Access Granted.\n");
    } else {
        printf("Access Denied.\n");
    }
}
