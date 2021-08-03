#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define DEFAULT 0
#define EXACT 10
#define PREFIX 20
#define ANYWHERE 30
#define SORT 100

#define MAX_WORD_LENGTH = 41     // Used for malloc(), so include space for the
                                 // '\0' character.
#define MAX_FILEPATH_LENGTH 256

/*
 * Struct created to contain all the necessary information to perform a search
 */
typedef struct {
    int searchMode;
    int sort;
    char* pattern;
    char* filepath;
} Options;

/*
 * Function used to create a new struct. Important to 
 */
Options new_option() {
    Options options;
    options.searchMode = EXACT;
    options.sort = 0;
    options.pattern = malloc(sizeof(char) * MAX_FILEPATH_LENGTH);
    options.pattern = "";
    options.filepath = "/usr/share/dict/words";

    return options;
}

/*
 * Function used to set the Options.searchMode value in the Options struct.
 */
Options set_search_mode(Options searchParameter, char* searchMode) {
    if (!strcmp(searchMode, "-exact")) {
        searchParameter.searchMode = EXACT;
    } else if (!strcmp(searchMode, "-prefix")) {
        searchParameter.searchMode = PREFIX;
    } else if (!strcmp(searchMode, "-anywhere")) {
        searchParameter.searchMode = ANYWHERE;
    } else {
    }
    return searchParameter;
}

/*
 * Function used to display the values stored in an Options struct
 */
void display_options(Options options) {
    char* searchMode;
    char* sort;

    switch (options.searchMode) {
        case EXACT:
            searchMode = "-exact";
            break;
        case PREFIX:
            searchMode = "-prefix";
            break;
        case ANYWHERE:
            searchMode = "-anywhere";
            break;
    }

    if (options.sort == 0) {
        sort = "false";
    } else {
        sort = "true";
    }

    printf("\tsearch mode: %s\n\tsort: %s\n\tpattern: %s\n\tfilepath: %s\n",
            searchMode, sort, options.pattern, options.filepath);
}

/*
 * A function that prints out the specified message when the inputs are invalid
 * Return: None
 */
void invalid_command() {
    fprintf(stderr, "Usage: search [-exact|-prefix|-anywhere] [-sort] "
            "pattern [filename]\n");
}

/*
 * A function that checks whether the string passed is a valid 'option'.
 * Option strings are either '-exact', '-prefix' or '-anywhere'
 *
 * Return: 1 if str is a valid option, else 0.
 */
int is_option(char* str) {
    return (!strcmp(str, "-exact") || !strcmp(str, "-prefix")
            || !strcmp(str, "-anywhere"));
}

/*
 * Search algorithm for the '-exact' option, also used as the base of the 
 * '-prefix' and '-anywhere' search implementation.
 * Return: 1 if the string word matches the string pattern, else 0.
 */
int exact(char* pattern, char* word) {
    if (strlen(pattern) != strlen(word)) {
        return 0;
    }

    int state = 1;

    for (int i = 0; i < strlen(word); i++) {
        if (pattern[i] == '?') {
            // Do nothing
        } else if (pattern[i] != word[i]) {
            state = 0;
        }
    }

    return state;
}

/*
 * Search algorithm for the '-prefix' option.
 * Return 1 if the string word matches the first var(strlen(pattern)) chars
 * of the pattern.
 */
int prefix(char* pattern, char* word) {
    if (strlen(pattern) == 0) {
        return 1;
    }
    if (strlen(pattern) > strlen(word)) {
        return 0;
    }
           
    char* substr = malloc(sizeof(char) * strlen(word));
    strncpy(substr, word, strlen(pattern));
    int result = exact(pattern, substr);
    free(substr);
    return result;
}

/*
 * Search algorithm for the 'anywhere' option.
 * Return 1 if the string pattern is found in the word, else 0.
 */
