//----------------------------------------------------------------------
//--
//--    casioCalcs.c
//--
//--            Types and defines specific to the casio targets
//--            & shared functions
//--
//----------------------------------------------------------------------

#include "casioCalcs.h"

#include <string.h>

//
// Rect. manipulation functions
//


// setRect() : Set rect dims & pos
//
//  @rect : pointer to the dest. rect
//  @x, @y : Top left corner of the rectangle
//  @w, @h : Width and height of the rectangle
//
//  @return : TRUE if values set
//
BOOL setRect(PRECT const rect, int x, int y, int w, int h){
    if (rect){
        rect->x=x;
        rect->y=y;
        rect->w=w;
        rect->h=h;
        return TRUE;
    }

    return FALSE;
}

// offsetRect() : Translate a rect
//
//  @rect : pointer to the rect.
//  @dx, @dy : Translation units
//
//  @return : TRUE if successfully translated
//
BOOL offsetRect(PRECT const rect, int dx, int dy){
    if (rect){
        rect->x+=dx;
        rect->y+=dy;
        return TRUE;
    }

    return FALSE;
}

// copyRect() : Make a copy of a rect. struct.
//
//  @dest : pointer to the destination rect.
//  @src : pointer to the source rect.
//
//  @return : TRUE if successfully copied
//
BOOL copyRect(PRECT const dest, PRECT const src){
    if (dest && src){
        memcpy(dest, src, sizeof(RECT));
        return TRUE;
    }

    return FALSE;
}

// inflateRect() : Inflate a rect
//
//  @rect : pointer to the rect.
//  @dx, @dy : Inflation units
//
//  @return : TRUE if successfully inflated
//
BOOL inflateRect(PRECT const rect, int dx, int dy){
    if (rect){
        rect->x-=dx;
        rect->y-=dy;
        rect->w+=2*dx;
        rect->h+=2*dy;
        return TRUE;
    }

    return FALSE;
}

// deflateRect() : deflate a rect
//
//  @rect : pointer to the rect.
//  @dx, @dy : Deflation units
//
//  @return : TRUE if successfully deflated
//
BOOL deflateRect(PRECT const rect, int dx, int dy){
    if (rect){
        rect->x+=dx;
        rect->y+=dy;
        rect->w-=2*dx;
        rect->h-=2*dy;
        return TRUE;
    }

    return FALSE;
}

#ifdef TRACE_MODE

//
// Debug & trace primitives
//


// __coordtoa() : Format point coord. to an output string
//
//  This specific method creates a string composed of the name of the value
//  and the value it self. It is equivalent to a sprintf(out, "%s : %d", name, value)
//
//  The base can't be changed it is always equal to 10
//
//  This method assumes the output buffer - ie. str - is large enough to contain
//  the name and the formated value.
//
//  @name : Name of the value (can be NULL)
//  @x,@y : Position to show
//  @str : Pointer to output string
//
//  @return : pointer to formated string
//
char* __coordtoa(const char* name, uint8_t x, uint8_t y, char* str){
    char* strVal = str;

    // Add name
    if (name){
        strcpy(str, name);
        strVal+=strlen(str);    // num. value starts here
    }

    // Append col. value
    __atoi(x, strVal);

    strVal = str + strlen(str);
    strVal[0] = ',';                // Add a separator
    strVal++;

    __atoi(y, strVal);

    return str;
}

// __atoi() : Convert a num. val to a string
//
//  @num : Numeric value to convert
//  @str : String to reverse
//
//  @return : a pointer to the string
//
char* __atoi(int num, char *str){
    char* strVal = str;
    int sum= ((num < 0)?-1*num:num);
    uint8_t i = 0, digit, dCount = 0;
    do{
        digit = sum % 10;
        strVal[i++] = '0' + digit;
        if (!(++dCount % 3)){
            strVal[i++] = ' ';  // for large numbers lisibility
        }

        sum /= 10;
    }while (sum);

    // A sign ?
    if (num < 0){
        strVal[i++] = '-';
    }
    strVal[i] = '\0';

    __strrev(strVal);   // reverse the string
    return str;
}

// __strrev() : Reverse a string
//
//  @str : String to reverse
//
void __strrev(char *str){
    int i, j;
    unsigned char a;
    size_t len = strlen((const char *)str);
    for (i = 0, j = len - 1; i < j; i++, j--){
        a = str[i];
        str[i] = str[j];
        str[j] = a;
    }
}

#endif // TRACE_MODE

// EOF
