#include <Addis/Tables/DefT.h>

#ifndef TABLES_H
#define TABLES_H

typedef struct {
	char code_splited[10000][100];
	char *code_splited1[100];
	int lines;
}splited_code;

typedef struct READINGINFO {
	// bootinfo
	splited_code *bootinfo;
	char **hold_event;
	int booton;

	struct READINGINFO *read_prev; // previuse sube code
	struct READINGINFO *read_next; // next sube code
	struct READINGINFO *read; // main sube code

	struct USER_WDB *fread; // reading sube code

} READINGINFO;

typedef struct task_struct {
  uint64_t rsp;
  uint32_t id;
  uint16_t attribute;
  uint16_t state;
	uint16_t isreloaded;
  uint8_t *kstack;
  // uint8_t kstack[KERNEL_STACK_SIZE];
  // For now we are supporting only 2MB programs :) 
  // We keep track of one entry in PDE that will be mapped to 0x0000000 (user program space)
  pde_t *pde;
  // pde_t pde[512]; 
  // Win manager reference if any
  void* window;
	void* fac;
	struct READINGINFO *holded_info;

  struct task_struct* next;
  struct task_struct* prev;
  struct task_struct* parent;
}  __attribute__((packed)) task_t;

//
struct ROW {
	// commen
	char *As;
	char *type;
	char *word;
	int on;

	union {
		char charcter;

		struct {
			int whole;
			int rational;
			int natural;
		}number;

		struct {
			uint8_t hex_8;
			//uint16_t hex_16;
			uint32_t hex_32;

			uint8_t *hexs_8;
			//uint16_t *hexs_16;
			uint32_t *hexs_32;
		}hex;
		
		struct {
			_Bool isvisabel, isupdate;
			char *x_code, *y_code, *w_code, *h_code;
			char *fg_color, *bg_color, *shap_code, *text;
			uint32_t *bmp; // this will save buffer
			
			char *bmp_h, *pixelw_code, *pixelbmp_h;
			
			// unchangable
			bmp_header_t *header;
			// this is for image more info will be in header
			int x0, y0, w0, h0, pixel_w, pixel_h;	
		}image;
		
		struct {
			struct USER_WDB *fuwdb;
			char *prop;
		}variable;
		
	}value;

	struct ROW *prev_row;
	struct ROW *next_row;
};

//
struct COLUMN {
	int columnid;
	struct ROW *row; // value
	struct ROW *last_row;
	struct ROW *focused_row;
	struct COLUMN *prev_column;
	struct COLUMN *next_column;
};

struct WORKTABLE {
	struct COLUMN *column;
	struct COLUMN *last_column;
	struct COLUMN *focused_column;
	struct WORKTABLE *prev_wt;
	struct WORKTABLE *next_wt;
};

typedef struct {
	struct WORKTABLE *worktable;
	struct WORKTABLE *focused_wt;
	struct WORKTABLE *last_wt;
}WORKTABLES;

/*
*
*	WORKINGTABLE
*/
void Create_worktable(struct WORKTABLE *n);
void Create_newworktable();
void Get_worktable_byindex(int wid);
void Return_lastworktable();

/*
*
*	struct COLUMN
*/
void Create_column(struct COLUMN *n);
void Create_newcolumn();
void Get_column_byindex(int cid);
void Create_lastcolumn_byindex();

/*
*
*	struct ROW
*/

void Create_row(struct ROW *n);
void Create_newrow();
struct ROW *Create_nextrow(struct ROW *n);
//void Create_nextrow_byindex(int wtid, int wcid);
void Create_lastrow_byindex(int wtid, int wcid);
void Get_row_byindex(int rid);
void copymainrow_value(struct ROW *fromrow, struct ROW *torow);
void Add_rowvalue(struct ROW *n, char *type, void *value);
void Add_row_to_frow(struct ROW *row);
void Addrow_to_row_byindex(struct ROW *row, int wt, int wr);

void Clear_worktables(int which);
void Create_workingtable();
struct ROW *get_worktable(list_t *read, int isret);

//
typedef struct {
	char *id_addrs;
	char *state;
	char *state_name;
	char *def;
	char *erorr;
} ERORR;



struct event {
	char *by;
  struct ROW *ishappen;
	struct ROW *compare;
  struct ROW *prop;
	struct ROW *event_value;
	struct ROW *do_value;
	
	struct event *prev_event; // previuse
	struct event *next_event; // Locale
};

// this is the main db contains
//
struct USER_WDB {
	int id;
  char *id_addrs;
	char state;
	char *name;
	char *user_name, *user_password;	
	
	char *type; // table type

	// for others value
	char *withcodes; // fanction with
	struct ROW value; // this will tall or give type and value  // DO or word Value

	// reading code
	char *read_new; // if new code added or moust be readen 
	char *main_readcode; // this will converted code if it is nessasery
	list_t *reading_value; // this will holed reading code words
	list_t *bodys; // this will hold grouped codes
	WORKTABLES worktables; // contanse 
	char *reading_for[10]; // this will tell reading for what
	struct task_struct *reading_task;
	char *getwhat, *getas;

	int rfor_id, reading_stoped, reading_on;


	// If
	_Bool ifon, ifren;
	int rnextcode; // rnextcode(readnextcode) will tall if the next code will be readn or not

	// LOOP
	_Bool Break;
	int read_loopoint;

	char varon; // on visiblete
  int tid;

	_Bool Continue, loopon; // loops
	ERORR erorr[100];
	int erorr_id;


	struct event *main_event;
	struct event *foucsed_event;
	struct event *last_event;

