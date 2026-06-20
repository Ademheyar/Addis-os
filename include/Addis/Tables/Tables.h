#ifndef TABLES_H
#define TABLES_H



#include <Addis/Libs/List/List.h> // for defining list_t, listnode_t, and other list-related functions
#include <Libs/Gui/Pictures/Bitmap/Bitmap.h>

#include <Addis/Drivers/Filse_system/Ata/Ata.h>

#include <Libs/Malloc/Mmu_frames.h>
#include <Libs/Malloc/Mmu_heap.h>
#include <Libs/Malloc/Mmu_paging.h>


typedef struct {
	char code_splited[10000][100];
	char *code_splited1[100];
	int lines;
}splited_code;

//
struct ROW {
	// commen
	char *As;
	char *type;
	char *word;
	int on;

	union {
		char *Buffer; // this will save string or path or any value that can be saved in char *
		char *istype;

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
			struct READINGINFO *fuwdb;
			char *prop;
		}variable;
		
	} value;

	struct ROW *prev_row;
	struct ROW *next_row;
};

//
struct COLUMN {
	int columnid;
	struct ROW *row; // the first row of this column will be saved here to add new row after it
	struct ROW *last_row; // The last row created in this column will be saved here to add new row after it
	struct ROW *focused_row; // this will be used to save where it is reading or processing or where it will add new row or if it was processing var it will be pointed at focused row
	struct COLUMN *prev_column; // chane to previuse column
	struct COLUMN *next_column; // chane to next column
};

struct WORKTABLE {
	struct COLUMN *column;
	struct COLUMN *focused_column;
	struct COLUMN *last_column;
	struct WORKTABLE *prev_wt; // chane to previuse worktable
	struct WORKTABLE *next_wt; // chane to next worktable
};


/*
*	WORKTABLES
* |--------------------------------------------_|
* | ROW 0  |                                    | Cloumn 0
* |--------------------------------------------_|
* |        |                                    | Cloumn 1
* |--------------------------------------------_|
* |        |                                    | Cloumn 2
* |--------------------------------------------_|
* |        |                                    | Cloumn 3
* |--------------------------------------------_|
* |________|____________________________________| .......
* |        |                                    |
* Warktable 0 Column 0 Rows All was Woring Processing Arias
* All Names Or Named Variable will be Stord In this Table
*/

typedef struct {
	struct WORKTABLE *worktable;
	struct WORKTABLE *focused_wt;
	struct WORKTABLE *last_wt;
}WORKTABLES;





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




typedef struct READINGINFO {
	// this is the used to save all data and code will reading and processing codes
	
	// Holding Variable that is on going to be used in Tasks
	// this is for reading code and processing code to save focused table and variable
	// NOTE: this is not for saving var but for holding var that is on going to be used in reading or processing code
	// Need to be Diclared in struct READINGINFO because it will be used in reading and processing code and it is nessasery to save it in main table to be able to use it in all code

	/*
	*	WORKTABLES
	* |--------------------------------------------_|
	* | ROW 0  |                                    | Cloumn 0
	* |--------------------------------------------_|
	* |        |                                    | Cloumn 1
	* |--------------------------------------------_|
	* |        |                                    | Cloumn 2
	* |--------------------------------------------_|
	* |        |                                    | Cloumn 3
	* |--------------------------------------------_|
	* |________|____________________________________| .......
	* |        |                                    |
	* Warktable 0 Column 0 Rows All was Woring Processing Arias
	* All Names Or Named Variable will be Stord In this Table
	*/
	WORKTABLES worktables; 
	
	





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
	int erorr_id;


	struct event *main_event;
	struct event *foucsed_event;
	struct event *last_event;

	//
	struct READINGINFO *chileds[1]; // this is sub chiled
	struct READINGINFO *first_chiled; 
	struct READINGINFO *foucsed_chiled; 
	struct READINGINFO *last_chiled;

	struct READINGINFO *chiled_Parent; 

	struct READINGINFO *variables[1]; // this is sub codes
	
	struct READINGINFO *Parent_table; // State
	struct READINGINFO *Main_table; // Locale
  int varsid, tvarsid, chiled_id;



	
	// bootinfo
	splited_code *bootinfo;
	char **hold_event;
	int booton;
	
	// makeing list of chains
	struct READINGINFO *read_prev; // previuse sube code
	struct READINGINFO *read_next; // next sube code
} READINGINFO;






/*
*
*	WORKINGTABLE
*/
void Create_working_place(char *newcode, char on);
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
//
typedef struct {
	// user working variables saved there is
	char *USER_NAME, *USER_PASSWORD;
	// READINGINFO : here is main application runs code will store
	struct READINGINFO *READINGINFO[100]; // use this for user codes

	// user_eventdb :  here will run event codes
	struct READINGINFO *user_eventdb[100];
	// user_intdb :  here will run interapt codes
	struct READINGINFO *user_intdb[100]; 
	int uwdb_id, uedb_id, uidb_id;
} Loged_user;


struct ROW *Find_Variable_by(char *name); // for finding varible from local up to global by name, 


void LogIn();
void Remove_ALLF_READING();
void Delete_lateworking_place();
void Create_readtable_bn();
struct READINGINFO *Create_vartable(list_t *read, struct READINGINFO *var, int isret);
void Remove_this_readtable();

void Add_chiled_to_parent(struct READINGINFO *parent, struct READINGINFO *chiled);

