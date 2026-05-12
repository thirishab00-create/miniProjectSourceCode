// Bank-account program reads a random-access file sequentially,
// updates data already written to the file, creates new data to
// be placed in the file, and deletes data previously in the file.

#include <stdio.h>
#include <stdlib.h>

// clientData structure definition
struct clientData
{
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);

// NEW FEATURES
void searchRecord(FILE *fPtr);
void displayAllRecords(FILE *fPtr);

int main(int argc, char *argv[])
{
    FILE *cfPtr;
    unsigned int choice;

    // open file
    if ((cfPtr = fopen("credit.dat", "rb+")) == NULL)
    {
        printf("%s: File could not be opened.\n", argv[0]);
        exit(-1);
    }

    // menu loop
    while ((choice = enterChoice()) != 7)
    {
        switch (choice)
        {
        case 1:
            textFile(cfPtr);
            break;

        case 2:
            updateRecord(cfPtr);
            break;

        case 3:
            newRecord(cfPtr);
            break;

        case 4:
            deleteRecord(cfPtr);
            break;

        // NEW FEATURE
        case 5:
            searchRecord(cfPtr);
            break;

        // NEW FEATURE
        case 6:
            displayAllRecords(cfPtr);
            break;

        default:
            puts("Incorrect choice");
            break;
        }
    }

    fclose(cfPtr);
}

// create formatted text file
void textFile(FILE *readPtr)
{
    FILE *writePtr;
    int result;

    struct clientData client = {0, "", "", 0.0};

    if ((writePtr = fopen("accounts.txt", "w")) == NULL)
    {
        puts("File could not be opened.");
    }
    else
    {
        rewind(readPtr);

        fprintf(writePtr,
                "%-6s%-16s%-11s%10s\n",
                "Acct",
                "Last Name",
                "First Name",
                "Balance");

        while (!feof(readPtr))
        {
            result = fread(&client,
                           sizeof(struct clientData),
                           1,
                           readPtr);

            if (result != 0 && client.acctNum != 0)
            {
                fprintf(writePtr,
                        "%-6d%-16s%-11s%10.2f\n",
                        client.acctNum,
                        client.lastName,
                        client.firstName,
                        client.balance);
            }
        }

        fclose(writePtr);

        printf("accounts.txt created successfully.\n");
    }
}

// update balance
void updateRecord(FILE *fPtr)
{
    unsigned int account;
    double transaction;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account to update (1 - 100): ");
    scanf("%d", &account);

    fseek(fPtr,
          (account - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account #%d has no information.\n",
               account);
    }
    else
    {
        printf("%-6d%-16s%-11s%10.2f\n\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        printf("Enter charge (+) or payment (-): ");
        scanf("%lf", &transaction);

        client.balance += transaction;

        printf("%-6d%-16s%-11s%10.2f\n",
               client.acctNum,
               client.lastName,
               client.firstName,
               client.balance);

        fseek(fPtr,
              -sizeof(struct clientData),
              SEEK_CUR);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);
    }
}

// delete record
void deleteRecord(FILE *fPtr)
{
    struct clientData client;

    struct clientData blankClient = {0, "", "", 0};

    unsigned int accountNum;

    printf("Enter account number to delete (1 - 100): ");
    scanf("%d", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account %d does not exist.\n",
               accountNum);
    }
    else
    {
        fseek(fPtr,
              (accountNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&blankClient,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Account deleted successfully.\n");
    }
}

// create record
void newRecord(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    unsigned int accountNum;

    printf("Enter new account number (1 - 100): ");
    scanf("%d", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum != 0)
    {
        printf("Account #%d already contains information.\n",
               client.acctNum);
    }
    else
    {
        printf("Enter lastname firstname balance\n? ");

        scanf("%14s%9s%lf",
              client.lastName,
              client.firstName,
              &client.balance);

        client.acctNum = accountNum;

        fseek(fPtr,
              (client.acctNum - 1) * sizeof(struct clientData),
              SEEK_SET);

        fwrite(&client,
               sizeof(struct clientData),
               1,
               fPtr);

        printf("Record added successfully.\n");
    }
}

// NEW FEATURE - SEARCH ACCOUNT
void searchRecord(FILE *fPtr)
{
    unsigned int accountNum;

    struct clientData client = {0, "", "", 0.0};

    printf("Enter account number to search (1 - 100): ");
    scanf("%u", &accountNum);

    fseek(fPtr,
          (accountNum - 1) * sizeof(struct clientData),
          SEEK_SET);

    fread(&client,
          sizeof(struct clientData),
          1,
          fPtr);

    if (client.acctNum == 0)
    {
        printf("Account not found.\n");
    }
    else
    {
        printf("\nAccount Details\n");

        printf("Account Number : %u\n",
               client.acctNum);

        printf("Last Name      : %s\n",
               client.lastName);

        printf("First Name     : %s\n",
               client.firstName);

        printf("Balance        : %.2f\n",
               client.balance);
    }
}

// NEW FEATURE - DISPLAY ALL RECORDS
void displayAllRecords(FILE *fPtr)
{
    struct clientData client = {0, "", "", 0.0};

    rewind(fPtr);

    printf("\n%-10s %-15s %-15s %-10s\n",
           "Acct No",
           "Last Name",
           "First Name",
           "Balance");

    while (fread(&client,
                 sizeof(struct clientData),
                 1,
                 fPtr))
    {
        if (client.acctNum != 0)
        {
            printf("%-10u %-15s %-15s %-10.2f\n",
                   client.acctNum,
                   client.lastName,
                   client.firstName,
                   client.balance);
        }
    }
}

// menu
unsigned int enterChoice(void)
{
    unsigned int menuChoice;

    printf("\nEnter your choice\n"
           "1 - store formatted text file\n"
           "2 - update account\n"
           "3 - add new account\n"
           "4 - delete account\n"
           "5 - search account\n"
           "6 - display all accounts\n"
           "7 - end program\n? ");

    scanf("%u", &menuChoice);

    return menuChoice;
}