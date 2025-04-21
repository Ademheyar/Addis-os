#include <Addis/Drivers/Screen/Vesa/Vesa.h>

void get_value_info()
{

}


int get_takeinfo(struct USER_WDB *var, char *text, char *main_name) 
{	
	DEBUG("name %s code %s \n", var->name, text);
  //listnode_t *readword = list_get_node_by_index(r, r->on);
  while(1)
  {
		DEBUG("going to read text %s as code\n", text);
		list_t *r = str_splitL(stradd(text," ", 0), " ", 0);
    //readword = list_get_node_by_index(r, r->on);
		int i = task_list_current->holded_info->read->fread->reading_on;
		int j = task_list_current->holded_info->read->fread->reading_stoped;
    task_list_current->holded_info->read->fread->reading_on = 0;
		task_list_current->holded_info->read->fread->reading_stoped = 0;
		struct ROW *row = Resive(r, 1, 1);
		DEBUG("out from gatting fixing take info\n");
    task_list_current->holded_info->read->fread->reading_on = i;
		task_list_current->holded_info->read->fread->reading_stoped = j;
		if (row->on == 3 || row->on >= 30 && row->on < 40)
		{
			if (row->on == 33) 
			{
				DEBUG("got number info %d\n", row->value.number.rational);
				return row->value.number.rational;
			}
		}
		else if(row->on == 2 && row->word)
		{
			DEBUG("got word info %s\n", row->word);
			text = strdup(row->word);
		}
		
		else if(row->on == 4)
		{

		}

		r->on++;
	}

	//var = var;
	main_name= main_name;
	//Resive(r, 1, 0);
	//struct ROW *got = Read_Math(r, 1);
	//got=got;

	/*convertto.integer = -1;
	char *out = "";
	struct USER_WDB *var0 = var;
	char *cal(char *f, char op, char *s){
		char t[100];
		int fi = 0, si = 0;
		DEBUG("going to cal(%s %c %s)~(", f, op, s);
		convertto.integer = -1;
		convert(f);
		if(convertto.integer != -1) {
			fi = convertto.integer;
			convertto.integer = -1;
			DEBUG("%d %c", fi, op);
			convert(s);
			if(convertto.integer == -1) return stradd(f, s, op);
			si = convertto.integer;
			DEBUG("%d)", si);
		}
		else return stradd(f, s, op);
		DEBUG("\n");
		switch(op){
			case '+' :
				sprintf(t,"%d", fi+si);
			break;
			case '-' :
				if(fi > si) sprintf(t,"%d", fi-si);
				else sprintf(t,"%d", si-fi);
			break;
			case '*' :
				sprintf(t,"%d", fi*si);
			break;
			case '/' :
				sprintf(t,"%d", fi/si);
			break;
		}
		return strdup(t);;
	}

	int chack(char *code, int c, char *from){
		char *f = ""; char op = 0;
		convertto.integer = -1;
		for(; *(code + c);){
			DEBUG(" chack %c\n", *(code + c));
			if (*(code + c) == ' ') { c++; continue; } // pass ' '
			DEBUG("geting %c ", *(code + c));
			if(var0 && var0->name) DEBUG("vr0->name(%s) ", var0->name);
			if (*(code + c) == '+' || *(code + c) == '-' || *(code + c) == '*' || *(code + c) == '/') {
				op = *(code + c);
				DEBUG("op %c\n", op);
				c++;
			}
			else if (*(code + c) >= '0' && *(code + c) <= '9') {
				DEBUG("getting num\n");
				int c1 = c; char *num = "";
				for (; *(code + c1);) {
					if (*(code + c1) >= '0' && *(code + c1) <= '9') num = stradd(num, "", *(code + c1));
					else break;
					c1++;
				}
				if(issame(num, "")){
					
				}
				c = c1;
				DEBUG("found num %s\n", num);
				if(op == 0) f = stradd(f, num, 0);
				else  { f = strdup(cal(f, op, num)); op = 0; }
				continue;
			}

			if (*(code + c) == 'P' || *(code + c) == 'C' || *(code + c) == 'D'){
				char *id = "", *name = "", f = *(code + c); c++; // pass C, D OR D
				if (*(code + c) == ' ') c++; //pass ' '
				int c1 = c, norl = -1;
				for(; *(code + c1);){
					//DEBUG("c %d cl %d norl %d getting |%c| ", c, c1, norl, *(code + c1));
					if (norl == -1 && *(code + c1) == ' ') { }//DEBUG(" skping space\n");}
					else if ((norl == -1 || norl == 0) && (*(code + c1) >= '0' && *(code + c1) <= '9')) { 
						//DEBUG(" number\n");
						id = stradd(id, "", *(code + c1));
						norl = 0;
					}
					else if ((norl == -1 || norl == 1) && (*(code + c1) >= 'a' && *(code + c1) <= 'z' || *(code + c1) >= 'A' && *(code + c1) <= 'Z')) {
						//DEBUG(" latter\n");
						name = stradd(name, "", *(code + c1));
						norl = 1;
					}
					else break;
					c1++;
				}
				//DEBUG("out cl %d c %c\n", c1, *(code + c1));
				
				int found = 0;
				struct USER_WDB *serchvar = var0;
				DEBUG("got id %s or name %s\n", id, name);
				if(f == 'P') {
					DEBUG("going to get chiled parent by ");
					for(int ii = 0; serchvar->chiled_Parent; ii++) {
						serchvar = serchvar->chiled_Parent;
						if(!issame(id, "")){
							convertto.integer = -1;
							convert(id);
							if(ii == convertto.integer){
								DEBUG("id %d == %d ", convertto.integer, ii);
								var0 = serchvar; found = 1;
								c = c1;
								if(serchvar->name) DEBUG(" name %s ", serchvar->name); 
								break;
							}	
						}
						else if(!issame(name, "")) {
							if(serchvar->name) {
								if(issame(serchvar->name, name)){
									DEBUG("name %s == %s", name, serchvar->name); 
									var0 = serchvar; found = 1;
									c = c1;
									DEBUG(" on id %d ", ii);
									break;
								}
							}	
						}
					}
					DEBUG("\n");
					continue;
				}
				else if(f == 'C') {
					DEBUG("going to get chiled by ");
					DEBUG("name "); 
					int ii = 0;
					for(; ii <= serchvar->chiled_id; ii++) {
						if(!serchvar->chileds[ii]) continue;
						DEBUG("[%d]-", ii);
						if(!issame(id, "")) {
							convertto.integer = -1;
							convert(id);
							if(convertto.integer != -1 && convertto.integer == ii) { 
								DEBUG("id %d ", convertto.integer);
								if(serchvar->chileds[convertto.integer]){
									var0 = serchvar->chileds[convertto.integer]; found = 1;
									main_name = var0->name;
									c = c1;
									if(var0->name) DEBUG(" name %s ", var0->name); 
									break;
								}
							}
						}
						else if (serchvar->chileds[ii]->name ) {
							DEBUG("%s ", serchvar->chileds[ii]->name);
							if(!issame(name, "") && issame(serchvar->chileds[ii]->name, name)) {	
								var0 = serchvar->chileds[ii]; found = 1;
								main_name = var0->name;
								c = c1;
								DEBUG("==%s", name);
								break;
							}
							else if(!issame(main_name, "") && issame(serchvar->chileds[ii]->name, main_name)){
								DEBUG("~%s so [%d](", main_name, ii-1);
								if (ii-1 < 0) { DEBUG("NULL)\n");  out = "0"; break;}
								else if (serchvar->chileds[ii-1] && serchvar->chileds[ii-1]->name) {
									var0 = serchvar->chileds[ii-1]; found = 1;
									main_name = var0->name;
									DEBUG("%s)", main_name);
									break;
								}
							}
						}
					}
					DEBUG("\n"); 
					continue;
				}
				else if (f == 'D') { 
					DEBUG("goingt to get D id %s\n", id); 
					c = c1; from = stradd("D", id, 0);
					continue;
				}
				if (found == 0) out = "0";
			}
			else if (issame(out, "") && *(code + c) == 'X'){ c++; // pass x
				DEBUG("x_code ");
				if(var0 && var0->value.value.image.x0 >= 0) { char t[100]; sprintf(t,"%d",  var0->value.value.image.x0); out = strdup(t); }
				else if(var0 && var0->value.value.image.x_code) out = var0->value.value.image.x_code;
				else out = "0";
			}
			else if (issame(out, "") && *(code + c) == 'Y'){ c++; // pass y
				DEBUG("y_code ");
				if(var0 && var0->value.value.image.y0 >= 0) { char t[100]; sprintf(t,"%d",  var0->value.value.image.y0); out = strdup(t); }
				else if(var0 && var0->value.value.image.y_code) out = var0->value.value.image.y_code;
				else out = "0";
			}
			else if (issame(out, "") && *(code + c) == 'W'){ c++; // pass w
				DEBUG("w_code ");
				if (issame(from, "D1")) { char t[100]; sprintf(t,"%d", screen_info.width); out = strdup(t); from = ""; }
				else if(var0 && var0->value.value.image.w0 >= 0) { char t[100]; sprintf(t,"%d",  var0->value.value.image.w0); out = strdup(t); }
				else if(var0 && var0->value.value.image.w_code) out = var0->value.value.image.w_code;
				else out = "0";
			}
			else if (issame(out, "") && *(code + c) == 'H') { c++; // pass h
				DEBUG("h_code ");
				if (issame(from, "D1")) { char t[100]; sprintf(t,"%d", screen_info.height); out = strdup(t); from = ""; }
				else if(var0 && var0->value.value.image.h0 >= 0) { char t[100]; sprintf(t,"%d",  var0->value.value.image.h0); out = strdup(t); }
				else if(var0 && var0->value.value.image.h_code) out = var0->value.value.image.h_code;
				else out = "0";
			}

			if(!issame(out, "")) {
				out = strdup(out);
				convertto.integer = -1;
				convert(out);
				if(convertto.integer != -1) { var0 = var; main_name = var->name;}
				if(op == 0) {
					f = stradd(f, out, 0); 
					DEBUG("got %s added to f %s \n", out, f);
					out = "";
				}
				else {
					DEBUG("f %s op %c s %s\n", f, op, out);
					f = strdup(cal(f, op, out));
					op = 0; out = ""; 
				}
				if(!(*(code + c) || *(code + c+1))) {
					convertto.integer = -1;
					convert(f);
					if(convertto.integer == -1) { code = f; c = 0; }
					else return convertto.integer;
				}
			}
			DEBUG("\n");
			c++;
		}
		convertto.integer = -1;
		convert(f);
		if(convertto.integer == -1) return chack(f, 0, "");
		else return convertto.integer;
	}
	return chack(text, 0, "");*/
	return 1;
}

