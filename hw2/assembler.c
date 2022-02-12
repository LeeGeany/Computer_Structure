#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

/******************************************************
 * Structure Declaration
 *******************************************************/

typedef struct Inst {
    char* name;
    char* op;
    char type;
    char* funct;
}Inst;

typedef struct Data {
    int value;
    struct Data* next;
}Data;

typedef struct Text {
    int idx;
    char* d;
    char* s;
    char* t;
    unsigned int address;
    struct Text* next;
}Text;

typedef struct Sym {
    char* name;
    int size;
    unsigned int address;
    struct Data* first;
    struct Sym* next;
    struct Data* last;
}Sym;


/******************************************************
 * Global Variable Declaration
 *******************************************************/

struct Inst inst[21] = {
    {"addiu",   "001001", 'I', ""},
    {"addu",    "000000", 'R', "100001"},
    {"and",     "000000", 'R', "100100"},
    {"andi",    "001100", 'I', ""},
    {"beq",     "000100", 'I', ""},
    {"bne",     "000101", 'I', ""},
    {"j",       "000010", 'J', ""},
    {"jal",     "000011", 'J', ""},
    {"jr",      "000000", 'R', "001000"},
    {"lui",     "001111", 'I', ""},
    {"lw",      "100011", 'I', ""},
    {"nor",     "000000", 'R', "100111"},
    {"or",      "000000", 'R', "100101"},
    {"ori",     "001101", 'I', ""},
    {"sltiu",   "001011", 'I', ""},
    {"sltu",    "000000", 'R', "101011"},
    {"sll",     "000000", 'S', "000000"},
    {"srl",     "000000", 'S', "000010"},
    {"sw",      "101011", 'I', ""},
    {"subu",    "000000", 'R', "100011"},
    {"lui",     "001111", 'I', ""}
};
Sym* Symbols = NULL;
Text* Texts = NULL;
int datasize, textsize;


/******************************************************
 * Function Declaration
 *******************************************************/

 /* You may need the following functions */
char* change_file_ext(char* str);
char* num_to_bits(int num, int len);

/* You may fill the following functions */
void read_asm();
char* subst_asm_to_num();
void print_bits();
int find_address();

