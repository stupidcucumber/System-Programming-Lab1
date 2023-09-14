#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>

typedef struct
{
    int size;
    int vowels;
    char arr[31];
} word;


bool is_vowel(char vowel)
{
    unsigned char vowels[] = {'a', 'e', 'u', 'i', 'o', 'y'};

    for (int i = 0; i < 6; i++)
    {
        if ((unsigned char) tolower(vowel) == vowels[i])
        {
            return true;
        }
    }

    return false;
}


void append_letter(word* word, char letter)
{
    int index = word->size;

    word->arr[index] = letter;
    word->size = index + 1;
}


word* get_next_word(FILE *file_ptr)
{
    word *found = malloc(sizeof(word));
    char current_char;
    
    while (((current_char = fgetc(file_ptr)) != EOF) && isalpha((unsigned char) current_char))
    {
        append_letter(found, current_char);

        if (is_vowel(current_char))
        {
            found->vowels++;
        }
    }
    
    return found;
}


bool contains(word* search_word, FILE* file_ptr)
{

    long int pos = ftell(file_ptr);

    if (pos == 0)
    {
        return false;
    }
    
    fseek(file_ptr, 0, SEEK_SET);
    while (!feof(file_ptr))
    {
        word* temp_word = get_next_word(file_ptr);
        printf("%s", temp_word->arr);
        if (strncmp(search_word->arr, temp_word->arr, search_word->size) == 0)
        {
            return true;
        }
    }

    fseek(file_ptr, pos, SEEK_SET);
    return false;
}


bool file_exists(const char filename[])
{
    struct stat buffer;
    return stat(filename, &buffer) == 0 ? true : false;
}


int main(int argc, char *argv[]) 
{
    /*
        In the CLI of this program you can pass the names of the files. Example of usage:

            root@root$ ./main text.txt result.txt

        The command above will parse file with name 'text.txt' and write all output 'result.txt'.
    */
    
    const char* source_filename = argv[1];
    const char* result_filename = argv[2];

    FILE *source_ptr, *result_ptr;
    source_ptr = fopen(source_filename, "r");

    if (file_exists(result_filename))
    {
        printf("Filename with name %s already exists! Aborting...\n", result_filename);
        return -1;
    }
    else
    {
        result_ptr = fopen(result_filename, "w+");
    }

    if (source_ptr == NULL)
    {
        printf("Error while opening the file!\n");
        return -1;
    }

    while (feof(source_ptr) == 0)
    {
        word* current_word = get_next_word(source_ptr);
        printf("Current word is of size %d, the number of vowels is %d, the word itself is %s\n", 
                current_word->size, current_word->vowels, current_word->arr);

        if (current_word->size != 0 && current_word->size == current_word->vowels && !contains(current_word, result_ptr))
        {
            fputs(current_word->arr, result_ptr);
            fputs("\n", result_ptr);
        }
        
        free(current_word); // Freeing the memory to escape memory leak.
    }

    fclose(source_ptr);
    fclose(result_ptr);

    return 0;
}