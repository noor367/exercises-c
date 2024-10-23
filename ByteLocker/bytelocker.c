////////////////////////////////////////////////////////////////////////
// Assignment 2: `bytelocker', a simple file encryptor
//
// Written on 20-07-2022.
//
// 2022-07-22   v1.2  

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

#include "bytelocker.h"


// ADD ANY EXTRA #defines HERE


char *generate_random_string(int seed);
void sort_by_count(struct text_find *files, size_t n);
void sort_by_name(char *filenames[], size_t num_files);

// ADD YOUR FUNCTION PROTOTYPES HERE
void print_perms(char pathname[MAX_SEARCH_LENGTH]);
int check_group_perms(char filename[MAX_SEARCH_LENGTH]);
void encrypt_decrypt_xor(FILE *in_file, FILE *out_file);
int search_by_errors(char filename[MAX_SEARCH_LENGTH]);

// Some provided strings which you may find useful. Do not modify.
const char *const MSG_ERROR_FILE_STAT  = "Could not stat file\n";
const char *const MSG_ERROR_FILE_OPEN  = "Could not open file\n";
const char *const MSG_ERROR_CHANGE_DIR = "Could not change directory\n";
const char *const MSG_ERROR_DIRECTORY  =
    "%s cannot be encrypted: bytelocker does not support encrypting directories.\n";
const char *const MSG_ERROR_READ       =
    "%s cannot be encrypted: group does not have permission to read this file.\n";
const char *const MSG_ERROR_WRITE      =
    "%s cannot be encrypted: group does not have permission to write here.\n";
const char *const MSG_ERROR_SEARCH     = "Please enter a search string.\n";
const char *const MSG_ERROR_RESERVED   =
    "'.' and '..' are reserved filenames, please search for something else.\n";


//////////////////////////////////////////////
//                                          //
//              SUBSET 0                    //
//                                          //
//////////////////////////////////////////////

//
//  Read the file permissions of the current directory and print them to stdout.
//
void show_perms(char filename[MAX_PATH_LEN]) {
    
    struct stat s;

    if (stat(filename, &s) != 0) {
        printf("%s", MSG_ERROR_FILE_STAT);
    } else {
        printf("%s: ", filename);
        print_perms(filename);
        printf("\n");
    }

}

//
//  Prints current working directory to stdout.
//
void print_current_directory(void) {

    char pathname[MAX_PATH_LEN];
    
    if (getcwd(pathname, sizeof pathname) == NULL) {
        perror("getcwd");
    } else {
        printf("Current directory is: %s\n", pathname);
    }

}

//
//  Changes directory to the given path.
//
void change_directory(char dest_directory[MAX_PATH_LEN]) {

    if (dest_directory[0] == '~') {
        char *home_pathname = getenv("HOME");
        chdir(home_pathname);
        printf("Moving to %s\n", home_pathname);
    } else if (chdir(dest_directory) != 0) {
        printf("%s\n", MSG_ERROR_CHANGE_DIR);
    } else {
        printf("Moving to %s\n", dest_directory);
    }

}

//
//  Lists the contents of the current directory to stdout.
//
void list_current_directory(void) {

    char pathname[MAX_PATH_LEN];
    getcwd(pathname, sizeof pathname);

    DIR *dirprint = opendir(pathname);
    struct dirent *store;

    char *names_string[MAX_NUM_FINDS];
    int i = 0;
    int num_files = 0;

// Read file names and store into names_string array
    while ((store = readdir(dirprint)) != NULL) {
        names_string[i] =  malloc(sizeof(char[MAX_PATH_LEN]));
        strcpy(names_string[i], store->d_name);
        i++;
        num_files++;
    }

// Order the names_string array alphabetically and prints name and perms
    sort_by_name(names_string, MAX_NUM_FINDS);
    i = 0;
    
    while (i < num_files) {
        print_perms(names_string[i]);
        printf("\t%s\n", names_string[i]);
        i++;
    }
        
    closedir(dirprint);

}

//////////////////////////////////////////////
//                                          //
//              SUBSET 1                    //
//                                          //
//////////////////////////////////////////////

//
//  Tests if a provided file can be encrypted
//
bool test_can_encrypt(char filename[MAX_PATH_LEN]) {
    
    struct stat s;

    if (stat(filename, &s) != 0) {
        printf(MSG_ERROR_FILE_STAT);
        return 0;
    } else if (S_ISREG(s.st_mode) == 0) {
        printf(MSG_ERROR_DIRECTORY, filename);
        return 0;
    } else if (!(s.st_mode & S_IRUSR)) {
        printf(MSG_ERROR_READ, filename);
        return 0;
    }

// Check group perms of current directory or as specified by input
    if (check_group_perms(filename) == 0) {
        return 0;
    }
    
    return 1;

}

