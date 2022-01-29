#define packed_strings 4

#define string_max_len 210

u8 opcodeattr[36]={131,129,129,1,1,1,1,129,130,2,3,3,2,2,2,4,4,131,129,130,131,129,129,130,131,131,131,131,131,131,130,129,130,131,129,2};

#define op_addvar 128
#define op_clear 129
#define op_dbg 130
#define op_else 131
#define op_end 132
#define op_endif 133
#define op_endwith 134
#define op_getkey 135
#define op_goto 136
#define op_if 137
#define op_ifis 138
#define op_ifisin 139
#define op_ifisroom 140
#define op_ifkey 141
#define op_ifnot 142
#define op_ifobjinattr 143
#define op_ifvar 144
#define op_list 145
#define op_load 146
#define op_msg 147
#define op_put 148
#define op_quit 149
#define op_save 150
#define op_set 151
#define op_setattr 152
#define op_setobjname 153
#define op_setroomimage 154
#define op_setroomname 155
#define op_setroomoverlayimage 156
#define op_setvar 157
#define op_showobj 158
#define op_start 159
#define op_unset 160
#define op_unsetattr 161
#define op_waitkey 162
#define op_withobj 163
#define op_ifstart 137
#define op_ifend 144

#define meta_any 243
#define meta_every 244
#define meta_here 245
#define meta_inventory 246
#define meta_none 247
#define meta_nowhere 248
#define meta_objdesc 249
#define meta_roomdesc 250
#define meta_roomname 251
#define meta_score 252
#define meta_this 253
#define meta_unknown 254
#define meta_everywhere 255

u16 opcode_vrbidx_count;
u8 obj_count;
u8 room_count;
u8 var_count;

#define advcartridgeondisk

u16 origram_len;
u8*advcartridge;
u8*advnames;
u8*advdesc;
u8*msgs;
u8*verbs;
u8*objs;
u8*packdata;
u8*opcode_vrbidx;
u16*opcode_pos;
u8*opcode_len;
u8*opcode_data;
u8*objnameid;
u8*objdescid;
u8*objimg;
u8*objattr;
u8*objloc;
u8*roomstart;
u8*roomnameid;
u8*roomdescid;
u8*roomimg;
u8*roomattr;
u8*bitvars;
u8*vars;
u8*origram;

#define room_foyer 0
#define room_bar 1
#define room_cloakroom 2
#define room_dark_bar 3
#define room_start 4

#define obj_chandeliers 0
#define obj_cloak 1
#define obj_door 2
#define obj_entrance 3
#define obj_hall 4
#define obj_hook 5
#define obj_hooks 6
#define obj_message 7
#define obj_wall 8

#define vrb_about 0
#define vrb_close 1
#define vrb_down 2
#define vrb_drop 3
#define vrb_e 4
#define vrb_hang 5
#define vrb_help 6
#define vrb_inventory 7
#define vrb_load 8
#define vrb_lock 9
#define vrb_n 10
#define vrb_ne 11
#define vrb_nw 12
#define vrb_onenter 13
#define vrb_onfirst 14
#define vrb_onleave 15
#define vrb_open 16
#define vrb_pick_up 17
#define vrb_pull 18
#define vrb_push 19
#define vrb_quit 20
#define vrb_read 21
#define vrb_restart 22
#define vrb_s 23
#define vrb_save 24
#define vrb_score 25
#define vrb_se 26
#define vrb_search 27
#define vrb_sw 28
#define vrb_switch_on 29
#define vrb_take 30
#define vrb_throw 31
#define vrb_turn_on 32
#define vrb_unlock 33
#define vrb_up 34
#define vrb_use 35
#define vrb_w 36
#define vrb_wait 37
#define vrb_x 38

