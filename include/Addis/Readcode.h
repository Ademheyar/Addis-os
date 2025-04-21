#include <Libs/Assert/Assert.h>

#define MAX_PAIRS 64

splited_code *get_pathlists(char *paths);
void READ_DATA_PORT(uint8_t pic_irq);
void register_driver(uint8_t pic_irq, char *name, char *handler, char *retbin);
void enable_driver(uint8_t pic_irq);
char *read_bodys(char text[]);
splited_code *split_code(char str1[], char split_by);
char *get_code(char *filepath);

struct ROW *get_varinfo(int id, char *id_addrs, char from, char *getwhat);