//
//  Creates an XOR encrypted file
//
void simple_xor_encryption(char filename[MAX_PATH_LEN]) {
    if (!test_can_encrypt(filename)) return;

// Append .xor to the end the file name
    char xor[5] = ".xor";
    char encrypted[MAX_PATH_LEN];
    strcpy(encrypted, filename);
    strcat(encrypted, xor);

    FILE *in_file = fopen(filename, "w+");
    FILE *out_file = fopen(encrypted, "w");

// Helper function to read and write to file
    encrypt_decrypt_xor(in_file, out_file);

    fclose(in_file);
    fclose(out_file);
    
}

//
//  Creates an XOR decrypted file
//
void simple_xor_decryption(char filename[MAX_PATH_LEN]) {
    if (!test_can_encrypt(filename)) return;

// Append .dec to the end of the file name
    char dec[5] = ".dec";
    char decrypted[MAX_PATH_LEN];
    strcpy(decrypted, filename);
    strcat(decrypted, dec);

    FILE *in_file = fopen(filename, "w+");
    FILE *out_file = fopen(decrypted, "w");

// Helper function to read and write to file
    encrypt_decrypt_xor(in_file, out_file);

    fclose(in_file);
    fclose(out_file);

}


//////////////////////////////////////////////
//                                          //
//              SUBSET 2                    //
//                                          //
//////////////////////////////////////////////
void search_by_filename(char filename[MAX_SEARCH_LENGTH]) {

    char pathname[MAX_PATH_LEN];
    getcwd(pathname, sizeof pathname);

    DIR *dirprint = opendir(pathname);
    struct dirent *current;

    char target[MAX_PATH_LEN];
    char *results[MAX_NUM_FINDS];

// Function to error check before reading the directory
    if (search_by_errors(filename)) {
        int i = 0;
        int num_files = 0;
        while((current = readdir(dirprint)) != NULL) {
            strcpy(target, current->d_name);
// Compare the read file name to the search name
            if (strcmp(target, filename) == 0) {
// If there is a match, save the file name to results array
                results[i] = malloc(sizeof(target));
                results[i] = target;
                num_files++;
            }
            i++;
        }
        i = 0;
// Order the file names alphabetically and prints name and perms
        sort_by_name(results, MAX_NUM_FINDS);
        while (i < num_files) {
            print_perms(results[i]);
            printf("\t%s\n", results[i]);
            i++;
        }
    }

}

void search_by_text(char text[MAX_SEARCH_LENGTH]) {

// Get current directory
    char pathname[MAX_PATH_LEN];
    getcwd(pathname, sizeof pathname);
    
    DIR *dirp = opendir(pathname);
    struct dirent *de;
    FILE *read;

    char *names_string[MAX_NUM_FINDS];
    char contents[MAX_LINE_LEN];
    int count = 0;
    int i = 0;

// Check if empty string
    if (search_by_errors(text)) {
// Read each file in the directory and open to check contents
        while ((de = readdir(dirp)) != NULL) {
            count = 0;
            read = fopen(de->d_name, "r");
            fgets(contents, MAX_LINE_LEN, read);
// Count how many times the text occurs
            while (strstr(contents, text) != NULL) {
                count++;
            }
// Add name of the file to names_string
            if (count > 0) {
                names_string[i] =  malloc(sizeof(char[MAX_PATH_LEN]));
                strcpy(names_string[i], de->d_name);
                i++;
            }
        }

    }

}


//////////////////////////////////////////////
//                                          //
//              SUBSET 3                    //
//                                          //
//////////////////////////////////////////////
void electronic_codebook_encryption(char filename[MAX_PATH_LEN], char password[CIPHER_BLOCK_SIZE + 1]) {
    if (!test_can_encrypt(filename)) return;
    
    printf("TODO: COMPLETE ME\n");
    exit(1);
}

void electronic_codebook_decryption(char filename[MAX_PATH_LEN], char password[CIPHER_BLOCK_SIZE + 1]) {
    if (!test_can_encrypt(filename)) return;

    printf("TODO: COMPLETE ME\n");
    exit(1);
}

char *shift_encrypt(char *plaintext, char *password) {
    printf("TODO: COMPLETE ME\n");
    exit(1);
    return NULL;
}

char *shift_decrypt(char *ciphertext, char *password) {
    printf("TODO: COMPLETE ME\n");
    exit(1);
    return NULL;
}


//////////////////////////////////////////////
//                                          //
//              SUBSET 4                    //
//                                          //
//////////////////////////////////////////////
void cyclic_block_shift_encryption(char filename[MAX_PATH_LEN], char password[CIPHER_BLOCK_SIZE + 1]) {
    if (!test_can_encrypt(filename)) return;
    
    printf("TODO: COMPLETE ME\n");
    exit(1);
}

