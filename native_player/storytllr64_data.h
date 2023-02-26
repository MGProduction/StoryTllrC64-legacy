#define packed_strings 4

#define string_max_len 251

u8 opcodeattr[40]={131,129,129,1,1,1,1,129,130,2,3,2,3,2,2,2,4,4,131,129,130,130,130,130,131,129,129,130,131,131,131,131,131,131,130,129,130,131,129,2};

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
#define op_ifisaroom 139
#define op_ifisin 140
#define op_ifisroom 141
#define op_ifkey 142
#define op_ifnot 143
#define op_ifobjinattr 144
#define op_ifvar 145
#define op_list 146
#define op_load 147
#define op_msg 148
#define op_msgobj 149
#define op_msgroom 150
#define op_msgvar 151
#define op_put 152
#define op_quit 153
#define op_save 154
#define op_set 155
#define op_setattr 156
#define op_setobjname 157
#define op_setroomimage 158
#define op_setroomname 159
#define op_setroomoverlayimage 160
#define op_setvar 161
#define op_showobj 162
#define op_start 163
#define op_unset 164
#define op_unsetattr 165
#define op_waitkey 166
#define op_withobj 167
#define op_ifstart 137
#define op_ifend 145

#define meta_any 240
#define meta_available 241
#define meta_every 242
#define meta_here 243
#define meta_inventory 244
#define meta_none 245
#define meta_nowhere 246
#define meta_objdesc 247
#define meta_oneofobj 248
#define meta_oneofroom 249
#define meta_roomdesc 250
#define meta_roomname 251
#define meta_that 252
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
u8*rooms;
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
u8*roomovrimg;
u8*roomattr;
u8*bitvars;
u8*vars;
u8*origram;

#define room_start 0
#define room_hallway 1
#define room_library 2
#define room_entrance 3
#define room_bedroom 4
#define room_dungeon 5

#define obj_bed 0
#define obj_blankets 1
#define obj_books 2
#define obj_boss 3
#define obj_button 4
#define obj_chair 5
#define obj_door 6
#define obj_gun 7
#define obj_knife 8
#define obj_note 9
#define obj_pepper 10
#define obj_picture 11
#define obj_plaque 12
#define obj_poison 13
#define obj_salt 14
#define obj_table 15
#define obj_thyme 16

#define vrb_abandon 0
#define vrb_about 1
#define vrb_accuse 2
#define vrb_close 3
#define vrb_d 4
#define vrb_drink 5
#define vrb_drop 6
#define vrb_e 7
#define vrb_elapsed 8
#define vrb_give_up 9
#define vrb_help 10
#define vrb_imacheater 11
#define vrb_inventory 12
#define vrb_invite 13
#define vrb_load 14
#define vrb_lock 15
#define vrb_mem 16
#define vrb_n 17
#define vrb_ne 18
#define vrb_nw 19
#define vrb_onenter 20
#define vrb_onfirst 21
#define vrb_onleave 22
#define vrb_open 23
#define vrb_pull 24
#define vrb_push 25
#define vrb_quit 26
#define vrb_read 27
#define vrb_restart 28
#define vrb_s 29
#define vrb_save 30
#define vrb_score 31
#define vrb_se 32
#define vrb_surrender 33
#define vrb_sw 34
#define vrb_take 35
#define vrb_talk 36
#define vrb_u 37
#define vrb_unlock 38
#define vrb_use 39
#define vrb_w 40
#define vrb_x 41
#define vrb_xyzzy 42

#define var_tmp 0
#define var_time 1
#define var_murderer 2
#define var_murderweapon 3
#define var_murderlocation 4
#define var_accused 5
#define var_accusedweapon 6
#define var_score 7


#define attr_visible 1
#define attr_listable 2
#define attr_takeable 4
#define attr_actor 8

