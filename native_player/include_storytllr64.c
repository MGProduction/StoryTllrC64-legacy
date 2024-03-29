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

// -----------------------------
// game data, built by the script compiler
// -----------------------------
#include "storytllr64_data.h"

// -----------------------------
// ui coordinates
// -----------------------------

#define split_y 96
#define status_y ((split_y/8)+1+1)
#define text_ty  (status_y+1)
#define text_stoprange (SCREEN_H-text_ty-1)

#define TVIDEORAM_OFFSET (text_ty*40)
#define TVIDEORAM_SIZE   (1000-TVIDEORAM_OFFSET)

// -----------------------------
// special characters
// -----------------------------

#define FAKE_CARRIAGECR 31
#define ESCAPE_CHAR     92

// -----------------------------
// GLOBALS
// -----------------------------
// most of variables are here
// I don't use parameters or locala
// if I can avoid them to optimize
// CC65 work
// -----------------------------
u8*txt,*etxt;
u8 txt_x,txt_y,txt_col,txt_rev,txt_wrap;
u8 text_y=0,text_x=0;
u8 _ch,_bch,al;
// -----------------------------
u8 room=meta_nowhere,nextroom=meta_nowhere,newroom,quit_request=0,text_attach,text_continue;
u8 rightactorimg=meta_none,leftactorimg=meta_none;
// -----------------------------
#define kind_none 0
#define kind_obj  1
#define kind_room 2
u8 cmd,obj1,obj2,obj1k,obj2k;
u8 clearfull;
u8*str,*ostr,*strcmds;
u8 strid,_strid;
u8 ch,imageid;
u8*imagemem;
u16 i,j,ii,freemem;
u8 cmdid,fail,opcode,thisopcode,var,varobj,varroom,varvalue,varattr,saved;
// -----------------------------
u8  key,len,istack=0,thisobj,used;
u8 *pcode;
u16 pcodelen;
u8  executed;
u8  ormask[8]={1,2,4,8,16,32,64,128},
    xormask[8]={255-1,255-2,255-4,255-8,255-16,255-32,255-64,255-128};
// -----------------------------


// -----------------------------
// PROTOTYPES
// -----------------------------
void room_load();
void adv_run();
void os_roomimage_load();
void os_core_roomimage_load();
void core_drawtext();
void cr();
void ui_text_write(u8*text);
void ui_waitkey();
void ui_getkey();
void ui_clear();
void ui_cr();
void ui_room_update();

#define ch_sep 124

/*void _gettmp()
{
 i=0;
 while(*ostr&&(*ostr!=' '))
  if(i<32)
   tmp[i++]=*ostr++;
  else
   ostr++;
 tmp[i]=0;
}*/

void _getstring()
{
 _strid=0;
 ostr=str;
 while(_strid<strid)
 {
  len=*str++;  
  if(len==255)
   {
    len=*str++;
    str+=255;
   } 
  str+=len; 
  _strid++;
  ostr=str;
 } 
len=*ostr++; 
etxt=ostr+len;
if(len==255)
 {
  len=*ostr++; 
  etxt+=1+len;
 }
_bch=0;
}

void _findstring()
{
 cmdid=0;i=0;
 while(str[i])
  {
   len=str[i++];   
   if(str[i]==ostr[0])
    if((ostr[len]==0)||(ostr[len]==' '))
     if(memcmp(ostr,str+i,len)==0)
      {
       memcpy(tmp,str+i,len);tmp[len]=0;
       i+=len;ostr+=len;
       cmdid=str[i];
       return;
      } 
   i+=len; 
   i++;
  }
 i=0;
 while(*ostr&&(*ostr!=' '))
  if(i<32)
   tmp[i++]=*ostr++;
  else
   ostr++;
 cmdid=255;
}

void draw_roomobj()
{
 u8 i,k,c; 
 al=0;txt=tmp2;
 for(k=c=i=0;i<obj_count;i++)
  if(objloc[i]==varroom)
   if((objattr[i]&varattr)==varattr)
    {     
     strid=objnameid[i];
     if(strid==255)
      ;
     else
      { 
       str=advnames;     
       _getstring();
       k=0;
       if(c)
        {
         txt[k++]=',';
         txt[k++]=' ';
        }
       while(ostr<etxt)
        txt[k++]=*ostr++;
       txt[k]=0;
       txt_col=COLOR_WHITE;
       core_drawtext();
       // tmp2[k++]=ESCAPE_CHAR;     
       // tmp2[k++]='w'-'a'+1;
       c++;
      }
    }
 if(c==0)
 {
 }
 else
  {
   k=0;
   txt[k++]='.';
   txt[k++]=0;
   txt_col=COLOR_WHITE;
   core_drawtext();
   text_y=txt_y-text_ty;
   text_attach=0;
   cr();
  } 
}

