
/*-------------------------------------------------------------------------*/
/**
   @file    iniparser.c
   @author  N. Devillard
   @date    Mar 2000
   @version $Revision: 1.4 $
   @brief   Parser for ini files.
*/
/*--------------------------------------------------------------------------*/

/*
    $Id: iniparser.c,v 1.4 2006/04/08 07:18:51 qbin Exp $
    $Author: qbin $
    $Date: 2006/04/08 07:18:51 $
    $Revision: 1.4 $
*/

/*---------------------------------------------------------------------------
                                Includes
 ---------------------------------------------------------------------------*/

#include <threads.h>
#include "iniparser.h"
#include "strlib.h"

#define ASCIILINESZ         1024
#define INI_INVALID_KEY     ((char*)-1)
#define CNOSK               0xFFFF      /*注释可依赖的section和key*/
#define KVCSIZE             512         /*KEY and VALUE and COMMENT'size is 512*/
static pthread_mutex_t g_tIniparserInitLock = PTHREAD_MUTEX_INITIALIZER;	// dlq modified at 20121013
/*---------------------------------------------------------------------------
                        Private to this module
 ---------------------------------------------------------------------------*/

/* Private: add an entry to the dictionary */
int iniparser_add_entry(
    dictionary * d,
    char * sec,
    char * key,
    char * val)
{
    char longkey[2*ASCIILINESZ+1];

    if((d == NULL) || (sec == NULL)) return -1;
    /* Make a key as section:keyword */
    if (key!=NULL) {
        sprintf(longkey, "%s:%s", sec, key);
    } else {
        strcpy(longkey, sec);
    }

    /* Add (key,val) to dictionary */
    if(0 != dictionary_add(d, longkey, val))
    {
        return 1;
    }
    
    return 0;
}


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

int iniparser_getnsec(dictionary * d)
{
    int i ;
    int nsec ;

    if (d==NULL) return -1 ;
    nsec=0 ;
    for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL)
            continue ;
        if (strchr(d->key[i], ':')==NULL) {
            nsec ++ ;
        }
    }
    return nsec ;
}


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

char * iniparser_getsecname(dictionary * d, int n)
{
    int i ;
    int foundsec ;

    if (d==NULL || n<0) return NULL ;
    foundsec=0 ;
    for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL)
            continue ;
        if (strchr(d->key[i], ':')==NULL) {
            foundsec++ ;
            if (foundsec>n)
                break ;
        }
    }
    if (foundsec<=n) {
        return NULL ;
    }
    return d->key[i] ;
}


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
void iniparser_dump(dictionary * d, FILE * f)
{
    int     i ;

    if (d==NULL || f==NULL) return ;
    for (i=0 ; i<d->size ; i++) {
        if (d->key[i]==NULL)
            continue ;
        if (d->val[i]!=NULL) {
            fprintf(f, "[%s]=[%s]\n", d->key[i], d->val[i]);
        } else {
            fprintf(f, "[%s]=UNDEF\n", d->key[i]);
        }
    }
    return ;
}


static unsigned char iniparser_save_comment(dictionary * d, FILE * f, unsigned int hash)
{
    int     ci;
    unsigned char flag = 0; /*标志该行是否有注释*/

    for(ci=0; ci < d->commSize; ci++)
    {
        if(d->commPlace[ci] == 0)
        {
            continue;
        }
        if(d->commHash[ci] == hash)
        {
            flag = 1;
            switch(d->commPlace[ci])
            {
                case 1:
                {
                    fprintf(f, "%s", d->comment[ci] ? d->comment[ci] : " ");
                    break;
                }
                case 2:
                {
                    fseek(f,-1,SEEK_CUR);
                    fprintf(f, "%s\n", d->comment[ci] ? d->comment[ci] : " ");
                    break;
                }
                case 3:
                {
                    fprintf(f, "%48s%s", " ", d->comment[ci] ? d->comment[ci] : " ");
                    break;
                }
                case 4:
                {
                    fprintf(f, "\r\n");
                    break;
                }
                /*case 5:
                {
                    fprintf(f, "%48s\n", " ");
                    break;
                }*/
                default:
                    break;
            }
        }
    }
    return flag;
}
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

