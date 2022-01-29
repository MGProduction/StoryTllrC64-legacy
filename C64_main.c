// ---------------------------------------------------------------
// Copyright (c) 2021/2022 Marco Giorgini
// ---------------------------------------------------------------
//
// Permission is hereby granted, free of charge, to any person
// obtaining a copy of this software and associated documentation
// files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use,
// copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following
// conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
// OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
// OTHER DEALINGS IN THE SOFTWARE.
//
// ---------------------------------------------------------------

#include "C64_main.h"

// ---------------------------------------------------------------
// MEMORY DEFINITIONS
// ---------------------------------------------------------------

#define TEMPAREA   ADDR(0xB000)
#define FILECACHE2 ADDR(0xBC00)

#define FILECACHE1 ADDR(0xC000)

#define TOPBITMAP  ADDR(0xE000)
#define VIDEOMEM   ADDR(0xF000)

#if defined(USE_FONT)
#define TTVIDEOMEM 0xF400
#define TVIDEOMEM  ADDR(TTVIDEOMEM)
#else
#define TVIDEOMEM  ADDR(0x0400)
#endif
#define FONTMEM    ADDR(0xF800)

u8*video_ram=TVIDEOMEM;
u8*video_colorram=ADDR(0xD800);
u8*bitmap_image=TOPBITMAP;

#define MAX_CMD   80
#define MAX_TMP   40
#define MAX_TMP2 120
u8*tmp=TEMPAREA;
u8*strcmd=TEMPAREA+MAX_TMP;
u8*tmp2=TEMPAREA+MAX_TMP+MAX_CMD;
u8*loadram=TEMPAREA+MAX_TMP+MAX_CMD+MAX_TMP2;
u8 icmd=0;

u8*ttemp;
u8*tbitmap_image;
u16 ww;

u8* m_bitmap;
u8* m_bitmapcol;
u8* m_bitmapscrcol;
u16 m_bitmap_w,m_bitmap_ox;
u8  m_bitmap_h,m_bitmap_oy;


#if defined(USE_DISK)
u8*buf_bitmap;
u8*buf_bitmapcol;
u8*buf_bitmapscrcol;
u16 buf_w,buf_ox;
u8  buf_h,buf_oy;
#if defined(WIN32)
char*FILENAME(char*nm)
{
 static char nn[256];
 sprintf(nn,"%simg/%s",basepath,nm);
 return nn;
}
#else
#define FILENAME(nm) nm
#endif
#endif

#define status_y 14
#define text_ty  (status_y+1)

// ---------------------------------------------------------------
#include "include_storytllr64.c"
// ---------------------------------------------------------------
#include "include_irq.c"
// ---------------------------------------------------------------
#include "include_ui.c"
// ---------------------------------------------------------------

// ---------------------------------------------------------------
// GLOBALS
// ---------------------------------------------------------------

#if defined(WIN32)
u8 video_scroll_x,video_scroll_y;
#endif



// ---------------------------------------------------------------

u8 cached_room=255,cached_h;
u16 cached_w;

void os_roomimage_load()
{
 IRQ_gfx_reset();
 do_textmode();
 REFRESH

 #if defined(USE_DISK)
 buf_w=buf_h=0;
 if(roomimg[room]!=255)  
  if(cached_room==roomimg[room])
   {
    buf_w=cached_w;
    buf_h=cached_h;
   }
  else 
   {
   FILE*fp;  
   char nm[]="room01";
   u8 r=roomimg[room]+1;
   nm[4]='0'+(r/10); 
   nm[5]='0'+(r%10); 
   
   fp = fopen(FILENAME(nm), "rb");  
   if(fp)
    {
     u16 w,ox,psize;
     u8  h,oy;  
     u8  b;
     
     fread(&b,sizeof(b),1,fp);
     
     fread(&w,sizeof(w),1,fp);
     fread(&h,sizeof(h),1,fp);
     fread(&ox,sizeof(ox),1,fp);
     fread(&oy,sizeof(oy),1,fp);
     
     fread(&b,sizeof(b),1,fp);
        
     buf_bitmapscrcol=FILECACHE2;  
     fread(&psize,sizeof(psize),1,fp);
     fread(buf_bitmapscrcol,psize,1,fp);
     
     buf_bitmapcol=buf_bitmapscrcol+psize;
     fread(&psize,sizeof(psize),1,fp);
     fread(buf_bitmapcol,psize,1,fp);
     
     buf_bitmap=FILECACHE1;
     fread(&psize,sizeof(psize),1,fp);
     fread(buf_bitmap,psize,1,fp);    

     fclose(fp);
     
     buf_w=w;
     buf_h=h;
     buf_ox=ox;
     buf_oy=oy;
     
     cached_room=roomimg[room];
     cached_w=w;
     cached_h=h;
    }     
   }
#endif
 
 ui_room_update();
 
 IRQ_gfx_init();
 REFRESH
 

 //str=advdesc;strid=roomdescid[room];getstring();txt=ostr;
 //text_write(ostr);

 /*m_bitmapscrcol=char01_screencol;
 m_bitmapcol=char01_col;
 m_bitmap=char01_bitmap;
 m_bitmap_h=char01_h;
 m_bitmap_w=char01_w;  
 m_bitmap_ox=8;
 m_bitmap_oy=8;

 IMAGE_load();

 m_bitmap_ox=320-char01_w-8;

 IMAGE_load();

 m_bitmapscrcol=obj01_screencol;
 m_bitmapcol=obj01_col;
 m_bitmap=obj01_bitmap;
 m_bitmap_h=obj01_h;
 m_bitmap_w=obj01_w;  
 m_bitmap_ox=8;
 m_bitmap_oy=8+char01_h+8;

 IMAGE_load();

 m_bitmap_ox=8+obj01_w;
 IMAGE_load();*/

}