void _getroom()
{
 varroom=pcode[i++];       
 if(varroom==meta_here)
  varroom=room;
 else
  if(varroom==meta_this)
   varroom=obj1;
  else
  if(varroom==meta_that)
   varroom=obj2;
  else
  if(varroom==meta_oneofroom)
   {
    ch=pcode[i++];
    varroom=pcode[i+rand()%ch];
    i+=ch;
   }
}

void _getobj()
{
 varobj=pcode[i++];       
 if(varobj==meta_this)
  varobj=thisobj;
 else
  if(varobj==meta_that)
   varobj=obj2;
  else
  if(varroom==meta_oneofobj)
   {
    ch=pcode[i++];
    varobj=pcode[i+rand()%ch];
    i+=ch;
   }
}

// -----------------------------
// OPCODES PLAYER
// -----------------------------

u16 stack_i;
u8  stack_used,stack_istack,stack_fail,stack_thisopcode;

void adv_gosub()
{
 stack_thisopcode=thisopcode;stack_i=i;stack_used=used;stack_istack=istack;stack_fail=fail;

 adv_run();

 thisopcode=stack_thisopcode;
 pcode=opcode_data+opcode_pos[thisopcode];
 pcodelen=opcode_len[thisopcode];
 i=stack_i;
 used=stack_used;
 istack=stack_istack;
 fail=stack_fail;
}