typedef struct {
	char array[10000];
	int integer;
	uint8_t hexa_8;
	uint16_t hexa_16;
	uint32_t hexa_32;
	char list[10000][100], *prevtex[10];
	int listline, linstcount, prevon;
}to;
extern to convertto;

typedef struct {
  char *driv_name, *main_name, *driv_handler, *irq_ack; // varabel main info
  uint8_t driv_port, driv_id;
  _Bool driv_enabled, driv_registered;
  struct READINGINFO *onwdb; 
//	isr_t interrupt_handlers;
}DRIVERS;
// to reagistor drivers
extern DRIVERS drivers[256];

// main wordking aria for focesed code
typedef struct {
	// fmainpon:- talls where the sysrem on global local or TEMPRARE
	char fgivenpon, fmainpon;
} SYSVARS;
// in this tabel all sysrem setting will be saved
//

typedef struct {
	// this will be loaded with boot loder and it will be saved until system is off or loged out	
	// here will be stored collective that and task or code need informations


	
	// Unversal Variables will be saved in those
	// this will be called or setted by external varible  ->extern
	struct ROW *first_extern_varible; // the first and main 
	struct ROW *focused_extern_varible; // focused extrnal variable 
	struct ROW *last_extern_varible; // the last setted varible
	







	struct READINGINFO *first_Unversal; // the first and main 
	struct READINGINFO *focused_Unversal; // focused extrnal variable 
	struct READINGINFO *last_Unversal; // the last setted varible

	// defened 
	// this is for defenition code and defenition Not Known Words that will be Explained by user in defenition code
	// this will be used in defenition code to save defenition code and defenition Not Known Words that will be Explained by user in defenition code
	// it can be local or global depanding on task and sub task reding or processing code
	// Note: this is not for saving var but for holding var that is on going to be used in reading or processing code
	// Not soure if hoding it here or in holded_info is better but for now we will hold it here because it is more easy to access it in reading and processing code and it is not nessasery to save it in main table to be able to use it in all code
	
	struct DEF_LIST *main_def; // this will save main defenition list that is used in this task 
	struct DEF_LIST *focused_def; // this will save focused defenition list that is used in this task 
	struct DEF_LIST *last_def; // this will save last defenition list that is used in this task

	


	// char
	char *USER_NAME, *USER_PASSWORD, *userpath;
	char *sys_mode, statce;

	char *Local_name;
	// NUM
	int user_id, started, wdb_rid, wdb_gid;
	
	// here will be stored focesd places or tables 
	struct Loged_user *loged_user; // this will hold focesd user
	
	


	// until it is done reading or tamprarly
	//
	struct READINGINFO *fRwdb; // this will hold reading wdb until it is done reading

	struct READINGINFO *fSREADINGINFO; // S this will hold reading READINGINFO until it is done reading
	struct READINGINFO *fTREADINGINFO; // T this will hold reading READINGINFO until it is done reading
	struct Loged_user *floged_user; // this will hold focesd user
	struct READINGINFO *ftgwdb; // this will hold found main(S) READINGINFO for sometime
	
	// here will be stored focesd places or tables

	struct READINGINFO *ftm_uwdb; // this will hold found main(S) READINGINFO for sometime
	struct READINGINFO *ftv_uwdb; // this will hold found READINGINFO for sometime

	int state_count, sys_id, uwdb_id, uidb_id;
	// wdb
	struct READINGINFO *ftread[10];

	int fsrid[10], fscid[10], fswid[10], fids;


	
} SYSTEMINFO;

extern SYSTEMINFO sysinfo;



// for setting varible extern
struct ROW *Set_varible_extern(struct ROW *var);

struct ROW *read_unknownvars(list_t *read, char *forwhat, int ret_resualt);
struct ROW *get_vars(list_t *read, int isret);



extern struct WORKTABLE *ReadDo();
void read_words();
uint32_t get_color(char *colorname);
void list_allin_table();
struct ROW *ReadResivers(list_t *read, char *read_what, char *dowhat, char from, int isret);
struct ROW *get_table(list_t *read, int isret);

// table.h
struct READINGINFO *Get_main_dt(struct READINGINFO *g_main_dt, list_t *read);

// workingT.h
char *get_row_type(struct ROW *row);
void copy_row(struct ROW *dest, struct ROW *row);
struct ROW *get_as_row(char *name);
struct ROW *marge_rows(char *op, struct ROW *v1, struct ROW *v2);

//fix_var.h
_Bool compar(char *comp, struct ROW *v1, struct ROW *v2);

// event.h
void _event(list_t *read);
void Chack_all_events(struct READINGINFO *var, char *happend, struct ROW *value);
struct ROW *Convert_text(char *type, char *value);

// Read_get.h
char *get_value_type(char *value);
struct READINGINFO *find_dt(struct READINGINFO *glist, char *USER_NAME, char *USER_PASSWORD, int id_db, char *Local_name, char *State_name, char *name, char *id_addrs);

// resesive.h
struct ROW *Resive(list_t *read, int isgroup, int isret);


extern struct ROW *Get_VESA_DRIVER(list_t *read, int isret);
extern struct ROW *VESA_DRIVER(list_t *read, int isret);


struct WORKTABLE *Find_worktable_by_indexs(int table_id, int column_id, int row_id);
struct WORKTABLE *Find_worktable_by_name(const char *name);


#endif