
/*-------------------------------------------------------------------------*/
/**
   @file	dictionary.c
   @author	N. Devillard
   @date	Aug 2000
   @version	$Revision: 1.3 $
   @brief	Implements a dictionary for string variables.

   This module implements a simple dictionary object, i.e. a list
   of string/string associations. This object is useful to store e.g.
   informations retrieved from a configuration file (ini files).
*/
/*--------------------------------------------------------------------------*/

/*
	$Id: dictionary.c,v 1.3 2006/04/08 07:08:14 qbin Exp $
	$Author: qbin $
	$Date: 2006/04/08 07:08:14 $
	$Revision: 1.3 $
*/

/*---------------------------------------------------------------------------
   								Includes
 ---------------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include "dictionary.h"

/** Maximum value size for integers and doubles. */
#define MAXVALSZ	512

/** Minimal allocated number of entries in a dictionary */
#define DICTMINSZ	512

/*2006/03/11 blair add : save comment and space Line*/
/** Minimal allocated number of commnet in a dictionary */
#define DICTCOMMENTMINSZ	512

/** Invalid key token */
#define DICT_INVALID_KEY    ((char*)-1)


/*---------------------------------------------------------------------------
  							Private functions
 ---------------------------------------------------------------------------*/

/* Doubles the allocated size associated to a pointer */
/* 'size' is the current allocated size. */
void * mem_double(void * ptr, int size)
{
    void    *   newptr ;
 
    newptr = (char **)calloc(2*size, 1);
    memcpy(newptr, ptr, size);
    free(ptr);
    return newptr ;
}


/*---------------------------------------------------------------------------
  							Function codes
 ---------------------------------------------------------------------------*/

/*-------------------------------------------------------------------------*/
/**
  @brief	Compute the hash key for a string.
  @param	key		Character string to use for key.
  @return	1 unsigned int on at least 32 bits.

  This hash function has been taken from an Article in Dr Dobbs Journal.
  This is normally a collision-free function, distributing keys evenly.
  The key is stored anyway in the struct so that collision can be avoided
  by comparing the key itself in last resort.
 */
/*--------------------------------------------------------------------------*/