int anywhere(char* pattern, char* word) {
    if (strlen(pattern) == 0) {
        return 1;
    }
    if (strlen(pattern) > strlen(word)) {
        return 0;
    }
                
    for (int i = 0; i <= strlen(word) - strlen(pattern); i++) {
        int wordSize = strlen(word) - strlen(pattern);
        char* searchWord = malloc(sizeof(char) * wordSize);
        strncpy(searchWord, word + i, strlen(pattern));
        if (exact(pattern, searchWord)) {
            return 1;
        }
        free(searchWord);
    }
    return 0;
}

/*
 * A method used to determine whether the character passed is either a
 * lower-case, upper-case character or not.
 *
 * Return: 1 if character is either lowercase or uppercase, else 0
 */
int is_letter(int asciiCode){
    return ((65 <= asciiCode && asciiCode <= 90) ||
            (97 <= asciiCode && asciiCode <= 122));
}

/*
 * A function that is used to determine whether the string parameter str is
 * a valid 'pattern' string. A valid pattern string is one that only contains
 * uppercase, lowercase or the question mark character ('?')
 *
 * Uppercase letters [65, 90]
 * Lowercase letters [97, 112]
 * Questionmark Char [63]
 *
 * Return: 1 if string is a valid pattern, else 0.
 */
int valid_pattern(char* str) {    
    for (int i = 0; i < strlen(str); i++) { 
        int asciiCode = ((int) str[i]);
        if (asciiCode != 63 && !is_letter(asciiCode)) {
            if (!strcmp(str, "-sort")) {
                invalid_command();
                exit(1);
            }

            fprintf(stderr, "search: pattern should only contain question "
                    "marks and letters\n");
            exit(1);
        }
    }
    return 1;
}

/*
 * A method that is used to determine whether an input is a valid 'word' to be 
 * parsed. A valid word is one that is:
 * 1. Less than or equal to 40 characters long.
 * 2. Only contains uppercase or lowercase characters (using valid_pattern)
 */
int valid_word(char* word) {
    if (strlen(word) > 40) {
        return 0; // Word is too long
    }

    for (int i = 0; i < strlen(word); i++) {
        int asciiCode = ((int) word[i]);
        if (!is_letter(asciiCode)) {
            return 0;
        }
    }
    return 1;
}

/*
 * A function that determines whether the filepath passed as a parameter
 * is a valid file or not.
 *
 * Return: 1 if filepath is a valid file path, else 0.
 */
int valid_file() {
    return 1;
}

Options argc_2_validation(char** argv);
Options argc_3_validation(char** argv);  
Options argc_4_validation(char** argv);  
Options argc_5_validation(char** argv);  

/*
 * A function that determines whether the passed arguments are in a valid order
 * and syntax.
 * Return: 1 if the arguments are valid, else 0.
 */
Options valid_args(int argc, char** argv) {
    if (argc == 2) {
        return argc_2_validation(argv);
    } else if (argc == 3) {
        //Options a = argc_3_validation(argv);
        //setSearchMode(a, "-prefix");
        return argc_3_validation(argv);
    } else if (argc == 4) {
        return argc_4_validation(argv);
    } else if (argc == 5) {
        return argc_5_validation(argv);
    } else {
        invalid_command();
        exit(0); // Incorrect number of arguments given
    }
}

/*
 * A function that determines whether the passed arguments are in valid order
 * and syntax.
 * Return: 1 if the arguments are valid, else 0
 */
Options argc_2_validation(char** argv) {
    if (!valid_pattern(argv[1])) {
        invalid_command();
        exit(0);
    } else {
        Options searchParameters = new_option();
        searchParameters.pattern = argv[1];
        return searchParameters;
    }
}

/*
 * A function that determines whether the passed arguments are in valid order
 * and syntax.
 * Return: 1 if the arguments are valid, else 0.
 */
