#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LIMIT_FOR_CHARACTERS_IN_LINES 82
#define SAVEM fullFileName[strlen(fullFileName) - 1] = 'm';/*switchs the extention of the file to save as am*/


/**
 * -----------------
 * Preprocesses C code files by performing the following tasks:
 * 1. Extracts words between the specified mcro and endmcro in the code and creates a linked list.
 * 2. Replaces occurrences of the extracted words in the code with corresponding macro words.
 * 3. Removes comments from the code.
 * 4. Removes empty lines and tabs from the code.
 * 5. Saves the preprocessed code back into the original file.
 *
 * Parameters:
 * - argc: The number of command-line arguments (including the program name).
 * - argv: An array of C strings containing the command-line arguments.
 *         The first element (argv[0]) is the program name.
 *         The rest of the elements (argv[1] to argv[argc-1]) are file names of C code files to preprocess.
 *         Example: char* argv[] = {"program_name", "file1.c", "file2.c"};
 *
 * Returns:
 * - None (void).
 *
 * Note:
 * - The function modifies the input files in-place, replacing their content with preprocessed code.
 * - Ensure that the startWord and endWord provided in the code match the desired macro extraction pattern.
 * - Ensure that the input C code files exist and are accessible for reading and writing.
 * - The function may display error messages on stderr for files that cannot be opened or processed.
 */
char* pre_process(char* filename);