	//
	struct USER_WDB *chileds[1000]; // this is sub chiled
	struct USER_WDB *first_chiled; 
	struct USER_WDB *foucsed_chiled; 
	struct USER_WDB *last_chiled;

	struct USER_WDB *chiled_Parent; 

	struct USER_WDB *prev_table; // previuse
	struct USER_WDB *next_table;

	struct USER_WDB *variables[1000]; // this is sub codes
	
	struct USER_WDB *Parent_table; // State
	struct USER_WDB *Main_table; // Locale
  int varsid, tvarsid, chiled_id;
};

//
//
typedef struct {
	// user working variables saved there is
	char *USER_NAME, *USER_PASSWORD;
	// user_wdb : here is main application runs code will store
	struct USER_WDB *user_wdb[100]; // use this for user codes

	// user_eventdb :  here will run event codes
	struct USER_WDB *user_eventdb[100];
	// user_intdb :  here will run interapt codes
	struct USER_WDB *user_intdb[100]; 
	int uwdb_id, uedb_id, uidb_id;
} Loged_user;

Loged_user *loged_user[5];


struct READINGINFO *get_reading_table(char c);;

void LogIn();
void Create_working_place();
void Remove_ALLF_READING();
void Remove_READING();
void Create_readinfo(char on);
void Delete_lateworking_place();
void Create_readtable_bn();
struct USER_WDB *Create_vartable(list_t *read, struct USER_WDB *var, int isret);
void Remove_this_readtable();

void Add_chiled_to_parent(struct USER_WDB *parent, struct USER_WDB *chiled);

typedef struct {
	char array[10000];
	int integer;
	uint8_t hexa_8;
	uint16_t hexa_16;
	uint32_t hexa_32;
	char list[10000][100], *prevtex[10];
	int listline, linstcount, prevon;
}to;
to convertto;

typedef struct {
  char *driv_name, *main_name, *driv_handler, *irq_ack; // varabel main info
  uint8_t driv_port, driv_id;
  _Bool driv_enabled, driv_registered;
  struct USER_WDB *onwdb; 
//	isr_t interrupt_handlers;
}DRIVERS;
// to reagistor drivers
DRIVERS drivers[256];

// main wordking aria for focesed code
typedef struct {
	// fmainpon:- talls where the sysrem on global local or TEMPRARE
	char fgivenpon, fmainpon;
} SYSVARS;
SYSVARS sysvars[5];
// in this tabel all sysrem setting will be saved
//

typedef struct {
// until system is off or loged out	
	// here will be stored informations
	// char
	char *USER_NAME, *USER_PASSWORD, *userpath;
	char *sys_mode, statce;

	char *Local_name;
	// NUM
	int user_id, started, wdb_rid, wdb_gid;
	
	// here will be stored focesd places or tables 
	struct Loged_user *loged_user; // this will hold focesd user

	struct USER_WDB *wdb_R[1000]; // this will holed prossesing
	struct USER_WDB *wdb_G[100]; // use this to save global vars

	struct USER_WDB *first_Unversal;
	struct USER_WDB *focused_Unversal;
	struct USER_WDB *last_Unversal;
	
// until it is done reading or tamprarly
	//
	struct USER_WDB *fRwdb; // this will hold reading wdb until it is done reading

	struct USER_WDB *fSuser_wdb; // S this will hold reading user_wdb until it is done reading
	struct USER_WDB *fTuser_wdb; // T this will hold reading user_wdb until it is done reading
	struct Loged_user *floged_user; // this will hold focesd user
	struct USER_WDB *ftgwdb; // this will hold found main(S) user_wdb for sometime
	
	// here will be stored focesd places or tables

	struct USER_WDB *ftm_uwdb; // this will hold found main(S) user_wdb for sometime
	struct USER_WDB *ftv_uwdb; // this will hold found user_wdb for sometime

	int state_count, sys_id, uwdb_id, uidb_id;
	// wdb
	struct USER_WDB *ftread[10];

	int fsrid[10], fscid[10], fswid[10], fids;


	// defened 
	struct DEF_LIST *main_def;
	struct DEF_LIST *focused_def;
	struct DEF_LIST *last_def;
} SYSTEMINFO;

SYSTEMINFO sysinfo;


extern void ReadDo();
void read_words();
uint32_t get_color(char *colorname);
void list_allin_table();
struct ROW *read_unknownvars(list_t *read, char *forwhat, int ret_resualt);
struct ROW *ReadResivers(list_t *read, char *read_what, char *dowhat, char from, int isret);
struct ROW *get_table(list_t *read, int isret);

// table.h
struct USER_WDB *Get_main_dt(struct USER_WDB *g_main_dt, list_t *read);

// workingT.h
char *get_row_type(struct ROW *row);
void copy_row(struct ROW *dest, struct ROW *row);
struct ROW *get_as_row(char *name);
struct ROW *marge_rows(char *op, struct ROW *v1, struct ROW *v2);

//fix_var.h
_Bool compar(char *comp, struct ROW *v1, struct ROW *v2);

// event.h
void _event(list_t *read);
void Chack_all_events(struct USER_WDB *var, char *happend, struct ROW *value);
struct ROW *Convert_text(char *type, char *value);

// Read_get.h
char *get_value_type(char *value);
struct USER_WDB *find_dt(struct USER_WDB *glist, char *USER_NAME, char *USER_PASSWORD, int id_db, char *Local_name, char *State_name, char *name, char *id_addrs);
struct ROW *get_vars(list_t *read, int isret);

// resesive.h
struct ROW *Resive(list_t *read, int isgroup, int isret);
#endif