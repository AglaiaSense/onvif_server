
/*-------------------------------------------------------------------------*/
/**
   @file    iniparser.h
   @author  N. Devillard
   @date    Mar 2000
   @version $Revision: 1.3 $
   @brief   Parser for ini files.
*/
/*--------------------------------------------------------------------------*/

/*
	$Id: iniparser.h,v 1.3 2006/04/08 06:16:55 qbin Exp $
	$Author: qbin $
	$Date: 2006/04/08 06:16:55 $
	$Revision: 1.3 $
*/

#ifndef _INIPARSER_H_
#define _INIPARSER_H_

/*---------------------------------------------------------------------------
   								Includes
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <unistd.h> 
*/
/*
 * The following #include is necessary on many Unixes but not Linux.
 * It is not needed for Windows platforms.
 * Uncomment it if needed.
 */


#include "dictionary.h"

int iniparser_add_entry(
    dictionary * d,
    char * sec,
    char * key,
    char * val);/*20060311 blair modify : ���Ӽ�¼*/

/*-------------------------------------------------------------------------*/
/**
  @brief    Get number of sections in a dictionary
  @param    d   Dictionary to examine
  @return   int Number of sections found in dictionary

  This function returns the number of sections found in a dictionary.
  The test to recognize sections is done on the string stored in the
  dictionary: a section name is given as "section" whereas a key is
  stored as "section:key", thus the test looks for entries that do not
  contain a colon.

  This clearly fails in the case a section name contains a colon, but
  this should simply be avoided.

  This function returns -1 in case of error.
 */
/*--------------------------------------------------------------------------*/

int iniparser_getnsec(dictionary * d);


/*-------------------------------------------------------------------------*/
/**
  @brief    Get name for section n in a dictionary.
  @param    d   Dictionary to examine
  @param    n   Section number (from 0 to nsec-1).
  @return   Pointer to char string

  This function locates the n-th section in a dictionary and returns
  its name as a pointer to a string statically allocated inside the
  dictionary. Do not free or modify the returned string!

  This function returns NULL in case of error.
 */
/*--------------------------------------------------------------------------*/

char * iniparser_getsecname(dictionary * d, int n);


/*-------------------------------------------------------------------------*/
/**
  @brief    Save a dictionary to a loadable ini file
  @param    d   Dictionary to dump
  @param    f   Opened file pointer to dump to
  @return   void

  This function dumps a given dictionary into a loadable ini file.
  It is Ok to specify @c stderr or @c stdout as output files.
 */
/*--------------------------------------------------------------------------*/

void iniparser_dump_ini(dictionary * d, FILE * f);

/*-------------------------------------------------------------------------*/
/**
  @brief    Dump a dictionary to an opened file pointer.
  @param    d   Dictionary to dump.
  @param    f   Opened file pointer to dump to.
  @return   void

  This function prints out the contents of a dictionary, one element by
  line, onto the provided file pointer. It is OK to specify @c stderr
  or @c stdout as output files. This function is meant for debugging
  purposes mostly.
 */
/*--------------------------------------------------------------------------*/
void iniparser_dump(dictionary * d, FILE * f);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, return NULL if not found
  @param    d   Dictionary to search
  @param    key Key string to look for
  @return   pointer to statically allocated character string, or NULL.

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  NULL is returned.
  The returned char pointer is pointing to a string allocated in
  the dictionary, do not free or modify it.

  This function is only provided for backwards compatibility with
  previous versions of iniparser. It is recommended to use
  iniparser_getstring() instead.
 */
/*--------------------------------------------------------------------------*/
char * iniparser_getstr(dictionary * d, char * key);


/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key
  @param    d       Dictionary to search
  @param    key     Key string to look for
  @param    def     Default value to return if key not found.
  @return   pointer to statically allocated character string

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the pointer passed as 'def' is returned.
  The returned char pointer is pointing to a string allocated in
  the dictionary, do not free or modify it.
 */
/*--------------------------------------------------------------------------*/
char * iniparser_getstring(dictionary * d, char * key, char * def);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to an int
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   integer

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.
 */