struct ROW *Get_VESA_DRIVER(list_t *read, int isret)
{
	struct ROW *ret = NULL;
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
	if (issame(readword->value, "WIDTH")) {
		DEBUG("%s ", readword->value);
		task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		ret = malloc(sizeof(struct ROW));
		ret->type = "NUMBER";
		ret->on = 33;
		ret->value.number.rational = screen_info.width;
		DEBUG("Done GETTING WIDTH %d \n", ret->value.number.rational);
		if (isret || !isret) return ret;
	}
	else if (issame(readword->value, "HEIGHT")) {
		DEBUG("%s ", readword->value);
		task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		ret = malloc(sizeof(struct ROW));
		ret->type = "NUMBER";
		ret->on = 33;
		ret->value.number.rational = screen_info.height;
		DEBUG("Done GETTING HEIGHT %d \n", ret->value.number.rational);
		if (isret || !isret) return ret;
	}
	return ret;
}

struct ROW *VESA_DRIVER(list_t *read, int isret) {
	struct ROW *ret0 = NULL;
	listnode_t *readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
	DEBUG("IN VESA_DRIVER ");
	ret0 = Get_VESA_DRIVER(read, isret);
	if(ret0 || isret) return ret0;

	if (issame(readword->value, "ENABLE")) {
		DEBUG("%s ", readword->value);
		task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		Set_VESA_DRIVER();
		DEBUG("Done ENABLEING\n");
		return ret0;
	}
	else if (issame(readword->value, "CLEAR")) {
		DEBUG("%s ", readword->value);
		task_list_current->holded_info->read->fread->reading_on = ++task_list_current->holded_info->read->fread->reading_stoped;
		readword = list_get_node_by_index(read, task_list_current->holded_info->read->fread->reading_on);
		Set_VESA_DRIVER();
		Draw_filled_rect(&screen_info, 0, 0, screen_info.width, screen_info.height, COLOR_BLACK);
		DEBUG("Done\n");
		return ret0;
	}

