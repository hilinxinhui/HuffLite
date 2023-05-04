#include "huf.h"


int main() {
    char infile_name[256], outfile_name[256]; /* ��������ļ��� */
    int option, flag; /* optionѡ�����, flag�ж�En/Decode״̬ */
    char ch[2]; /* ��gets(ch)�ﵽgetchar()�ȴ����������Ч�� */
    
    while (TRUE) {
        flag = 0;
    /* ѡ����� */
    LOOP:
        OutputInfo();
        fflush(stdin); /* ��ձ�׼���뻺����, ��ͬ���� */
        scanf("%d", &option);

        if (option == 3) {
            /* ֱ���˳� */
            printf("The program has run to completion.\n");
            printf("Thanks for using this program!\n");
            break;
        }
        else if (option == 1 || option == 2) {
            /* ��ȡ��������ļ��ģ����ԣ�·�� */
            printf("Please enter the absolute path of the input file:\n");
            fflush(stdin);
            gets(infile_name);
            printf("Please enter the absolute path of the output file:\n");
            fflush(stdin);
            gets(outfile_name);
        }
        else {
            /* ������� */
            printf("The selected function does not exist, please try again!\n");
            printf("Press any key to continue...\n");
            fflush(stdin);
            gets(ch);
            goto LOOP;
        }

        /* ѡ����� */
        switch (option) {
            case 1:
                printf("Checking input validity, please wait......\n");
                flag = HuffmanEncoder(infile_name, outfile_name);
                if (flag == ERROR) {
                    printf("Error(file not found), please try again!\n");
                    printf("Press any key to continue...");
                    fflush(stdin);
                    gets(ch);
                    goto LOOP;
                }
                printf("Compressing, please wait......\n");
				printf("Operation completed!\n");
				printf("Press any key to continue...");
                fflush(stdin);
                gets(ch);
                break;
            case 2:
                printf("Checking input validity, please wait......\n");
                flag = HuffmanDecoder(infile_name, outfile_name);
                if (flag == ERROR) {
                    printf("Error(file not found), please try again!\n");
                    printf("Press any key to continue...");
                    fflush(stdin);
                    gets(ch);
                    goto LOOP;
                }
                printf("Extracting, please wait......\n");
				printf("Operation completed!\n");
                printf("Press any key to continue...");
                fflush(stdin);
                gets(ch);
                break;
        }
    }

    system("pause");
    return 0;
}


void OutputInfo() {
    /* ����, macOS/Linux��ʹ��system("clear"); */
    system("cls");

    /* �򵥽��� */
    printf("-------Huffman En-Decoder-------\n");
    printf("@author: _linxinhui_\n");
    /* Other Info:
    printf("@class: CS1901\n");
    printf("@No: 120191080104\n"); */
    printf("@version: 0.1\n\n");
    printf("Operations are as follows:\n1. To Compress\n2. To Extract\n3. To Quit\n");
    printf("Please choose an operation(enter the number): ");

    return ;
}


int GetFrequency(char *infile_name, CharFreqTable *t, unsigned int *num, unsigned int *len) {
    FILE *infile;
    unsigned char temp_char; /* �ݴ��ַ���8bitsλ���� */
    int i;

    /* ����˳���, ��8λ�洢�ַ�, ����ܴ�256���ַ�, ����/˳�������Ϊ256 */
    *t = (CharFreqTable)malloc(MAXCHARTYPE * sizeof(CharFreqNode));
    if (*t == NULL) {
        printf("Space applying failed, please try again.");
        return FAILED;
    }

    /* ��ʼ���ݴ��ַ��ڵ� */
    for (i = 0; i < 256; ++ i) {
        /* �����256���±��루���ܵģ�256���ַ���Ӧ */
        (*t)[i].ch = (unsigned char)i;
        (*t)[i].freq = 0;
    }

    /* ���ļ� */
    infile = fopen(infile_name, "rb");
    if (infile == NULL) {
        return ERROR;
    }

    /* ���ļ�, ��ȡ�ַ�Ƶ�� */
    /* fread(buffer_pointer, size, count, file_pointer) */
    fread((char *)(&temp_char), sizeof(unsigned char), 1, infile);
    while (!feof(infile)) {
        ++ (*t)[(int)temp_char].freq;
        ++ (*len);
        fread((char *)(&temp_char), sizeof(unsigned char), 1, infile);
    }

    /* �ر��ļ� */
    fclose(infile);

    /* ��table��Ƶ������ */
    SortTable(t);

    /* ͳ��ʵ���ַ�����, ���޳�Ƶ��Ϊ0���ַ� */
    for (i = 0; i < MAXCHARTYPE; ++ i) {
        if ((*t)[i].freq == 0) {
            break;
        }
    }
    (*num) = i;

    return OK;
}


