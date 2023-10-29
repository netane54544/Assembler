#include "Parser.h"
#include "Utils/Base64.h"
#include "Utils/DecimalToBinary.h"
#include "Utils/FileSystem.h"

#define SAVEBITS bits = saveToBits(bits, storeBits); /* saves our bits in the list */
#define BINOPSIZE 5
#define BITSSIZE 13
#define REGSIZE 5
#define REGISTERLABSIZE 10
#define CHECKNODE(head, x) (head->data->tokenType == x)
#define CHECKTOKEN(token, x) (token->tokenType == x)
#define CHECKFIRSTTYPE (CHECKNODE(current, mov) || CHECKNODE(current, cmp) || CHECKNODE(current, add) || CHECKNODE(current, sub) || CHECKNODE(current, lea))
#define CHECKSECANDTHIRDTYPE (CHECKNODE(current, not) || CHECKNODE(current, clr) || CHECKNODE(current, inc) || CHECKNODE(current, dec) || CHECKNODE(current, jmp) || CHECKNODE(current, bne) || CHECKNODE(current, red) || CHECKNODE(current, prn) || CHECKNODE(current, jsr) || CHECKNODE(current, rts) || CHECKNODE(current, stop))
#define ISREGISTER(x) (x->tokenType >= 16 && x->tokenType <= 23)

/* Saves the given data as a new node in the linked list of NodeParse. */
NodeParse* saveToBits(NodeParse* bits, char* data)
{
    char* storageContainer = NULL;

    storageContainer = malloc(BITSSIZE);   /* Allocate memory for a storage container to hold the data */
    if(storageContainer==NULL)
    {
        fprintf(stderr, "Error Allocating memory");
        exit(0);
    }
    strcpy(storageContainer, data); /* Copy the provided data into the storage container */

    if(bits == NULL)  /* If the linked list is empty (bits is NULL), create a new node with the data */
        return createNode_p(data);
    else /* If the linked list is not empty, add a new node with the data to the list */
        addNode_p(&bits, data);

    return bits;
}