void adv_exec()
{ 
 used=istack=fail=0;thisopcode=opcode;
 thisobj=obj1;
 i=0;
 while(i<pcodelen) 
  {     
   opcode=pcode[i++];
   if(opcode==op_endwith)
    break;
   if(opcodeattr[opcode-128]&128)
    {
     used++; 
     switch(opcode)
      {
     case op_msgroom:
     case op_msgobj:
      {
       strid=pcode[i++];
       strid=vars[strid];      
       str=advnames;
       if(opcode==op_msgobj)
        strid=objnameid[strid];
       else
        strid=roomnameid[strid];
       _getstring();
       ui_text_write(ostr);
      }
      break;
     case op_msgvar:
      {
       strid=pcode[i++];
       strid=vars[strid];
       ch=0;
       if(strid>99)
        {tmp[ch++]='0'+strid/100;strid%=100;}
       if(strid>9)
        {tmp[ch++]='0'+strid/10;strid%=10;}
       tmp[ch++]='0'+strid;
       tmp[ch]=0;
       ostr=tmp;etxt=ostr+ch;
       ui_text_write(ostr);
      }
      break;
     case op_msg:
      {
       var=pcode[i++];
       switch(var)
        {
        case meta_roomdesc:
        case meta_objdesc:
         {
          str=advdesc;
          if(var==meta_objdesc)
           strid=objdescid[thisobj];
          else
           strid=roomdescid[room];
         }
        break;
        case meta_roomname:
        case meta_objname:
         {
          str=advnames;
          if(var==meta_objname)
           strid=objnameid[thisobj];
          else
           strid=roomnameid[room];
         }
        break;
        default:
         str=msgs;
         strid=var;
        }
       if(strid==255)
        {
         if(var==meta_roomdesc)
         {
          cmd=vrb_ondesc;
          adv_gosub();
         }
        }
       else
        {
        _getstring();
        ui_text_write(ostr);       
        }
      }
     break;

     case op_quit:
       quit_request=1;
     break;
     case op_start:
      quit_request=2;
     break;
     case op_load:
      if(saved) quit_request=3;
     break;
     case op_save:
      *roomstart=room;
      memcpy(loadram,objnameid,origram_len);
      saved++;
     break;
     case op_dbg:
      {       
       itoa(freemem,tmp,10);       
       ostr=tmp;
       ui_text_write(ostr);       
      }
     break;
     case op_clear:
      ui_clear();
     break;
     case op_getkey:
      ui_getkey();
      key=ch;     
     break;
     case op_waitkey:
      ui_waitkey();
     break;
     case op_showobj:
      rightactorimg=pcode[i++];
      ui_room_update();
     break;
     case op_goto:
      nextroom=pcode[i++];       
     break;
     case op_setattr:
     case op_unsetattr:
      {
       _getobj();
       var=pcode[i++];    
       switch(opcode)
        {
         case op_unsetattr:
          objattr[varobj]&=(0xFF-var);
         break;
         case op_setattr:
          objattr[varobj]|=var;
         break;
        }
      }
     break;
     case op_setroomoverlayimage:
      _getroom();
      var=pcode[i++];
      //roomimg[varroom]=var;
      roomovrimg[varroom]=var;
      os_roomimage_load();
     break;
     case op_setroomimage:
      _getroom();
      var=pcode[i++];
      roomimg[varroom]=var;
      os_roomimage_load();
     break;
     case op_list:
      _getroom();
      varattr=pcode[i++];
      draw_roomobj();              
     break;
     case op_put:
      {
       _getobj();
       _getroom();
       objloc[varobj]=varroom;
      }
     break;
     case op_set:
     case op_unset: 
      varobj=pcode[i++];       
      var=varobj>>3;
       if(opcode==op_set)
        bitvars[var]|=ormask[varobj&0x7];
       else
        bitvars[var]&=xormask[varobj&0x7]; 
     break;
     case op_addvar:
     case op_setvar:
      varobj=pcode[i++];       
      var=pcode[i++];   
      if((var==meta_oneofroom)||(var==meta_oneofobj))
       {
        ch=pcode[i++];   
        var=pcode[i+rand()%ch];
        i+=ch;
       }
      else
       if(var==meta_this)
        var=obj1;
       else
        if(var==meta_that)
         var=obj2;
      switch(opcode)
       {
        case op_addvar:
         vars[varobj]+=var;          
        break;
        case op_setvar:
         vars[varobj]=var;
        break;
       }
      break;
      default:
       fail=1;
     }
    }
   else  
    {
     switch(opcode)
     {
      case op_else:
        while((i<pcodelen)&&(pcode[i]!=op_endif)&&(pcode[i]!=op_endwith)) 
        {
         ch=pcode[i];
         i+=opcodeattr[ch-128]&0x7f;
        }
      break;
      case op_endif:
      break;
      case op_ifkey:
       var=pcode[i++];            
       if(var!=key)
        fail=3;
      break;
      // attr
      case op_ifis:
       _getobj();
       var=pcode[i++];    
       if(varobj==meta_none)
        fail=3;
       else
       if((objattr[varobj]&var)!=var)
        fail=3;
      break;
      case op_ifisin:
      case op_ifobjinattr:
       {
        _getobj();
        _getroom();
        switch(opcode)
         {
          case op_ifobjinattr:
           varattr=pcode[i++];
          if(varobj==meta_any)
           {
            ch=0;
            while(ch<obj_count)
             if(objnameid[ch]==255)
              ch++;
             else
              if((objloc[ch]==varroom)&&((objattr[ch]&varattr)==varattr))
               break;
              else              
               ch++;
            if(ch==obj_count)
             fail=3;
           }
          else
          if((objloc[varobj]!=varroom)||((objattr[varobj]&varattr)!=varattr))
           fail=3;
          break;
         case op_ifisin:
          if(varobj==meta_any)
           {
            ch=0;
            while(ch<obj_count)
             if(objloc[ch]==varroom)
              break;
             else
              ch++;
            if(ch==obj_count)
             fail=3;
           }
          else
          if(varroom==meta_available)
           {
            if(objloc[varobj]==meta_inventory)
             ;
            else
             if((objloc[varobj]==room)&&(objattr[varobj]&attr_visible))
              ;
             else
              fail=3;
           }
          else
          if(objloc[varobj]!=varroom)
           fail=3;
          break;
         }
        }
      break;           
      case op_ifisaroom:
       var=pcode[i++];
       if((var==meta_this)&&(obj1k==kind_room))
        ;
       else
       if((var==meta_that)&&(obj2k==kind_room))
        ;
       else
        fail=3;
      break;
      case op_ifundef:
       var=pcode[i++];
       switch(var)
        {
         case meta_objdesc:
          strid=objdescid[thisobj];
         break;
         case meta_roomname:
          strid=roomnameid[room];
         break;
         case meta_roomdesc:
          strid=roomdescid[thisobj];
         break;
        }
       if(strid!=255)
        fail=3;
      break;
      case op_ifisroom:
       var=pcode[i++];
       if(var!=room)
        fail=3;
      break;
      case op_withobj:
       thisobj=var=pcode[i++];
       if(var!=obj1)
        if(obj1==meta_none)
         if(var==meta_unknown)        
          ;
         else 
          fail=2;
        else
         if(var==meta_every)
          thisobj=obj1;
         else
          fail=2;
      break;          
      case op_if:
      case op_ifnot:
       varobj=pcode[i++];     
       var=varobj>>3;
       var=bitvars[var]&ormask[varobj&0x7];
       if(opcode==op_if)
        {
         if(var==0)
          fail=3;
        }  
       else
        if(var)
         fail=3;
      break; 
      case op_ifvar:
       varobj=pcode[i++];       
       var=pcode[i++];       
       varroom=pcode[i++];      
       switch(varroom)
        {
        case meta_this:
         varroom=obj1;
         break;
        case meta_that:
          varroom=obj2;
          break;
         case meta_here:
          varroom=room;
          break;
        }
       switch(var)
        {
         case 0:
          if(vars[varobj]!=varroom)
           fail=3;
         break;
         case 1:
          if(vars[varobj]==varroom)
           fail=3;
         break;
         case 2:
          if(vars[varobj]<=varroom)
           fail=3;
         break;
         case 3:
          if(vars[varobj]>=varroom)
           fail=3;
          break;
        }
      break;
      default:
       fail=1;
     }     
    if(fail)
     {
      if(fail==3)
       istack++;
      if(fail>=2)
       {        
        while((i<pcodelen)&&(pcode[i]!=op_endwith)) 
         {
          ch=pcode[i];
          if((ch>=op_ifstart)&&(ch<=op_ifend))
           istack++;
          else
           if(ch==op_else)
            {
             if((fail==3)&&(istack==1))
              {
               fail=istack=0;
               break;
              }
            }
           else
           if(ch==op_endif)
            if((fail==3)&&(istack==1))
              {
               if((i+1==pcodelen)&&(used==0))
                ;
               else
                fail=istack=0;
               break;
              }
            else
            if(istack==0)
             break;
            else
             istack--;
          if(ch>=128)
           i+=opcodeattr[ch-128]&0x7f;
          else
           break;
         }
        i++;
        if(pcode[i-1]==op_endwith)
         fail=0;
        if(fail)
         break;
       }
      else
       break;
     }
    }
  }
 if((fail==0)&&used)
  executed=1;
 else
  executed=0;
}

