//******************************************************************************
//* File Name: Cloud_Protocol_Cfg.c
//* Project Name: JDL _AC_Ultra-Fast_Charger
//* Version: v1.0
//* Date: 2025-08-18 10:00:00
//* Author: JDLzhou
//* Description: Cloud Protocol module configuration source file
/*******************************************************************************
|    Other Header File Inclusion
|******************************************************************************/
#include "Cloud_Protocol_Cfg.h"

/*******************************************************************************
|    Macro Definition
|******************************************************************************/

/*******************************************************************************
|    Enum Definition
|******************************************************************************/

/*******************************************************************************
|    Typedef Definition
|******************************************************************************/

/*******************************************************************************
|    Static local KAM variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Static local variables Declaration
|******************************************************************************/

/*******************************************************************************
|    Table Const Definition
|******************************************************************************/

/*******************************************************************************
|    Static Local Functions Declaration
|******************************************************************************/

/*******************************************************************************
|    Function Source Code
|******************************************************************************/
char *Cloud_Protocol_Strdup(const char *s)
{
    if (s == NULL)
    {
        return NULL;
    }
    size_t len = strlen(s) + 1;
    char *new_str = (char *)CLOUDM_MALLOC(len);
    if (new_str != NULL)
    {
        strncpy(new_str, s, len);
        new_str[len - 1] = '\0';
    }
    return new_str;
}

/**
 * @brief Convert hex array to ASCII string using sprintf
 * @param hex_array Input hex array
 * @param array_size Size of hex array
 * @param ascii_string Output ASCII string buffer
 * @param buffer_size Size of output buffer
 * @return Length of ASCII string, -1 on error
 */
int Cloud_Protocol_Hex2Ascii(const uint8_t *hex_array, size_t array_size, char *ascii_string, size_t buffer_size)
{
    if (!hex_array || !ascii_string || buffer_size == 0)
    {
        return -1;
    }

    size_t required_size = array_size + 1;
    
    if (required_size > buffer_size)
    {
        return -1;
    }

    for (size_t i = 0; i < array_size; i++)
    {
        sprintf(ascii_string + i, "%c", hex_array[i]);
    }

    ascii_string[array_size] = '\0';

    return array_size;
}
/* EOL */