/*--------------------------------------------------------------------------*/
int iniparser_getint(dictionary * d, char * key, int notfound);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a double
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   double

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.
 */
/*--------------------------------------------------------------------------*/
double iniparser_getdouble(dictionary * d, char * key, double notfound);

/*-------------------------------------------------------------------------*/
/**
  @brief    Get the string associated to a key, convert to a boolean
  @param    d Dictionary to search
  @param    key Key string to look for
  @param    notfound Value to return in case of error
  @return   integer

  This function queries a dictionary for a key. A key as read from an
  ini file is given as "section:key". If the key cannot be found,
  the notfound value is returned.

  A true boolean is found if one of the following is matched:

  - A string starting with 'y'
  - A string starting with 'Y'
  - A string starting with 't'
  - A string starting with 'T'
  - A string starting with '1'

  A false boolean is found if one of the following is matched:

  - A string starting with 'n'
  - A string starting with 'N'
  - A string starting with 'f'
  - A string starting with 'F'
  - A string starting with '0'

  The notfound value returned if no boolean is identified, does not
  necessarily have to be 0 or 1.
 */
/*--------------------------------------------------------------------------*/
int iniparser_getboolean(dictionary * d, char * key, int notfound);


/*-------------------------------------------------------------------------*/
/**
  @brief    Set an entry in a dictionary.
  @param    ini     Dictionary to modify.
  @param    entry   Entry to modify (entry name)
  @param    val     New value to associate to the entry.
  @return   int 0 if Ok, -1 otherwise.

  If the given entry can be found in the dictionary, it is modified to
  contain the provided value. If it cannot be found, -1 is returned.
  It is Ok to set val to NULL.
 */
/*--------------------------------------------------------------------------*/

int iniparser_setstr(dictionary * ini, char * entry, char * val);

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete an entry in a dictionary
  @param    ini     Dictionary to modify
  @param    entry   Entry to delete (entry name)
  @return   void

  If the given entry can be found, it is deleted from the dictionary.
 */
/*--------------------------------------------------------------------------*/
void iniparser_unset(dictionary * ini, char * entry);

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete an section in a dictionary
  @param    ini     Dictionary to modify
  @param    entry   section to delete (entry name)
  @return   void

  If the given entry can be found, it is deleted from the dictionary.
 */
/*--------------------------------------------------------------------------*/

void iniparser_sec_unset(dictionary * d, char *sec);


/*-------------------------------------------------------------------------*/
/**
  @brief    Finds out if a given entry exists in a dictionary
  @param    ini     Dictionary to search
  @param    entry   Name of the entry to look for
  @return   integer 1 if entry exists, 0 otherwise

  Finds out if a given entry exists in the dictionary. Since sections
  are stored as keys with NULL associated values, this is the only way
  of querying for the presence of sections in a dictionary.
 */
/*--------------------------------------------------------------------------*/
int iniparser_find_entry(dictionary * ini, char * entry) ;

/*-------------------------------------------------------------------------*/
/**
  @brief    Parse an ini file and return an allocated dictionary object
  @param    ininame Name of the ini file to read.
  @return   Pointer to newly allocated dictionary

  This is the parser for ini files. This function is called, providing
  the name of the file to be read. It returns a dictionary object that
  should not be accessed directly, but through accessor functions
  instead.

  The returned dictionary must be freed using iniparser_freedict().
 */
/*--------------------------------------------------------------------------*/
dictionary * iniparser_load(char * ininame);


/*2006/03/11 blair add : save comment and space Line*/
int iniparser_add_comment(
    dictionary          *d, 
    const char          *sec, 
    const char          *key, 
    const char          *comment,
    const unsigned char place);

    
/*-------------------------------------------------------------------------*/
/**
  @brief    Free all memory associated to an ini dictionary
  @param    d Dictionary to free
  @return   void

  Free all memory associated to an ini dictionary.
  It is mandatory to call this function before the dictionary object
  gets out of the current context.
 */
/*--------------------------------------------------------------------------*/
void iniparser_freedict(dictionary * d);

int iniparser_sec_getNLowLever(dictionary * d,char* UperLever);
char* iniparser_sec_getLowLever(dictionary * d,char* UpLever, int n);
#endif