void parseCommand(Token_Node* head, Simbols_Table* table, size_t* dataCounter, size_t* imageCounter)
{
    Token_Node* current = head; /* we don't want to change the original pointer */
    Token_Node* extTable = NULL; /* This tables store external labels (if there's any)*/
    /** We use memory1, memory2, memory3 to check the structure in assembly.
 * If need we use only memory1 and memory2. According to the assembly rules */
    Token* memory1 = NULL;
    Token* memory2 = NULL;
    Token* memory3 = NULL;
    NodeParse* bits = NULL;
#if DEBUG
    NodeParse* currentP = NULL; /* In case we need to print our results */
#endif
    char storeBits[BITSSIZE]; /* Stores the bits of each encoding of a token so we can store it in a list */
    char binaryOp[BINOPSIZE]; /* Stores the binary operator bits so we can save them */
    char registerOp[REGSIZE]; /* Stores the register operator bits so we can save them */
    char registerLable[REGISTERLABSIZE]; /* Stores the call for the label's bits so we can save them */

    while(current != NULL)
    {
        if(current->data->tokenType == label)
        {
            /* We don't encode labels, so we skip them */
            current = (Token_Node*)current->next;
            continue;
        }

        if(current != NULL && CHECKFIRSTTYPE) /* Check for commands that receive 2 operands and encode them properly */
        {
            /* Get the first word of the command */
            memory1 = (Token*)current->data;
            current = (Token_Node*)current->next;

            if(current != NULL && (ISREGISTER(current->data) ||  CHECKNODE(current, number) || CHECKNODE(current, labelCall)))
            {
                /* Get the second word of the command */
                memory2 = (Token*)current->data;
                current = (Token_Node*)current->next;

                if(current != NULL && (ISREGISTER(current->data) || CHECKNODE(current, number) || CHECKNODE(current, labelCall)))
                {
                    /* Get the third word of the command */
                    memory3 = (Token*)current->data;
                    current = (Token_Node*)current->next;
                    dec2bin(memory1->tokenType, binaryOp, BINOPSIZE, 0); /*encodes our operator to binary, so we can save the bits into the 12 bits word*/
                    (*imageCounter) += 3; /* Update the counter of the image */

                    if(ISREGISTER(memory2) && ISREGISTER(memory3)) /* Check if our two operands are registers */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '0';
                        storeBits[7] = '1';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '1';
                        storeBits[1] = '0';
                        storeBits[0] = '1';

                        SAVEBITS

                        /*Encodes the operands */
                        /*second operand*/
                        dec2bin(memory3->value[2] - '0', registerOp, REGSIZE, 0);
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = registerOp[0];
                        storeBits[8] = registerOp[1];
                        storeBits[7] = registerOp[2];
                        storeBits[6] = registerOp[3];
                        storeBits[5] = registerOp[4];

                        /*first operand*/
                        dec2bin(memory2->value[2] - '0', registerOp, REGSIZE, 0);
                        storeBits[4] = registerOp[0];
                        storeBits[3] = registerOp[1];
                        storeBits[2] = registerOp[2];
                        storeBits[1] = registerOp[3];
                        storeBits[0] = registerOp[4];

                        SAVEBITS

                        (*imageCounter)--; /*Two register use the same memory slot */
                    }
                    else if(CHECKTOKEN(memory1, cmp) && ISREGISTER(memory2) && CHECKTOKEN(memory3, number)) /* cmp has it's own logic and can also accept register as the 1 operand and a number as the 2 operand */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '0';
                        storeBits[7] = '0';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '1';
                        storeBits[1] = '0';
                        storeBits[0] = '1';

                        SAVEBITS

                        /*Encodes the operands */
                        /* 1th operand*/
                        dec2bin(memory2->value[2] - '0', registerOp, REGSIZE, 0);
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '0';
                        storeBits[8] = '0';
                        storeBits[7] = '0';
                        storeBits[6] = '0';
                        storeBits[5] = '0';
                        storeBits[4] = registerOp[0];
                        storeBits[3] = registerOp[1];
                        storeBits[2] = registerOp[2];
                        storeBits[1] = registerOp[3];
                        storeBits[0] = registerOp[4];

                        SAVEBITS

                        /*second operand*/
                        dec2bin(atoi(memory3->value), storeBits, BITSSIZE, 2);

                        SAVEBITS
                    }
                    else if(ISREGISTER(memory3) && CHECKTOKEN(memory2, number)) /* Checks if the first operand is a number and the second is a register */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '0';
                        storeBits[7] = '1';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '1';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS
                        /*Encodes the operands */
                        /*1th operand*/
                        dec2bin(atoi(memory2->value), storeBits, BITSSIZE, 2);

                        SAVEBITS

                        /*second operand*/
                        dec2bin(memory3->value[2] - '0', registerOp, REGSIZE, 0);
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = registerOp[0];
                        storeBits[8] = registerOp[1];
                        storeBits[7] = registerOp[2];
                        storeBits[6] = registerOp[3];
                        storeBits[5] = registerOp[4];
                        storeBits[4] = '0';
                        storeBits[3] = '0';
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS
                    }
                    else if(CHECKTOKEN(memory2, labelCall) && ISREGISTER(memory3)) /* Checks if the first operand is a label call and the second is a register */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '0';
                        storeBits[7] = '1';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '1';
                        storeBits[1] = '1';
                        storeBits[0] = '0';

                        SAVEBITS

                        /*Encodes the operands */
                        /*Checks the flag of the label if the flag is 2, then the label is extern and we need to encode it differently*/
                        /*first operand*/
                        if(findInTable_s(table, memory2->value)->flag == 2)
                        {
                            /* Extern label */
                            storeBits[12] = '\0';
                            storeBits[11] = '1';
                            storeBits[10] = '0';
                            storeBits[9] = '0';
                            storeBits[8] = '0';
                            storeBits[7] = '0';
                            storeBits[6] = '0';
                            storeBits[5] = '0';
                            storeBits[4] = '0';
                            storeBits[3] = '0';
                            storeBits[2] = '0';
                            storeBits[1] = '0';
                            storeBits[0] = '0';

                            addNode_t(&extTable, memory2); /* Add the extern label to the list, so we will be able to save it later */
                        }
                        else
                        {
                            /* Entry or regular label */
                            dec2bin(findInTable_s(table, memory2->value)->line, registerLable, REGISTERLABSIZE, 0);

                            storeBits[12] = '\0';
                            storeBits[11] = '0';
                            storeBits[10] = '1';
                            storeBits[9] = registerLable[0];
                            storeBits[8] = registerLable[1];
                            storeBits[7] = registerLable[2];
                            storeBits[6] = registerLable[3];
                            storeBits[5] = registerLable[4];
                            storeBits[4] = registerLable[5];
                            storeBits[3] = registerLable[6];
                            storeBits[2] = registerLable[7];
                            storeBits[1] = registerLable[8];
                            storeBits[0] = registerLable[9];
                        }

                        SAVEBITS

                        /*second operand*/
                        dec2bin(memory3->value[2] - '0', registerOp, REGSIZE, 0);
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = registerOp[0];
                        storeBits[8] = registerOp[1];
                        storeBits[7] = registerOp[2];
                        storeBits[6] = registerOp[3];
                        storeBits[5] = registerOp[4];
                        storeBits[4] = '0';
                        storeBits[3] = '0';
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS
                    }
                    else if(CHECKTOKEN(memory2, number) && CHECKTOKEN(memory3, labelCall)) /* Checks if the first operand is a number and the second is a call to a label */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '1';
                        storeBits[7] = '0';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '1';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS

                        /*Encodes the operands */
                        /*1th operand*/
                        dec2bin(atoi(memory2->value), storeBits, BITSSIZE, 2);

                        SAVEBITS

                        /*Checks the flag of the label if the flag is 2, then the label is extern and we need to encode it differently*/
                        /*second operand*/
                        if(findInTable_s(table, memory3->value)->flag == 2)
                        {
                            /* Extern label */
                            storeBits[12] = '\0';
                            storeBits[11] = '1';
                            storeBits[10] = '0';
                            storeBits[9] = '0';
                            storeBits[8] = '0';
                            storeBits[7] = '0';
                            storeBits[6] = '0';
                            storeBits[5] = '0';
                            storeBits[4] = '0';
                            storeBits[3] = '0';
                            storeBits[2] = '0';
                            storeBits[1] = '0';
                            storeBits[0] = '0';

                            addNode_t(&extTable, memory3); /* Add the extern label to the list, so we will be able to save it later */
                        }
                        else
                        {
                            /* Entry or regular label */
                            dec2bin(findInTable_s(table, memory3->value)->line, registerLable, REGISTERLABSIZE, 0);

                            storeBits[12] = '\0';
                            storeBits[11] = '0';
                            storeBits[10] = '1';
                            storeBits[9] = registerLable[0];
                            storeBits[8] = registerLable[1];
                            storeBits[7] = registerLable[2];
                            storeBits[6] = registerLable[3];
                            storeBits[5] = registerLable[4];
                            storeBits[4] = registerLable[5];
                            storeBits[3] = registerLable[6];
                            storeBits[2] = registerLable[7];
                            storeBits[1] = registerLable[8];
                            storeBits[0] = registerLable[9];
                        }

                        SAVEBITS
                    }
                    else if(CHECKTOKEN(memory3, labelCall) && ISREGISTER(memory2)) /* Checks if the first operand is a register and the second operand is a call to a label */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '1';
                        storeBits[7] = '0';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '1';
                        storeBits[1] = '0';
                        storeBits[0] = '1';

                        SAVEBITS

                        /*Encodes the operands */
                        /*first operand*/
                        dec2bin(memory2->value[2] - '0', registerOp, REGSIZE, 0);
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '0';
                        storeBits[8] = '0';
                        storeBits[7] = '0';
                        storeBits[6] = '0';
                        storeBits[5] = '0';
                        storeBits[4] = registerOp[0];
                        storeBits[3] = registerOp[1];
                        storeBits[2] = registerOp[2];
                        storeBits[1] = registerOp[3];
                        storeBits[0] = registerOp[4];

                        SAVEBITS
                        /*Checks the flag of the label if the flag is 2, then the label is extern and we need to encode it differently*/
                        /*second operand*/
                        if(findInTable_s(table, memory3->value)->flag == 2)
                        {
                            /* Extern label */
                            storeBits[12] = '\0';
                            storeBits[11] = '1';
                            storeBits[10] = '0';
                            storeBits[9] = '0';
                            storeBits[8] = '0';
                            storeBits[7] = '0';
                            storeBits[6] = '0';
                            storeBits[5] = '0';
                            storeBits[4] = '0';
                            storeBits[3] = '0';
                            storeBits[2] = '0';
                            storeBits[1] = '0';
                            storeBits[0] = '0';

                            addNode_t(&extTable, memory3); /* Add the extern label to the list, so we will be able to save it later */
                        }
                        else
                        {
                            /* Entry or regular label */
                            dec2bin(findInTable_s(table, memory3->value)->line, registerLable, REGISTERLABSIZE, 0);

                            storeBits[12] = '\0';
                            storeBits[11] = '0';
                            storeBits[10] = '1';
                            storeBits[9] = registerLable[0];
                            storeBits[8] = registerLable[1];
                            storeBits[7] = registerLable[2];
                            storeBits[6] = registerLable[3];
                            storeBits[5] = registerLable[4];
                            storeBits[4] = registerLable[5];
                            storeBits[3] = registerLable[6];
                            storeBits[2] = registerLable[7];
                            storeBits[1] = registerLable[8];
                            storeBits[0] = registerLable[9];
                        }

                        SAVEBITS
                    }
                    else if(CHECKTOKEN(memory2, labelCall) && CHECKTOKEN(memory3, labelCall)) /* Check if our two operands are calls to a label */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '1';
                        storeBits[7] = '0';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '1';
                        storeBits[1] = '1';
                        storeBits[0] = '0';

                        SAVEBITS

                        /*Encodes the operands */
                        /*Checks the flag of the label if the flag is 2, then the label is extern and we need to encode it differently*/
                        /*1th operand*/
                        if(findInTable_s(table, memory2->value)->flag == 2)
                        {
                            /* Extern label */
                            storeBits[12] = '\0';
                            storeBits[11] = '1';
                            storeBits[10] = '0';
                            storeBits[9] = '0';
                            storeBits[8] = '0';
                            storeBits[7] = '0';
                            storeBits[6] = '0';
                            storeBits[5] = '0';
                            storeBits[4] = '0';
                            storeBits[3] = '0';
                            storeBits[2] = '0';
                            storeBits[1] = '0';
                            storeBits[0] = '0';

                            addNode_t(&extTable, memory2); /* Add the extern label to the list, so we will be able to save it later */
                        }
                        else
                        {
                            /* Entry or regular label */
                            dec2bin(findInTable_s(table, memory2->value)->line, registerLable, REGISTERLABSIZE, 0);

                            storeBits[12] = '\0';
                            storeBits[11] = '0';
                            storeBits[10] = '1';
                            storeBits[9] = registerLable[0];
                            storeBits[8] = registerLable[1];
                            storeBits[7] = registerLable[2];
                            storeBits[6] = registerLable[3];
                            storeBits[5] = registerLable[4];
                            storeBits[4] = registerLable[5];
                            storeBits[3] = registerLable[6];
                            storeBits[2] = registerLable[7];
                            storeBits[1] = registerLable[8];
                            storeBits[0] = registerLable[9];
                        }

                        bits = saveToBits(bits, storeBits);

                        /*Checks the flag of the label if the flag is 2, then the label is extern and we need to encode it differently*/
                        /*second operand*/
                        if(findInTable_s(table, memory3->value)->flag == 2)
                        {
                            /* Extern label */
                            storeBits[12] = '\0';
                            storeBits[11] = '1';
                            storeBits[10] = '0';
                            storeBits[9] = '0';
                            storeBits[8] = '0';
                            storeBits[7] = '0';
                            storeBits[6] = '0';
                            storeBits[5] = '0';
                            storeBits[4] = '0';
                            storeBits[3] = '0';
                            storeBits[2] = '0';
                            storeBits[1] = '0';
                            storeBits[0] = '0';

                            addNode_t(&extTable, memory3); /* Add the extern label to the list, so we will be able to save it later */
                        }
                        else
                        {
                            /* Entry or regular label */
                            dec2bin(findInTable_s(table, memory3->value)->line, registerLable, REGISTERLABSIZE, 0);

                            storeBits[12] = '\0';
                            storeBits[11] = '0';
                            storeBits[10] = '1';
                            storeBits[9] = registerLable[0];
                            storeBits[8] = registerLable[1];
                            storeBits[7] = registerLable[2];
                            storeBits[6] = registerLable[3];
                            storeBits[5] = registerLable[4];
                            storeBits[4] = registerLable[5];
                            storeBits[3] = registerLable[6];
                            storeBits[2] = registerLable[7];
                            storeBits[1] = registerLable[8];
                            storeBits[0] = registerLable[9];
                        }

                        SAVEBITS
                    }
                    else if(!(CHECKTOKEN(memory1, cmp)) && ISREGISTER(memory2) && CHECKTOKEN(memory3, number)) /* cmp doesn't allow a register as it's first operand and a numbers as it's second operand */
                    {
                        fprintf(stderr, "%d: Error! %s only accept data types as it's destination operand\n", memory2->lineNumber, memory1->value);
                        foundError = true;
                        continue;
                    }
                    else if(CHECKTOKEN(memory1, lea) && (CHECKTOKEN(memory2, number) || CHECKTOKEN(memory3, number)))
                    {
                        /* lea has it's own logic and doesn't accept any number */
                        if(CHECKTOKEN(memory2, number) && !CHECKTOKEN(memory3, number))
                            fprintf(stderr, "%d: Error! in lea you are not allowed to use numbers as data\n", memory2->lineNumber);
                        else if(!CHECKTOKEN(memory2, number) && CHECKTOKEN(memory3, number))
                            fprintf(stderr, "%d: Error! in lea you are not allowed to use numbers as data\n", memory3->lineNumber);
                        else
                            fprintf(stderr, "%d-%d: Error! in lea you are not allowed to use numbers as data\n", memory2->lineNumber, memory3->lineNumber);

                        foundError = true;
                        continue; /* We need to find all the errors */
                    }
                    else if(!(CHECKTOKEN(memory1, cmp)) && CHECKTOKEN(memory3, number)) /* only cmp can use number as the destination operator */
                    {
                        fprintf(stderr, "%d: Error! You are not allowed to use a number as the destination operator\n", memory3->lineNumber);
                        current = (Token_Node*)current->next; /* We need to manually move to the next token */
                        foundError = true;
                        continue; /* We need to find all the errors */
                    }
                    else
                    {
                        fprintf(stderr, "%d: Error! You %s is not a valid statement\n", current->data->lineNumber, current->data->value);
                        current = (Token_Node*)current->next; /* We need to manually move to the next token */
                        foundError = true;
                        continue; /* We need to find all the errors */
                    }
                }
                else
                {
                    fprintf(stderr, "%d: Error! You %s is not a valid statement\n", current->data->lineNumber, current->data->value);
                    current = (Token_Node*)current->next; /* We need to manually move to the next token */
                    foundError = true;
                    continue;
                }
            }
            else
            {
                fprintf(stderr, "%d: Error! You %s is not a valid statement\n", current->data->lineNumber, current->data->value);
                current = (Token_Node*)current->next; /* We need to manually move to the next token */
                foundError = true;
                continue; /* We need to find all the errors */
            }
        }
        else if(current != NULL && CHECKSECANDTHIRDTYPE) /* Checks if our commands are the type with only a destination operator */
        {
            memory1 = (Token*)current->data; /* Our command */
            current = (Token_Node*)current->next;
            dec2bin(memory1->tokenType, binaryOp, BINOPSIZE, 0); /*encodes our operator to binary, so we can save the bits into the 12 bits word*/

            if(CHECKTOKEN(memory1, rts) || CHECKTOKEN(memory1, stop)) /* Those commands don't use and operands */
            {
                /* Encode the command */
                storeBits[12] = '\0';
                storeBits[11] = '0';
                storeBits[10] = '0';
                storeBits[9] = '0';
                storeBits[8] = '0';
                storeBits[7] = '0';
                storeBits[6] = binaryOp[0];
                storeBits[5] = binaryOp[1];
                storeBits[4] = binaryOp[2];
                storeBits[3] = binaryOp[3];
                storeBits[2] = '0';
                storeBits[1] = '0';
                storeBits[0] = '0';

                SAVEBITS
                (*imageCounter)++;/* Update the counter of the image */
            }
            else if(current != NULL)
            {
                memory2 = (Token*)current->data; /* Destination operands */
                current = (Token_Node*)current->next;
                memory3 = NULL;

                if(CHECKTOKEN(memory1, prn)) /* prn can accept all the data types, so we need a special logic */
                {
                    (*imageCounter) += 2;/* Update the counter of the image */

                    if(CHECKTOKEN(memory2, number)) /* The destination operand is a number */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '0';
                        storeBits[7] = '0';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS
                        /*Encodes the operands */
                        dec2bin(atoi(memory2->value), storeBits, BITSSIZE, 2);

                        SAVEBITS
                    }
                    else if(CHECKTOKEN(memory2, labelCall)) /* The destination operand is a label */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '1';
                        storeBits[7] = '0';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS

                        /*Encodes the operands */
                        /*Checks the flag of the label if the flag is 2, then the label is extern and we need to encode it differently*/
                        if(findInTable_s(table, memory2->value)->flag == 2)
                        {
                            /* Extern label */
                            storeBits[12] = '\0';
                            storeBits[11] = '1';
                            storeBits[10] = '0';
                            storeBits[9] = '0';
                            storeBits[8] = '0';
                            storeBits[7] = '0';
                            storeBits[6] = '0';
                            storeBits[5] = '0';
                            storeBits[4] = '0';
                            storeBits[3] = '0';
                            storeBits[2] = '0';
                            storeBits[1] = '0';
                            storeBits[0] = '0';

                            addNode_t(&extTable, memory2); /* Add the extern label to the list, so we will be able to save it later */
                        }
                        else
                        {
                            /* Entry or regular label */
                            dec2bin(findInTable_s(table, memory2->value)->line, registerLable, REGISTERLABSIZE, 0);

                            storeBits[12] = '\0';
                            storeBits[11] = '0';
                            storeBits[10] = '1';
                            storeBits[9] = registerLable[0];
                            storeBits[8] = registerLable[1];
                            storeBits[7] = registerLable[2];
                            storeBits[6] = registerLable[3];
                            storeBits[5] = registerLable[4];
                            storeBits[4] = registerLable[5];
                            storeBits[3] = registerLable[6];
                            storeBits[2] = registerLable[7];
                            storeBits[1] = registerLable[8];
                            storeBits[0] = registerLable[9];
                        }

                        SAVEBITS
                    }
                    else if(ISREGISTER(memory2)) /* The destination operand is a register */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '0';
                        storeBits[7] = '1';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS

                        /*Encodes the operands */
                        dec2bin(memory2->value[2] - '0', registerOp, REGSIZE, 0);
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = registerOp[0];
                        storeBits[8] = registerOp[1];
                        storeBits[7] = registerOp[2];
                        storeBits[6] = registerOp[3];
                        storeBits[5] = registerOp[4];
                        storeBits[4] = '0';
                        storeBits[3] = '0';
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS
                    }
                }
                else
                {
                    /* if the command is not prn in the commands with only a destination operand */
                    (*imageCounter) += 2;/* Update the counter of the image */

                    if(CHECKTOKEN(memory2, labelCall)) /* The destination operand is a label call */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '1';
                        storeBits[7] = '0';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS

                        /*Encodes the operands */
                        /*Checks the flag of the label if the flag is 2, then the label is extern and we need to encode it differently*/
                        if(findInTable_s(table, memory2->value)->flag == 2)
                        {
                            /* Extern label */
                            storeBits[12] = '\0';
                            storeBits[11] = '1';
                            storeBits[10] = '0';
                            storeBits[9] = '0';
                            storeBits[8] = '0';
                            storeBits[7] = '0';
                            storeBits[6] = '0';
                            storeBits[5] = '0';
                            storeBits[4] = '0';
                            storeBits[3] = '0';
                            storeBits[2] = '0';
                            storeBits[1] = '0';
                            storeBits[0] = '0';

                            addNode_t(&extTable, memory2); /* Add the extern label to the list, so we will be able to save it later */
                        }
                        else
                        {
                            /* Entry or regular label */
                            dec2bin(findInTable_s(table, memory2->value)->line, registerLable, REGISTERLABSIZE, 0);

                            storeBits[12] = '\0';
                            storeBits[11] = '0';
                            storeBits[10] = '1';
                            storeBits[9] = registerLable[0];
                            storeBits[8] = registerLable[1];
                            storeBits[7] = registerLable[2];
                            storeBits[6] = registerLable[3];
                            storeBits[5] = registerLable[4];
                            storeBits[4] = registerLable[5];
                            storeBits[3] = registerLable[6];
                            storeBits[2] = registerLable[7];
                            storeBits[1] = registerLable[8];
                            storeBits[0] = registerLable[9];
                        }

                        SAVEBITS
                    }
                    else if(ISREGISTER(memory2)) /* The destination operand is a register */
                    {
                        /* Encode the command */
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = '1';
                        storeBits[8] = '0';
                        storeBits[7] = '1';
                        storeBits[6] = binaryOp[0];
                        storeBits[5] = binaryOp[1];
                        storeBits[4] = binaryOp[2];
                        storeBits[3] = binaryOp[3];
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS

                        /*Encodes the operands */
                        dec2bin(memory2->value[2] - '0', registerOp, REGSIZE, 0);
                        storeBits[12] = '\0';
                        storeBits[11] = '0';
                        storeBits[10] = '0';
                        storeBits[9] = registerOp[0];
                        storeBits[8] = registerOp[1];
                        storeBits[7] = registerOp[2];
                        storeBits[6] = registerOp[3];
                        storeBits[5] = registerOp[4];
                        storeBits[4] = '0';
                        storeBits[3] = '0';
                        storeBits[2] = '0';
                        storeBits[1] = '0';
                        storeBits[0] = '0';

                        SAVEBITS
                    }
                    else
                    {
                        /* Only prn can use a number in the commands with only a destination operand */
                        fprintf(stderr, "%d: Error! You are not allowed to use a number in the operation %s\n", memory2->lineNumber, memory1->value);
                        foundError = true;
                        current = (Token_Node*)current->next; /* We need to manually move to the next token */
                        continue; /* We need to find all the errors */
                    }

                }
            }
            else
            {
                fprintf(stderr, "%d: Error! You are not allowed to use %s without adding the destination operand\n", memory2->lineNumber, memory1->value);
                foundError = true;
                continue; /* We need to find all the errors */
            }
        }
        else if(current != NULL && CHECKNODE(current, data)) /* if we find ".data" we need a special if to process all the data that after the call */
        {
            current = (Token_Node*)current->next;
            memory2 = NULL;
            memory3 = NULL;

            /*if there is noting after .data*/
            /* we don't allow empty data label */
            if(current == NULL && !CHECKNODE(current, number))
            {
                fprintf(stderr, "%d: Error! Can't use .data without specification about the data that is going to be stored\n", memory2->lineNumber);
                foundError = true;
                continue;
            }

            /* Encode all the data into binary and count it */
            while(current != NULL && CHECKNODE(current, number))
            {
                dec2bin(atoi(current->data->value), storeBits, BITSSIZE, 0);

                SAVEBITS
                (*dataCounter)++; /* Count the data */
                current = (Token_Node*)current->next; /* Go to the next number in the data */
            }

        }
        else if(current != NULL && CHECKNODE(current, string))
        {
            current = (Token_Node*)current->next;
            memory2 = NULL;
            memory3 = NULL;

            while(current != NULL && CHECKNODE(current, stringData))
            {
                if(current->data->value != NULL)
                {
                    /* Encode each char into binary using it's Asci value */
                    dec2bin((int)current->data->value[0], storeBits, BITSSIZE, 0);
                }
                else
                {
                    /* We need to add the null terminator */
                    storeBits[12] = '\0';
                    storeBits[11] = '0';
                    storeBits[10] = '0';
                    storeBits[9] = '0';
                    storeBits[8] = '0';
                    storeBits[7] = '0';
                    storeBits[6] = '0';
                    storeBits[5] = '0';
                    storeBits[4] = '0';
                    storeBits[3] = '0';
                    storeBits[2] = '0';
                    storeBits[1] = '0';
                    storeBits[0] = '0';
                }

                SAVEBITS
                (*dataCounter)++; /* Count the data */
                current = (Token_Node*)current->next; /* Go to the next char in the string */
            }
        }
        else if(current != NULL && (CHECKNODE(current, entryL) || CHECKNODE(current, externL)))
        {
            current = (Token_Node*)current->next;
            memory2 = NULL;
            memory3 = NULL;

            /** We assume entry the extern can use as many label as they want.
             * We already done the checks to ensure .extern and .entry are not empty in the laxer.
             * Also, entry can't call an non-existent label.
             * In this while, we just skip over each call because we don't need to encode them.
             */
            while(current->data != NULL && CHECKNODE(current, labelCall))
                current = (Token_Node*)current->next;
        }
        else
        {
            /* If noting was match we can assume the expression is incorrect */
            fprintf(stderr, "%d: Error! %s is not a valid statement\n", current->data->lineNumber, current->data->value);
            foundError = true;
            current = (Token_Node*)current->next; /* We need to manually move to the next token */
            continue; /* We need to find all the errors */
        }
    }

#if DEBUG
    currentP = bits;
    while(currentP != NULL)
    {
        printf("%s\n", currentP->bits);
        printf("Base 64: %s\n", encodeBase64(currentP->bits));
        currentP = (NodeParse*)currentP->next;
    }
#endif

    /* If we didn't find an error write to the files */
    if(!foundError)
    {
        /* Create the encoded information file */
        saveObject(bits, fileName, dataCounter, imageCounter);

        /* Save the extern labels if there's any in a file, if there aren't any a file will not be created */
        if(extTable != NULL)
        {
            saveExt(extTable, fileName);
            deleteList_t(extTable); /* We don't need the extern labels table anymore */
        }

        /* Save the entry labels if there's any in a file, if there aren't any a file will not be created */
        if(isEntry_s(table))
            saveEnt(table, fileName);
    }

    /* We don't need the encoded list anymore */
    deleteList_p(bits);
}