void iniparser_dump_ini(dictionary * d, FILE * f)
{
    int     i, j ;
    char    keym[ASCIILINESZ+1];
    int     nsec ;
    char *  secname ;
    int     seclen ;
    unsigned char ret;

    if (d==NULL || f==NULL) return ;

    nsec = iniparser_getnsec(d);
    if (nsec<1) {
        /* No section in file: dump all keys as they are */

        /*2006/03/13 blair add : save comment and space Line*/
        iniparser_save_comment(d, f, CNOSK);
        
        for (i=0 ; i<d->size ; i++) 
        {
            if (d->key[i]==NULL)
            {
                continue ;
            }

            /*2006/03/13 blair modify : save comment and space Line*/
            fprintf(f, "%-30s = %-15s\n", d->key[i], d->val[i]);
            fprintf(f, "%-30s = %-15s\n", d->key[i],
                    d->val[i] ? d->val[i] : " ");
            /*2006/03/13 blair add : save comment and space Line*/
            ret = iniparser_save_comment(d, f, d->hash[i]);

            /*if(ret == 0) 
            {
                fprintf(f, "\n");
            }*/
        }
        return ;
    }

    /*2006/03/13 blair add : save comment and space Line*/
    iniparser_save_comment(d, f, CNOSK);

    for (i=0 ; i<nsec ; i++) 
    {
        int hash;
        secname = iniparser_getsecname(d, i) ;
        seclen  = (int)strlen(secname);
        fprintf(f, "[%-48s\n", secname);
        fseek(f, 0-(49-strlen(secname)), SEEK_CUR);
        fprintf(f, "%c", ']');
        fseek(f, 0, SEEK_END);
        /*2006/03/13 blair add : save comment and space Line*/
        hash = dictionary_hash(secname);
        ret = iniparser_save_comment(d, f, hash);
        /*if(ret == 0) 
        {
            fprintf(f, "\n");
        }*/
        
        sprintf(keym, "%s:", secname);
        for (j=0 ; j<d->size ; j++) {
            if (d->key[j]==NULL)
                continue ;
			if(d->sec_hash[j] == hash)
			{
	            if (!strncmp(d->key[j], keym, seclen+1)) 
	            {
	                fprintf(f, "%-30s = %-15s\n", d->key[j]+seclen+1,
	                        d->val[j] ? d->val[j] : " ");
	                /*2006/03/13 blair add : save comment and space Line*/
	                ret = iniparser_save_comment(d, f, d->hash[j]);
	                /*if(ret == 0) 
	                {
	                    fprintf(f, "\n");
	                }*/
	            }
			}
        }
    }
    //fprintf(f, "\n");
    return ;
}




/*-------------------------------------------------------------------------*/
/**
  @brief	Get the string associated to a key, return NULL if not found
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
char * iniparser_getstr(dictionary * d, char * key)
{
    return iniparser_getstring(d, key, NULL);
}


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
char * iniparser_getstring(dictionary * d, char * key, char * def)
{
    char * lc_key ;
    char * sval ;

    if (d==NULL || key==NULL)
        return def ;
	pthread_mutex_lock(&g_tIniparserInitLock);
    lc_key = strdup(strlwc(key));
	pthread_mutex_unlock(&g_tIniparserInitLock);
	if (NULL == lc_key)
	{
		return def;
	}
    sval = dictionary_get(d, lc_key, def);
    free(lc_key);
    return sval ;
}



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
int iniparser_getint(dictionary * d, char * key, int notfound)
{
    char    *   str ;

    str = iniparser_getstring(d, key, INI_INVALID_KEY);
    if (str==INI_INVALID_KEY) return notfound ;
    return atoi(str);
}


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
double iniparser_getdouble(dictionary * d, char * key, double notfound)
{
    char    *   str ;

    str = iniparser_getstring(d, key, INI_INVALID_KEY);
    if (str==INI_INVALID_KEY) return notfound ;
    return atof(str);
}



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
int iniparser_getboolean(dictionary * d, char * key, int notfound)
{
    char    *   c ;
    int         ret ;

    c = iniparser_getstring(d, key, INI_INVALID_KEY);
    if (c==INI_INVALID_KEY) return notfound ;
    if (c[0]=='y' || c[0]=='Y' || c[0]=='1' || c[0]=='t' || c[0]=='T') {
        ret = 1 ;
    } else if (c[0]=='n' || c[0]=='N' || c[0]=='0' || c[0]=='f' || c[0]=='F') {
        ret = 0 ;
    } else {
        ret = notfound ;
    }
    return ret;
}


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

int iniparser_find_entry(
    dictionary  *   ini,
    char        *   entry
)
{
    int found=0 ;
    if (iniparser_getstring(ini, entry, INI_INVALID_KEY)!=INI_INVALID_KEY) {
        found = 1 ;
    }
    return found ;
}



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

int iniparser_setstr(dictionary * ini, char * entry, char * val)
{
    char * lc_key ;
	pthread_mutex_lock(&g_tIniparserInitLock);
    lc_key = strdup(strlwc(entry));
	pthread_mutex_unlock(&g_tIniparserInitLock);
	if (NULL == lc_key)
	{
		return 1;
	}
    if(0 != dictionary_set(ini, lc_key, val))
    {
		free(lc_key);
        return 1;
    }
	free(lc_key);
    return 0 ;
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete an entry in a dictionary
  @param    ini     Dictionary to modify
  @param    entry   Entry to delete (entry name)
  @return   void

  If the given entry can be found, it is deleted from the dictionary.
 */
