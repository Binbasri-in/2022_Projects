#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// Structs
typedef struct _dna // tag name is _dna
{
    char name[20]; // name of the person
    int STR[8]; // STR counts
    //unsigned long long int counts_hash; // the combined counts (Try)
    struct _dna *next; // pointer to the next node
} dna; // name of the data structure

// Global variables
char *STRs[8] = {"AGATC","TTTTTTCT","AATG","TCTAG","GATA","TATC","GAAA","TCTG"};
int STRs_number = 8; // number of STRs used



// Function prototypes
void load_csv_dna(char *file, dna **data);
void print_dna(dna *data);
void add_dna_option(dna **data);
void find_match_option(dna *data);
void find_match_name(dna *data);
void display_database(dna *data);
void save_database(dna *data, char *filename);
void database_menu(char *filename);
dna* find_match(dna *current_dna, dna *test_dna);
void fast_track();
void no_database_menu();

// The main function
int main(int argc, char *argv[])
{
    int state = 0; // 0 for no database, 1 for database
    // check the command line inputs for any database
    if (argc == 2)
    {
        // choose the state of the program
        state = 1;
    }
    else
    {
        // choose the state of the program
        state = 0;
        printf("Check: ./DNA_STR help\nTo get information about the program\n");
    }

    // switch between the states
    switch(state)
    {
        // case 0: no database
        case 0: no_database_menu();
                return 0;
        // case 1: database
        case 1: 
        {
            if (strcmp(argv[1], "help") == 0)
            {
                printf("This program is used to compare DNA sequences\n");
                printf("To use the program, you need to create a database\n");
                printf("To create a database, run the program with no arguments\n");
                printf("To use the database, run the program with the database filename as an argument\n");
                printf("Example: ./DNA_STR database.csv");
                return 0;
            }
            database_menu(argv[1]);
            return 0;
        }
    }
}

// database menu
void database_menu(char *filename)
{
    // load the database
    dna *head = NULL;
    load_csv_dna(filename, &head);
    for(;;)
    {
        // print the menu
        printf("1. Add a new DNA sequence to the database\n");
        printf("2. Search for a DNA sequence in the database from a DNA\n");
        printf("3. Display the info of a given name\n");
        printf("4. Display the database\n");
        printf("5. Save and Exit\n");
        printf("Enter your choice: ");
        // get the choice
        int choice;
        scanf("%d", &choice);
        // switch between the choices
        switch(choice)
        {
            case 1: // add new sequence
            {
                add_dna_option(&head);
                break;
            }
            case 2: // search for a sequence
            {
                find_match_option(head);
                break;
            }
            case 3: // display the info of a given name
            {
                find_match_name(head);
                break;
            }
            case 4: // display the database
            {
                display_database(head);
                break;
            }
            case 5: // save and exit
            {
                save_database(head, filename);
                // free the allocated memory used for the linked list
                while (head != NULL)
                {
                    dna *temp = head;
                    head = head->next;
                    free(temp);
                }
                return;
            }
        }
    }

}

