#include <stdio.h>
#include <stdlib.h>

//Ryan Schlosbon - COP3502C - Lab Assignment 9 - Hash Tables

// RecordType
struct RecordType
{
    int     id;
    char    name;
    int     order; 
};

//Node strucutre to be used with linked list implementation in hash table
struct Node
{
    struct RecordType record;
    struct Node* next;
};

// HashType
struct HashType
{
    struct Node* head; //creates a node pointer
};

// Compute the hash function
int hash(int x, int hashSz)
{
    return x % hashSz; //returns x % the hashSz input to find the appropriate index in hashtable
}

// Parses input file to an array of RecordType
int parseData(char* inputFileName, struct RecordType** ppData)
{
    FILE* inFile = fopen(inputFileName, "r");
    int dataSz = 0;
    int i, n;
    char c;
    struct RecordType *pRecord;
    *ppData = NULL;

    if (inFile)
    {
        fscanf(inFile, "%d\n", &dataSz);
        *ppData = (struct RecordType*)malloc(sizeof(struct RecordType) * dataSz);
        if (*ppData == NULL)
        {
            printf("Cannot allocate memory\n");
            exit(-1);
        }
        for (i = 0; i < dataSz; ++i)
        {
            pRecord = *ppData + i;
            fscanf(inFile, "%d ", &n);
            pRecord->id = n;
            fscanf(inFile, "%c ", &c);
            pRecord->name = c;
            fscanf(inFile, "%d ", &n);
            pRecord->order = n;
        }
        fclose(inFile);
    }

    return dataSz;
}

// Prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
    int i;
    printf("\nRecords:\n");
    for (i = 0; i < dataSz; ++i)
    {
        printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
    }
    printf("\n\n");
}

// Display records in the hash structure
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
    int i;
    struct Node* temp;

    for (i = 0; i < hashSz; ++i)
    {
        printf("Index %d -> ", i);
        temp = pHashArray[i].head;
        while (temp != NULL)
        {
            printf("%d %c %d", temp->record.id, temp->record.name, temp->record.order);
            if (temp->next != NULL)
            {
                printf(" -> ");
            }
            temp = temp->next;
        }
        printf("\n");
    }
}

//Inserts a record into the hash table using separate chaining
void insertRecord(struct HashType* pHashArray, int hashSz, struct RecordType record)
{
    int idx = hash(record.id, hashSz); //finds the index for the given record by using the hash function
    struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
    if (newNode == NULL) //checks if the memory was properly allocated
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    newNode->record = record;
    newNode->next = NULL;

    //if the head at the given idx for pHashArray is null
    if (pHashArray[idx].head == NULL)
    {
        pHashArray[idx].head = newNode; //set the head at the given index as newNode
    }
    else
    {
        //If not, insert it at the beginning of the linked list.
        newNode->next = pHashArray[idx].head;
        pHashArray[idx].head = newNode;
    }
}

int main(void)
{
    struct RecordType *pRecords;
    struct HashType *pHashArray;
    int recordSz = 0, i;

    recordSz = parseData("input.txt", &pRecords);
    printRecords(pRecords, recordSz);

    // Initialize the hash table
    int hashSz = recordSz / 2;
    pHashArray = (struct HashType*)malloc(sizeof(struct HashType) * hashSz); //declare a hash array
    if (pHashArray == NULL) //check if memory was succesfully allocated
    {
        printf("Cannot allocate memory\n");
        exit(-1);
    }
    for (i = 0; i < hashSz; ++i)
    {
        pHashArray[i].head = NULL; //set the head of each index as null
    }

    //Insert records into the hash table
    for (i = 0; i < recordSz; ++i)
    {
        insertRecord(pHashArray, hashSz, pRecords[i]); //insert a record at each hash index 
    }

    //Display the records within the hash table
    displayRecordsInHash(pHashArray, hashSz);

    //Free the memory
    for (i = 0; i < hashSz; ++i)
    {
        struct Node* temp = pHashArray[i].head;
        while (temp != NULL)
        {
            struct Node* prev = temp;
            temp = temp->next;
            free(prev);
        }
    }
    free(pRecords); //free the heads of both pRecord and pHashArray
    free(pHashArray);

    return 0;
}
