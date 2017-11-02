#include<stdio.h>
#include<stdlib.h>
#include<wchar.h>
#include<locale.h>//if you want to putchar ... emm ... like in Chinese or Japanese
#include<Windows.h>
#include"Words.h"
#define DIS_BUTTOM 5
#define WHITE FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED
#define INTENSITY_WHITE FOREGROUND_GREEN|FOREGROUND_BLUE|FOREGROUND_RED|FOREGROUND_INTENSITY
#define STOP_AFTER_PUTCHAR 80//Sleep(STOP_AFTER_PUTCHAR)

void set_cursor_position(HANDLE StdOut, int x, int y) {
	COORD pos{ x,y };
	SetConsoleCursorPosition(StdOut, pos);
}

/*
function: show_rain(HANDLE StdOut,COORD boundary,const Words *want_say,COORD** pcursor_point,Read_section** preading,wchar_t** plast_output);
StdOut: standard output handle. Get from GetStdHandle()
want_say:		a Words contains a file's all content that you want to put to screen
parray_cursor£º	a array of cursor. Every col has one.
preading:		a array of Reading_section. Every col has one.
plast_output:	a array of char that remebers last char of every col has putchar last time. Every col has one.
*/
void show_rain(HANDLE StdOut,COORD boundary,const Words *want_say,COORD** parray_cursor,Read_section** preading,wchar_t** plast_output) {
	//check if the boundary is changed
	static COORD last_boundary = boundary;

	if ((*parray_cursor) == nullptr || last_boundary.X != boundary.X||last_boundary.Y!=boundary.Y) {
		system("CLS");
		if ((*parray_cursor) != nullptr) {
			free(*parray_cursor);
			*parray_cursor = nullptr;
		}
		if ((*preading) != nullptr) {
			free(*preading);
			*preading = nullptr;
		}
		if ((*plast_output) != nullptr) {
			free(*plast_output);
			*plast_output = nullptr;
		}

		(*parray_cursor) = (COORD*)malloc(sizeof(COORD)*(boundary.X + 1));
		(*preading )= (Read_section*)malloc(sizeof(Read_section)*(boundary.X + 1));
		(*plast_output )= (wchar_t *)malloc(sizeof(wchar_t)*(boundary.X + 1));

		for (int i = 0; i <= boundary.X; i++) {
			(*parray_cursor)[i].X = i;
			(*parray_cursor)[i].Y = rand()%(boundary.Y-DIS_BUTTOM);
			(*plast_output)[i] = L' ';
			(*preading)[i].output_space = rand() % (boundary.Y / 2) + boundary.Y / 2;
			(*preading)[i].point = rand() % (want_say->num_of_point-1);
			(*preading)[i].offset = 0;
		}
		last_boundary = boundary;
	}

	for (int i = 0; i < boundary.X; i++) {
			SetConsoleTextAttribute(StdOut, WHITE);
			set_cursor_position(StdOut, (*parray_cursor)[i].X, (*parray_cursor)[i].Y);
			putwchar((*plast_output)[i]);

		(*parray_cursor)[i].Y = ((*parray_cursor)[i].Y + 1) % (boundary.Y - DIS_BUTTOM);

		if ((*preading)[i].output_space >0) {
			(*preading)[i].output_space--;
			(*plast_output)[i] = L' ';
		}
		else if (is_paragraph_end(want_say, (*preading) + i)) {
			(*preading)[i].offset = 0;
			(*preading)[i].output_space = rand() % (boundary.Y / 2) + boundary.Y / 2;
			(*preading)[i].point = rand() % (want_say->num_of_point - 1);
			(*plast_output)[i] = L' ';
		}
		else {
			SetConsoleTextAttribute(StdOut,INTENSITY_WHITE);
			set_cursor_position(StdOut,(*parray_cursor)[i].X, (*parray_cursor)[i].Y);
			wchar_t output = get_char_from_words(want_say, (*preading)+i);
			putwchar(output);
			(*plast_output)[i] = output;
			(*preading)[i].offset++;
		}
	}

}

int main(void) {

	srand(2017);
	setlocale(LC_CTYPE, "");//Allow flying text to be Chinese or Japanese
	HANDLE screen_buffer = GetStdHandle(STD_OUTPUT_HANDLE);

	//render cursor invisible
	{	
		CONSOLE_CURSOR_INFO cursor_info = { 1,FALSE };
		SetConsoleCursorInfo(screen_buffer, &cursor_info);
	}

	Words want_say;
	read_words("words.txt",&want_say);

	COORD *cp=nullptr;
	Read_section *rs=nullptr;
	wchar_t * cl=nullptr;

	while (true) {
		CONSOLE_SCREEN_BUFFER_INFO screen_info;
		GetConsoleScreenBufferInfo(screen_buffer, &screen_info);
		//if the screen size changes, clear the screen and restart
		show_rain(screen_buffer, screen_info.dwMaximumWindowSize, &want_say,&cp,&rs,&cl);
		Sleep(80);
	}

	free(cp);
	free(rs);
	free(cl);
	free(want_say.words);
	free(want_say.pos_of_point);
	return 0;
}