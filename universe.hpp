

#ifndef local_build
#	include <actionswf/actionswf.h>
#else
#	include "/home/bc/s/actionswf-1/dev/actionswf.h"
#endif

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

#define bar_dimensiune 20

#define width_nr 800
#define width (width_nr-bar_dimensiune)
#define height 600

#define fps 16
#define cell 20

#define bar_x width
#define bar_y 0
#define bar_w bar_dimensiune
#define bar_h height

#define text_off_dividend 3
#define text_off_dispenser 2

#define maxuint 10

void declare_grid(int grid_width,int grid_height,char* grid,char* init_value);
void debug_grid(char* container,int grid_width,int grid_height,char* grid,int dist,int color);

void shared_write(); void shared_write_ex(bool circle, int fuel, int rotation, bool space_env, int keys);

int presprite;int sprite;
int bmp;//in ep 10 si altele
int dbl;

#define buf_size 131072
char buf[buf_size];
char buf2[buf_size];//la din_buf_in_buf
//#define information "informer"

typedef struct key__struct
{
    char* name;
    char* defaultKey;
    char* disp_name;
}key_struct;
key_struct left_arrow={"left_arrow","LEFT","Left Direction"};//hardcodat la ep7
key_struct right_arrow={"right_arrow","RIGHT","Right Direction"};//hardcodat la ep7
key_struct up_arrow={"up_arrow","UP","Up Direction"};
key_struct down_arrow={"down_arrow","DOWN","Down Direction"};//hardcodat la ep7
key_struct action_key={"action_key","SPACE","Primary Action"};//atentie ca e hardcodat la ep24
key_struct aux_key={"aux_key","CONTROL","Secondary Action"};
key_struct strafe_left={"strafe_left","Z","Strafe Left"};
key_struct strafe_right={"strafe_right","X","Strafe Right"};
key_struct auto_shot={"auto_shot","A","Auto-Shot"};
key_struct auto_move={"auto_move","S","Auto-Move"};
char* mouse_disp_name = "Touch";

key_struct* keyboards[]={&left_arrow,&right_arrow,&up_arrow,&down_arrow,&action_key,&aux_key,&strafe_left,&strafe_right,&auto_shot,&auto_move};
int keyboards_total=sizeof(keyboards)/sizeof(key_struct*);

typedef struct ep__keys{
    key_struct* key;
    char* key_desc;
	char* key_alt;
}ep_keys;
#define flashixy 0
#define kongregate 1
typedef struct episooade_struct
{
    char* name;
    char* description;
    ep_keys* episod_chei;
    char*mousePress;
	char*id;
	char base;
	char*designation;
//unsigned int id2;//prin anul 2106 unsigned maximum
}episoade_pack;

