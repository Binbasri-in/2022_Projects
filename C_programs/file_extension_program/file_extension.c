#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>

typedef struct node
{
    char *line_data[6];
    struct node *next;

}
node;

//prototypes
int help(void);
int add(void);
void get_search_word(char* file_name, char* extension);
int result(node* data, char* search_word);
void free_data(node* data);
void get_element(char* buffer, char* str, int start, int end);


int main(int argc, char* argv[])
{
    //check the user entered 2 command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./file_ex help\t  ----this is to see how to use this tool----\n");
        return 1;
    }
    //if he want help then explain how the program works
    if (strcmp(argv[1], "help") == 0)
    {
        int check0 = help();
        if (check0 == 0)
        {
            printf("something went wrong in help function\n");
            return 2;
        }
        else
        {
            return 0;
        }
    }
    // when the user want to add a new row in the file
    if (strcmp(argv[1], "add") == 0)
    {
        int check1 = add();
        if (check1 == 0)
        {
            printf("something went wrong in add function\n");
            return 3;
        }
        else
        {
            return 0;
        }
    }

    // read the data from the .csv file
    node* data = NULL;
    //open the file where the data already stored
    FILE *file = fopen("file_ext_data1.csv", "r");
    //set a buffer for reading the file line by line until the end
    char buffer[1000];
    while (true)
    {
        fscanf(file, "/n");
        if (fscanf(file, "%[^\n]%*c", buffer) == EOF)
        {
            break;
        }
        //split the line comma by comma and build the linked list
        node *n = (node*)malloc(sizeof(node));
        int n_column = 0;
        int start = 0;
        int end = 0;
        for (int i = 0,l = strlen(buffer); i < l+1; i++)
        {
            if (buffer[i] == ',' || i == l)
            {
                end = i - 1;
                n->line_data[n_column] = malloc((i + 2 - start) * sizeof(char));
                for (int j = 0; j < end+1-start; j++)
                {
                    n->line_data[n_column][j] = buffer[start+j];
                }
                start = i + 1;
                n_column++;
            }
        }
        n->next = data;
        data = n;
    }
    if (data == NULL)
    {
        printf("something went wrong in read_data function\n");
        return 4;
    }
    fclose(file);
    // use the provided file name and get the information of the extension printed out
    char search_word[11];
    get_search_word(argv[1], search_word);
    int check2 = result(data, search_word);
    if (check2 == 0)
    {
        printf("something went wrong in result function\n");
        return 5;
    }
    // free all the alocated space from memory
    while (data != NULL)
    {
        node* tmp = data->next;
        for (int i = 0; i < 6; i++)
        {
            free(data->line_data[i]);
        }
        free(data);
        data = tmp;
    }
    return 0;
}

void get_search_word(char* file_name, char* extension)
{
    int found = 0;
    int ext_counter = 0;
    for (int i = 0, l = strlen(file_name); i < l; i++)
    {
        if (file_name[i] == '.')
        {
            found = 1;
            continue;
        }
        if (found == 1)
        {
            extension[ext_counter] = file_name[i];
            ext_counter++;
        }
    }
}

int result(node* data, char* search_word)
{
    for (node* tmp = data; tmp != NULL; tmp = tmp->next)
    {
        if (strcasecmp(tmp->line_data[0], search_word) == 0)
        {
            printf("\nshort name: %s\n\n", tmp->line_data[0]);
            printf("full name: %s\n\n",tmp->line_data[1]);
            printf("genre: %s\n\n",tmp->line_data[2]);
            printf("Usage: %s\n\n",tmp->line_data[3]);
            printf("Operating systems: %s\n\n",tmp->line_data[4]);
            printf("for more information visit: %s\n\n",tmp->line_data[5]);
            return 1;
        }
    }
    printf("Not found in our database.\nfor more extensions visit: https://www.file-extensions.org/\n");
    return 1;
}

int add(void)
{
    //open the database
    FILE *file = fopen("file_ext_data1.csv", "a");
    if (file == NULL)
    {
        return 0;
    }
    //take the short name of the extension
    printf("short name without dot: ");
    char n_short[11];
    scanf("%s", n_short);
    printf("\n");
    //take the full name of the extension
    printf("The full name: ");
    char n_full[50];
    scanf("\n");
    scanf("%[^\n]%*c", n_full);
    printf("\n");
    //take the genre of the extension
    printf("The Genre: ");
    char genre[20];
    scanf("%s", genre);
    printf("\n");
    // take the Usage
    printf("The Usage: ");
    char usage[150];
    scanf("\n");
    scanf("%[^\n]%*c", usage);
    printf("\n");
    // take the os type
    printf("The Operating system: ");
    char o_s[20];
    scanf("%s", o_s);
    printf("\n");
    //take the link
    printf("The Link: ");
    char link[100];
    scanf("\n");
    scanf("%[^\n]%*c", link);
    printf("\n");

    //write all the collected data to the file
    fprintf(file, "\n%s,%s,%s,%s,%s,%s", n_short, n_full, genre, usage, o_s, link);
    fclose(file);
    return 1;
}

int help(void)
{
    printf("\nThis program is for detecting the extension at the end of a file provided from user.\n"
    "you can use it in two ways:\n1- if you want to add new extension to the database use: \"./file_extension add\" command-line.\n"
    "2- you can use it to have all the information available about this extension by using the command \"./file_extension <file_name>\"\n"
    "where file_name is the file that you want to see more information about it, like how to open it and how to use it\n"
    "all the information provided is from: https://www.file-extensions.org/\n"
    "Thank you for using my program :)\n\n");
    return 1;
}