	char *fgc="", *bgc="", *x="", *y="", *w="", *h="", *pixel_w="", *pixel_h="";

	struct USER_WDB *ft = NULL;
	struct ROW *ret = NULL;
	//Clear_worktables(0);
	ret = Resive(read, 1, 1);
	DEBUG("out getting info x,y(%s, %s) w, h(%s, %s) with color(fgc(%s) bgc(%s))\n", x, y, w, h, fgc, bgc); 
	while(1)
	{
		if(ret->on == 6 && ret->value.variable.fuwdb){
			ft = ret->value.variable.fuwdb;
			DEBUG("gatting value from var name = %s type = %s | %s\n", ft->name, ft->type, ft->value.type);
			if(issame(ft->value.type, "IMAGE")) ret = &ft->value;
			if(ft->value.value.image.fg_color && !issame(ft->value.value.image.fg_color, "")) { fgc = ft->value.value.image.fg_color; ret = &ft->value; }
			if(ft->value.value.image.bg_color && !issame(ft->value.value.image.bg_color, "")) { bgc = ft->value.value.image.bg_color; ret = &ft->value; }
			if(ft->value.value.image.x_code && !issame(ft->value.value.image.x_code, "")) { x = ft->value.value.image.x_code; ret = &ft->value; }
			if(ft->value.value.image.y_code && !issame(ft->value.value.image.y_code, "")) { y = ft->value.value.image.y_code; ret = &ft->value; }
			if(ft->value.value.image.w_code && !issame(ft->value.value.image.w_code, "")) { w = ft->value.value.image.w_code; ret = &ft->value; }
			if(ft->value.value.image.h_code && !issame(ft->value.value.image.h_code, "")) { h = ft->value.value.image.h_code; ret = &ft->value; }
			if(ft->value.value.image.pixelw_code && !issame(ft->value.value.image.pixelw_code, "")) { pixel_w = ft->value.value.image.pixelw_code; ret = &ft->value; }
			if(ft->value.value.image.pixelbmp_h && !issame(ft->value.value.image.pixelbmp_h, "")) { pixel_h = ft->value.value.image.pixelbmp_h; ret = &ft->value; }	
		}
		// TODO: get working table 
		else ret = NULL;
		break;
	}