ep_keys Universe_keys[]={{NULL}};//straight {{NULL}} will not work
episoade_pack Universe={"Universe","Multiple games.",Universe_keys,NULL,"universe",kongregate,"Universe"};
//
#define Ores_for_Ship_ep "Ores_for_Ship"
ep_keys Ores_for_Ship_keys[]={{&left_arrow,"Movement","Left Direction"},{&right_arrow,"Movement","Right Direction"},{&up_arrow,"Movement","Up Direction"},{&down_arrow,"Movement","Down Direction"},{NULL}};
episoade_pack Ores_for_Ship={Ores_for_Ship_ep,"Gather all the required minerals.",Ores_for_Ship_keys,NULL,"digger",kongregate,"Digger"};//,128541 ,5706
//
#define Driving_the_Ship_ep "Driving_the_Ship"
ep_keys Driving_the_Ship_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&up_arrow,"Movement","Up Direction" },{&down_arrow,"Movement","Down Direction" },{NULL}};
episoade_pack Driving_the_Ship = { Driving_the_Ship_ep,"Pass through the blue clouds and bonuses. Bonuses: Green = cloud to ship; Blue = free taker; Yellow = cable taker; Red = shield.",Driving_the_Ship_keys,NULL,"driver",kongregate,"Driver"};//,128542
//
#define Planet_Landing_ep "Planet_Landing"
ep_keys Planet_Landing_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&down_arrow,"Movement","Down Direction" },{NULL}};
episoade_pack Planet_Landing={Planet_Landing_ep,"Pass through the obstacles and land on the planet.",Planet_Landing_keys,NULL,"lander",kongregate,"Lander"};//,128543
//
#define Rocks_Fall_ep "Rocks_Fall"
ep_keys Rocks_Fall_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&up_arrow,"Move gun","Gun Up Direction"},{&down_arrow,"Move gun","Gun Down Direction"},{&action_key,"Shot","Shot"},{&aux_key,"Reset gun position","Reset gun position" },{ &auto_shot,"Continuous shot","Auto-Shot" },{ &auto_move,"Continuous right","Auto-Move" },{NULL}};
episoade_pack Rocks_Fall={Rocks_Fall_ep,"Pass through the obstacles.",Rocks_Fall_keys,NULL,"expeditioner",kongregate,"Expeditioner"};//,128544
//
#define Balls_Play_ep "Balls_Play"
ep_keys Balls_Play_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&action_key,"Launch circle","Launch circle" },{&aux_key,"Move slower (pressing)","Move slower (pressing)" },{NULL}};
episoade_pack Balls_Play={Balls_Play_ep,"The field circles must hit the galaxy. The field circles must not hit the defender and must hit the line behind the defender.",Balls_Play_keys,NULL,"balls",kongregate,"Balls"};//,128545
//
#define Asteroids_ep "Asteroids"
ep_keys Asteroids_keys[]={{&left_arrow,"Movement","Rotate Left" },{&right_arrow,"Movement","Rotate Right" },{&up_arrow,"Movement","Forward" },{&down_arrow,"Movement","Backward" },{&action_key,"Shot","Shot"},{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack Asteroids={Asteroids_ep,"Shot the asteroids. There are four types of bonuses and a progress bar.",Asteroids_keys,NULL,"asteroids",kongregate,"Asteroids"};//,128546
//
ep_keys Cubes_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&down_arrow,"Cube fall","Cube fall" },{NULL}};
episoade_pack Cubes={"Cubes","Match the cubes. A match is formed by three cubes or more placed one after another horizontally or vertically. Hold Left/Right to move faster.",Cubes_keys,NULL,NULL,kongregate,"cubes"};//,128548
//
#define Moving_Forward_ep "Moving_Forward"
ep_keys Moving_Forward_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{NULL}};
episoade_pack Moving_Forward={Moving_Forward_ep,"Pass through the obstacles.",Moving_Forward_keys,NULL,"watersport",kongregate,"Watersport"};//,128547
//
#define Jump_ep "Jump"
ep_keys Jump_keys[]={{&left_arrow,"Movement","Left Direction" },{&right_arrow,"Movement","Right Direction" },{&action_key,"Jump","Jump" },{NULL}};
episoade_pack Jump={Jump_ep,"Reach the top of the zones.",Jump_keys,NULL,"jump",kongregate,"Jump"};//,128549
//
#define On_the_Rail_ep "On_the_Rail"
ep_keys On_the_Rail_keys[]={{&left_arrow,"Rotation","Rotate Left" },{&right_arrow,"Rotation","Rotate Right"},{&action_key,"Shot","Shot" },{&aux_key,"Move faster (pressing)","Move faster (pressing)" },{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack On_the_Rail={On_the_Rail_ep,"Shot all the targets.",On_the_Rail_keys,NULL,"train",kongregate,"Train"};//,128550
//
#define Space_Trip_ep "Space_Trip"
ep_keys Space_Trip_keys[]={{&up_arrow,"Movement","Up Direction"},{&down_arrow,"Movement","Down Direction"},{&action_key,"Shot","Shot" },{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack Space_Trip={Space_Trip_ep,"Pass through the obstacles.",Space_Trip_keys,NULL,"voyage",kongregate,"Voyage"};//,128551
//
#define Space_Zones_ep "Space_Zones"
ep_keys Space_Zones_keys[]={{&left_arrow,"Rotation","Rotate Left"},{&right_arrow,"Rotation","Rotate Right"},{&up_arrow,"Movement","Forward"},{&down_arrow,"Movement","Backward"},{&action_key,"Shot","Shot" },{&aux_key,"Rotate slower (pressing)","Rotate slower (pressing)" },{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack Space_Zones={Space_Zones_ep,"Reach the end platforms, dispose of the enemies, shot all the targets or avoid the enemies.",Space_Zones_keys,NULL,"space-adventure",kongregate,"Space_Adventure"};//,128552
//
#define Rooms_ep "Rooms"
#define Rooms_desc "Shot all the ships."
#define Rooms_touch "Change opening door"
ep_keys Rooms_keys[]={{&left_arrow,"Rotation","Rotate Left"},{&right_arrow,"Rotation","Rotate Right"},{&up_arrow,"Movement","Forward"},{&down_arrow,"Movement","Backward"},{&strafe_left,"Movement","Strafe Left"},{&strafe_right,"Movement","Strafe Right"},{&action_key,"Shot","Shot" },{&aux_key,"Switch auto-move","Change auto-move sense"},{ &auto_shot,"Continuous shot","Auto-Shot" },{ &auto_move,"Continuous move","Auto-Move" },{NULL}};
episoade_pack Rooms={Rooms_ep,Rooms_desc,Rooms_keys,Rooms_touch,NULL,kongregate,"rooms"};//,128553
//
#define Road_ep "Road"
ep_keys Road_keys[]={{&left_arrow,"Rotation","Rotate Left"},{&right_arrow,"Rotation","Rotate Right"},{&action_key,"Shot","Shot" },{&strafe_left,"Movement","Strafe Left"},{&strafe_right,"Movement","Strafe Right"},{ &auto_shot,"Continuous shot","Auto-Shot" },{NULL}};
episoade_pack Road={Road_ep,"Follow the road until the last zone.",Road_keys,NULL,"road",kongregate,"Road"};//,128554
//
episoade_pack Racecourse={"Racecourse","A chance-dependent game with running horses. Choose a horse before the race starts.",NULL,"Choose Horse",NULL,kongregate,"racecourse"};//,128511
episoade_pack Card_Flip={"Card_Flip","Flip the cards and find the same ones.",NULL,"Choose Card",NULL,kongregate,"card-flip"};//,128512
episoade_pack Treasure_Finder={"Treasure_Finder","Find all the treasures under the sand.",NULL,"Dig Area",NULL,kongregate,"treasure-finder"};//,128513
episoade_pack Fruit_Cocktail={"Fruit_Cocktail","Fruit Cocktail is a puzzle game where it is required to put fruits on the table to make the cocktail.",NULL,"Place/Remove Fruit",NULL,kongregate,"fruit-cocktail"};//,129353
episoade_pack Naval_War= { "Naval_War","Naval War is a strategy game. The scope is to destroy the computer ships as fast as possible.",NULL,"Place/Target Ships",NULL,kongregate,"naval-war"};//,129481
episoade_pack Turret_Defence = { "Turret_Defence","Turret Defence is a strategy game. The scope is to destroy an incoming enemy.",NULL,"Take/Place Turrets",NULL,kongregate,"turret-defence"};//,129748
episoade_pack Ball_Hit= { "Ball_Hit","Hit the ball with the baseball bat at the best angle.",NULL,"Hit",NULL,kongregate,"ball-hit"};//,129933
episoade_pack Lotto= { "Lotto","Lotto is a chance game with 5 numbers selected from 30.",NULL,"Select Ball",NULL,kongregate,"lotto"};//,130064
episoade_pack Roulette= { "Roulette","Roulette is a casino game. The luck plays an important role to obtain a good score.",NULL,"Start Power/Spin",NULL,kongregate,"roulette"};//,130137
//
ep_keys Mahjong_keys[]={{&action_key,"Undo","Special Undo" },{NULL}};
episoade_pack Mahjong = { "Mahjong","Mahjong is a puzzle game. Match all the tiles.",Mahjong_keys,"Select Tile","mahjong",kongregate,"Mahjong"};//,131988
//
//ep_keys Test_keys[]={{&left_arrow,"Movement","Left Direction"},{&right_arrow,"Movement","Right Direction"},{&up_arrow,"Movement","Up Direction"},{&down_arrow,"Movement","Down Direction"},{&strafe_left,"Rotate Left","Strafe Left"},{&strafe_right,"Rotate Right","Strafe Right"},{&action_key,"Create a Sprite","Primary Action"},{&aux_key,"Reset Rotation","Secondary Action"},{&auto_shot,"Colorize","Auto-Shot"},{&auto_move,"Rotate Continuously","Auto-Move"},{NULL}};
//episoade_pack Test = { "Test","A simple test.",Test_keys,"Resolve Sprite",NULL,flashixy};
#define Chambers_ep "Chambers"
episoade_pack Chambers={Chambers_ep,Rooms_desc,Rooms_keys,Rooms_touch,NULL,flashixy};// on 25-may-2026 first score in firestore
//
episoade_pack* episoade[]={&Universe,&Ores_for_Ship,&Driving_the_Ship,&Planet_Landing,&Rocks_Fall,&Balls_Play,&Asteroids,&Cubes,&Moving_Forward,&Jump,&On_the_Rail,&Space_Trip,&Space_Zones,&Rooms,&Road,&Racecourse,&Card_Flip,&Treasure_Finder,&Fruit_Cocktail,&Naval_War,&Turret_Defence,&Ball_Hit,&Lotto,&Roulette,&Mahjong,&Chambers};

int numarulEpisoadelor_total=sizeof(episoade)/sizeof(episoade_pack*);

#define lista_ferestre_pagina 11
#define episoade_pagina lista_ferestre_pagina-1

bool is_universe(episoade_pack*ep){
	return ep->id==NULL&&ep->base==kongregate;
}
bool is_flashix(episoade_pack*ep){
	return ep->base==flashixy;
}
int get_iterator(bool is_flashixy){
	if(!is_flashixy){
		int i=0;
		for(int numarulEpisodului=0; numarulEpisodului<numarulEpisoadelor_total; numarulEpisodului++){
			if(is_universe(episoade[numarulEpisodului]))i++;
		}
		return i;
	}
	return numarulEpisoadelor_total;
}

int get_ep_nr(ep_keys* ep){
    int i=0;
    for(;;){
        int* relevant_pointer=(int*)&ep[i];
        //de 0 ca am pus un {NULL}
        if(relevant_pointer[0]==0)return i;
        i++;
    }
}
char prepare_def[100];
char* get_def_code(char* key_def){
    if(strlen(key_def)!=1)sprintf(prepare_def,"Key.%s",key_def);
    else sprintf(prepare_def,"ord('%s')",key_def);
    return prepare_def;
}
int find_ep(char*ep) {
	int ep_nr = 0; int sz = strlen(ep);
	while (strcmp(episoade[ep_nr]->name, ep) != 0){
		ep_nr++;
	}
	return ep_nr;
}

void init_episod(char* ep,bool e_simplu){
	//if (ep == NULL)return;
	action("var com=_root['shared_level']");
	if(!e_simplu){
		ep_keys*ep_chei=episoade[find_ep(ep)]->episod_chei;
		int key_nr=get_ep_nr(ep_chei);
		for(int i=0;i<key_nr;i++){
			char* key_id=ep_chei[i].key->name;
			actionf(buf,"if(!com.%s)com.%s=%s",key_id,key_id,get_def_code(ep_chei[i].key->defaultKey));
			//getCode: The key code of the last key pressed. This method returns 0 if no key was pressed or released, or if the key code is not accessible for security reasons.
			//asha ca ==undefined nu sta in picioare
		}
	}
}
void init_episode(){
	init_episod(NULL,true);
}

#define static_text_off_subtract 10
#define list_lineSz 2

#define sorts_stable_length 4
char*stable_sort[sorts_stable_length]={"Chronology","Newness","Plays","Rating"};//Compiler Location
char*stable_short_sort[sorts_stable_length]={"Chronicle","New","Play","Rate"};//  Builder  Site

#define sorts1_length 2
char* sorts1[sorts1_length]={stable_sort[0],stable_sort[1]};

#define sorts2_length sorts_stable_length
char* sorts_f[sorts2_length]={stable_sort[3],stable_sort[2],stable_short_sort[0],stable_sort[1]};

//la automatics
void din_buf_in_buf(char* s, ...) {
	va_list arglist;
	va_start(arglist, s);
	vsprintf(buf2, s, arglist);
	va_end(arglist);
	strcat(buf, buf2);
	//sprintf(buf2, "%s", buf);
	//sprintf(buf, "%s%s", buf2, buf3);
}

void automatic(int n,char* extra_move=NULL){
	bool shot = (n & 1) != 0; bool move = (n & 2) != 0;
	//asta daca era cu toate, era continua, asa e doar odata
	if (shot)action("var auto_shot=false");
	if (move)action("var auto_move=false");
	sprintf(buf, "var listener_automatic=new Object();listener_automatic.onKeyDown=function(){");
	if (shot)din_buf_in_buf("if(Key.getCode()==com.auto_shot)auto_shot=auto_shot==false?true:false;");
	if (move) {
		if (shot)din_buf_in_buf("else ");
		din_buf_in_buf("if(Key.getCode()==com.auto_move){auto_move=auto_move==false?true:false;");
		if (extra_move != NULL)din_buf_in_buf(extra_move);
		din_buf_in_buf("}");
	}
	din_buf_in_buf("}"); action(buf);
	action(R"(
	Key.addListener(listener_automatic);
	//nu merge cu onUnload cand se da Home Back la Universe, comportament ciudat
	_root.automatics_func=function(){
//e testat, asta se executa bine
//_root.createTextField('qwe',_root.getNextHighestDepth(),0,0,100,100);
//_root.qwe.text=Key._listeners.length+' ';
		Key.removeListener(listener_automatic);
//_root.qwe.text+=Key._listeners.length+' ';
	}
	//onUnload=function(){
		//Key.removeListener(listener);
	//}
	)");
}

#define shot_interval fps/3
#define swf_submit_color 0xff0011ff

#define adjusted(a) (a*3)/2

int static_text_submit(int bound_width,EditText ed,char*initialtext){
	ed.initialtext=initialtext;
	ed.rgba=swf_submit_color^0xffFFff00;
	int hg=adjusted(ed.font_height);
	int text_id=swf_text(bound_width,hg,"",(HasFont|HasText|HasTextColor|NoSelect|ReadOnly),&ed);
	return text_id;
}
void submitform(int deffont,char*nume,char*returner,char*la_load_extra=NULL){//,bool is_flashixy=false
	int submit_width=540;
	int submit_height=380;

	int submitsprite = swf_sprite_new();
	int submitshape=swf_shape_basic(submit_width,submit_height,swf_submit_color,0x00FF01ff);
	int depths=1;
	swf_sprite_placeobject(submitsprite,submitshape,depths);depths++;

	int submitform_y=20+20;
	int left_name=100;
	int text_font_height=20;
	int a=sizeof(EditText);
	EditText ed;memset(&ed,0,a);ed.fontid=deffont;
	EditText ed2;memset(&ed,0,a);ed2.fontid=deffont;ed2.font_height=text_font_height;

	int text_handler=static_text_submit(left_name,ed2,"Title");

	int margin=40;

	swf_sprite_placeobject_coords(submitsprite,text_handler,depths,margin,submitform_y);depths++;

	int both_margins=2*margin;
	int input_width=submit_width-both_margins-left_name;
	int entry_height=50;
	ed.font_height=35;//40 e bine local dar online la "pjg" jos mai trebuie
	int input_left=margin+left_name;

	int name;
	if(nume!=NULL){
		ed.initialtext=nume;
		name=swf_text(input_width,entry_height,"",(HasFont|HasText|Border|ReadOnly),&ed);
	}else{
		name=swf_text(input_width,entry_height,"name_text",(HasFont|Border|ReadOnly),&ed);
	}
	swf_sprite_placeobject_coords(submitsprite,name,depths,input_left,submitform_y);depths++;
	submitform_y+=entry_height;

	submitform_y+=20;
	text_handler=static_text_submit(left_name,ed2,"Score");
	swf_sprite_placeobject_coords(submitsprite,text_handler,depths,margin,submitform_y);depths++;
	int score=swf_text(input_width,entry_height,"score_text",(HasFont|Border|ReadOnly),&ed);
	swf_sprite_placeobject_coords(submitsprite,score,depths,input_left,submitform_y);depths++;
	submitform_y+=entry_height;

	submitform_y+=20;
	text_handler=static_text_submit(left_name,ed2,"Outcome");
	swf_sprite_placeobject_coords(submitsprite,text_handler,depths,margin,submitform_y);depths++;

	int comment_sz=2*entry_height;
	ed.font_height=30;
	ed.initialtext="Well Done & Congratulations!";//comment_text
	int comment=swf_text(input_width,comment_sz,"",(HasFont|HasText|Border|Multiline|WordWrap|ReadOnly),&ed);
	swf_sprite_placeobject_coords(submitsprite,comment,depths,input_left,submitform_y);depths++;
	submitform_y+=comment_sz;

	int stampsprite = swf_sprite_new();
	int stamp=swf_imagex("../tmp/root/stamp.dbl");
	swf_sprite_placeobject(stampsprite,stamp,1);
	if(nume!=NULL){//pe la simplu
		action_sprite(stampsprite,R"(
			onPress=function(){
				_root.bar.bar_back.onPress();
			}
		)");
	}else{//univers
		action_sprite(stampsprite,R"(
			onPress=function(){
				_root.bar.home.onPress();
			}
		)");
	}
	stampsprite=swf_sprite_done(stampsprite);
	int swf_shape_basic_border=submit_height/20;//this is an inner border , wd>hg?hg/20
	int free_space=(input_left-100-swf_shape_basic_border+1)/2;//+1 to round to the not dominant part
	swf_sprite_placeobject_coords(submitsprite,stampsprite,depths,swf_shape_basic_border+free_space,submitform_y-50);depths++;

	submitform_y+=20;

	action_sprite(submitsprite,R"(
		var kongregate=_global.kongregate;kongregate.services.connect();
		var end_time=new Date();var s=int((end_time.valueOf()-_root.start_time.valueOf())/1000)
	)");

	char ac[8192];//char*g1;char*g2;
	char*stats;
	if(nume!=NULL){//pe la simplu
		action_sprite(submitsprite,R"(
//			kongregate.stats.submit('Highscore',s);
//			kongregate.stats.submit('History',s);
			flash.external.ExternalInterface.call('submitScore','Highscore',s);
//			flash.external.ExternalInterface.call('submitScore','History',s);
		)");
		//g1="var game_name='";
		//g2="';";stats="'Highscore'";
	}else{
	//pe la universe
		action_sprite(submitsprite,R"(
			name_text=_root.strip_underscores(game_name);
//			kongregate.stats.submit(name_text,s);
//			kongregate.stats.submit(name_text+' History',s)
			flash.external.ExternalInterface.call('submitScore',name_text,s);
		)");
//		if(!is_flashixy)action_sprite(submitsprite,"flash.external.ExternalInterface.call('submitScore',name_text+' History',s)");
		//g1="";
		//nume=g1;//g2=g1;stats="_root.strip_underscores(game_name)";
	}
	//sprintf(ac,R"(
	//	%s%s%s
	//	getURL('https://colin-i.github.io','_self','GET')
	//)",g1,nume,g2);

	int button_width=150;

	//ButtonData bd = { 0x11FF11ff,10,0x11FF1188, 0xaa1122ff,10,0xaa112288, 0x1122FFff,10,0x1122FF88,24,24,"Submit",deffont,text_font_height,-(text_font_height/4),0xff,ac};
	ButtonData bd = { 0x11FF11ff,10,0x11FF1188, 0xaa1122ff,10,0xaa112288, 0x1122FFff,10,0x1122FF88,24,24,"New Game",deffont,text_font_height,-(text_font_height/4),0xff,returner};
	//int submit=swf_button(button_width,entry_height,&bd);
	//swf_sprite_placeobject_coords(submitsprite,submit,depths,margin,submitform_y);depths++;
	int newgame=swf_button(button_width,entry_height,&bd);//swf_button_last("New Game",returner);
	swf_sprite_placeobject_coords(submitsprite,newgame,depths,submit_width-button_width-margin,submitform_y);

	actionf_sprite(submitsprite,ac,R"(
		score_text=s;
		//comment_text="Well Done & Congratulations!";
		_x=%u;_y=%u
	)",(width-submit_width)/2,(height-submit_height)/2);

	if(la_load_extra)action_sprite(submitsprite,la_load_extra);

	swf_sprite_showframe(submitsprite);
	int submitform=swf_sprite_done(submitsprite);
	swf_exports_add(submitform,"scenario_done");
}

int float_extract(double d,int*f){
//astea pe double(poate si float) sunt acum trimise la printf pe xmm
//inainte: puneam %lf sau %f ambele erau double la printf
//precizia e pe 30 biti, floatul e 1+8+23
	double intpart;
	double frpart=modf(d,&intpart);
	f[0]=(int)(1000*1000*1000*frpart);
	return (int)intpart;
}
#define scale_factor 2
#define scale_coef (scale_factor-1)
#define scale_at_x bar_w*scale_coef
#define scale_at_game bar_dimensiune*scale_coef
#define scale_at_bar 100*scale_factor

void touch_or_mouse(bool is_universe){
	char*s=R"(
		if(flash.external.ExternalInterface.call('onSwfReady')){
			function game_xscale_set(){
				%s
			}
			bar_x=bar_x-%u;
			bar._xscale=%u;
			game_xscale_set();
		}
	)";
	double d=100*(width-scale_at_game);
	d/=width;
	int d_f;int d_i=float_extract(d,&d_f);
	char bf[100];sprintf(bf,"game._xscale=%u.%u;",d_i,d_f);
	if(is_universe)actionf(buf,s,bf,scale_at_x,scale_at_bar);
	else din_buf_in_buf(s,bf,scale_at_x,scale_at_bar);
}

#ifdef __cplusplus
}
#endif

#define flags_macro int flags; if(argc==1)flags=prex_flags|flag_noDuplicate|flag_verbose; else { \
		if(argc==2)flags=xu_flags|flag_noDuplicate|flag_compress; \
		else sscanf(argv[1],"%x",&flags); \
	} printf("%x\n",flags);
#define flags_macro_env int flags; if(getenv("poolwrite"))flags=prex_flags|flag_noDuplicate|flag_verbose; else { \
		if(getenv("poolread"))flags=xu_flags|flag_noDuplicate|flag_compress; \
		else sscanf(getenv("flags"),"%x",&flags); \
	} printf("%x\n",flags);

/*void securitate(){
    action(R"(
        //asta trebuie cand e de la //www la //
        //fapt divers: kongregate.com devine www.kongregate.com
        System.security.allowDomain(_parent._url);
        //System.security.allowDomain('www.universe-game.com');
        //if(_root.kongregate!=undefined)System.security.allowDomain('chat.kongregate.com');
    )");
}*/
