#include <stdio.h>
#include <stdlib.h>

// Structure for customer details
struct Account
{
    int accNo;
    char name[20];
    char branch[20];
    float amount;
};

// Function declarations
int choiceMenu();
void showAccounts(FILE *fp);
void addData(FILE *fp);
void editData(FILE *fp);
void deleteData(FILE *fp);

int main()
{
    FILE *fp;
    int option;

    // Open file
    fp = fopen("bankrecord.dat", "rb+");

    // Create file if not exists
    if(fp == NULL)
    {
        fp = fopen("bankrecord.dat", "wb+");

        if(fp == NULL)
        {
            printf("Unable to open file!\n");
            exit(1);
        }
    }

    // Menu loop
    while((option = choiceMenu()) != 5)
    {
        switch(option)
        {
            case 1:
                showAccounts(fp);
                break;

            case 2:
                editData(fp);
                break;

            case 3:
                addData(fp);
                break;

            case 4:
                deleteData(fp);
                break;

            default:
                printf("Wrong Choice!\n");
        }
    }

    fclose(fp);

    return 0;
}

// Menu function
int choiceMenu()
{
    int ch;

    printf("\n\n===== BANK FILE SYSTEM =====");
    printf("\n1. Display All Accounts");
    printf("\n2. Update Account Balance");
    printf("\n3. Add New Account");
    printf("\n4. Delete Account");
    printf("\n5. Exit");
    printf("\nEnter your choice: ");

    scanf("%d", &ch);

    return ch;
}

// Display all accounts
void showAccounts(FILE *fp)
{
    struct Account a;

    rewind(fp);

    printf("\n-------------------------------------------");
    printf("\nAccNo\tName\t\tBranch\tBalance");
    printf("\n-------------------------------------------");

    while(fread(&a, sizeof(struct Account), 1, fp))
    {
        if(a.accNo != 0)
        {
            printf("\n%d\t%s\t\t%s\t%.2f",
                   a.accNo,
                   a.name,
                   a.branch,
                   a.amount);
        }
    }
}

// Add new account
void addData(FILE *fp)
{
    struct Account a;
    int no;

    printf("\nEnter New Account Number: ");
    scanf("%d", &no);

    fseek(fp, (no - 1) * sizeof(struct Account), SEEK_SET);

    fread(&a, sizeof(struct Account), 1, fp);

    if(a.accNo != 0)
    {
        printf("Account already exists!\n");
    }
    else
    {
        a.accNo = no;

        printf("Enter Customer Name: ");
        scanf("%s", a.name);

        printf("Enter Branch Name: ");
        scanf("%s", a.branch);

        printf("Enter Balance: ");
        scanf("%f", &a.amount);

        fseek(fp, (no - 1) * sizeof(struct Account), SEEK_SET);

        fwrite(&a, sizeof(struct Account), 1, fp);

        printf("New Account Added Successfully!\n");
    }
}

// Update account
void editData(FILE *fp)
{
    struct Account a;
    int no;
    float money;

    printf("\nEnter Account Number to Update: ");
    scanf("%d", &no);

    fseek(fp, (no - 1) * sizeof(struct Account), SEEK_SET);

    fread(&a, sizeof(struct Account), 1, fp);

    if(a.accNo == 0)
    {
        printf("Account not found!\n");
    }
    else
    {
        printf("\nCurrent Balance = %.2f", a.amount);

        printf("\nEnter Amount to Add/Withdraw: ");
        scanf("%f", &money);

        a.amount = a.amount + money;

        fseek(fp, -sizeof(struct Account), SEEK_CUR);

        fwrite(&a, sizeof(struct Account), 1, fp);

        printf("Balance Updated!\n");
    }
}

// Delete account
void deleteData(FILE *fp)
{
    struct Account a;
    struct Account empty = {0, "", "", 0};
    int no;

    printf("\nEnter Account Number to Delete: ");
    scanf("%d", &no);

    fseek(fp, (no - 1) * sizeof(struct Account), SEEK_SET);

    fread(&a, sizeof(struct Account), 1, fp);

    if(a.accNo == 0)
    {
        printf("Account does not exist!\n");
    }
    else
    {
        fseek(fp, (no - 1) * sizeof(struct Account), SEEK_SET);

        fwrite(&empty, sizeof(struct Account), 1, fp);

        printf("Account Deleted Successfully!\n");
    }
}