// -----------------------------
// USER INPUT HANDLING
// -----------------------------

void adv_run()
{   
 u8  theroom;
 u16 i;
 theroom=room;executed=0;
 i=0;
 while(i<opcode_vrbidx_count)
  {
   opcode=opcode_vrbidx[i];          
   if(opcode<cmd)
    i+=3;
   else
    break;
  }
 while(i<opcode_vrbidx_count)
  { 
   opcode=opcode_vrbidx[i++];    
   if(opcode!=cmd)
    break;
   else
    {
    varroom=opcode_vrbidx[i++];    
    if((varroom==theroom)||(varroom==meta_everywhere))
     {
      opcode=opcode_vrbidx[i++];
      pcode=opcode_data+opcode_pos[opcode];
      pcodelen=opcode_len[opcode];
      adv_exec();
      if(executed)
       break;
     }
    else
     i++;
    }
  }
}

void adv_parse()
{
 ostr=str;
 cmd=meta_unknown;obj1=obj2=meta_none,obj1k=obj2k=kind_none;
 while(*ostr&&(*ostr==' ')) ostr++;
 while(*ostr)
 {
  //_gettmp();
  if(cmd==meta_unknown)
   str=verbs;
  else
   str=objs;
  _findstring();
  if(cmdid!=255)
   {
    if(cmd==meta_unknown)
     {
      cmd=cmdid;
      strncpy(vrb,tmp,VRBLEN-1);
     } 
    else
     if(obj1k==kind_none)
      {obj1=cmdid;obj1k=kind_obj;}
     else
      if(obj2k==kind_none)
       {obj2=cmdid;obj2k=kind_obj;}
   }
  else
   if(cmd!=meta_unknown)
    {
     str=rooms;
     _findstring();
     if(obj1k==kind_none)
      {
       if(cmdid!=255)
        {obj1=cmdid;obj1k=kind_room;}
       else
       if(obj1==meta_none)
        obj1=meta_unknown;
      }
     else
      if(obj2k==kind_none)
       if(cmdid!=255)
        {obj2=cmdid;obj2k=kind_room;}
    }
  while(*ostr&&(*ostr==' ')) ostr++;
 }
 adv_run();
 if(nextroom!=meta_nowhere)
  {
   newroom=nextroom;nextroom=meta_nowhere;
   room_load();
  }
 else
 {
  cmd=vrb_onturn;obj1=255;adv_run();
 }
}

// -----------------------------
// GAME RESET/LOAD/START
// -----------------------------

void adv_reset()
{
 hunpack(origram,objnameid);
}

void adv_load()
{
 memcpy(objnameid,loadram,origram_len);
}

void adv_start()
{
 clearfull=1;ui_clear();
 newroom=*roomstart;
 room_load();
}