#if defined(USE_FONT)
#include "font.h"
void font_load()
{
 u8*tmp=FILECACHE1;
 unpack(font,tmp);
 memcpy(FONTMEM,tmp,128*8);
 for(i=0;i<128*8;i++)
  tmp[i]=255-tmp[i];
 memcpy(FONTMEM+128*8,tmp,128*8);
}
#endif

#if defined(advcartdrigeondisk)
void loadcartdrige()
{
 FILE*f;
 char card[256];
 sprintf(card,"%sadvcartdrige",basepath);
 f=fopen(card,"rb");
 if(f)
  {
   u16 iln,ln;
   
   fread(&iln,sizeof(iln),1,f); // size of cartdrige
   advcartdrige=ADDR(0x6000);
   fread(advcartdrige,iln,1,f);
   
   fread(&iln,sizeof(iln),1,f);
   
   fread(&ln,sizeof(ln),1,f);
   opcode_vrbidx_count=ln;
   fread(&ln,sizeof(ln),1,f);
   obj_count=(u8)ln;
   fread(&ln,sizeof(ln),1,f);
   room_count=(u8)ln;
   fread(&ln,sizeof(ln),1,f);
   var_count=(u8)ln;
   
   fread(&ln,sizeof(ln),1,f);
   advnames=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   advdesc=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   msgs=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   verbs=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   objs=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   packdata=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   opcode_vrbidx=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   opcode_pos=(u16*)(advcartdrige+ln);    
   fread(&ln,sizeof(ln),1,f);
   opcode_len=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   opcode_data=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   objnameid=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   objdescid=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   objimg=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   objattr=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   objloc=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   roomstart=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   roomnameid=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   roomdescid=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   roomimg=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   roomattr=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   bitvars=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   vars=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   origram=advcartdrige+ln;    
   fread(&ln,sizeof(ln),1,f);
   origram_len=ln;
   fclose(f);
  }
}
#endif 

void os_init()
{
 vid_setcolorBKG(COLOR_BLACK);
 vid_setcolorBRD(COLOR_BLACK);

#if defined(USE_FONT) 
 font_load();
#endif 

 // key autorepeat OFF
 __asm__("lda $028A");
 __asm__("and #$3f");
 __asm__("ora #$40");
 __asm__("sta $028A");
 
 memset(video_ram,' ',1000);
 REFRESH
 
 IRQ_gfx_init();  
 
 

 
}

void os_reset()
{
 __asm__("JSR $FCE2");
}

#if defined(WIN32)
int c64_main()
#else
int main()
#endif
{

 os_init(); 

 while(1)
 {
 
#if defined(advcartdrigeondisk)
   loadcartdrige();
#endif 

   IMAGE_clear();
   adv_start();
   
   parser_update();
   
   quit_request=ch=0;
   while(quit_request==0)
   {  
  #if defined(WIN32)
    ch=cgetc();
  #else
    __asm__("JSR %w",SCNKEY);
    __asm__("JSR %w",GETIN);
    __asm__("STA %v",ch);
  #endif
    if(ch)
     {
      hide_blink();
      if(ch==CHAR_RETURN)
       execute();
      else
       {
        if(ch==CHAR_BACKSPACE)
        {
         if(icmd)
          strcmd[--icmd]=0;
        }
        else
        {
         if(icmd<MAX_CMD)
          {
           ch=charmap(ch);
           strcmd[icmd++]=ch;
           strcmd[icmd]=0;
          }    
        }
        parser_update();
       }
     }
    else
     do_blink();
     
    REFRESH
   }
   
  if(quit_request>=2)
   {
    if(quit_request==2)
     adv_reset();
    else
     adv_load();
    quit_request=0;
   }
  else
   break;
 }
 
 os_reset();
 
 return 0;
}