Options argc_3_validation(char** argv) {
    Options searchParameters = new_option();
    if (is_option(argv[1]) && valid_pattern(argv[2])) {
        // ./search -option $pattern
        searchParameters = set_search_mode(searchParameters, argv[1]);
        searchParameters.pattern = argv[2];
        return searchParameters;
    } else if (!strcmp(argv[1], "-sort") && valid_pattern(argv[2])) {
        // ./search -sort $pattern, equivalent to
        // ./search -exact -sort $pattern
        searchParameters.sort = 1;
        searchParameters.pattern = argv[2];
        return searchParameters;
    } else if (valid_pattern(argv[1]) && valid_file(argv[2])) {
        // ./search $pattern $filename
        searchParameters.pattern = argv[1];
        searchParameters.filepath = argv[2];
        return searchParameters;
    }
    invalid_command();
    exit(1);
}

/*
 * A funtion that determines whether the arguments passed are in valid order 
 * and syntax.
 * Return: 1 if the arguments are valid, else 0.
 */
Options argc_4_validation(char** argv) {
    Options searchParameters = new_option();
    if (is_option(argv[1])) {
        if (!strcmp(argv[2], "-sort") && valid_pattern(argv[3])) {
            // ./search -option -sort $pattern
            searchParameters.sort = 1;
            searchParameters.pattern = argv[3];
            searchParameters = set_search_mode(searchParameters, argv[1]);
            return searchParameters;
        } else if (valid_pattern(argv[2]) && valid_file(argv[3])) {
            // ./search -option $pattern $filename
            searchParameters = set_search_mode(searchParameters, argv[1]);
            searchParameters.pattern = argv[2];
            searchParameters.filepath = argv[3];
            return searchParameters;
        } 
    } else if (!strcmp(argv[1], "-sort")) {
        if (is_option(argv[2]) && valid_pattern(argv[3])) {
            // ./search -sort -option $pattern
            // Options searchParameters = newOption();
            searchParameters.sort = 1;
            searchParameters = set_search_mode(searchParameters, argv[2]);
            // searchParameters.searchMode = argv[2];
            searchParameters.pattern = argv[3];
            return searchParameters;
        } else if (valid_pattern(argv[2]) && valid_file(argv[3])) {
            // ./search -sort $pattern $filename
            // Options searchParameters = newOption();
            searchParameters.sort = 1;
            searchParameters.pattern = argv[2];
            searchParameters.filepath = argv[3];
            return searchParameters;
        }
    }
    invalid_command();
    exit(1);
}

/*
 * A function that determiens whether the passed arguments are in a valid order
 * and syntax.
 * Return: 1 if the arguments are valid, else 0.
 */
Options argc_5_validation(char** argv) {
    Options searchParameters = new_option();
    if (is_option(argv[1]) && !strcmp(argv[2], "-sort")) {
        // ./search -option -sort $pattern $filename
        // Options searchParameters = newOption();
        searchParameters = set_search_mode(searchParameters, argv[1]);
        // searchParameters.searchMode = argv[1];
        searchParameters.sort = 1;
        searchParameters.pattern = argv[3];
        searchParameters.filepath = argv[4];
        return searchParameters;
    } else if (!strcmp(argv[1], "-sort") && is_option(argv[2])) {
        // ./search -sort -option $pattern $filename
        // Options searchParameters = newOption();
        searchParameters.sort = 1;
        searchParameters = set_search_mode(searchParameters, argv[2]);
        // searchParameters.searchMode = argv[2];
        searchParameters.pattern = argv[3];
        searchParameters.filepath = argv[4];
        return searchParameters;
    }
    invalid_command();
    exit(0);
}

/*
 * Replace all of the characters in the string str with it's lowercase
 * representation.
 */
void lower_string(char* str) {
    for (int i = 0; i < strlen(str); i++) {
        str[i] = tolower(str[i]);
    }
}

/*
 * Comparison function for the qsort algorithm, used to determine the 
 * precendence of words in the array to be sorted.
 */
int cmp(const void* a, const void* b) {
    return strcmp(*(char **)a, *(char **)b);
}

