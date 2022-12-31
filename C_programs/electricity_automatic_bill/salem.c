#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct account
{
    char name [25];
    int account_number;
    float amount; 
} account;

// prototypes
int read(char *file, account data[], int *account_no);
void display(account data[]);
void add_new_account(account data[], int *account_no);
void increase_balance(account data[]);
void decrease_balance(account data[]);
void export_database(char *file, account data[]);



int n = 0;//number of records

int main()
{
    //create an array of structs
    account data[100];

    //file path
    char fp[20] = "accounts.csv";

    // get account_no tracker
    int account_no = 1;

    // read the file and check if it is opened
    if (read(fp, data, &account_no) == 0)
    {
        printf("Error in opening file");
        return 0;
    }
    // menu for the user
    int choice;
    for(;;)
    {
        printf("1. Display all records\n");
        printf("2. Add new account user\n");
        printf("3. increase Balance of the account\n");
        printf("4. decrease Balance of the account\n");
        printf("5. Exit\n");
        printf("Your choice: ");
        scanf("%d", &choice);
        switch (choice)
        {
        case 1:
            display(data);
            break;
        case 2:
            add_new_account(data, &account_no);
            break;
        case 3:
            increase_balance(data);
            break;
        case 4:
            decrease_balance(data);
            break;
        case 5:
            export_database(fp, data);
            return 0;
        }
        
        
    }
    return 0
}

int read(char *file, account data[], int *account_no)
{   
    int row = 0, column = 0, i = 0;
    //open the file
    FILE *fp = fopen(file, "r");
    //check if the file is opened
    if (!fp) {
        // Error in file opening
        printf("Can't open file\n");
        return 0;
    }
    //set a buffer for reading the file line by line until the end
    char buffer[500];
    // temporary variable for conversions
    char temp[10];

    for (;;) // you can specifie a limit by modifiying the loop
    {
        if (fscanf(fp, "%%[^\n]*c", buffer) == EOF) // read the line 
        {
            // get the account_no
            *account_no = i != 0 ? data[i-1].account_number + 1 : 1;
            fclose(fp);
            return 1;// if the end of file is reached, close and return 1
        }

        // skip the first line
        if (row == 0)
        {
            row++;
            continue;
        }

        //split the line into tokens
        char *token = strtok(buffer, ", ");
        column = 0;
        while (token) // read the tokens column by column
        {
            //store the tokens in the struct
            switch (column)
            {
            case 0:
                strcpy(data[i].name, token);
                break;
            case 1:
                strcpy(temp, token);
                data[i].account_number = atoi(temp);
                break;
            case 2:
                strcpy(temp, token);
                data[i].amount = atof(temp);
                break;
            }
            token = strtok(NULL, ", "); // read the next token (column)
            column++; // increment the column
        }
        n++;//increment the number of records
        i++;//increment the index of the array
        row++;//increment the row
    }
}

// function to display all records
void display(account data[])
{
    int i;
    for(i=0;i<n;i++)
    {
        // for each record, print the name, account number and amount
        printf("%s, %d, %f\n",data[i].name,data[i].account_number,data[i].amount);
    }
}

// function to increase the balance of an account
void increase_balance(account data[])
{
    int i;
    int account_number;
    float amount;
    printf("Enter the account number: ");
    scanf("%d",&account_number);
    printf("Enter the amount to be added: ");
    scanf("%f",&amount);
    for(i=0;i<n;i++)
    {
        if(data[i].account_number == account_number)
        {
            data[i].amount += amount;
            printf("The new balance is: %f\n",data[i].amount);
            return;
        }
    }
    printf("Account not found\n");
}

// function to decrease the balance of an account
void decrease_balance(account data[])
{
    int i;
    int account_number;
    float amount;
    printf("Enter the account number: ");
    scanf("%d",&account_number);
    printf("Enter the amount to be deducted: ");
    scanf("%f",&amount);
    for(i=0;i<n;i++)
    {
        if(data[i].account_number == account_number)
        {
            data[i].amount -= amount;
            printf("The new balance is: %f\n",data[i].amount);
            return;
        }
    }
    printf("Account not found\n");
}

// function to add a new account
void add_new_account(account data[], int *account_no)
{
    int i;
    float amount;
    char name[25];
    printf("Enter the name: ");
    scanf("%s",name);
    printf("The account number is: %d\n", *account_no);
    printf("Enter the amount: ");
    scanf("%f",&amount);
    strcpy(data[n].name,name);
    data[n].account_number = *account_no;
    data[n].amount = amount;
    n++;
    *account_no = *account_no + 1;
    printf("Account added successfully\n");
}

// function to save the current data (database)
void export_database(char *file, account data[])
{
    int i;
    FILE *fp = fopen(file, "w");
    if (!fp) {
        // Error in file opening
        printf("Can't open file\n");
        return;
    }
    fprintf(fp, "name, account_number, amount\n");
    for (i = 0; i < n; i++)
    {
        fprintf(fp, "%s, %d, %f\n", data[i].name, data[i].account_number, data[i].amount);
    }
    fclose(fp);
}