/*--------------------------------------------------------------------------*/
void iniparser_unset(dictionary * ini, char * entry)
{
    dictionary_unset(ini, strlwc(entry));
}

/*-------------------------------------------------------------------------*/
/**
  @brief    Delete an section in a dictionary
  @param    ini     Dictionary to modify
  @param    entry   section to delete (entry name)
  @return   void

  If the given entry can be found, it is deleted from the dictionary.
 */
/*--------------------------------------------------------------------------*/

void iniparser_sec_unset(dictionary * d, char *sec)
{
    int     j ;
    char    keym[ASCIILINESZ+1];
    char *  secname ;
    int     seclen ;
	int     hash;

    if (d==NULL || sec==NULL) return ;

    secname = sec;
    seclen  = (int)strlen(secname);
	hash   = dictionary_hash(secname);
	
    sprintf(keym, "%s:", secname);
    for (j=0 ; j<d->size ; j++) {
        if (d->key[j]==NULL)
            continue ;
		if(d->sec_hash[j] == hash)
		{
	        if (!strncmp(d->key[j], keym, seclen+1)) 
	        {
				dictionary_unset(d, strlwc(d->key[j]));
	        }
		}
    }

	dictionary_unset(d, strlwc(sec));
	
    return ;
}


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

#if 0
int fget_line(char *lin, char *p)
{
	int cnt;
	cnt = strchr(p, '\n')- p;
	if(cnt < 0)
	{
		return -1;
	}
	memcpy(lin, p, cnt);
	lin[cnt] = '\0';
	return cnt+1;
}
#endif