	DEBUG(" chaking new var x,y(%s, %s) w, h(%s, %s) with color(fgc(%s) bgc(%s))\n", x, y, w, h, fgc, bgc);
	if (ret != NULL || ft != NULL) {
		// to fill a box need starting point (x, y) box size (width, hight) and fgc/s
		void draw_this(char *dx, char *dy, char *dw, char *dh, char *dpixelw_code, char *dpixelbmp_h, char *dfgc, char *dbgc, struct ROW *dret, struct USER_WDB *var){
			char *dpx = "", *dpy = "", *dpw = "", *dph = "";
			if(dret == NULL && ft != NULL) dret = &var->value;
			if(issame(dx, "") && dret->value.image.x_code && !issame(dret->value.image.x_code, "")) dx = dret->value.image.x_code;
			if(issame(dy, "") && dret->value.image.y_code && !issame(dret->value.image.y_code, "")) dy = dret->value.image.y_code;
			if(issame(dw, "") && dret->value.image.w_code && !issame(dret->value.image.w_code, "")) dw = dret->value.image.w_code;
			if(issame(dh, "") && dret->value.image.h_code && !issame(dret->value.image.h_code, "")) dh = dret->value.image.h_code;
			if(issame(dfgc, "") && dret->value.image.fg_color && !issame(dret->value.image.fg_color, "")) dfgc = dret->value.image.fg_color;
			if(issame(dbgc, "") && dret->value.image.bg_color && !issame(dret->value.image.bg_color, "")) dbgc = dret->value.image.bg_color;
			if(issame(dpixelw_code, "") && dret->value.image.pixelw_code && !issame(dret->value.image.pixelw_code, "")) dpixelw_code = dret->value.image.pixelw_code;
			if(issame(dpixelbmp_h, "") && dret->value.image.pixelbmp_h && !issame(dret->value.image.pixelbmp_h, "")) dpixelbmp_h = dret->value.image.pixelbmp_h;

			if(var){
				if(issame(dx, "")) dx = "0";
				if(issame(dy, "")) dy = "0";

				if(issame(dpixelw_code, ""))
				{
					if (var->type && (issame(var->type, "VALUE") || issame(var->type, "TEXT")) ||
					dret->type && (issame(dret->type, "VALUE") || issame(dret->type, "TEXT"))) dpixelw_code = "8";
					else dpixelw_code = "1";
				}
				if(issame(dpixelbmp_h, ""))
				{
					if (var->type && (issame(var->type, "VALUE") || issame(var->type, "TEXT")) ||
					dret->type && (issame(dret->type, "VALUE") || issame(dret->type, "TEXT"))) dpixelbmp_h = "8";
					else dpixelbmp_h = "1";
				}

				if(issame(dw, "")) 
				{  
					char t[100];
					if (var->type && (issame(var->type, "VALUE") || issame(var->type, "TEXT")) ||
					dret->type && (issame(dret->type, "VALUE") || issame(dret->type, "TEXT"))) dw = dpixelw_code;
					else { sprintf(t,"%d", screen_info.width); dw = t; }
				}
				if(issame(dh, "")) 
				{
					char t[100];
					if (var->type && (issame(var->type, "VALUE") || issame(var->type, "TEXT")) ||
					dret->type && (issame(dret->type, "VALUE") || issame(dret->type, "TEXT"))) dh = dpixelbmp_h;
					else  { sprintf(t,"%d", screen_info.height); dh = t; }
				} 
				
				if(issame(dfgc, "")) {
					if(var->value.value.image.fg_color && !issame(var->value.value.image.fg_color, "")) dfgc = var->value.value.image.fg_color;
					else if(dret->value.image.fg_color && !issame(dret->value.image.fg_color, "")) dfgc = dret->value.image.fg_color;
					else dfgc = "WHITE";
				}

				if(issame(dbgc, "")) {
					if(var->value.value.image.bg_color && !issame(var->value.value.image.bg_color, "")) dbgc = var->value.value.image.bg_color;
					if(dret->value.image.bg_color && !issame(dret->value.image.bg_color, "")) dbgc = dret->value.image.bg_color;
					else dbgc = "BLACK";
				}
			}

			int x0 = 0, y0 = 0, w0 = 0, h0 = 0, pxw0 = 0, pxh0 = 0, px0 = 0, py0 = 0, pw0 = 0, ph0 = 0, ppxw0 = 0, ppxh0 = 0;
			
			if(var->chiled_Parent){
				if(var->chiled_Parent->value.value.image.pixel_w) ppxw0 = var->chiled_Parent->value.value.image.pixel_w+1;
				if(var->chiled_Parent->value.value.image.pixel_h) ppxh0 = var->chiled_Parent->value.value.image.pixel_h+1;
				if(var->chiled_Parent->value.value.image.x0) px0 = var->chiled_Parent->value.value.image.x0 + ppxw0;
				if(var->chiled_Parent->value.value.image.y0) py0 = var->chiled_Parent->value.value.image.y0 + ppxh0;
				if(var->chiled_Parent->value.value.image.w0) pw0 = var->chiled_Parent->value.value.image.w0;
				if(var->chiled_Parent->value.value.image.h0) ph0 = var->chiled_Parent->value.value.image.h0;
			}
			
			if(dret->value.image.x_code && issame(dret->value.image.x_code, "")) dret->value.image.x_code = strdup(dx);
			if(dret->value.image.y_code && issame(dret->value.image.y_code, "")) dret->value.image.y_code = strdup(dy);
			if(dret->value.image.w_code && issame(dret->value.image.w_code, "")) dret->value.image.h_code = strdup(dw);
			if(dret->value.image.h_code && issame(dret->value.image.h_code, "")) dret->value.image.w_code = strdup(dh);
			if(dret->value.image.fg_color && issame(dret->value.image.fg_color, "")) dret->value.image.fg_color = strdup(dfgc);
			if(dret->value.image.bg_color && issame(dret->value.image.bg_color, "")) dret->value.image.bg_color = strdup(dbgc);
			if(dret->value.image.pixelw_code && issame(dret->value.image.pixelw_code, "")) dret->value.image.pixelw_code = strdup(dpixelw_code);
			if(dret->value.image.pixelbmp_h && issame(dret->value.image.pixelbmp_h, "")) dret->value.image.pixelbmp_h = strdup(dpixelbmp_h);
			
			DEBUG("Startting to draw ");
			char *this_name = "";
			if(var->name) { this_name = var->name; DEBUG("name(%s) ", this_name); }
			if(var->type) DEBUG("type(%s) ", var->type);
			if(var->value.type) DEBUG("V.type(%s) ", var->value.type);
			if(var->value.word) DEBUG("V.value(value) ");
			if(var->value.value.image.text) DEBUG("V.text(%s) ", var->value.value.image.text);
			if(var->value.value.image.fg_color) DEBUG("V.fg_color(%s) ", var->value.value.image.fg_color);
			if(var->value.value.image.bg_color) DEBUG("V.bg_color(%s) ", var->value.value.image.bg_color);
			DEBUG("values x,y(%s, %s) w, h(%s, %s) px,py(%s, %s) pw,ph(%s, %s) with color(fgc(%s) bgc(%s))\n", dx, dy, dw, dh, dpx, dpy, dpw, dph, dfgc, dbgc);
			
			DEBUG("converting char info to ");
			convertto.integer = -1;
			for(int i = 0; i <= 6; i++) {
				if(i == 1 && !issame(dx, "")) x0 = get_takeinfo(var, dx, this_name);
				else if(i == 2 && !issame(dy, "")) y0 = get_takeinfo(var, dy, this_name);
				else if(i == 3 && !issame(dw, "")) w0 = get_takeinfo(var, dw, this_name);
				else if(i == 4 && !issame(dh, "")) h0 = get_takeinfo(var, dh, this_name);
				else if(i == 5 && !issame(dpixelw_code, "")) pxw0 = get_takeinfo(var, dpixelw_code, this_name);
				else if(i == 6 && !issame(dpixelbmp_h, "")) pxh0 = get_takeinfo(var, dpixelbmp_h, this_name);
			}
			DEBUG(" int \n going to get color\n");
			
			uint32_t dfgcc = -1, dbgcc = -1;
			if(dret != NULL && issame(dfgc, "")) dfgcc = dret->value.hex.hex_32;
			if(dret != NULL && issame(dbgc, "")) dbgcc = dret->value.hex.hex_32;

			dfgcc = get_color(dfgc);
			if(!(dfgcc) && dfgcc != 0x00000000) { DEBUG(" going to give WHITE COLOR for fgc\n"); dfgcc = COLOR_WHITE; }
			dbgcc = get_color(dbgc);
			if(!(dbgcc) && dbgcc != 0x00000000) { DEBUG(" going to give BLACK COLOR for bgc\n"); dbgcc = COLOR_BLACK; }
			

			var->value.value.image.x0 = x0;
			var->value.value.image.y0 = y0;
			var->value.value.image.w0 = w0;
			var->value.value.image.h0 = h0;
			var->value.value.image.pixel_w = pxw0;
			var->value.value.image.pixel_h = pxh0;

			char *var_type = "", *v = "";
			if (var && var->type) { var_type = var->type; dret = &var->value; }
			if(dret->value.image.shap_code) v = dret->value.image.shap_code;

			if(dret != NULL) {
				DEBUG(" by dret\n");
				if(dret->type) DEBUG("t1%s//t2%s|", var_type, dret->type);
				if(dret->type && issame(dret->type, "IMAGE")) { // if image given
					if (issame(var_type, "IMAGE") || issame(var_type, "")){ // given image will be drawen as it is
						DEBUG(" going to fill IMAGE x,y(%s, %s) w,h(%d, %d)-pix(%d, %d) with IMGE (%s)\n", dx, dy, w0, h0, dret->value.image.header->width_px, dret->value.image.header->height_px , dret->type);
						gfx_blit_v(&screen_info, x0, y0, w0, h0 , dret->value.image.bmp);
					}
					else if (issame(var_type, "COLOR")){  // given image will be drawen as with out given color
						DEBUG(" going to fill IMAGE x,y(%s, %s) w,h(%d, %d)-pix(%d, %d) with IMGE without color\n", dx, dy, w0, h0, dret->value.image.header->width_px, dret->value.image.header->height_px);
						gfx_blit_transparent_v(&screen_info, x0, y0, w0, h0 , dret->value.image.bmp, dfgcc);
						// if there is new idea rewright this ^^^^^^^^
					}
					else if (issame(var_type, "SHAPE")){  // given image will be filled with given shape
						DEBUG(" going to fill IMAGE x,y(%s, %s) w,h(%d, %d)-pix(%d, %d) in %s\n", dx, dy, w0, h0, dret->value.image.header->width_px, dret->value.image.header->height_px , var_type);
						if(issame(v, "RECTANGLE")) {
							Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw horizontal line
							Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw horizontal line
							gfx_blit_v(&screen_info, x0+2, y0+2, w0+2, h0+2, dret->value.image.bmp);
						}
						if(issame(v, "ROUNDED RECTANGLE")) {
							Draw_line(&screen_info, x0, y0+20, x0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, x0, y0+20, x0+20, y0, dfgcc);
							
							Draw_line(&screen_info, x0+20, y0, w0-20, y0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, w0-20, y0, w0, y0+20, dfgcc);
							
							Draw_line(&screen_info, w0, y0+20, w0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, w0, h0-20, w0-20, h0, dfgcc);
							
							Draw_line(&screen_info, x0+20, h0, w0-20, h0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, x0, h0-20, x0+20, h0, dfgcc);
							
							gfx_blit_v(&screen_info, x0+2, y0+2, w0+2, h0+2, dret->value.image.bmp);
						}
						if(issame(v, "OVAL")) {
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
						if(issame(v, "CIRCLE")) {
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
					}
				}
				else if(dret->type && issame(dret->type, "SHAPE")) { // if shape given
					if (issame(var_type, "SHAPE") || issame(var_type, "")){ // given shape will be drawen as line no bg
						if(issame(v, "RECTANGLE")) {
							DEBUG("RECTANGLE \n");
							Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw horizontal line
							Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw horizontal line
						}
						if(issame(v, "LINE")) {
							DEBUG("LINE \n");
							Draw_line(&screen_info, x0, y0, w0, h0, dfgcc);
						}
						if(issame(v, "ROUNDED RECTANGLE")) {
							DEBUG("ROUNDED RECTANGLE \n");
							Draw_line(&screen_info, x0, y0+20, x0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, x0, y0+20, x0+20, y0, dfgcc);
							
							Draw_line(&screen_info, x0+20, y0, w0-20, y0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, w0-20, y0, w0, y0+20, dfgcc);
							
							Draw_line(&screen_info, w0, y0+20, w0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, w0, h0-20, w0-20, h0, dfgcc);
							
							Draw_line(&screen_info, x0+20, h0, w0-20, h0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, x0, h0-20, x0+20, h0, dfgcc);
						}
						if(issame(v, "OVAL")) {
							DEBUG("OVAL \n");
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
						if(issame(v, "CIRCLE")) {
							DEBUG("CIRCLE \n");
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
					}
					else if (issame(var_type, "COLOR")){  // given shape will be drawen as filled with COLOR
						DEBUG(" going to fill IMAGE x,y(%s, %s) w,h(%d, %d)-pix(%d, %d) in %s \n", dx, dy, w0, h0, dret->value.image.header->width_px, dret->value.image.header->height_px , var_type);
						if(issame(v, "RECTANGLE")) {
							Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw horizontal line
							Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw horizontal line
							gfx_blit_v(&screen_info, x0+2, y0+2, w0+2, h0+2, dret->value.image.bmp);
						}
						if(issame(v, "ROUNDED RECTANGLE")) {
							Draw_line(&screen_info, x0, y0+20, x0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, x0, y0+20, x0+20, y0, dfgcc);
							
							Draw_line(&screen_info, x0+20, y0, w0-20, y0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, w0-20, y0, w0, y0+20, dfgcc);
							
							Draw_line(&screen_info, w0, y0+20, w0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, w0, h0-20, w0-20, h0, dfgcc);
							
							Draw_line(&screen_info, x0+20, h0, w0-20, h0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, x0, h0-20, x0+20, h0, dfgcc);
							
							gfx_blit_v(&screen_info, x0+2, y0+2, w0+2, h0+2, dret->value.image.bmp);
						}
						if(issame(v, "OVAL")) {
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
						if(issame(v, "CIRCLE")) {
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
					}
					else if (issame(var_type, "IMAGE")){  // given shape will be drawen as filled with image
						DEBUG(" going to fill IMAGE x,y(%s, %s) w,h(%d, %d)-pix(%d, %d) in %s\n", dx, dy, w0, h0, dret->value.image.header->width_px, dret->value.image.header->height_px , var_type);
						if(issame(v, "RECTANGLE")) {
							Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw horizontal line
							Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw horizontal line
							gfx_blit_v(&screen_info, x0+2, y0+2, w0+2, h0+2, dret->value.image.bmp);
						}
						if(issame(v, "ROUNDED RECTANGLE")) {
							Draw_line(&screen_info, x0, y0+20, x0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, x0, y0+20, x0+20, y0, dfgcc);
							
							Draw_line(&screen_info, x0+20, y0, w0-20, y0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, w0-20, y0, w0, y0+20, dfgcc);
							
							Draw_line(&screen_info, w0, y0+20, w0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, w0, h0-20, w0-20, h0, dfgcc);
							
							Draw_line(&screen_info, x0+20, h0, w0-20, h0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, x0, h0-20, x0+20, h0, dfgcc);
							
							gfx_blit_v(&screen_info, x0+2, y0+2, w0+2, h0+2, dret->value.image.bmp);
						}
						if(issame(v, "OVAL")) {
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
						if(issame(v, "CIRCLE")) {
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
					}
				}
				else if(dret->type && (issame(dret->type, "VALUE") || issame(dret->type, "TEXT"))) {
					DEBUG(" going to Draw VALUE TEXT useing x,y(%d, %d) w, h(%d, %d) pxw, pxh(%d, %d)  px,py(%d, %d) pw, ph(%d, %d) with color(fgc(%s) bgc(%s))\n", var->value.value.image.x0, var->value.value.image.y0, var->value.value.image.w0, var->value.value.image.h0, var->value.value.image.pixel_w, var->value.value.image.pixel_h, px0, py0, pw0, ph0, dfgc, dbgc);
					if(dret->word)
						Draw_String(&screen_info, px0+x0, py0+y0, w0, h0, pxw0, pxh0, pw0, ph0, dfgcc, dbgcc, dret->word);
					if(dret->value.image.text)
					Draw_String(&screen_info, px0+x0, py0+y0, w0, h0, pxw0, pxh0, pw0, ph0, dfgcc, dbgcc, dret->value.image.text);
				}
				else if(dret->type && (issame(dret->type, "COLOR") || dret->value.image.fg_color && !issame(dret->value.image.fg_color, "") || dret->value.image.bg_color && !issame(dret->value.image.bg_color, ""))) { // if shape given
					if (issame(var_type, "COLOR") || issame(var_type, "")){ // given COLOR will be drawen as it is
						DEBUG(" going to fill x,y(%s, %s) w,h(%d, %d)-\n", dx, dy, w0, h0);
						Draw_filled_rect(&screen_info, x0, y0, w0, h0 , dfgcc);
					}
					else if (issame(var_type, "VALUE") || issame(var_type, "TEXT")){  // given shape will be drawen as filled with image
						DEBUG(" going to Draw colored VALUE TEXT x,y(%d, %d) w, h(%d, %d) pxw, pxh(%d, %d)  px,py(%d, %d) pw, ph(%d, %d) with color(fgc(%s) bgc(%s))\n", var->value.value.image.x0, var->value.value.image.y0, var->value.value.image.w0, var->value.value.image.h0, var->value.value.image.pixel_w, var->value.value.image.pixel_h, px0, py0, pw0, ph0, dfgc, dbgc);
						if(dret->on == 2 && dret->word)
						Draw_String(&screen_info, px0+x0, py0+y0, w0, h0, pxw0, pxh0, pw0, ph0, dfgcc, dbgcc, dret->word);
						if(dret->value.image.text)
						Draw_String(&screen_info, px0+x0, py0+y0, w0, h0, pxw0, pxh0, pw0, ph0, dfgcc, dbgcc, dret->value.image.text);
					} 
					else if (issame(var_type, "IMAGE")){  // given shape will be drawen as filled with image
						DEBUG(" going to fill IMAGE x,y(%s, %s) w,h(%d, %d)-pix(%d, %d) with IMGE without color\n", dx, dy, w0, h0, dret->value.image.header->width_px, dret->value.image.header->height_px);
						gfx_blit_transparent_v(&screen_info, x0, y0, w0, h0 , dret->value.image.bmp, dfgcc);
					}
					else if (issame(var_type, "SHAPE")){  // given COLOR will be filled in the shape
						if(issame(v, "RECTANGLE")) {
							DEBUG("RECTANGLE \n");
							Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw vertical line
							Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw horizontal line
							Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw horizontal line
						}
						if(issame(v, "LINE")) {
							DEBUG("LINE \n");
							Draw_line(&screen_info, x0, y0, w0, h0, dfgcc);
						}
						if(issame(v, "ROUNDED RECTANGLE")) {
							DEBUG("ROUNDED RECTANGLE \n");
							Draw_line(&screen_info, x0, y0+20, x0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, x0, y0+20, x0+20, y0, dfgcc);
							
							Draw_line(&screen_info, x0+20, y0, w0-20, y0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, w0-20, y0, w0, y0+20, dfgcc);
							
							Draw_line(&screen_info, w0, y0+20, w0, h0-20, dfgcc); // to draw vertical line
							Draw_round(&screen_info, w0, h0-20, w0-20, h0, dfgcc);
							
							Draw_line(&screen_info, x0+20, h0, w0-20, h0, dfgcc); // to draw horizontal line
							Draw_round(&screen_info, x0, h0-20, x0+20, h0, dfgcc);
						}
						if(issame(v, "OVAL")) {
							DEBUG("OVAL \n");
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
						if(issame(v, "CIRCLE")) {
							DEBUG("CIRCLE \n");
							//Draw_line(&screen_info, x0, y0, x0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, w0, y0, w0, h0, dfgcc); // to draw horizontal line
							//Draw_line(&screen_info, x0, y0, w0, y0, dfgcc); // to draw vertical line
							//Draw_line(&screen_info, x0, h0, w0, h0, dfgcc); // to draw vertical line
						}
					}
				}
				else if (!(issame(dfgc, "")) || !(issame(dbgc, ""))) {
					DEBUG(" going to fill rect useing x,y(%d, %d) w, h(%d, %d) pxw, pxh(%d, %d)  px,py(%d, %d) pw, ph(%d, %d) with color(fgc(%s) bgc(%s))\n", var->value.value.image.x0, var->value.value.image.y0, var->value.value.image.w0, var->value.value.image.h0, var->value.value.image.pixel_w, var->value.value.image.pixel_h, px0, py0, pw0, ph0, dfgc, dbgc);
					Draw_filled_rect(&screen_info, x0, y0, w0, h0, dbgcc);
				}
			}
			else if (!(issame(dfgc, "")) || !(issame(dbgc, ""))) {
				DEBUG(" going to fill fgc x,y(%s, %s) w, h(%s, %s) with color(fgc(%s) bgc(%s))\n", dx, dy, dw, dh, dfgc, dbgc);
				Draw_filled_rect(&screen_info, x0, y0, w0, h0, dfgcc);
			}

			if(var && var->first_chiled){
				struct USER_WDB *chiled = var->first_chiled; 
				while(1)
				{
					if(chiled && chiled->value.on == 4){
						draw_this("", "", "", "", "", "", "", "", &chiled->value, chiled);
					}
					if(chiled->next_table) chiled = chiled->next_table;
					else break;
				}
			}
		}
		DEBUG("This can be drown\n");
		draw_this(x, y, w, h, pixel_w, pixel_h, fgc, bgc, ret, ft);
	}
	else DEBUG("This can not been drown\n");
	return ret0;
}

//struct Screen_info_struct screen_info;
void Set_VESA_DRIVER() {
  multiboot_tag_t *tag0; 
  for (tag0 = (multiboot_tag_t *) multiboot_info->tags; tag0->type != MULTIBOOT_TAG_TYPE_END; 
			tag0 = (multiboot_tag_t *)((uint8_t *) tag0 + ((tag0->size + 7) & ~7))) {

		if (tag0->type == MULTIBOOT_TAG_TYPE_FRAMEBUFFER) {
			//struct multiboot_tag_framebuffer *tagfb = (struct multiboot_tag_framebuffer *)multiboot_info->tag;
			screen_info.linear_addr = (uint64_t)(unsigned long) tag0->framebuffer_addr;
			screen_info.addr = (uint64_t)KERNEL_VIDEO_MEMORY;
			screen_info.width = tag0->framebuffer_width;
			screen_info.height = tag0->framebuffer_height;
			screen_info.x = 0;
			screen_info.y = 0;
			screen_info.bits = tag0->framebuffer_bpp;
			screen_info.pitch = tag0->framebuffer_pitch;
			screen_info.type = tag0->framebuffer_type;
			DEBUG("VESA: Frame buffer addr: 0x%X\n", screen_info.addr);
			DEBUG("VESA: Frame buffer linear addr: 0x%X\n", screen_info.linear_addr);
			DEBUG("VESA: Frame buffer info: %i x %i : %ibpp\n", screen_info.width, screen_info.height, screen_info.bits);
			DEBUG("VESA: Frame buffer pitch: %i\n", screen_info.pitch);
			DEBUG("VESA: Frame buffer type: %i\n", screen_info.type);
			break;
		}
		
  }

  uint64_t video_framebuffer = screen_info.linear_addr;
  DEBUG("MMU: Video memory %0x\n", video_framebuffer);
	DEBUG("MMU: Video memory : %i x %i : %ibpp\n", screen_info.width, screen_info.height, screen_info.bits);

  memset(&pdpe_video, 0, sizeof(pdpe_t) * 512);
  memset(&pde_video, 0, sizeof(pde_t) * 512);

  pml4e[0x180].all = TO_PHYS_U64(&pdpe_video) | 3; // KERNEL_VMA: Present + Write (0x180 : KERNEL_VIDEO_MEMORY)
  pdpe_video[0].all = TO_PHYS_U64(&pde_video) | 3; 
  // Dummy way : map whole PDE ... (more than needed but it's simpel enough)
  // TODO : if user have memory on this address - there will be a PROBLEM :D
  for (uint64_t i=0; i<256; i++) {
    pde_video[i].all = (video_framebuffer + (i * PAGE_SIZE)) | 0x83; // Present + Write + Large (2MB)
  }
}