/*
 * Entry point of the script.
 */
int main(int argc, char** argv) {
    Options searchParameters = valid_args(argc, argv);
    // Method should exit if the arguments provided are invalid.
    // Open the file
    FILE* file;
    int bufferLength = 40; // Max line size
    char* filename = searchParameters.filepath;
    char buffer[bufferLength];
    file = fopen(filename, "r");

    // Deal with the case where the filename is invalid;
    if (file == NULL) {
        fprintf(stderr, "search: file \"%s\" can not be opened\n", filename);     
        exit(1);
    }

    int lineNumber = 0;
    int count = 0;
    
    while (fgets(buffer, bufferLength, file)) {
        char* word = malloc(sizeof(char) * (strlen(buffer) + 1));
        char* displayWord = malloc(sizeof(char) * (strlen(buffer) + 1));
        
        // Remove the line break from the end of the buffer
        if (buffer[strlen(buffer) - 1] == '\n') {
            strncpy(word, buffer, strlen(buffer) - 1);
            strncpy(displayWord, buffer, strlen(buffer) - 1);
        }
        
        lower_string(word);
        if (valid_word(word)) { 
            if (searchParameters.searchMode == EXACT) {
                if (exact(searchParameters.pattern, word)) {
                    if (searchParameters.sort) {
                        count++;
                    } else {
                        fprintf(stdout, "%s\n", displayWord);
                    }
                }
            } else if (searchParameters.searchMode == PREFIX) {
                if (prefix(searchParameters.pattern, word)) {
                    if (searchParameters.sort) {
                        count++;
                    } else {
                        fprintf(stdout, "%s\n", displayWord);
                    }
                }
            } else if (searchParameters.searchMode == ANYWHERE) {
                if (anywhere(searchParameters.pattern, word)) {
                    if (searchParameters.sort) {
                        count++;
                    } else { 
                        fprintf(stdout, "%s\n", displayWord);
                    }
                }
            }
        }
        lineNumber++;
    }
    
    // Seek to the start of the file
    rewind(file);
    
    // Algorithm for populating and sorting the dynamically created array
    if (searchParameters.sort) {
        // Construct the array;
        char** toSort = malloc(count * sizeof(char*));

        // Allocate memory for each element in the array;
        for (int i = 0; i < count; i++) {
            toSort[i] = malloc(sizeof(char) * 50);
        }

        int ctr = 0;
        int searchMode = searchParameters.searchMode;
        char* pattern = malloc(sizeof(char) * bufferLength);
        pattern = strdup(searchParameters.pattern);

        while (fgets(buffer, bufferLength, file)) {
            char* displayWord = malloc(sizeof(char) * strlen(buffer) + 1);
            char* word = malloc(sizeof(char) * strlen(buffer) + 1);
            if (buffer[strlen(buffer) - 1] == '\n') {
                strncpy(word, buffer, strlen(buffer) - 1);
                strncpy(displayWord, buffer, strlen(buffer) - 1);
            } else {
                strncpy(word, buffer, strlen(buffer));
                strncpy(displayWord, buffer, strlen(buffer));
            }

            if (valid_word(word)) {
                if (searchMode == EXACT && exact(pattern, word)) {
                    strncpy(toSort[ctr], strdup(word), 50);   
                    ctr++;
                } else if (searchMode == PREFIX && prefix(pattern, word)) {
                    strncpy(toSort[ctr], strdup(word), 50);
                    ctr++;
                } else if (searchMode == ANYWHERE && anywhere(pattern, word)) {
                    strncpy(toSort[ctr], strdup(word), 50);
                    ctr++;
                } else {
                    // Weird error state :(
                }
            }
        }

        // Now, sort the array;
        qsort(toSort, ctr, sizeof(char*), cmp);

        // Display the results:
        for (int i = 0; i < ctr; i++) {   
            printf("%s\n", toSort[i]);
        }
    }

    return 0; // Program finished executing nominally.
}

