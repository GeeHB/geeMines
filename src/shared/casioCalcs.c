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

// centerRect() : center a rect
//
//  @rect : pointer to the rect.
//  @width, @height : Screen dimensions
//
void centerRect(PRECT const rect, int width, int height){
    if (rect){
        if (rect->w < width){
            rect->x =(width - rect->w)/2;
        }

        if (rect->h < height){
            rect->y =(height - rect->h)/2;
        }
    }
}

// isEmptyRect() : Is the rect empty ?
//
//  @rect : pointer to the rect.
//
//  @return : TRUE if the rect is empty
//
BOOL isEmptyRect(PRECT const rect){
    return (rect && (!rect->w || !rect->h));
}

//
// Display rotation
//

//  rotatePoint() : Rotate (trig. 90°) and translate a single point
//
//  @pos : Pointer to point coordinates
//
void rotatePoint(PPOINT const pos){
    int16_t ny = CASIO_HEIGHT - pos->x;
    pos->x = pos->y;
    pos->y = ny;
}

//  rotateRect() : Rotate (trig. 90°) and translate a rectangle
//
//  @rect : Pointer to the rect
//
void rotateRect(PRECT const rect){
    POINT topLeft = {rect->x, rect->y};
    POINT bottomRight = {rect->x + rect->w - 1, rect->y + rect->h - 1};
    int16_t ow = rect->w;

    rotatePoint(&topLeft);
    rotatePoint(&bottomRight);

    rect->x = topLeft.x;
    rect->y = bottomRight.y;
    rect->w = rect->h;
    rect->h = ow;
}


#ifdef SCREEN_CAPTURE
#ifdef DEST_CASIO_CALC
//
// Screen capture for casio calculator : if  SCREEN_CAPTURE defined
//
//      using as root fxlink command -[ fxlink -iw ]
//

// capture_install() : Set/install screen capture
//
void capture_install(){
    // List of interfaces
    usb_interface_t const *intf[] = { &usb_ff_bulk, NULL };

    // Waiting for connexion
    usb_open((usb_interface_t const **)&intf, GINT_CALL_NULL);
    usb_open_wait();

    // Set the hook
    dupdate_set_hook(GINT_CALL(usb_fxlink_videocapture, 0));
}

// capture_remove() : Remove screen capture
//
void capture_remove(){
    dupdate_set_hook(GINT_CALL_NULL);
}

#endif // #ifdef DEST_CASIO_CALC
#endif // #ifdef  SCREEN_CAPTURE

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

#endif // TRACE_MODE

// __itoa() : Convert a num. val to a string
//
//  @num : Numeric value to convert
//  @format : format numbers
//  @str : destination buffer
//
//  @return : a pointer to the string
//
char* __itoa(int num, BOOL format, char *str){
    char* strVal = str;
    int sum= ((num < 0)?-1*num:num);
    uint8_t i = 0, digit, dCount = 0;
    do{
        digit = sum % 10;
        strVal[i++] = '0' + digit;
        if (format && !(++dCount % 3)){
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

// EOF