unsigned dictionary_hash(char * key)
{
	int			len ;
	unsigned	hash ;
	int			i ;

	len = strlen(key);
	for (hash=0, i=0 ; i<len ; i++) {
		hash += (unsigned)key[i] ;
		hash += (hash<<10);
		hash ^= (hash>>6) ;
	}
	hash += (hash <<3);
	hash ^= (hash >>11);
	hash += (hash <<15);
	return hash ;
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Create a new dictionary object.
  @param	size	Optional initial size of the dictionary.
  @return	1 newly allocated dictionary objet.

  This function allocates a new dictionary object of given size and returns
  it. If you do not know in advance (roughly) the number of entries in the
  dictionary, give size=0.
 */
/*--------------------------------------------------------------------------*/

dictionary * dictionary_new(int size)
{
	dictionary	*	d ;

	/* If no size was specified, allocate space for DICTMINSZ */
	if (size<DICTMINSZ) size=DICTMINSZ ;

	d = (dictionary	*)calloc(1, sizeof(dictionary));
	d->size = size ;
	
	d->val  = (char **)calloc(size, sizeof(char*));
	memset(d->val, 0, size*sizeof(char*));
	
	d->key  = (char **)calloc(size, sizeof(char*));
	memset(d->key, 0, size*sizeof(char*));
	
	d->hash = (unsigned int *)calloc(size, sizeof(unsigned));
	memset(d->hash, 0, size*sizeof(unsigned));

	//add by sy
	d->sec_hash = (unsigned int*)calloc(size, sizeof(unsigned));
	memset(d->sec_hash, 0, size*sizeof(unsigned));

	/*2006/03/11 blair add : save comment and space Line*/
	if (size<DICTCOMMENTMINSZ) size=DICTCOMMENTMINSZ ;
	d->commSize = size ;

	d->comment  = (char **)calloc(size, sizeof(char*));
	memset(d->comment, 0, size*sizeof(char*));
	
	d->commHash = (unsigned int*)calloc(size, sizeof(unsigned));
	memset(d->commHash, 0, size*sizeof(unsigned));
	
	d->commPlace= (unsigned char *)calloc(size, sizeof(unsigned char));
	memset(d->commPlace, 0, size*sizeof(unsigned char));

	return d ;
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Delete a dictionary object
  @param	d	dictionary object to deallocate.
  @return	void

  Deallocate a dictionary object and all memory associated to it.
 */
/*--------------------------------------------------------------------------*/

void dictionary_del(dictionary * d)
{
	int		i ;

	if (d==NULL) return ;
	for (i=0 ; i<d->size ; i++) {
		if (d->key[i]!=NULL)
			free(d->key[i]);
		if (d->val[i]!=NULL)
			free(d->val[i]);
	}

    /*2006/03/11 blair add : save comment and space Line*/
	for (i=0 ; i<d->commSize ; i++) {
		if (d->comment[i]!=NULL)
			free(d->comment[i]);
	}

	free(d->comment);
	free(d->commHash);
	free(d->commPlace);
	
	free(d->val);
	free(d->key);
	free(d->hash);
	free(d->sec_hash);//by sy
	free(d);
	return ;
}



/*-------------------------------------------------------------------------*/
/**
  @brief	Get a value from a dictionary.
  @param	d		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @param    def     Default value to return if key not found.
  @return	1 pointer to internally allocated character string.

  This function locates a key in a dictionary and returns a pointer to its
  value, or the passed 'def' pointer if no such key can be found in
  dictionary. The returned character pointer points to data internal to the
  dictionary object, you should not try to free it or modify it.
 */
/*--------------------------------------------------------------------------*/
char * dictionary_get(dictionary * d, char * key, char * def)
{
	unsigned	hash ;
	int			i ;

	hash = dictionary_hash(key);
	for (i=0 ; i<d->size ; i++) {
        if (d->key==NULL)
            continue ;
        /* Compare hash */
		if (hash==d->hash[i]) {
            /* Compare string, to avoid hash collisions */
            if (!strcmp(key, d->key[i])) {
				return d->val[i] ;
			}
		}
	}
	return def ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief	Get a value from a dictionary, as a char.
  @param	d		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @param	def		Default value for the key if not found.
  @return 	char	

  This function locates a key in a dictionary using dictionary_get,
  and returns the first char of the found string.
 */
/*--------------------------------------------------------------------------*/
char dictionary_getchar(dictionary * d, char * key, char def)
{
	char * v ;

	if ((v=dictionary_get(d,key,DICT_INVALID_KEY))==DICT_INVALID_KEY) {
		return def ;
	} else {
		return v[0] ;
	}
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Get a value from a dictionary, as an int.
  @param	d		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @param	def		Default value for the key if not found.
  @return	int

  This function locates a key in a dictionary using dictionary_get,
  and applies atoi on it to return an int. If the value cannot be found
  in the dictionary, the default is returned.
 */
/*--------------------------------------------------------------------------*/
int dictionary_getint(dictionary * d, char * key, int def)
{
	char * v ;

	if ((v=dictionary_get(d,key,DICT_INVALID_KEY))==DICT_INVALID_KEY) {
		return def ;
	} else {
		return atoi(v);
	}
}

/*-------------------------------------------------------------------------*/
/**
  @brief		Get a value from a dictionary, as a double.
  @param	d		dictionary object to search.
  @param	key		Key to look for in the dictionary.
  @param	def		Default value for the key if not found.
  @return	double

  This function locates a key in a dictionary using dictionary_get,
  and applies atof on it to return a double. If the value cannot be found
  in the dictionary, the default is returned.
 */
/*--------------------------------------------------------------------------*/
double dictionary_getdouble(dictionary * d, char * key, double def)
{
	char * v ;

	if ((v=dictionary_get(d,key,DICT_INVALID_KEY))==DICT_INVALID_KEY) {
		return def ;
	} else {
		return atof(v);
	}
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Set a value in a dictionary.
  @param	d		dictionary object to modify.
  @param	key		Key to modify or add.
  @param	val 	Value to add.
  @return	void

  If the given key is found in the dictionary, the associated value is
  replaced by the provided one. If the key cannot be found in the
  dictionary, it is added to it.

  It is Ok to provide a NULL value for val, but NULL values for the dictionary
  or the key are considered as errors: the function will return immediately
  in such a case.

  Notice that if you dictionary_set a variable to NULL, a call to
  dictionary_get will return a NULL value: the variable will be found, and
  its value (NULL) is returned. In other words, setting the variable
  content to NULL is equivalent to deleting the variable from the
  dictionary. It is not possible (in this implementation) to have a key in
  the dictionary without value.
 */
/*--------------------------------------------------------------------------*/
/*if exist then set and return 0;
  if not exist then return 1;*/
int dictionary_set(dictionary * d, char * key, char * val)
{
	int			i ;
	unsigned	hash ;

	if (d==NULL || key==NULL) return -1;
	
	/* Compute hash for this key */
	hash = dictionary_hash(key) ;
	/* Find if value is already in blackboard */
	if (d->n>0) {
		for (i=0 ; i<d->size ; i++) {
            if (d->key[i]==NULL)
                continue ;
			if (hash==d->hash[i]) { /* Same hash value */
				if (!strcmp(key, d->key[i])) {	 /* Same key */
					/* Found a value: modify and return */
					if (d->val[i]!=NULL)
						free(d->val[i]);
                    d->val[i] = val ? strdup(val) : NULL ;
                    /* Value has been modified: return */
					return 0;
				}
			}
		}
	}

	return -1;

}

/*if exist then return 1;
  if not exist then add and return 0;*/
int dictionary_add(dictionary * d, char * key, char * val)
{
	int			i ;
	unsigned	hash ;

	if (d==NULL || key==NULL) return -1;
	
	/* Compute hash for this key */
	hash = dictionary_hash(key) ;
	/* Find if value is already in blackboard */
	if (d->n>0) {
		for (i=0 ; i<d->size ; i++) {
            if (d->key[i]==NULL)
                continue ;
			if (hash==d->hash[i]) { /* Same hash value */
				if (!strcmp(key, d->key[i])) {	 /* Same key */
					/* Found a value: return 1*/
    				return -1;
				}
			}
		}
	}
	/* Add a new value */
	/* See if dictionary needs to grow */
	if (d->n==d->size) {

		/* Reached maximum size: reallocate blackboard */
		d->val  = (char **)mem_double(d->val,  d->size * sizeof(char*)) ;
		d->key  = (char **)mem_double(d->key,  d->size * sizeof(char*)) ;
		d->hash = (unsigned int*)mem_double(d->hash, d->size * sizeof(unsigned)) ;
		d->sec_hash = (unsigned int*)mem_double(d->sec_hash, d->size * sizeof(unsigned)) ;

		/* Double size */
		d->size *= 2 ;
	}

    /* Insert key in the first empty slot */
    for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL) {
            /* Add key here */
            break ;
        }
    }
	/* Copy key */
	d->key[i]  = strdup(key);
    d->val[i]  = val ? strdup(val) : NULL ;
	d->hash[i] = hash;
	//by sy
	char sec[128];
	strcpy(sec, key);
	char *psec = strstr(sec, ":");
	if(psec != NULL)
	{	
		*psec = '\0';
		d->sec_hash[i] = dictionary_hash(sec);
	}
	else
	{
		d->sec_hash[i] = hash;
	}
	d->n ++ ;
	return 0;
}

/*if exist then set and return 0;
  if not exist then add and return 0;*/
int dictionary_modify(dictionary * d, char * key, char * val)
{
	int			i ;
	unsigned	hash ;

	if (d==NULL || key==NULL) return -1;
	
	/* Compute hash for this key */
	hash = dictionary_hash(key) ;
	/* Find if value is already in blackboard */
	if (d->n>0) {
		for (i=0 ; i<d->size ; i++) {
            if (d->key[i]==NULL)
                continue ;
			if (hash==d->hash[i]) { /* Same hash value */
				if (!strcmp(key, d->key[i])) {	 /* Same key */
					/* Found a value: modify and return */
					if (d->val[i]!=NULL)
						free(d->val[i]);
                    d->val[i] = val ? strdup(val) : NULL ;
                    /* Value has been modified: return */
					return 0;
				}
			}
		}
	}
	/* Add a new value */
	/* See if dictionary needs to grow */
	if (d->n==d->size) {

		/* Reached maximum size: reallocate blackboard */
		d->val  = (char**)mem_double(d->val,  d->size * sizeof(char*)) ;
		d->key  = (char**)mem_double(d->key,  d->size * sizeof(char*)) ;
		d->hash = (unsigned int*)mem_double(d->hash, d->size * sizeof(unsigned)) ;
		d->sec_hash = (unsigned int*)mem_double(d->sec_hash, d->size * sizeof(unsigned)) ;

		/* Double size */
		d->size *= 2 ;
	}

    /* Insert key in the first empty slot */
    for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL) {
            /* Add key here */
            break ;
        }
    }
	/* Copy key */
	d->key[i]  = strdup(key);
    d->val[i]  = val ? strdup(val) : NULL ;
	d->hash[i] = hash;
	//by sy
	char sec[128];
	strcpy(sec, key);
	char *psec = strstr(sec, ":");
	if(psec != NULL)
	{	
		*psec = '\0';
		d->sec_hash[i] = dictionary_hash(sec);
	}
	else
	{
		d->sec_hash[i] = hash;
	}
	d->n ++ ;
	return 0;
}
/*-------------------------------------------------------------------------*/
/**
  @brief	Delete a key in a dictionary
  @param	d		dictionary object to modify.
  @param	key		Key to remove.
  @return   void

  This function deletes a key in a dictionary. Nothing is done if the
  key cannot be found.
 */
/*--------------------------------------------------------------------------*/
void dictionary_unset(dictionary * d, char * key)
{
	unsigned	hash ;
	int			i ;

	hash = dictionary_hash(key);
	for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL)
            continue ;
        /* Compare hash */
		if (hash==d->hash[i]) {
            /* Compare string, to avoid hash collisions */
            if (!strcmp(key, d->key[i])) {
                /* Found key */
                break ;
			}
		}
	}
    if (i>=d->size)
        /* Key not found */
        return ;

    free(d->key[i]);
    d->key[i] = NULL ;
    if (d->val[i]!=NULL) {
        free(d->val[i]);
        d->val[i] = NULL ;
    }
    d->hash[i] = 0 ;
	d->sec_hash[i] = 0;
    d->n -- ;
    return ;
}



