#include"Words.h"
bool is_index_of_PosOfParagraph_legal(const Words * rec, int index_of_point) {
	if (index_of_point >= rec->num_of_point - 1)
		return false;
	if (index_of_point < 0)
		return false;
	return true;
}
bool is_paragraph_end(const Words *get_from, const Read_section *reading) {
	if (get_from->pos_of_point[reading->point] + reading->offset >= get_from->pos_of_point[reading->point + 1])
		return true;
	return false;
}
wchar_t get_char_from_words(const Words *get_from, const Read_section *reading) {
	return get_from->words[get_from->pos_of_point[reading->point] + reading->offset];
}

void read_words(const char *file_name, Words *rec) {
	FILE *file = fopen(file_name, "r");
	if (file == nullptr) {
		printf("file:%s doesn't exist\n", file_name);
		system("pause");
		exit(0);
	}
	int max_words = 500;
	int max_sentence = 10;
	rec->words = (wchar_t *)malloc(sizeof(wchar_t)*max_words);
	rec->pos_of_point = (int *)malloc(sizeof(int)*max_sentence);

	rec->num_of_point = 0;
	rec->pos_of_point[rec->num_of_point++] = 0;

	if (rec->words == nullptr || rec->pos_of_point == nullptr) {
		printf("run out of memeory\n");
		system("pause");
		exit(0);
	}

	int count_words = 0;
	while ((rec->words[count_words] = fgetwc(file)) != WEOF) {
		if (rec->words[count_words] == '\n') {
			rec->pos_of_point[rec->num_of_point++] = count_words;
			if (rec->num_of_point == max_sentence - 1) {
				int *temp = (int *)malloc(sizeof(int)*max_sentence + 5);
				for (int i = 0; i < max_sentence; i++)
					temp[i] = rec->pos_of_point[i];
				max_sentence += 5;
				free(rec->pos_of_point);
				rec->pos_of_point = temp;
			}
			continue;
		}
		if (count_words == max_words - 1) {
			max_words += 100;
			wchar_t a_char_temp = rec->words[count_words];
			rec->words[count_words] = L'\0';
			wchar_t *temp = (wchar_t *)malloc(sizeof(wchar_t)*max_words);
			wcscpy(temp, rec->words);
			free(rec->words);
			rec->words = temp;
			rec->words[count_words] = a_char_temp;
		}
		count_words++;
	}
	rec->words[count_words] = L'\0';
	if (rec->pos_of_point[rec->num_of_point - 1] != count_words)
		rec->pos_of_point[rec->num_of_point++] = count_words;

	//adjust arrays
	wchar_t *words_copy = (wchar_t *)malloc(sizeof(wchar_t)*(count_words + 1));
	wcscpy(words_copy, rec->words);
	int *pos_of_sentence_copy = (int *)malloc(sizeof(int)*rec->num_of_point);
	for (int i = 0; i < rec->num_of_point; i++) {
		pos_of_sentence_copy[i] = rec->pos_of_point[i];
	}

	free(rec->words); free(rec->pos_of_point);
	rec->words = words_copy; rec->pos_of_point = pos_of_sentence_copy;
	fclose(file);
}
