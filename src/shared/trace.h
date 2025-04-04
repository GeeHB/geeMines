//---------------------------------------------------------------------------
//--
//--	trace.h
//--
//---------------------------------------------------------------------------

#ifndef __GEE_TRACE_h__
#define __GEE_TRACE_h__    1

#ifdef DEST_CASIO_CALC

#ifdef TRACE_MODE
#ifdef FX9860G
#define TRACE_POS_X     (CASIO_WIDTH - 40)
#define TRACE_POS_Y     5 // (CASIO_HEIGHT - 20)
#define TRACE_WIDTH     100
#define TRACE_HEIGHT    12
#else
#define TRACE_POS_X     (CASIO_WIDTH - 140)
#define TRACE_POS_Y     (CASIO_HEIGHT - 60)
#define TRACE_WIDTH     100
#define TRACE_HEIGHT    15
#endif // #ifdef FX9860G

// TRACE : add a line of text in the screen
//
//  @val : text to display
//  @tCol : text colour
//  @bkCol : Background colour
//
#define TRACE(val, tCol, bkCol)      { if (NO_COLOR != bkCol) drect(TRACE_POS_X, TRACE_POS_Y, TRACE_POS_X + TRACE_WIDTH - 1, TRACE_POS_Y + TRACE_HEIGHT -1, bkCol);dtext(TRACE_POS_X, TRACE_POS_Y, tCol, val);dupdate();}
#define TRACE_DEF(val)                   TRACE(val, COLOUR_BLACK, NO_COLOR);
#define TRACE_WAIT(val, tCol, bkCol) {TRACE(val, tCol, bkCol); key_event_t evt; do { evt = pollevent();} while(evt.type != KEYEV_DOWN && evt.key != KEY_EXE);}
#else
#define TRACE(val, tCol, tbk)      {}
#define TRACE_WAIT(val, tCol, tbk)      {}
#define TRACE_DEF(val) {}
#endif // #ifdef TRACE_MODE

#endif // #ifdef DEST_CASIO_CALC

#endif // __GEE_TRACE_h__

// EOF