/*-------------------------------------------------------------------------*/
/**
  @brief	Set a key in a dictionary, providing an int.
  @param	d		Dictionary to update.
  @param	key		Key to modify or add
  @param	val		Integer value to store (will be stored as a string).
  @return	void

  This helper function calls dictionary_set() with the provided integer
  converted to a string using %d.
 */
/*--------------------------------------------------------------------------*/


int dictionary_setint(dictionary * d, char * key, int val)
{
	char	sval[MAXVALSZ];
	sprintf(sval, "%d", val);
	return dictionary_set(d, key, sval);
}


/*-------------------------------------------------------------------------*/
/**
  @brief	Set a key in a dictionary, providing a double.
  @param	d		Dictionary to update.
  @param	key		Key to modify or add
  @param	val		Double value to store (will be stored as a string).
  @return	void

  This helper function calls dictionary_set() with the provided double
  converted to a string using %g.
 */
/*--------------------------------------------------------------------------*/


int dictionary_setdouble(dictionary * d, char * key, double val)
{
	char	sval[MAXVALSZ];
	sprintf(sval, "%g", val);
	return dictionary_set(d, key, sval);
}



/*-------------------------------------------------------------------------*/
/**
  @brief	Dump a dictionary to an opened file pointer.
  @param	d	Dictionary to dump
  @param	f	Opened file pointer.
  @return	void

  Dumps a dictionary onto an opened file pointer. Key pairs are printed out
  as @c [Key]=[Value], one per line. It is Ok to provide stdout or stderr as
  output file pointers.
 */
