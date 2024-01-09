#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define INSTRUCTIN_START 0x00400000

struct label
{
    int address;
    char name[50];
};

int buildInstructionR(char *args[4]);
int assembleR(int *args);
int encodingR(char *opcode);
int intRegister(char *reg);
char *instructionType(char opcode[]);
char *decToBinary(int n, char* error);
int assembleJ(int *args);
int buildInstructionJ(int adressToJump);
int encodingJ(char *opcode);
int buildInstructionI(char *args[]);
int assembleI(int *args);
int encodingI(char *opcode);


int main(int argc, char const *argv[])
{

    int instructionAddrs = INSTRUCTIN_START;
    int labelIndex = 0;
    struct label labels[100];
    FILE *readFile;
    FILE *writeFile;
    char line[100];
    char *sep_line[50];
    int count = 0;
    readFile = fopen(argv[1], "r");
    writeFile = fopen("object.txt", "w");
    while (fgets(line, 100, readFile))
    {
        // Split line with whitespaces add every item to the sep_line array
        char delim[] = "$ ,\n\0";
        char *ptr = strtok(line, delim);

        while (ptr != NULL)
        {
            sep_line[count] = ptr;
            count += 1;
            ptr = strtok(NULL, delim);
        }

        for (int i = 0; i < count; i++)
        {
            int length = strlen(sep_line[i]);

            if (sep_line[i][length - 1] == ':')
            {
                sep_line[i][length - 1] = '\0';
                labels[labelIndex].address = instructionAddrs;
                strcpy(labels[labelIndex].name, sep_line[i]);
                labelIndex += 1;
            }
            else
            {

                char *instrType = instructionType(sep_line[i]);

                if (!strcmp(instrType, "INVALID_TYPE") && i == 0)
                {
                    printf("Invalid operations..\n");
                    break;
                }
                else if (!strcmp(instrType, "R_TYPE"))
                {
                    int value = buildInstructionR(&sep_line[i]);
                    char *binary = decToBinary(value, instrType);
                    fprintf(writeFile, "%s\n", binary);
                }
                else if (!strcmp(instrType, "I_TYPE"))
                {
                    int value = buildInstructionI(&sep_line[i]);
                    char *binary = decToBinary(value, instrType);
                    fprintf(writeFile, "%s\n", binary);
                }
                else if (!strcmp(instrType, "J_TYPE"))
                {
                    for (int j = 0; j < labelIndex; j++)
                    {
                        if (!strcmp(labels[j].name, sep_line[1]))
                        {
                            int value = buildInstructionJ(labels[j].address);
                            char *binary = decToBinary(value, instrType);
                            fprintf(writeFile, "%s\n", binary);
                        }
                    }
                }
            }
        }

        instructionAddrs += 0x4;
        count = 0;
        // Dosya okumanın sonu
    }

    fclose(readFile);
    fclose(writeFile);
    // Main Fonksiyonunun Son
    return 0;
}

/*
 *
 *
 *
 *
 *   ALL FUNCTIONS
 *
 *
 *
 */

char *instructionType(char opcode[])
{
    if (!strcmp(opcode, "add"))
    {
        return "R_TYPE";
    }
    else if (!strcmp(opcode, "sub"))
    {
        return "R_TYPE";
    }
    else if (!strcmp(opcode, "and"))
    {
        return "R_TYPE";
    }
    else if (!strcmp(opcode, "or"))
    {
        return "R_TYPE";
    }
    else if (!strcmp(opcode, "addi"))
    {
        return "I_TYPE";
    }
    else if (!strcmp(opcode, "andi"))
    {
        return "I_TYPE";
    }
    else if (!strcmp(opcode, "sll"))
    {
        return "R_TYPE";
    }
    else if (!strcmp(opcode, "beq"))
    {
        return "I_TYPE";
    }
    else if (!strcmp(opcode, "bne"))
    {
        return "I_TYPE";
    }
    else if (!strcmp(opcode, "j"))
    {
        return "J_TYPE";
    }

    return "INVALID_TYPE";
}

// Build R -- J -- I
int buildInstructionR(char *args[])
{

    int returnlist[6];
    char *opcode = args[0];
    int dest;
    int rs = 0;
    int rt = 0;
    int sa = 0;
    dest = intRegister(args[1]);
    
    
    int opEncoding = encodingR(opcode);

    if (!strcmp(args[0], "sll"))
    {
        rt = intRegister(args[2]);
        sa = atoi(args[3]);
    } else {
        rs = intRegister(args[2]);
        rt = intRegister(args[3]);
    }

    // add all the values to the returnlist
    returnlist[0] = 0;
    returnlist[1] = rs;
    returnlist[2] = rt;
    returnlist[3] = dest;
    returnlist[4] = sa;
    returnlist[5] = opEncoding;

    return assembleR(returnlist);
}

int buildInstructionJ(int adressToJump)
{
    int returnList[2];
    int jValue = encodingJ("j");

    returnList[0] = jValue;
    returnList[1] = adressToJump;

    return assembleJ(returnList);
}

