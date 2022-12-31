#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<string.h>

FILE *datas=NULL;
typedef struct {
    char name[50];
    char password[50];
    int score;
}players;

int match_password(char userName[],char userPassword[],players p[],int count)
{
    int i;
    for(i=0;i<count;i++)
    {
        if(!strcmp(userName,p[i].name))
        {
            if(!strcmp(userPassword,p[i].password))
            {
                //clrsrn();
                printf("welcome to the game !!!\n");
                return 1;
            }
        }
    }
    return 0;   
}

void load_data(players p[],int *count)
{
    int i=0;
    datas=fopen("data.csv","r");
    if(datas==NULL)
    {
        printf("unable to open file\n");
        exit(0);
    }
    ;
    
    while( fscanf(datas, "%[^,]%*c%[^,]%*c%d",p[i].name, p[i].password, &p[i].score) !=EOF)
    {
        i=i+1;
    }
    *count=i;
    fclose(datas);
}

void edit_data(players p[],int *count)
{
    int i;
    datas=fopen("data.csv","w");
    for(i=0;i<*count;i++)
    {
        fprintf(datas,"%s,%s,%d",p[i].name,p[i].password,p[i].score);
    }
}

void main()
{
    int choice,i,count,key;
    
    char userName[50],userPassword[50];
    //clrscr();
    players p[100];
    printf("1.Log in\n2.create a new account\n3.exit\n");
    printf("Enter your choice\n");
    scanf("%d",&choice);
    switch (choice)
    {
        case 1: 
                datas=fopen("data.csv","r");
                if(datas=NULL)
                {
                    printf("unable to open file\n");
                    exit(0);
                }
                load_data(p,&count);
                printf("%d\n",count);
                printf("Enter your name\n");
                scanf("%s",userName);
                printf("Enter your password\n");
                scanf("%s",userPassword);
                key=match_password(userName,userPassword,p,count);
                if(key==0)
                {
                    printf("Incorrect password or username\n");
                    return;
                }
                //clrsrn();
                printf("Welcome to the game!!!!\n");





                break;
    
        case 2: load_data(p,&count);
                printf("Enter your name\n");
                scanf("%s",p[count].name);
                printf("create a new password\n");
                scanf("%s",p[count].password);
                count++;
                edit_data(p,&count);
                
                

                break;
    }
}