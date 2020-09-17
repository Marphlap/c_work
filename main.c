#include <stdio.h>
#include <dirent.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

void main_menu();
void list_directory(const char* user_input_directory);
void list_all_directories(const char* user_input_directory);
void encrypt_decrypt();


int main() {

    long user_selection = 1;
    char user_input_directory[100];
    char user_input[100];
    char *ptr;



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
                break;

            case 2:
                list_directory(user_input_directory);
                break;

            case 3:
                list_all_directories(user_input_directory);
                break;

             case 6:
                encrypt_decrypt();
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



void encrypt_decrypt() {

    char password[256];
    char filename[100];
    FILE *input_file;
    FILE *output_file;

    printf("Please input a password.\n");
    fgets(password, sizeof(password), stdin);
    password[strcspn(password, "\n")] = 0;

    printf("Please input a filename.\n");
    fgets(filename, sizeof(filename), stdin);
    filename[strcspn(filename, "\n")] = 0;

    input_file = fopen(filename, "rb");
    output_file = fopen("C:\\Users\\drosarius\\CLionProjects\\file_system_hw5\\test5.pdf", "wb");

    int c;
    int i = 0;

    while((c = getc(input_file)) != EOF){

        c ^= password[i];
        putc(c,output_file);
        ++i;
        if ( i >= strlen(password) )   i = 0;
    }
    fclose(output_file);
    fclose(input_file);

}