void SortTable(CharFreqTable *t) {
    int i, j;
    CharFreqNode temp;

    /* ���ڵ�Ƶ��ð������ */
    for (i = 0; i < MAXCHARTYPE - 1; ++ i) {
        for (j = i + 1; j < MAXCHARTYPE; ++ j) {
            if ((*t)[i].freq < (*t)[j].freq) {
                temp = (*t)[i];
                (*t)[i] = (*t)[j];
                (*t)[j] = temp;
            }
        }
    }

    return ;
}


int SelectNode(HufTree huf_tree, unsigned int n, int *minimum, int *second_minimum) {
    unsigned int i;
    unsigned long min;

    /* ����С�ڵ� */
    min = ULONG_MAX; /* ULONG_MAX��unsigned long int��ʮ�������ֵ */
    for (i = 0; i < n; ++ i) {
        if (huf_tree[i].parent == 0 && huf_tree[i].freq < min) {
            min = huf_tree[i].freq;
            *minimum = i;
        }
    }
    huf_tree[*minimum].parent = CHOSEN;

    /* �Ҵ�С�ڵ� */
    min = ULONG_MAX;
    for (i = 0; i < n; ++ i) {
        if (huf_tree[i].parent == 0 && huf_tree[i].freq < min) {
            min = huf_tree[i].freq;
            *second_minimum = i;
        }
    }

    return OK;
}


int CreateHufTree(HufTree huf_tree, unsigned int char_type_num, unsigned int node_num) {
    unsigned int i;
    int minimum, second_minimum;

    for (i = char_type_num; i < node_num; ++ i) {
        SelectNode(huf_tree, i, &minimum, &second_minimum); /* ѡȡ����Ƶ����С�Ľڵ� */
        huf_tree[i].lchild = minimum;
        huf_tree[i].rchild = second_minimum;
        huf_tree[i].freq = huf_tree[minimum].freq + huf_tree[second_minimum].freq;
        huf_tree[minimum].parent = i;
        huf_tree[second_minimum].parent = i;
    }

    return OK;
}


int CreateHufCode(HufTree huf_tree, unsigned int char_type_num) {
    unsigned int i;
    int cur, parent;
    int index;
    char *temp_code = NULL;

    temp_code = (char *)malloc(MAXCHARTYPE * sizeof(char));
    temp_code[MAXCHARTYPE - 1] = '\0';

    for (i = 0; i < char_type_num; ++ i) {
        index = MAXCHARTYPE - 1;
        cur = i;
        parent = huf_tree[cur].parent;

        /* ��Ҷ�ӽڵ㵽���ڵ�, ��������Huffman�� */
        while (parent != 0) {
            if (huf_tree[parent].lchild == cur) {
                /* ���֧��'0' */
                index -= 1;
                temp_code[index] = '0';
            }
            else {
                /* ���֧��'1' */
                index -= 1;
                temp_code[index] = '1';
            }

            cur = parent;
            parent = huf_tree[cur].parent;
        }

        /* �����ɵı��뱣�浽���ڵ� */
        huf_tree[i].huf_code = (char *)malloc((MAXCHARTYPE - index) * sizeof(char));
        strcpy(huf_tree[i].huf_code, &temp_code[index]); /* ���򱣴�Huffman�� */
    }

    /* �ͷ���ʱ����ռ�õĿռ� */
    free(temp_code);

    return OK;
}


