#ifndef TEXT_H_INCLUDED
#define TEXT_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <ctype.h>
#include <time.h>

const int CURRENT_TIME_LENGTH = 22;

enum read_regime
{
    NO_COMMENTS    = 0,
    DEFAULT_REGIME = 1,
};

#ifndef DEBUG_PRINT
    #define DEBUG_PRINT printf
#endif

#ifndef TRUE
    #define TRUE 1
#endif

//-----------------------------------------------------------------------------
//! The structure of line
//! @version 2.0
//! @authors Vokerlee
//! @brief Consists of the pointer the beginning of line, its length, and real position in text
//-----------------------------------------------------------------------------

struct line_t
{
    char *line = nullptr;

    size_t length = 0;

    size_t real_num_line = 0;
};

//-----------------------------------------------------------------------------
//! The structure of text
//! @version 3.0
//! @authors Vokerlee
//! @brief Consists of buffer (text from file), massive of lines, and useful info about text
//-----------------------------------------------------------------------------

struct text_t
{
    char *buffer  = nullptr;
    line_t *lines = nullptr;

    size_t n_symbols    = 0;
    size_t n_real_lines = 0;
    size_t n_lines      = 0;

    int line_counter = 0;
    char *counter    = nullptr;
};

//-----------------------------------------------------------------------------
//! Text-constructor
//! @param [in] text - the structure of text
//! @version 1.0
//! @authors Vokerlee
//! @brief Constructs text
//-----------------------------------------------------------------------------

void construct_text (text_t *text);

//-----------------------------------------------------------------------------
//! Text-destructor
//! @param [in] text - the structure of text
//! @version 1.0
//! @authors Vokerlee
//! @brief Destructs text, releasing all buff-memory
//-----------------------------------------------------------------------------

void destruct_text (text_t *text);

//-----------------------------------------------------------------------------
//! Fills all elements in text-structure, using source-file
//! @param [in] source      - the stream of source-file
//! @param [in] text        - the structure of text
//! @param [in] read_regime - the regime of text-filling
//! @version 4.0
//! @authors Vokerlee
//! @brief Fills all elements in text-structure, using source-file. If regime = "no comments", everything after "//" is ignored
//-----------------------------------------------------------------------------

void fill_text (FILE *source, text_t *text, char read_regime = DEFAULT_REGIME);

//-----------------------------------------------------------------------------
//! Reads file and writes in text-structure
//! @param [in] source      - the stream of source-file
//! @param [in] text        - the structure of text
//! @param [in] read_regime - the regime of text-filling (look function fill_text(...))
//! @version 4.0
//! @authors Vokerlee
//! @brief Reads file and writes in text-structure, filling text-elements with correct values
//-----------------------------------------------------------------------------

void read_file (FILE *source, text_t *text, char read_regime);

//-----------------------------------------------------------------------------
//! Creates massive of lines
//! @param [in] text        - the structure of text
//! @param [in] read_regime - the regime of text-filling (look function fill_text(...))
//! @version 3.0
//! @authors Vokerlee
//! @brief Creates massive of lines, using the info about text, founded by other functions (vide supra)
//-----------------------------------------------------------------------------

void fill_text_lines (text_t *text, char read_regime);

//-----------------------------------------------------------------------------
//! Fills the info in text about the amount of symbols in source
//! @param [in] source - the stream of source-file
//! @param [in] text   - the structure of text
//! @version 3.0
//! @authors Vokerlee
//! @brief Fills the info in text about the amount of symbols in source
//-----------------------------------------------------------------------------

void find_length_of_file (FILE *source, text_t *text);

//-----------------------------------------------------------------------------
//! Fills the info in text about the amount of lines and real lines in source
//! @param [in] text   - the structure of text
//! @param [in] regime - the regime of text-filling (look function fill_text(...))
//! @version 3.0
//! @authors Vokerlee
//! @brief Fills the info in text about the amount of lines and real lines in source
//-----------------------------------------------------------------------------

void find_lines_of_file (text_t *text, char read_regime);

//-----------------------------------------------------------------------------
//! Prints all structures
//! @param [in] res  - the stream of results-file
//! @param [in] text - the structure of text
//! @version 3.0
//! @authors Vokerlee
//! @brief Print all structures "lines" (in struct text) in "res"
//-----------------------------------------------------------------------------

void print_text_lines (FILE *res, text_t *text);

//-----------------------------------------------------------------------------
//! Prints the current time in time_line
//! @param [in] time_line - the line, where the current time will be written
//! @version 1.0
//! @authors Vokerlee
//! @brief Prints the current time in time_line
//-----------------------------------------------------------------------------

void current_time (char *time_line);

#endif // TEXT_H_INCLUDED