// no database menu
void no_database_menu()
{
    for(;;)
    {
        // print the menu
        printf("1. Create a new database\n");
        printf("2. Fast comparasion between DNAs files\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        // get the choice
        int choice;
        scanf("%d", &choice);
        // switch between the choices
        switch(choice)
        {
            case 1: // create a new database
            {
                // get the filename
                char filename[20];
                printf("Enter the filename: ");
                scanf("%s", filename);
                // create the file
                FILE *fp = fopen(filename, "w");
                // check if the file is created
                if (fp == NULL)
                {
                    printf("Error in creating the file\n");
                    return;
                }
                // write the first line
                fprintf(fp, "name,AGATC,TTTTTTCT,AATG,TCTAG,GATA,TATC,GAAA,TCTG\n");
                // close the file
                fclose(fp);
                // call the database menu
                database_menu(filename);
                return;
            }
            case 2: // call the fast track function
            {
                fast_track();
                return;
            }
            case 3: // exit
            {
                return;
            }
        }
    }
}

void load_csv_dna(char *file, dna **data)
{   
    int row = 0, column = 0, i = 0;
    //open the file
    FILE *fp = fopen(file, "r");
    //check if the file is opened
    if (!fp) {
        // Error in file opening
        printf("Can't open file\n");
        return;
    }
    //set a buffer for reading the file line by line until the end
    char buffer[500];
    // temporary variable for conversions
    char temp[10];

    for (;;) // you can specifie a limit by modifiying the loop
    {
        if (fscanf(fp, "%[^\n]%*c", buffer) == EOF) // read the line 
        {
            // get the account_no
            fclose(fp);
            return;// if the end of file is reached, close and return 1
        }

        // skip the first line
        if (row == 0)
        {
            row++;
            continue;
        }

        //split the line into tokens
        char *token = strtok(buffer, ",");
        column = 0;
        // create a new dna structure
        dna *new_dna = malloc(sizeof(dna));
        while (token) // read the tokens column by column
        {
            // get the name
            if (column == 0)
            {
                // copy the name
                strcpy(new_dna->name, token);
                
            }
            // get the STR counts
            else
            {
                // convert the string to int
                int count = atoi(token);
                // add the count to the dna
                new_dna->STR[column - 1] = count;
            }
            // get the next token
            token = strtok(NULL, ",");
            // increment the column
            column++;
        }
        // add the new dna to the database
        new_dna->next = *data;
        *data = new_dna;
        // print_dna(*data);
        // increment the row
        row++;
    }
    // print row for testing
    printf("number of row: %d\n", row);
    // close the file
    fclose(fp);
}

int STR_count_file(char STR[], char *file_path)
{
    // open the file in read mode
    FILE *file = fopen(file_path, "r");
    // check if the file is opened successfully
    if (file == NULL)
    {
        printf("Error opening the file\n");
        exit(10);
    }
    // get the length of the STR
    int STR_length = strlen(STR);
    // read the file letter by letter
    int count = 0;
    int max_count = 0;
    char current_letter = fgetc(file);

    while (current_letter != EOF)
    {
        // while the current letter is the same as the first letter of the STR
        while (current_letter == STR[0])
        {
            // check if the next letters are the same as the rest of the STR
            bool is_same = true;
            for (int i = 1; i < STR_length; i++)
            {
                // get the next letter
                current_letter = fgetc(file);
                if ( current_letter != STR[i])
                {
                    is_same = false;
                    break;
                }
            }
            // if the next letters are the same as the rest of the STR
            if (is_same)
            {
                // increase the count
                count++;
                
            }
            // if the next letters are not the same as the rest of the STR
            else
            {
                break;
            }
            // get the next letter
            current_letter = fgetc(file);
            
        }
        // if the count is bigger than the max count
        if (count > max_count)
        {
            // update the max count
            max_count = count;
        }
        // reset the count
        count = 0;
        // get the next letter
        current_letter = fgetc(file);
        
    }
    // close the file
    fclose(file);
    // return the max count
    return max_count;
}

int STR_count_string(char STR[], char *dna_sequence )
{
    // this function counts the number of an STR in a string
    // get the length of the STR
    int STR_length = strlen(STR);
    // read the string letter by letter
    int count = 0;
    int max_count = 0;
    char current_letter = dna_sequence[0];
    int index = 0;
    
    while (current_letter != '\0')
    {
        // while the current letter is the same as the first letter of the STR
        while (current_letter == STR[0])
        {
            // check if the next letters are the same as the rest of the STR
            bool is_same = true;
            for (int i = 1; i < STR_length; i++)
            {
                // get the next letter
                index += 1;
                current_letter = dna_sequence[index];
                if ( current_letter != STR[i])
                {
                    is_same = false;
                    break;
                }
            }
            // if the next letters are the same as the rest of the STR
            if (is_same)
            {
                // increase the count
                count++;
                
            }
            // if the next letters are not the same as the rest of the STR
            else
            {
                break;
            }
            // get the next letter
            index += 1;
            current_letter = dna_sequence[index];
            
        }
        // if the count is bigger than the max count
        if (count > max_count)
        {
            // update the max count
            max_count = count;
        }
        // reset the count
        count = 0;
        // get the next letter
        index += 1;
        current_letter = dna_sequence[index];
        
    }
    // return the max count
    return max_count;
}

void find_match_option(dna *data)
{
    // get the DNA sequence
    printf("Enter the DNA sequence if it's small or filename: ");
    char sequence[500];
    scanf("%s", sequence);
    // create a new dna structure
    dna *test_dna = malloc(sizeof(dna));
    // get the STR counts
    // if the sequence is a file
    if (strstr(sequence, ".txt") != NULL)
    {
        for (int i = 0; i < STRs_number; i++)
        {
            test_dna->STR[i] = STR_count_file(STRs[i], sequence);
        }
    }
    // if the sequence is a string
    else
    {
        for (int i = 0; i < STRs_number; i++)
        {
            test_dna->STR[i] = STR_count_string(STRs[i], sequence);
        }
    }
    // find the match
    dna *match = find_match(data, test_dna);
    // print the result
    if (match->name != NULL)
    {
        printf("The DNA sequence belongs to %s\n", match->name);
    }
    else
    {
        printf("The DNA sequence doesn't belong to anyone\n");
    }
}

dna* find_match(dna *current_dna, dna *test_dna)
{
    // iterate through the linked list
    // for each node compare the STR counts
    while (current_dna != NULL)
    {
        int match = 1;
        for (int i = 0; i < STRs_number; i++)
        {
            if (current_dna->STR[i] != test_dna->STR[i])
            {
                match = 0;
                break;
            }
        }
        // if there was any match return the node where the match happen 
        if (match == 1)
        {
            return current_dna;
        }
        current_dna = current_dna->next;
    }
    // if nothing was found return NULL
    return NULL;
}

void find_match_name(dna *data)
{
    // get the name of the person
    printf("Enter the name of the person: ");
    char name[20];
    scanf("%s", name);
    // search for the name
    dna *current = data;
    while (current != NULL)
    {
        if (strcmp(current->name, name) == 0)
        {
            // print the info
            printf("Name: %s\n", current->name);
            printf("STR counts: ");
            for (int i = 0; i < STRs_number; i++)
            {
                printf("%d ", current->STR[i]);
            }
            printf("\n");
            break;
        }
        current = current->next;
    }
    
}

void add_dna_option(dna **data)
{
    // get the name of the person
    printf("Enter the name of the person: ");
    char name[20];
    scanf("%s", name);
    // get the DNA sequence
    printf("Enter the DNA sequence if it's small or filename: ");
    char sequence[500];
    scanf("%s", sequence);
    // create a new dna structure
    dna *new_dna = malloc(sizeof(dna));
    // copy the name
    strcpy(new_dna->name, name);
    // get the STR counts
    // if the sequence is a file
    if (strstr(sequence, ".txt") != NULL)
    {
        for (int i = 0; i < STRs_number; i++)
        {
            new_dna->STR[i] = STR_count_file(STRs[i], sequence);
        }
    }
    // if the sequence is a string
    else
    {
        for (int i = 0; i < STRs_number; i++)
        {
            new_dna->STR[i] = STR_count_string(STRs[i], sequence);
        }
    }
    // add the new dna to the database
    new_dna->next = *data;
    *data = new_dna;
}

/*
void add_dna(dna **data, dna *item)
{
    // create a new node
    dna *new_dna = malloc(sizeof(dna));

    // copy the data
    strcpy(new_dna->name, item->name);
    for (int i = 0; i < STRs_number; i++)
    {
        new_dna->STR[i] = item->STR[i];
    }
    // add it to the beginning of the linked list
    // add it to the structure
    new_dna->next = *data;
    *data = new_dna;
    print_dna(*data);
}
*/

bool comp_two_str(dna *dna1, dna *dna2)
{
    // simple comparasion between the STR counts in each dna and returns false if there was any difference
    for (int i = 0; i < STRs_number; i++)
    {
        if (dna1->STR[i] != dna2->STR[i])
        {
            return false;
        }
    }
    return true;
}

void display_database(dna *data)
{
    int count = 0;
    // iterate through the linked list and print the info of each node
    while (data != NULL)
    {
        printf("Name: %s\n", data->name);
        printf("STR counts: ");
        for (int i = 0; i < STRs_number; i++)
        {
            printf("%d ", data->STR[i]);
        }
        printf("\n");
        data = data->next;
        count++;
    }
    // print the number of nodes in the database
    printf("There are %d people in the database\n", count);
}

void save_database(dna *data, char *filename)
{
    // save the current structure into a csv file with the same name of the given file if any
    // open the file
    FILE *file = fopen(filename, "w");
    // iterate through the linked list and print the info of each node
    while (data != NULL)
    {
        fprintf(file, "%s,", data->name);
        for (int i = 0; i < STRs_number; i++)
        {
            fprintf(file, "%d,", data->STR[i]);
        }
        fprintf(file, "\n");
        data = data->next;
    }
    // close the file
    fclose(file);

}

void fast_track()
{
    // get filename of the DNA sequence of the evidence found in crime scene
    printf("Enter the filename of the DNA sequence of the evidence found in crime scene: ");
    char filename[20];
    scanf("%s", filename);
    // create a dna for the evidence
    dna *evidence = malloc(sizeof(dna));
    // give it a name
    strcpy(evidence->name, "evidence");
    // get the STR counts
    for (int i = 0; i < STRs_number; i++)
    {
        evidence->STR[i] = STR_count_file(STRs[i], filename);
    }
    // print the evidence
    print_dna(evidence);

    // get number of suspects
    printf("Enter the number of suspects: ");
    int number_of_suspects;
    scanf("%d", &number_of_suspects);

    for (int i = 0; i < number_of_suspects; i++)
    {
        // get file name of the suspect
        printf("Enter the filename of the suspect: ");
        char filename[20];
        scanf("%s", filename);
        // create a dna for the suspect
        dna *suspect = malloc(sizeof(dna));
        // give it a name
        strcpy(suspect->name, "suspect");
        // get the STR counts
        for (int j = 0; j < STRs_number; j++)
        {
            suspect->STR[j] = STR_count_file(STRs[j], filename);
        }
        // print the suspect
        print_dna(suspect);
        // compare the two dna
        if (comp_two_str(evidence, suspect))
        {
            printf("Match found!\n");
            break;
        }
        // free the memory
        free(suspect);
    }
    // if no match found
    printf("No match found!\n");
    // free the memory
    free(evidence);
}

void print_dna(dna *data)
{
        // print the content of data
    printf("Name: %s\n", data->name);
    printf("STR counts: ");
    for (int i = 0; i < STRs_number; i++)
    {
        printf("%d ", data->STR[i]);
    }
    printf("\n");
}
 