int HuffmanEncoder(char *infile_name, char *outfile_name) {
    FILE *infile, *outfile;
    CharFreqTable table; /* �ַ�Ƶ�ȱ�, ͳ�ƽڵ�Ƶ�Ȳ����������ڵ�֮�󼴱��ͷ� */
    unsigned char temp_char; /* �ݴ��ַ�(8bitsλ��) */
    unsigned int char_type_num; /* �ַ�(8bitsλ��)������ */
    unsigned int file_len = 0; /* �ļ����� */
    unsigned int node_num; /* Huffman���н����еĽڵ���, ��Ҷ�ӽڵ���(�ַ���)ȷ�� */
    HufTree huf_tree;
    char buffer[MAXCHARTYPE] = "\0"; /* ���뻺���� */
    unsigned int code_len;
    unsigned int i;

    /* ��Դ�ļ�, ͳ�ƴ�Ƶ, ����table���� */
    if (GetFrequency(infile_name, &table, &char_type_num, &file_len) == ERROR) {
        return ERROR;
    }

    /* ��ֻ��һ���ַ� */
    if (char_type_num == 1) {
        /* ���ļ� */
        outfile = fopen(outfile_name, "wb");
        if (outfile == NULL) {
            return ERROR;
        }

        /* д���ַ������ַ����ַ�Ƶ��(��) */
        fwrite((char *)(&char_type_num), sizeof(unsigned int), 1, outfile);
        fwrite((char *)(&table[0].ch), sizeof(unsigned char), 1, outfile);
        fwrite((char *)(&table[0].freq), sizeof(unsigned long), 1, outfile);

        free(table);

        fclose(outfile);
    }
    /* ����ֹһ���ַ� */
    else {
        node_num = 2 * char_type_num - 1;

        huf_tree = (HufTree)malloc(node_num * sizeof(HufTreeNode));

        /* ��ʼ��Huffman����ǰchar_type_num���ڵ� */
        for (i = 0; i < char_type_num; ++ i) {
            /* ��table�����ݿ��������ڵ� */
            huf_tree[i].ch = table[i].ch;
            huf_tree[i].freq = table[i].freq;
            huf_tree[i].parent = 0;
        }

        /* �ͷ�table */
        free(table);

        /* ��ʼ��Huffman����ʣ��ڵ� */
        for (i = char_type_num; i < node_num; ++ i) {
            huf_tree[i].parent = 0;
        }

        /* ����Huffman�� */
        CreateHufTree(huf_tree, char_type_num, node_num);

        /* ����Huffman�� */
        CreateHufCode(huf_tree, char_type_num);

        /* ��ʼд�ļ� */
        outfile = fopen(outfile_name, "wb");
        if (outfile == NULL) {
            return ERROR;
        }

        /* ���ļ�д���ַ�����Ƶ�� */
        fwrite((char *)(&char_type_num), sizeof(unsigned int), 1, outfile);
        for (i = 0; i < char_type_num; ++ i) {
            fwrite((char *)(&huf_tree[i].ch), sizeof(unsigned char), 1, outfile);
            fwrite((char *)&huf_tree[i].freq, sizeof(unsigned long), 1, outfile);
        }

        /* ���ļ�д���ļ����� */
        fwrite((char *)(&file_len), sizeof(unsigned long), 1, outfile);

        /* �ַ����� */
        infile = fopen(infile_name, "rb");
        if (infile == NULL) {
            return ERROR;
        }
        fread((char *)(&temp_char), sizeof(unsigned char), 1, infile);
        while (!feof(infile)) {
            /* ƥ���infile��ȡ���ַ��ʹ洢��Huffman���е��ַ�, ������ӦHuffman�븴�Ƶ�buffer */
            for (i = 0; i < char_type_num; ++ i) {
                if (temp_char == huf_tree[i].ch) {
                    strcat(buffer, huf_tree[i].huf_code);
                }
            }

            /* ��8������(����ĳ��ȼȿ���ָbit, Ҳ��ָbyte)Ϊ��Ԫ���� */
            while (strlen(buffer) >= 8) {
                temp_char = '\0'; /* '\0'�ȼ���(0x00)_16�ȼ���(00000000)_2 */
                for (i = 0; i < 8; ++ i) {
                    temp_char <<= 1;
                    if (buffer[i] == '1') {
                        temp_char |= 1;
                    }
                }
                fwrite((char *)(&temp_char), sizeof(unsigned char), 1, outfile);
                strcpy(buffer, buffer + 8);
            }
            fread((char *)(&temp_char), sizeof(unsigned char), 1, infile);
        }
        
        /* ����ĩβbuffer��(���п��ܴ��ڵ�)����8�����ȵı��� */
        code_len = strlen(buffer);
        if (code_len > 0) {
            temp_char = '\0';
            for (i = 0; i < code_len; ++ i) {
                temp_char = temp_char << 1;
                if (buffer[i] == '1') {
                    temp_char = temp_char | 1;
                }
            }
            temp_char <<= (8 - code_len);
            fwrite((char *)(&temp_char), sizeof(unsigned char), 1, outfile);
        }

        /* �ر��ļ� */
        fclose(infile);
        fclose(outfile);

        /* �ͷ�Huffman��ռ�õĿռ� */
        for (i = 0; i < char_type_num; ++ i) {
            free(huf_tree[i].huf_code);
        }
        free(huf_tree);
    }

    return OK;
}


