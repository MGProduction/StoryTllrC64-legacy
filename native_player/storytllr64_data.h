#define packed_strings 4

#define string_max_len 379

u8 opcodeattr[41]={131,129,129,1,1,1,1,129,130,2,3,2,3,2,2,2,4,2,4,131,129,130,130,130,130,131,129,129,130,131,131,131,131,131,131,130,129,130,131,129,2};

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
#define op_ifundef 145
#define op_ifvar 146
#define op_list 147
#define op_load 148
#define op_msg 149
#define op_msgobj 150
#define op_msgroom 151
#define op_msgvar 152
#define op_put 153
#define op_quit 154
#define op_save 155
#define op_set 156
#define op_setattr 157
#define op_setobjname 158
#define op_setroomimage 159
#define op_setroomname 160
#define op_setroomoverlayimage 161
#define op_setvar 162
#define op_showobj 163
#define op_start 164
#define op_unset 165
#define op_unsetattr 166
#define op_waitkey 167
#define op_withobj 168
#define op_ifstart 137
#define op_ifend 146

#define meta_any 239
#define meta_available 240
#define meta_every 241
#define meta_everywhere 242
#define meta_here 243
#define meta_inventory 244
#define meta_none 245
#define meta_nowhere 246
#define meta_objdesc 247
#define meta_objname 248
#define meta_oneofobj 249
#define meta_oneofroom 250
#define meta_roomdesc 251
#define meta_roomname 252
#define meta_that 253
#define meta_this 254
#define meta_unknown 255

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

#define vrb_onfirst 0
#define vrb_onenter 1
#define vrb_onleave 2
#define vrb_onturn 3
#define vrb_ondesc 4

#define attr_visible 1