int main(int argc, char* argv[]) {

    FILE* input; FILE* output;
    char* filename = NULL;
    
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <*.s>\n", argv[0]);
        fprintf(stderr, "Example: %s sample_input/example?.s\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // reading the input file
    input = freopen(argv[1], "r", stdin);
    if (input == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }

    
    // creating the output file (*.o)
    filename = strdup(argv[1]); // strdup() is not a standard C library but fairy used a lot.
    if (change_file_ext(filename) == NULL) {
        fprintf(stderr, "'%s' file is not an assembly file.\n", filename);
        exit(EXIT_FAILURE);
    }

    output = freopen(filename, "w", stdout);
    if (output == NULL) {
        perror("ERROR");
        exit(EXIT_FAILURE);
    }
    read_asm();
    print_bits();

    // freeing the memory
    free(filename);
    free(Symbols);
    free(Texts);

    fclose(input);
    fclose(output);
    exit(EXIT_SUCCESS);
}

char* change_file_ext(char* str) {
    char* dot = strrchr(str, '.');

    if (!dot || dot == str || (strcmp(dot, ".s") != 0))
        return NULL;

    str[strlen(str) - 1] = 'o';
    return "";
}

char* num_to_bits(int num, int len) {
    if (num >= 0) {
        char* bits = (char*)malloc(len + 1);
        int idx = len - 1, i;

        while (num > 0 && idx >= 0) {
            if (num % 2 == 1) {
                bits[idx--] = '1';
            }
            else {
                bits[idx--] = '0';
            }
            num /= 2;
        }

        for (i = idx; i >= 0; i--) {
            bits[i] = '0';
        }
        bits[len] = NULL;
        return bits;
    }
    else if (num < 0) {
        char* bits = (char*)malloc(len + 1);
        int idx = len - 1, i;
        num = num * (-1);
        while (num > 0 && idx >= 0) {
            if (num % 2 == 1) {
                bits[idx--] = '0';
            }
            else {
                bits[idx--] = '1';
            }
            num /= 2;
        }

        for (i = idx; i >= 0; i--) {
            bits[i] = '1';
        }
        bits[len] = NULL;
        
        for (int j = len - 1; j >= 0; j--) {
            if (bits[j] == '1') { bits[j] = '0'; }
            else if (bits[j] == '0') { bits[j] = '1'; break; }
        }
        
        return bits;
    }
}

void read_asm() {
    int tmp, i;
    unsigned int address;
    char temp[0x1000] = { 0 };
    Sym* last_sym = NULL;
    Data* last_data1 = NULL;
    Data* last_data2 = NULL;
    Text* last_text = NULL;

    //Read .data region
    address = 0x10000000;
    for (i = 0; scanf("%s", temp) == 1;) {
        if (strcmp(temp, ".text") == 0) {
            break;
        }
        else if (temp[strlen(temp) - 1] == ':') {
            Sym* newSym = (Sym*)malloc(sizeof(Sym));
            newSym->first = NULL; newSym->last = NULL;
            newSym->name = (char*)malloc(strlen(temp)); strncpy(newSym->name, temp, strlen(temp) - 1);
            newSym->name[strlen(temp) - 1] = NULL;
            newSym->address = address;
            newSym->next = NULL;

            if (Symbols == NULL) { Symbols = newSym; last_sym = newSym; }
            else { last_sym = last_sym->next = newSym; }
        }
        else if (strcmp(temp, ".word") == 0) {
            scanf("%s", temp);
            if (temp[1] == 'x') { tmp = strtol(temp, NULL, 16); }
            else { tmp = atoi(temp); }
            
            int last = tmp % 32768; int first = tmp - last;

            Data* newData1 = (Data*)malloc(sizeof(Data));
            Data* newData2 = (Data*)malloc(sizeof(Data));
            newData1->next = NULL; newData2->next = NULL;
            newData1->value = first; newData2->value = last;

            if (last_sym->first == NULL) { last_sym->first = newData1; last_data1 = newData1; last_sym->last = newData2; last_data2 = newData2; }
            else { last_data1 = last_data1->next = newData1; last_data2 = last_data2->next = newData2; }
            last_sym->size = 4;
            address += 4;
        }
    }

    datasize = address - 0x10000000;

    address = 0x400000;
    for (i = 0; scanf("%s", temp) == 1;) {
        if (temp[strlen(temp) - 1] == ':') {
            Sym* newSym = (Sym*)malloc(sizeof(struct Sym));
            newSym->next = NULL; newSym->first = NULL; newSym->last = NULL;

            newSym->name = (char*)malloc(sizeof(strlen(temp))); strncpy(newSym->name, temp, strlen(temp) - 1); 
            newSym->name[strlen(temp) - 1] = NULL;

            newSym->address = address;
            newSym->size = 4;

            //address += 4;

            if (Symbols == NULL) { Symbols = newSym; last_sym = newSym; }
            else { last_sym = last_sym->next = newSym; }

        }
        else {
            Text* newText = (Text*)malloc(sizeof(struct Text));
            newText->next = NULL;
            for (int i = 0; i < 20; i++) {
                if (strcmp(inst[i].name, temp) == 0) {
                    newText->idx = i;
                    break;
                }
            }

            if (strcmp(temp, "la") == 0) {

                newText->idx = 9;
                newText->s = NULL;
                scanf("%s", temp); newText->t = (char*)malloc(strlen(temp)); strcpy(newText->t, temp);
                scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
                newText->address = address;
                if (Texts == NULL) { Texts = newText; last_text = newText; }
                else { last_text->next = newText; last_text = newText; }

                for (Sym* now = Symbols; now != NULL; now = now->next) {
                    if (strcmp(now->name, temp) == 0 && ((now->address) - 268435456) > 0) {
                        //
                        address +=4;
                        //
                        Text* newText1 = (struct Text*)malloc(sizeof(struct Text));
                        newText1->next = NULL;
                        newText1->idx = 13;
                        newText1->s = (char*)malloc(strlen(temp)); strcpy(newText1->s, last_text->t);
                        newText1->d = (char*)malloc(strlen(temp)); strcpy(newText1->d, last_text->t);
                        newText1->t = (char*)malloc(strlen(temp)); strcpy(newText1->t, last_text->d);
                        newText1->address = address;

                        last_text->next = newText1; last_text = newText1;
                        break;
                    }
                }
                address += 4;
                continue;
            }
            else if (inst[newText->idx].type == 'R') {
                if (newText->idx == 8) {
                    scanf("%s", temp); newText->s = (char*)malloc(strlen(temp)); strcpy(newText->s, temp);
                    newText->d = NULL;
                    newText->t = NULL;
                }
                else {
                    scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
                    scanf("%s", temp); newText->s = (char*)malloc(strlen(temp)); strcpy(newText->s, temp);
                    scanf("%s", temp); newText->t = (char*)malloc(strlen(temp)); strcpy(newText->t, temp);
                }
            }
            else if (inst[newText->idx].type == 'I') {
                //lui
                if (newText->idx == 9) {
                    //*
                    newText->idx =20;
                    //*
                    newText->s = NULL;
                    scanf("%s", temp); newText->t = (char*)malloc(strlen(temp)); strcpy(newText->t, temp);
                    scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
                }
                //ori
                else if (newText->idx == 13) {
                    scanf("%s", temp); newText->s = (char*)malloc(strlen(temp)); strcpy(newText->s, temp);
                    scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
                    scanf("%s", temp); newText->t = (char*)malloc(strlen(temp)); strcpy(newText->t, temp);
                }
                //lw
                else if (newText->idx == 10) {
                    scanf("%s", temp); newText->t = (char*)malloc(strlen(temp)); strcpy(newText->t, temp);
                    scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
                                       newText->s = (char*)malloc(strlen(temp)); strcpy(newText->s, temp);
                }
                //sw
                else if (newText->idx == 18) {
                    scanf("%s", temp); newText->t = (char*)malloc(strlen(temp)); strcpy(newText->t, temp);
                    scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
                                       newText->s = (char*)malloc(strlen(temp)); strcpy(newText->s, temp);
                }
                else {
                    scanf("%s", temp); newText->s = (char*)malloc(strlen(temp)); strcpy(newText->s, temp);
                    scanf("%s", temp); newText->t = (char*)malloc(strlen(temp)); strcpy(newText->t, temp);
                    scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
                }
            }
            else if (inst[newText->idx].type == 'S') {
                scanf("%s", temp); newText->s = (char*)malloc(strlen(temp)); strcpy(newText->s, temp);
                scanf("%s", temp); newText->t = (char*)malloc(strlen(temp)); strcpy(newText->t, temp);
                scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
            }
            else if (inst[newText->idx].type == 'J') {
                scanf("%s", temp); newText->d = (char*)malloc(strlen(temp)); strcpy(newText->d, temp);
                newText->s = NULL;
                newText->t = NULL;
            } 

            newText->address = address;
            address += 4;

            if (Texts == NULL) { Texts = newText; last_text = newText; }
            else { last_text->next = newText; last_text = newText; }
        }

    }
    textsize = address - 0x400000;
}

char* subst_asm_to_num(char* str) {
    if (str == NULL) { return "00000"; }
    int len = strlen(str);
    char* bits = (char*)malloc(len);
    
    if (str[0] == '$') { bits = str + 1; bits[len] = NULL; }
    else {
        int flag = 0; int j = 0;
        for (int i = 0; i < len; i++) {
            if (flag == 1) { bits[j++] = str[i]; }
            if (str[i] == '$') { flag = 1; }
            if (str[i] == ')') { bits[i] == NULL; break; }
        }
    }

    int value = atoi(bits);
    return  num_to_bits(value, 5);
}

char* subst_asm_to_num2(char* str) {
    int len = strlen(str);
    char* bits = (char*)malloc(len);

    for (int i = 0; i < len; i++) {
        if (str[i] == '(') { bits[i] == NULL; }
        bits[i] = str[i];
    }

    int value = atoi(bits);
    return num_to_bits(value, 16);
}

int find_address(char* str) {
    int value = 0;
    for (Sym* now = Symbols; now != NULL; now = now->next) {
        if (strcmp(str, now->name) == 0) {
            return now->address;
        }
    }

    if (str[1] == 'x') { value = strtol(str, NULL, 16); }
    else { value = atoi(str); }

    return value;
}

char* cut_upperbit(char* str) {
    char* bits = (char*)malloc(17);

    for (int i = 0; i < 16; i++) {
        bits[i] = str[i];
    }
    bits[16] = NULL;
    return bits;
}

void print_bits() {
    printf("%s", num_to_bits(textsize, 32));
    printf("%s", num_to_bits(datasize, 32));
    
    for (Text *text = Texts; text != NULL; text = text->next) {
        if (inst[text->idx].type == 'R') {
            printf("%s%s%s%s%s%s", inst[text->idx].op, subst_asm_to_num(text->s), subst_asm_to_num(text->t), subst_asm_to_num(text->d), subst_asm_to_num(NULL), inst[text->idx].funct);
        }
        else if (inst[text->idx].type == 'I') {
            //bne일때
            if (text->idx == 5 || text->idx == 4) {
                printf("%s%s%s%s", inst[text->idx].op, subst_asm_to_num(text->s), subst_asm_to_num(text->t), num_to_bits( (find_address(text->d)/4 - ((text->address)+4)/4)  , 16)  );
            }
            else if (text->idx == 9) {
                printf("%s%s%s%s", inst[text->idx].op, subst_asm_to_num(text->s), subst_asm_to_num(text->t), cut_upperbit( num_to_bits( find_address(text->d), 32)));
            }
            else if (text->idx == 13) {
                printf("%s%s%s%s", inst[text->idx].op, subst_asm_to_num(text->d), subst_asm_to_num(text->s), num_to_bits((find_address(text->t) % 65536), 16));
            }
            else if (text->idx == 10) {
                printf("%s%s%s%s", inst[text->idx].op, subst_asm_to_num(text->s), subst_asm_to_num(text->t), subst_asm_to_num2(text->d));
            }
            else if (text->idx == 18) {
                printf("%s%s%s%s", inst[text->idx].op, subst_asm_to_num(text->s), subst_asm_to_num(text->t), subst_asm_to_num2(text->d));
            }
            //*
            else if (text->idx == 20) {
                printf("%s%s%s%s", inst[text->idx].op, subst_asm_to_num(text->s), subst_asm_to_num(text->t), num_to_bits(find_address(text->d), 16));
            }
            //*
            else {
                printf("%s%s%s%s", inst[text->idx].op, subst_asm_to_num(text->t), subst_asm_to_num(text->s), num_to_bits(find_address(text->d), 16));
            }
        }
        else if (inst[text->idx].type == 'S') {
            printf("%s%s%s%s%s%s", inst[text->idx].op, subst_asm_to_num(NULL), subst_asm_to_num(text->t), subst_asm_to_num(text->s), num_to_bits(find_address(text->d), 5) , inst[text->idx].funct);
        }
        else if (inst[text->idx].type == 'J') {
            if (text->idx == 7) {
                printf("%s%s", inst[text->idx].op, num_to_bits((((find_address(text->d)) / 4)), 26));
            }
            else {
                printf("%s%s", inst[text->idx].op, num_to_bits((((find_address(text->d)) / 4)), 26));
            }
        }
    }
    
    for (Sym* sym = Symbols; sym != NULL; sym = sym->next) {
        Data* now1 = sym->first;
        for ( Data* now2 = sym->last; now1 != NULL; now1 = now1->next, now2= now2->next) {
            printf("%s", num_to_bits(now1->value + now2->value, 32));
            if (sym->address < 0x10000000) { break; }
        }

    }
    printf("\n"); // To exclude "No newline at end of file"
}
