#include "dir_file.h"
#include "utils.h"

void open_file(char* fname, FILE** input) {
	assert(fname != NULL);

	if (access(fname, F_OK) == -1) {
		fprintf(stderr, "The '%s' file does not exists\n",  fname);
		exit(1);
	}

	*input = fopen(fname, "r");

	if (input == NULL) {
		fprintf(stderr, "The '%s' file did not open\n", fname);
		exit(1);
	}

}

// This parse_str_to_list() split string to the tokens, and put the tokens in token_list.
// The value of return is the number of tokens.

int parse_str_to_list(const char* str, char*** token_list) {
    int len = 0; int tkCount = 0;
    for (int i = 0; str[i] != NULL; i++) {
        len++;
    }

    *token_list = (char**)malloc(sizeof(char*) * MAX_TOKEN_NUM);

    int start = 0;
    int buffer_flag = 0;
    while (start < len) {
        char* result;

        int end_index;
        int cnt = 0;
        buffer_flag = 0;

        for (int k = start; k <= len; k++) {            
            if (cnt >= MAX_BUFFER_SIZE) buffer_flag = 1;
            if (str[k] == '/' || str[k] == '\n' || str[k] == '\0' || cnt >= MAX_BUFFER_SIZE) {
                end_index = k;
                break;
            }
            cnt++;
        }

        //파싱할 단어가 있으면
        if (cnt != 0) {
            result = (char*)malloc(sizeof(char) * (cnt + 1));
            strncpy(result, str + start, cnt);
            result[cnt] = '\0';

            (*token_list)[tkCount] = result;

            tkCount++;
        }

        start += cnt + 1;
        if (buffer_flag == 1) start--;
    }

    return tkCount;
}


void free_token_list(char** token_list, int num_token) {
	int index;

	for (index = 0; index < num_token; index++) {
		free(token_list[index]);
	}

	free(token_list);
}