int buildInstructionI(char *args[]){

    int returnList[4];
    char *opcode = args[0];
    int opEncoded = encodingI(opcode);
    int dest = intRegister(args[1]);
    int rs = intRegister(args[2]);
    int imm = atoi(args[3]);

    returnList[0] = opEncoded;
    returnList[1] = rs;
    returnList[2] = dest;
    returnList[3] = imm;

    return assembleI(returnList);
}

// Assemble R -- J -- I
int assembleR(int *args)
{
    int value = ((args[0] << 26) | (args[1] << 21) | (args[2] << 16) | (args[3] << 11) | (args[4] << 6) | args[5]);
    return value;
}

int assembleJ(int *args)
{
    int value = ((args[0] << 26) | args[1]);
    return value;
}

int assembleI(int *args)
{
    int value = ((args[0] << 26) | (args[1] << 21) | (args[2] << 16) | args[3]);
    return value;
}

// Encoding R -- J -- I
int encodingR(char *opcode)
{

    int returnValue;
    if (!strcmp(opcode, "add"))
    {
        returnValue = 32;
    }
    else if (!strcmp(opcode, "sub"))
    {
        returnValue = 34;
    }
    else if (!strcmp(opcode, "and"))
    {
        returnValue = 36;
    }
    else if (!strcmp(opcode, "or"))
    {
        returnValue = 37;
    }
    else if (!strcmp(opcode, "sll"))
    {
        returnValue = 0;
    }
    else
    {
        returnValue = -1;
    }

    return returnValue;
}

int encodingJ(char *opcode)
{
    // J tipi instruction'lar için opcode değerini belirle
    if (!strcmp(opcode, "j"))
    {
        return 2;
    }
    return -1;
}

int encodingI(char *opcode)
{
   
    if (!strcmp(opcode, "andi"))
    {
        return 12;
    }
    else if (!strcmp(opcode, "addi"))
    {
        return 8;
    }
    else if (!strcmp(opcode, "beq"))
    {
        return 4;
    }
    else if (!strcmp(opcode, "bne"))
    {
        return 5;
    }
    return -1;
}

// Register Value
int intRegister(char reg[])
{
    int returnVal;

    if (strcmp(reg, "zero") == 0 || strcmp(reg, "0") == 0)
    {
        returnVal = 0;
    }
    else if (strcmp(reg, "at") == 0)
    {
        returnVal = 1;
    }
    else if (strcmp(reg, "gp") == 0)
    {
        returnVal = 28;
    }
    else if (strcmp(reg, "sp") == 0)
    {
        returnVal = 29;
    }
    else if (strcmp(reg, "fp") == 0)
    {
        returnVal = 30;
    }
    else if (strcmp(reg, "ra") == 0)
    {
        returnVal = 31;
    }
    else
    {
        char regChar = reg[0];
        int regNum;

        if (strlen(reg) > 2)
        {
            return -1; // No support for registers > 9
        }

        regNum = reg[1] - '0';

        switch (regChar)
        {
        case 'v':
            // must be v0 or v1
            if (regNum <= 1)
            {
                returnVal = regNum + 2;
            }
            else
            {
                returnVal = -1;
            }
            break;
        case 'a':
            // must be a0-a3
            if (regNum <= 3)
            {
                returnVal = regNum + 4;
            }
            else
            {
                returnVal = -1;
            }
            break;
        case 't':
            // must be t0-t9
            if (regNum < 8)
            {
                returnVal = regNum + 8;
            }
            else
            {
                returnVal = regNum + 16;
            }
            break;
        case 's':
            // must be s0-s7
            if (regNum < 8)
            {
                returnVal = regNum + 16;
            }
            else
            {
                returnVal = -1;
            }
            break;
        case 'k':
            // must be k0 or k1
            if (regNum <= 1)
            {
                returnVal = regNum + 26;
            }
            else
            {
                returnVal = -1;
            }
            break;
        default:
            returnVal = -1;
        }
    }

    return returnVal;
}

// Turn integer to binary
char* decToBinary(int n, char* error) {
    char* binaryNum = (char*)malloc(33); // 32 bit + 1 null terminator
    if (binaryNum == NULL) {
        printf("Bellek ayirma hatasi\n");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    // Sayı sıfır veya negatif değilse devam et
    if (n >= 0) {
        while (n > 0) {
            binaryNum[i] = (n % 2) + '0'; // Kalanını al ve karaktere çevir
            n = n / 2; // 2'ye böl
            i++;
        }

        // Elde edilen ikilik sayıyı ters çevirip sonlandırıcı karakter ekleyerek döndür
        while (i < 32) {
            binaryNum[i] = '0'; // Eksik bitleri sıfır ile doldur
            i++;
        }

        binaryNum[i] = '\0';
        int start = 0;
        int end = i - 1;
        while (start < end) {
            // Swap işlemi
            char temp = binaryNum[start];
            binaryNum[start] = binaryNum[end];
            binaryNum[end] = temp;
            start++;
            end--;
        }

        return binaryNum;
    } else {
        printf("Negatif sayilar icin bu fonksiyon desteklenmiyor.\n%s\n",error);
        free(binaryNum); // Bellek serbest bırak
        exit(EXIT_FAILURE);
    }
}