int HuffmanDecoder(char *infile_name, char *outfile_name) {
    FILE *infile, *outfile;
    HufTree huf_tree;
    unsigned int root; /* ���ڵ��ַ */
    unsigned int char_type_num;
    unsigned int node_num;
    unsigned char temp_char; /* �ݴ�8bits���� */
    unsigned long file_len = 0;
    unsigned long writen_len = 0;
    unsigned int i;

    /* ���ļ� */
    infile = fopen(infile_name, "rb");
    if (infile == NULL) {
        return ERROR;
    }

    /* ��ȡ�ļ�ͷ��Ϣ(�ַ���) */
    fread((char *)(&char_type_num), sizeof(unsigned int), 1, infile);

    /* ����, ��ֻ��һ���ַ�ʱ */
    if (char_type_num == 1) {
        /* ���ļ�ͷ��Ϣ(�ַ�, �ļ�����) */
        fread((char *)(&temp_char), sizeof(unsigned char), 1, infile);
        fread((char *)(&file_len), sizeof(unsigned int), 1, infile);

        /* ���� */
        outfile = fopen(outfile_name, "wb");
        if (outfile == NULL) {
            return ERROR;
        }
        while (file_len > 0) {
            fwrite((char *)(&temp_char), sizeof(unsigned char), 1, outfile);
            -- file_len;
        }

        /* �ر��ļ� */
        fclose(infile);
        fclose(outfile);
    }
    /* ����, ����ֹһ���ַ�ʱ */
    else {
        node_num = 2 * char_type_num - 1;
        
        huf_tree = (HufTree)malloc(node_num * sizeof(HufTreeNode));

        /* ��ʼ��Huffman�� */
        for (i = 0; i < char_type_num; ++ i) {
            fread((char *)(&huf_tree[i].ch), sizeof(unsigned char), 1, infile);
            fread((char *)(&huf_tree[i].freq), sizeof(unsigned long), 1, infile);
            huf_tree[i].parent = 0;
        }
        for (i = char_type_num; i < node_num; ++ i) {
            huf_tree[i].parent = 0;
        }

        /* �ؽ�Huffman�� */
        CreateHufTree(huf_tree, char_type_num, node_num);

        /* ���� */
        fread((char *)(&file_len), sizeof(unsigned long), 1, infile);
        outfile = fopen(outfile_name, "wb");
        if (outfile == NULL) {
            return ERROR;
        }
        root = node_num - 1;
        while (TRUE) {
            /* �� 8bits Ϊһ������Ԫ */
            fread((char *)(&temp_char), sizeof(unsigned char), 1, infile);
            for (i = 0; i < 8; ++ i) {
                if ((temp_char & 128) == 0) {
                    /* 128�ȼ���0b10000000 */
                    root = huf_tree[root].lchild;
                }
                else {
                    root = huf_tree[root].rchild;
                }
                if (root < char_type_num) {
                    fwrite((char *)(&huf_tree[root].ch), sizeof(unsigned char), 1, outfile);
                    ++ writen_len;
                    if (writen_len == file_len) {
                        break;
                    }
                    root = node_num - 1;
                }
                temp_char <<= 1;
            }
            if (writen_len == file_len) {
                break;
            }
        }

        /* �ر��ļ� */
        fclose(infile);
        fclose(outfile);

        /* �ͷ�Huffman��ռ�õĿռ� */
        free(huf_tree);
    }

    return OK;
}
