//----------------------------------------------------------------------
//--
//--    casioCalcs.c
//--
//--            Shared functions
//--
//----------------------------------------------------------------------

#include "casioCalcs.h"

#include <string.h>

//
// Rect. manipulation fucntions
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

// EOF