void cyclic_block_shift_decryption(char filename[MAX_PATH_LEN], char password[CIPHER_BLOCK_SIZE + 1]) {
    if (!test_can_encrypt(filename)) return;
    
    printf("TODO: COMPLETE ME\n");
    exit(1);
}


// PROVIDED FUNCTIONS, DO NOT MODIFY

// Generates a random string of length RAND_STR_LEN.
// Requires a seed for the random number generator.
// The same seed will always generate the same string.
// The string contains only lowercase + uppercase letters,
// and digits 0 through 9.
// The string is returned in heap-allocated memory,
// and must be freed by the caller.
char *generate_random_string(int seed) {
    if (seed != 0) {
        srand(seed);
    }

    char *alpha_num_str =
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "0123456789";

    char *random_str = malloc(RAND_STR_LEN);

    for (int i = 0; i < RAND_STR_LEN; i++) {
        random_str[i] = alpha_num_str[rand() % (strlen(alpha_num_str) - 1)];
    }

    return random_str;
}

// Sorts the given array (in-place) of files with
// associated counts into descending order of counts.
// You must provide the size of the array as argument `n`.
void sort_by_count(struct text_find *files, size_t n) {
    if (n == 0 || n == 1) return;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (files[j].count < files[j + 1].count) {
                struct text_find temp = files[j];
                files[j] = files[j + 1];
                files[j + 1] = temp;
            } else if (files[j].count == files[j + 1].count && strcmp(files[j].path, files[j + 1].path) > 0) {
                struct text_find temp = files[j];
                files[j] = files[j + 1];
                files[j + 1] = temp;
            }
        }
    }
}

// Sorts the given array (in-place) of strings alphabetically.
// You must provide the size of the array as argument `n`.
void sort_by_name(char *filenames[], size_t num_filenames) {
    if (num_filenames == 0 || num_filenames == 1) return;
    for (int i = 0; i < num_filenames - 1; i++) {
        for (int j = 0; j < num_filenames - i - 1; j++) {
            if (strcmp(filenames[j], filenames[j + 1]) > 0) {
                char *temp = filenames[j];
                filenames[j] = filenames[j + 1];
                filenames[j + 1] = temp;
            }
        }
    }
}

// ADD YOUR FUNCTION DEFINITIONS HERE

// Print the file perms
void print_perms(char *pathname) {
    
    struct stat filestat;
    if (stat(pathname, &filestat) != 0) {
        perror(pathname);
        exit(1);
    }

    if (S_ISDIR(filestat.st_mode)) {
        printf("d");
    } else {
        printf("-");
    }

    // First trio rwx
    if (S_IRUSR & filestat.st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWUSR & filestat.st_mode) {
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXUSR & filestat.st_mode) {
        printf("x");
    } else {
        printf("-");
    }

    // Second trio rwx
    if (S_IRGRP & filestat.st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWGRP & filestat.st_mode) {
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXGRP & filestat.st_mode) {
        printf("x");
    } else {
        printf("-");
    }

    // Third trio rwx
    if (S_IROTH & filestat.st_mode) {
        printf("r");
    } else {
        printf("-");
    }
    if (S_IWOTH & filestat.st_mode) {
        printf("w");
    } else {
        printf("-");
    }
    if (S_IXOTH & filestat.st_mode) {
        printf("x");
    } else {
        printf("-");
    }

}

// Check the writing permissions for a file
int check_group_perms(char *filename) {
    
    struct stat s;
    char directory[MAX_PATH_LEN];
    
    char stop = '/';
    char dest[MAX_PATH_LEN];
    strcpy(dest, filename);
    strtok(dest, &stop);
    stat(dest, &s);
    if (!(s.st_mode & S_IWUSR)) {
        printf(MSG_ERROR_WRITE, filename);
        return 0;
    }

    getcwd(directory, sizeof directory);
    stat(directory, &s);
    if (!(s.st_mode & S_IWUSR)) {
        printf(MSG_ERROR_WRITE, filename);
        return 0;
    }

    return 1;

}

// XORs the provided files to encrypt or decrypt
void encrypt_decrypt_xor(FILE *in_file, FILE *out_file) {
    
    int c;
    while ((c = fgetc(in_file)) != EOF) {
        fputc(c ^ 0xFF, out_file);
    }

}


// Error checks the search_by_name function
int search_by_errors(char filename[MAX_SEARCH_LENGTH]) {

    if (strlen(filename) == 0) {
        printf(MSG_ERROR_SEARCH);
        return 0;
    } else if (filename[0] == '.') {
        printf(MSG_ERROR_RESERVED);
        return 0;
    } else {
        return 1;
    }

}