dictionary * iniparser_load(char * ininame)
{
    dictionary  *   d ;
    char        lin[ASCIILINESZ+1]  = {0};
    char        sec[KVCSIZE+1]      = {0};
    char        key[KVCSIZE+1]      = {0};
    char        val[KVCSIZE+1]      = {0};
    char        comment[KVCSIZE+1]  = {0};
    char    *   where ;
    FILE    *   ini ;
    int         lineno ;

    if ((ini=fopen(ininame, "r"))==NULL) {
        return NULL ;
    }

    sec[0]=0;

    /*
     * Initialize a new dictionary entry
     */
    d = dictionary_new(0);
    lineno = 0 ;
#if 0
//modify by tcf 20110705
	char *buf = NULL;
	char *tmp = NULL;
	int size = 0;
	int offset = 0;
	buf = malloc(1024 * 1024);
	
	if(!buf)
	{
		return NULL;
	}
	size = fread(buf, sizeof(char), 1024 * 1024, ini);
	if(size < 0)
	{
		goto ERR;
	}
	else
	{
		buf[size + 1] = '\0';
	}
	while(1)
	{
		offset += fget_line(lin, buf + offset);
		if(offset >= size)
		{
			goto ERR;
		}
//    while (fgets(lin, ASCIILINESZ, ini)!=NULL) 
//	{
//end    

#else
	while (fgets(lin, ASCIILINESZ, ini)!=NULL) 
	{
#endif
        lineno++ ;
        where = lin; /* Skip leading spaces */
        comment[0] = 0;
        /*2006/03/13 blair modify : save comment and space Line*/
        //if (*where==';' || *where=='#' || *where==0)
        if (*where==';' || *where=='#')/*; comment*/
        {
            strcpy(comment,where);
            /*no space commnet line*/
            iniparser_add_comment(d, sec, key, comment, CL_NSBC);
        }
        else
        {
            where = strskp(lin); /* Skip leading spaces */
            /*if((sscanf (where, " ; %[^\n]", comment) == 1)
                ||(sscanf (where, " # %[^\n]", comment) == 1))*//*   ; comment*/
            if (*where==';' || *where=='#')
            {
                strcpy(comment,where);
                /*comment line with space*/
                iniparser_add_comment(d, sec, key, comment, CL_CLINE);
                continue ; /* Comment lines */
            }
            else 
            {
                if ((sscanf(where, "[%[^]] ] %[^\n]", sec, comment)==2)
                    || (sscanf(where, "[%[^]]", sec)==1))/*[sec];comment*/
                {
                    /* Valid section name */
                    key[0] = 0;
                    strcpy(sec, strlwc(sec));
                    iniparser_add_entry(d, sec, NULL, NULL);
                    if(comment[0] != 0)
                    {
                        /*The comment at key line's right*/
                        iniparser_add_comment(d, sec, key, comment, CL_AKLR);
                    }
                } 
                else /*key = value ;comment*/
                {
                    if (sscanf (where, "%[^=] = \"%[^\"]\" %[^\n]", key, val, comment) == 3
                       ||  sscanf (where, "%[^=] = \"%[^\"]\"", key, val) == 2
                       ||  sscanf (where, "%[^=] = '%[^\']' %[^\n]", key, val, comment) == 3
                       ||  sscanf (where, "%[^=] = '%[^\']'",   key, val) == 2
                       ||  sscanf (where, "%[^=] = %[^;] %[^\n]", key, val, comment) == 3
                       ||  sscanf (where, "%[^=] = %[^;]", key, val ) == 2) /*2006/03/13 blair modify : save comment and space Line*/
                    {
                        strcpy(key, strlwc(strcrop(key)));
                        /*
                         * sscanf cannot handle "" or '' as empty value,
                         * this is done here
                         */
                        if (!strcmp(val, "\"\"") || !strcmp(val, "''")) {
                            val[0] = (char)0;
                        } else {
                            strcpy(val, strcrop(val));
                        }
                        iniparser_add_entry(d, sec, key, val);
                        
                        if(comment[0] != 0)
                        {
                            /*The comment at key line's right*/
                            iniparser_add_comment(d, sec, key, comment, CL_AKLR);
                        }
                    }
                    else
                    {
                        comment[0] = 0;
                        /*Space Line*/
                        iniparser_add_comment(d, sec, key, comment, CL_SLINE);
                    }
                }
            }
        }
    }
#if 0
ERR:
	free(buf);
#endif
    fclose(ini);
    return d ;
}

/*2006/03/11 blair add : save comment and space Line*/
int iniparser_add_comment(
    dictionary          *d, 
    const char          *sec, 
    const char          *key, 
    const char          *comment,
    const unsigned char place)
{
    char        longkey[1024] = {0};
    unsigned    hash;
    int         i;

    if ((d == NULL) || (sec == NULL)  || (place < 1)) return -1;


    /* Make a key as section:keyword */
    if(sec[0] != 0)
    {
        if (key[0]!=0) {
            sprintf(longkey, "%s:%s", sec, key);
        } else {
            strcpy(longkey, sec);
        }
    }
    else
    {
        if (key[0]!=0) {
            sprintf(longkey, "%s", key);
        } else {
            longkey[0] = 0;
        }
    }
    
    /* Compute hash for this key */
    if(longkey[0]!='\0')
    {
        hash = dictionary_hash(longkey);
    }
    else
    {
        hash = CNOSK;
    }

	/* Add a new value */
	/* See if dictionary needs to grow */
	if (d->commN == d->commSize) 
	{
		/* Reached maximum size: reallocate blackboard */
		d->comment  = (char **)mem_double((void*)d->comment, d->commSize * sizeof(char*));
		d->commHash = (unsigned *)mem_double((void*)d->commHash, d->commSize * sizeof(unsigned));
		d->commPlace= (unsigned char *)mem_double((void*)d->commPlace, d->commSize * sizeof(unsigned char));

		/* Double size */
		d->commSize *= 2 ;
	}

    /* Insert comment in the first empty slot */
    for (i=0 ; i<d->commSize ; i++) 
    {
        if (d->commPlace[i] == 0) 
        {
            /* Add comment here */
            break ;
        }
    }
    d->commHash[i] = hash;
    d->commPlace[i]= place;
    d->comment[i] ? free(d->comment[i]),d->comment[i]=NULL: NULL;
    d->comment[i]  = (comment[0] != 0) ? strdup(comment) : NULL;
	d->commN ++ ;
	return 0;
}
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

void iniparser_freedict(dictionary * d)
{
    dictionary_del(d);
}


int iniparser_sec_getNLowLever(dictionary * d,char* UperLever)
{
/*    int nsec;
    int i;
    int n = 0;

    if((HI_NULL_PTR == d) || (HI_NULL_PTR == UperLever)) 
    {
        return 0;
    }
    
    nsec = iniparser_getnsec(d);
    for(i = 0; i<nsec; i++)
    {
        
    }

    return n;
*/
    d = d;
    UperLever = UperLever;
    return 0;
}


char* iniparser_sec_getLowLever(dictionary * d,char* UpLever, int n)
{
    d = d;
    UpLever = UpLever;
    n = n;
    return 0;
}
/* vim: set ts=4 et sw=4 tw=75 */