/*--------------------------------------------------------------------------*/

void dictionary_dump(dictionary * d, FILE * out)
{
	int		i ;

	if (d==NULL || out==NULL) return ;
	if (d->n<1) {
		//fprintf(out, "empty dictionary\n");
		return ;
	}
	for (i=0 ; i<d->size ; i++) {
        if (d->key[i]) {
            fprintf(out, "%20s\t[%s]\n",
                    d->key[i],
                    d->val[i] ? d->val[i] : "UNDEF");
        }
	}
	return ;
}



/* Example code */
#ifdef TESTDIC
#define NVALS 20000
int main(int argc, char *argv[])
{
	dictionary	*	d ;
	char	*	val ;
	int			i ;
	char		cval[90] ;

	/* allocate blackboard */
	td_printf(0,"allocating...\n");
	d = dictionary_new(0);
	
	/* Set values in blackboard */
	td_printf(0,"setting %d values...\n", NVALS);
	for (i=0 ; i<NVALS ; i++) {
		sprintf(cval, "%04d", i);
		dictionary_set(d, cval, "salut");
	}
	td_printf(0,"getting %d values...\n", NVALS);
	for (i=0 ; i<NVALS ; i++) {
		sprintf(cval, "%04d", i);
		val = dictionary_get(d, cval, DICT_INVALID_KEY);
		if (val==DICT_INVALID_KEY) {
			td_printf(0,"cannot get value for key [%s]\n", cval);
		}
	}
    td_printf(0,"unsetting %d values...\n", NVALS);
	for (i=0 ; i<NVALS ; i++) {
		sprintf(cval, "%04d", i);
		dictionary_unset(d, cval);
	}
    if (d->n != 0) {
        td_printf(0,"error deleting values\n");
    }

	td_printf(0,"deallocating...\n");
	dictionary_del(d);
	return 0 ;
}
#endif
/* vim: set ts=4 et sw=4 tw=75 */
