#pragma once
#include<stdio.h>
#include<Windows.h>
//this struct designed to store the words read from file
struct Words {
	wchar_t *words;
	int *pos_of_point;
	int num_of_point;
};
/*
struct Words:
words: stores a file's all content.

pos_of_point: marks that divides a file's content line by line. 
Every beginning of a paragraph has a mark, and position of EOF also has one. 
Pay attention to that there must be only one mark point to EOF even if EOF is in the position of the beginning of a paragraph.

num_of_point: number of mark-point.

--------------------
|	*this will be stored in words
|	| here is a mark-point
|	*but '\n' will not be stored
|	| here is a mark-point
|	*else at the end there is a EOF
|	| here is a mark-point			| here is a mark-point as EOF
|--------------------

*/

//Read_section works like a iterator points to struct Words
struct Read_section {
	int point;
	int offset;
	int output_space;
};
/*
struct Read_section:
point:	the paragraph where the char want to be putchar() is
offset:	the char want to be putchar() far from mark
output_space:	when want to putchar() a char, it should be 0. Otherwise, putchar() a sapce

*/


//read from a file
void read_words(const char *file_name, Words *rec);
//check if the index of pos of sentence is legal. It will not be used in main.cpp directly.
bool is_index_of_PosOfParagraph_legal(const Words * rec, int index_of_point);
//check if a paragraph ends
bool is_paragraph_end(const Words *get_from, const Read_section *reading);
wchar_t get_char_from_words(const Words *get_from, const Read_section *reading);