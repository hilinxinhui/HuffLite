#ifndef _HUF_H
#define _HUF_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>


#define TRUE 1
#define FALSE 0
#define OK 1
#define ERROR -1
#define FAILED
#define MAXCHARTYPE 256
#define CHOSEN 1


typedef struct {
    unsigned char ch;
    unsigned long freq;
    char *huf_code;
    int parent, lchild, rchild;
} HufTreeNode, *HufTree;


typedef struct {
    unsigned char ch;
    unsigned long freq;
} CharFreqNode, *CharFreqTable;


void OutputInfo();
int GetFrequency(char *infile_name, CharFreqTable *t, unsigned int *num, unsigned int *len);
int SelectNode(HufTree huf_tree, unsigned int n, int *minimum, int *second_minimum);
void SortTable(CharFreqTable *t);
int CreateHufTree(HufTree huf_tree, unsigned int char_type_num, unsigned int node_num);
int CreateHufCode(HufTree huf_tree, unsigned int char_type_num);
int HuffmanEncoder(char *infile_name, char *outfile_name);
int HuffmanDecoder(char *infile_name, char *outfile_name);


#endif