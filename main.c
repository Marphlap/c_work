#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <unistd.h>
#include <errno.h>

void main_menu();
void list_directory(const char* user_input_directory);
void list_all_directories(const char* user_input_directory);
void encrypt_decrypt(int method, const char* user_input_directory);
void display_file(const char* user_input_directory);
void delete_file(const char* user_input_directory);

int main() {

    long user_selection = 1;
    char user_input_directory[100];
    char user_input[100];
    char *ptr;
    int method = 0; //  encrypt = 0, decrypt = 1
    DIR *selected_directory;



    while (user_selection != 0){
        main_menu();
        fgets(user_input, sizeof(user_input), stdin);
        user_selection = strtol(user_input, &ptr, 10);
        printf("You've selected option: %ld\n", user_selection);


        switch(user_selection){

            case 1:
                printf("Please select directory: ");
                fgets(user_input_directory, 100, stdin);
                user_input_directory[strcspn(user_input_directory, "\n")] = 0;
                selected_directory = opendir(user_input_directory);
                if (errno == 2){
                    printf("%s\n", strerror(errno));
                    strcpy(user_input_directory, "ASCII");
                    errno = 0;
                }
                break;

            case 2:
                if (strcmp(user_input_directory, "ASCII") == 0){
                    printf("Please input a directory\n");
                }
                else{
                    list_directory(user_input_directory);
                }
                break;

            case 3:
                if (strcmp(user_input_directory, "ASCII") == 0){
                    printf("Please input a directory\n");
                }
                else{
                    list_all_directories(user_input_directory);
                }

                break;

            case 4:
                if (strcmp(user_input_directory, "ASCII") == 0){
                    printf("Please input a directory\n");
                }
                else{
                    delete_file(user_input_directory);
                }

                break;

            case 5:
                if (strcmp(user_input_directory, "ASCII") == 0){
                    printf("Please input a directory\n");
                }
                else{
                    display_file(user_input_directory);
                }

                break;

            case 6:
                if (strcmp(user_input_directory, "ASCII") == 0){
                    printf("Please input a directory\n");
                }
                else{
                    method = 0;
                    encrypt_decrypt(method, user_input_directory);
                }

                break;

            case 7:
                if (strcmp(user_input_directory, "ASCII") == 0){
                    printf("Please input a directory\n");
                }
                else{
                    method = 1;
                    encrypt_decrypt(method, user_input_directory);
                }
                break;

            default:

                break;

        }

    }

    return 0;

}

void main_menu() {

    printf("0 – Exit\n");
    printf("1 – Select directory\n");
    printf("2 – List directory content (first level)\n");
    printf("3 – List directory content (all levels)\n");
    printf("4 – Delete file\n");
    printf("5 – Display file (hexadecimal view)\n");
    printf("6 – Encrypt file (XOR with password)\n");
    printf("7 – Decrypt file (XOR with password)\n");
    printf("Select option:\n");

}

void list_directory(const char* user_input_directory) {
    struct dirent *de;

    DIR *selected_directory;

    selected_directory = opendir(user_input_directory);

    while ((de = readdir(selected_directory)) != NULL){
            printf("%s\n", de->d_name);
    }
   closedir(selected_directory);

}

void list_all_directories(const char* user_input_directory) {
    struct dirent *de;

    DIR *selected_directory;

    selected_directory = opendir(user_input_directory);

    while ((de = readdir(selected_directory)) != NULL){
        if (de->d_type == DT_DIR) {
            char path[1024];
            if (strcmp(de->d_name, ".") == 0 || strcmp(de->d_name, "..") == 0)
                continue;
            snprintf(path, sizeof(path), "%s/%s", user_input_directory, de->d_name);
            printf("%s\n", de->d_name);
            list_all_directories(path);
        } else {
            printf("%s\n", de->d_name);
        }
    }
    closedir(selected_directory);
}



void encrypt_decrypt(int method, const char* user_input_directory) {

    char password[256];
    char filename[100];
    FILE *input_file;
    FILE *output_file;
    char full_filename[100];


    char output_file_name[100];

    printf("Please input a password.\n");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    printf("Please input a filename.\n");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    strcat(user_input_directory, "\\");
    strcpy(full_filename, user_input_directory);
    strcat(full_filename, filename);

    if (method == 0){
        strcpy(output_file_name, full_filename);
        strcat(output_file_name, ".enc");
    }
    else if (method == 1){
        strcpy(output_file_name, full_filename);
        strcat(output_file_name, ".dec");
    }
//Qwertyuiop[123$4$567]
    input_file = fopen(full_filename, "rb");
    output_file = fopen(output_file_name, "wb");

    int c;
    int i = 0;

    if (input_file){
        while((c = getc(input_file)) != EOF){

            c ^= password[i];
            putc(c,output_file);
            ++i;
            if ( i >= strlen(password) )   i = 0;
        }

    }
    else {
        printf("File Does Not Exist\n");
    }


    fclose(output_file);
    fclose(input_file);

}

void display_file(const char* user_input_directory){
    FILE *input_file;

    char filename[100];
    char full_filename[100];
    printf("Please input a filename.\n");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;
    strcat(user_input_directory, "\\");
    strcpy(full_filename, user_input_directory);
    strcat(full_filename, filename);


    input_file = fopen(full_filename, "r");
    if (input_file) {
        int c;

        while((c = getc(input_file)) != EOF){
            printf("Offset: 0x%04x Hex: %02x\n", ftell(input_file), c);

        }

        fclose(input_file);
    }
    else {
        printf("File Does Not Exist\n");
    }


}


void delete_file(const char* user_input_directory){

    char filename[100];
    char full_filename[100];

    printf("Please input a filename.\n");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    strcat(user_input_directory, "\\");
    strcpy(full_filename, user_input_directory);
    strcat(full_filename, filename);

    if (remove(full_filename) == 0){
        printf("Deleted File: %s Successfully\n", filename);
    }
    else {
        printf("Unable to delete File: %s\n", filename);
    }
}


