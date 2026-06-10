
//#include<windows.h>
#include<stdio.h>
#include <string.h>
#include <stdarg.h>

#include "universe.hpp"

#ifdef __cplusplus
extern "C" {
#endif

//SharedObject: keys levelIsDone listSort music showIntro showImg

int main(int argc,char**argv){
//rooted_swf_path("universe") "test/universe.swf"
	flags_macro
	char*nm=getenv("site");
	bool is_flashixy=false;
	if(!nm)nm="";
	else is_flashixy=true;
	char nmbuf[100];sprintf(nmbuf,"%suniverse/universe.swf",nm);
	swf_new_ex(nmbuf,width_nr,height,0x000000,fps,flags);

    int defFont=swf_font_basic("_sans");

    //bar

    char* bar_path="../tmp/root/bar.dbl";
    dbl=swf_img(bar_path);
    swf_exports_add(dbl,"bar_texture");
    //root
    int button_sz[2];
    dbl=swf_img_ex("../tmp/root/root.dbl",button_sz);
    swf_exports_add(dbl,"bar_home");
    //sound
    dbl=swf_img("../tmp/root/sound.dbl");
    swf_exports_add(dbl,"bar_sound");
    dbl=swf_img("../tmp/root/sound_off.dbl");
    swf_exports_add(dbl,"bar_sound_off");
    //keyboard
    dbl=swf_img("../tmp/root/keyboard.dbl");
    swf_exports_add(dbl,"bar_keyboard");
	//intro
	dbl=swf_img("../tmp/root/intro.dbl");
	swf_exports_add(dbl,"bar_intro");
	dbl=swf_img("../tmp/root/intro_off.dbl");
	swf_exports_add(dbl,"bar_intro_off");
	//img
	dbl=swf_img("../tmp/root/pimg.dbl");
	swf_exports_add(dbl,"bar_pimg");
	dbl=swf_img("../tmp/root/pimg-off.dbl");
	swf_exports_add(dbl,"bar_pimg_off");
    //
    int button_lat=button_sz[1];
#define pointless_add 1
    int button_szAndInter=button_lat*(2+pointless_add);
    int bar_coord=button_lat;
    presprite=swf_sprite_new();
	action_sprite(presprite,R"(
        function draw_bmp(mc,img,wd,hg){
            bmp=flash.display.BitmapData.loadBitmap(img);
            fill_mc(mc,bmp,wd,hg);
        }
        function fill_mc(mc,bmp,wd,hg){
            mc.beginBitmapFill(bmp,mx,true);
            mc.lineTo(wd,0);mc.lineTo(wd,hg);
            mc.lineTo(0,hg);mc.lineTo(0,0);
            mc.endFill();
        }
        function draw_desc_bmp(mc,dbl){
            draw_bmp(mc,dbl,button_lat,button_lat);
        }
        //
        function add_button(button_name,description,uniquename){
            var nm='bar_'+button_name;
            var mc=createEmptyMovieClip(uniquename,getNextHighestDepth());
            draw_desc_bmp(mc,nm);
            var mv=_parent.attachMovie('barDescription',nm,_parent.getNextHighestDepth());
            mv['desc']=description;mv._visible=false;mv['rel']=mc;
            //
            mc['description']=mv;
            mc.onRollOver=function(){
                this.description._visible=true;
            }
            mc.onRollOut=function(){
                this.description._visible=false;
            }
			mc._yscale=_xscale;//for pointless
        }
	)");
	actionf_sprite(presprite,buf,R"(
	function bar_button(shared,name,desc,distance,container,dflt,f_on,f_off){
		var dsc=desc+' on/off';

		var so_m=SharedObject.getLocal(shared);
		var b=so_m.data['on'];
		if(b==undefined)b=dflt;
		if(b){
			var soundstate=name;
			add_button(soundstate,dsc,name);
			var sound_on_bmp=bmp;
			var sound_off_bmp=flash.display.BitmapData.loadBitmap('bar_'+name+'_off');
		}else{
			var soundstate=name+'_off';
			add_button(soundstate,dsc,name);
			var sound_on_bmp=flash.display.BitmapData.loadBitmap('bar_'+name);
			var sound_off_bmp=bmp;
		}
		var mc=this[name];
		mc._%c=distance;
		mc.onPress=function(){
			if(container['is_'+name]){
				f_off();
				fill_mc(this,sound_off_bmp,button_lat,button_lat);
				so_m.data['on']=false;so_m.flush();
				container['is_'+name]=false;
			}else{
				f_on();
				fill_mc(this,sound_on_bmp,button_lat,button_lat);
				so_m.data['on']=true;so_m.flush();
				container['is_'+name]=true;
			}
		}
		return b;
	}
	function set_sound_on(){
		_root.game.music.start();
	}
	function set_sound_off(){
		_root.game.music.stop();
	}
	)",(bar_x<bar_y?'x':'y'));
	sprintf(buf,"var bar_w=%u;var bar_h=%u;",bar_w,bar_h);
	din_buf_in_buf("var button_lat=%u;",button_lat);
	din_buf_in_buf(R"(
		var bmp;
		var mx=new flash.geom.Matrix();
		draw_bmp(this,'bar_texture',bar_w,bar_h);
	)");
	//home
	din_buf_in_buf(R"(
		add_button('home','Back to home','home');
		home._%c=%u;
		home.onPress=_root.homeBack;
	)",(bar_x<bar_y?'x':'y'),bar_coord);
	//music
	bar_coord+=button_szAndInter;
	din_buf_in_buf(R"(
		//_root.game.music inca nu este aici
		var is_sound=bar_button('music','sound','Music',%u,this,true,set_sound_on,set_sound_off);
	)",bar_coord);
	//chei
	bar_coord+=button_szAndInter;
	din_buf_in_buf(R"(
		add_button('keyboard','Set keys','keyboard');
		keyboard._%c=%u;
		//
		keyboard.onPress=function(){
			_root.launch_keys_panel();
		}
	)",(bar_x<bar_y?'x':'y'),bar_coord);
	//calculations
	int bar_normal_startPos=bar_coord+button_lat*(1+pointless_add);
	bar_coord=bar_normal_startPos+button_lat;
	//intro
	actionf_sprite(presprite,buf2,R"(
	function intro_button_show(){
		return bar_button('showIntro','intro','Intro',%u,this,true);
	}
	function img_button_show(){
		return bar_button('showImg','pimg','Preview',%u,this,false);
	}
	function extra_buttons_show(){
		intro_button_show();
		img_button_show();
	}
	function extra_buttons_hide(){
		intro.removeMovieClip();
		pimg.removeMovieClip();
	}
	)",bar_coord,bar_coord+button_szAndInter);
	action_sprite(presprite,buf);
	action_sprite(presprite,R"(
		//se putea si is_intro= in functie, ca acolo e set la nivel de movieclip, dar sa nu mai fie degeaba la celalalt show
		var is_intro=intro_button_show();
		var is_pimg=img_button_show();
	)");

    action_sprite(presprite,"delete bmp;");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"bar");

    //barDescription
    presprite=swf_sprite_new();
    //
    int bar_text_h=2*button_sz[1];int bar_text_w=150;
    EditText bar_ed;bar_ed.fontid=defFont;bar_ed.font_height=button_sz[1];
    int txt=swf_text(bar_text_w,bar_text_h,"message",(HasFont|ReadOnly|NoSelect),&bar_ed);
    swf_sprite_placeobject(presprite,txt,0);
    //
    actionf_sprite(presprite,buf,"var text_h=%u;var off=%u",bar_text_h,bar_text_w);
    action_sprite(presprite,R"(
        var rel_bounds=rel.getBounds(_parent);_x=rel_bounds.xMin;_y=rel_bounds.yMin;
        _x-=off;
        beginFill(0xffFFff);
        lineTo(off,0);lineTo(off,text_h);
        lineTo(0,text_h);lineTo(0,0);
        endFill();
        message=desc;
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"barDescription");

    //

    //space env
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var wd=%u;var hg=%u",width,height);
    action_sprite(presprite,R"(
        var env_matrix=new flash.geom.Matrix();
        beginBitmapFill(flash.display.BitmapData.loadBitmap('space_env_bmp'),env_matrix,true);
        lineTo(wd,0);lineTo(wd,hg);
        lineTo(0,hg);lineTo(0,0);
        endFill();
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"main_space_env");

    //

    action(R"(
        function launch_keys_panel(){
            if(!keys_panel)attachMovie('keys_panel','keys_panel',getNextHighestDepth());//mc
        }
        function set_key_fill(mc){
            mc.lineStyle(1,0xFF0000);
            mc.beginFill(0xffFF40);
            mc.lineTo(set_key_w,0);mc.lineTo(set_key_w,set_key_h);
            mc.lineTo(0,set_key_h);mc.lineTo(0,0);
            mc.endFill();
        }
    )");

	#define list_inset 100
	int list_w=width-(2*list_inset);
	int list_h=height-(2*list_inset);
	int list_unit_h=list_h/lista_ferestre_pagina;
	int list_unit_w=list_w;
	int text_height=list_unit_h-static_text_off_subtract;

	char*load_extern;
	#define start_scenario_ante_sz 2000
	char start_scenario_ante[start_scenario_ante_sz];
	char*bar_star_at_exit;
	char*start_scenario_post;
	char*list_loaded;

	char*location_mark;
	int color1=0xFF0000;
	int color2=0x11aa11;//green//mai inchis ca nu se vede
	char f1[]="xPos-=button_w;if(_root.singleTraining_ids[pos])draw_check(0x";
	char f2[]=");";
	char f3[]="var fmt=new TextFormat();fmt.align='right';fmt.size=";
	char f4[]=";xPos-=100;createTextField('txt_inf',getNextHighestDepth(),xPos,0,100,list_unit_h);txt_inf.text=_root.sorter_play[pos];txt_inf.setTextFormat(fmt);";//100 aici vine cam de 7+ cifre
	char f5[]="fmt.align='left';createTextField('txt_infr',getNextHighestDepth(),xPos-100,0,100,list_unit_h);txt_infr.text=_root.sorter_rate[pos];txt_infr.setTextFormat(fmt);delete fmt;";//4 cifre
	char color1buf[sizeof(f1)-1+6+sizeof(f2)-1+sizeof(f3)-1+maxuint+sizeof(f4)-1+sizeof(f5)-1+1];
	//char*desc_height;
	//char*under1;char*under2;
	char counterBarTestChar;
	char*preview_prefix;
	int bar_startPos;
	int text_x;int rest_x;

	if(!is_flashixy){
		load_extern="";*start_scenario_ante='\0';bar_star_at_exit="";start_scenario_post="";list_loaded="";location_mark="";//desc_height="";
		//under1="_root.strip_underscores(";under2=")";
		counterBarTestChar='>';
		preview_prefix="'https://flashixy.com/'+";
		bar_startPos=bar_normal_startPos;
		text_x=list_unit_w/4;
		rest_x=list_unit_w-text_x;
	}else{
		presprite=swf_sprite_new();
		action_sprite(presprite,R"(
			//prima data era doar in function temp_solution(){
			//	asta ca apare la get request (si cu _root.getURL la fel).    pos, ep_name, extension   si   super si arguments
			//	asa apar doar super si arguments, NU LE BAG LA POST CA NU MAI MERGE KONGREGATE, SI AR FI SI REFRESH CU POPUP ALA DE RESUBMIT
			//in sprite separat, in plus ramane doar "?" si atat, dar ruffle zice SOMETHING WENT WRONG, pe la 3 frameuri merge/nu merge
			//	LA POST la fel ca mai devreme, si cu popul tot la fel
			//trebuie dupa ce list se incarca cu alea
			getURL('https://www.kongregate.com/games/bitplayer/'+_root.link_id,'_blank','GET');
			delete _root.link_id;
			removeMovieClip();
			//getURL('qqq/'+_root.temp_solution_get,'_blank','GET');
		)");
		swf_sprite_showframe(presprite);
		sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"get_url");

		load_extern=R"(
			link_id=singleTraining_ids[pos];
			if(link_id){
				mark_done(pos);
				return undefined;
			}
		)";

		dbl=swf_img("../tmp/root/star.dbl");
		swf_exports_add(dbl,"bar_star");
		dbl=swf_img("../tmp/root/s1.dbl");swf_exports_add(dbl,"bar_s1");
		dbl=swf_img("../tmp/root/s2.dbl");swf_exports_add(dbl,"bar_s2");
		dbl=swf_img("../tmp/root/s3.dbl");swf_exports_add(dbl,"bar_s3");
		dbl=swf_img("../tmp/root/s4.dbl");swf_exports_add(dbl,"bar_s4");
		dbl=swf_img("../tmp/root/s5.dbl");swf_exports_add(dbl,"bar_s5");
		presprite=swf_sprite_new();
		action_sprite(presprite,R"(
			var mx=new flash.geom.Matrix();
			if(star)var bmp=flash.display.BitmapData.loadBitmap('bar_star');
			else var bmp=flash.display.BitmapData.loadBitmap('bar_s'+rate);
			var wd=bmp.width;var hg=bmp.height;
			beginBitmapFill(bmp,mx,false);
			lineTo(wd,0);lineTo(wd,hg);
			lineTo(0,hg);endFill();
			//_x=-i*wd;
			_y=rate*(hg*_root.bar.touch_factor);
			delete bmp;
		)");
		swf_sprite_showframe(presprite);
		sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"rate");

		char*s=R"(
			if(nm){
				bar.add_button('star','Rate the game','star');
				var roll_store=bar.star.onRollOver;

				bar.star['dbRate']=0;
				flash.external.ExternalInterface.call("requestRate",singleTraining[pos]);

				bar.star.onPress=function(){
					if(!this.stars){
						this.onRollOut();//to hide the description
						this.onRollOver=undefined;
						var mc=this.createEmptyMovieClip('stars',this.getNextHighestDepth());
						for(var i=5;i>=1;){
							var mv=mc.attachMovie('rate','rate'+i,mc.getNextHighestDepth());
							mv.rate=i;
							//mv.i=6-i;
							if(i<=this.dbRate)mv.star=true;
							i--;
						}
					}else{
						for(var mc in this.stars){
							var mv=this.stars[mc];
							if(mv.hitTest(_xmouse,_ymouse,false)){
								flash.external.ExternalInterface.call('rateGame',singleTraining[pos],mv.rate);
								break;
							}
						}
						this.stars.removeMovieClip();
						this.onRollOver=roll_store;
					}
				}
				bar.star._y=)";
		if(snprintf(start_scenario_ante,start_scenario_ante_sz,"%s%u;",s,bar_coord)>=start_scenario_ante_sz)exit(1);
		action(R"(
			function onRateLoaded(rate){
				bar.star.dbRate=rate;
			}
			flash.external.ExternalInterface.addCallback("onRateLoaded",null,onRateLoaded);
		)");
		bar_star_at_exit="bar.star.removeMovieClip();";

		start_scenario_post=R"(
				flash.external.ExternalInterface.call('addPlay',singleTraining[pos]);
			}else homeAgain();
		)";
		list_loaded=R"(
			if(link_id)attachMovie('get_url','get_url',getNextHighestDepth());
		)";
		sprintf(color1buf,"%s%x%s%s%u%s%s",f1,color1,f2,f3,text_height,f4,f5);location_mark=color1buf;

		//desc_height="if(_root.singleTraining_mouse[pos])desc_hg-=oneLine_h;";
		//under1="";under2="";
		counterBarTestChar='=';
		preview_prefix="";
		bar_startPos=bar_coord+button_lat*(1+pointless_add)*(1+5);
		text_x=list_unit_w/10;
		rest_x=list_unit_w-text_x;
	}

	actionf(buf,R"(
        function counterBar_init(max_pos){
            var mc=bar.attachMovie('counterBar','counterBar',bar.getNextHighestDepth());
            mc._%s=%u;mc['barNr']=0;mc['barMax']=max_pos;
        }
	)",(bar_x<bar_y?"x":"y"),bar_startPos);
	actionf(buf,R"(
        function counterBar_step(){
			counterBar_jump(1);
        }
	function want_end(){
		//se scoate dupa
		//game.scenario._visible=false;

		//raman alea pe acolo, cine le mai tine socoteala
		game.scenario.removeMovieClip();

		var mc=env_set();
		var endfps=%u;
		mc.onEnterFrame=function(){
			endfps--;
			if(endfps<1){
			//e scos ca e in game
				end_scenario();
			}
		}
	}
	function counterBar_jump(a){
		bar.counterBar.barNr+=a;

		if(bar.counterBar.barNr%c=bar.counterBar.barMax)want_end();
		//in cel mai rau caz la ruleta ar trasa in afara ecranului la end la ruleta
		//13rooms 15racecourse 17t-f 21ball-hit 23roulette
		//==      ==           ==    ==         >=

		bar.counterBar.draw_progressBar();
	}
	)",2*fps,counterBarTestChar);

	action(R"(
        function draw_list_entry(mc,wd,hg){
            draw_list_entry_ex(mc,wd,hg,0x0000ff);
        }
		function draw_list_entry_ex(mc,wd,hg,col){//is ok, is not z index onrollover problem, test it with zzz=list_lineSz+10 and col+random(0xffFF)
			mc.lineStyle(list_lineSz,col);
			mc.beginBitmapFill(list_bmp,list_matrix,true);
			var marginVal=list_lineSz/2;
			mc.moveTo(marginVal,marginVal);
			mc.lineTo(wd-marginVal,marginVal);mc.lineTo(wd-marginVal,hg-marginVal);
			mc.lineTo(marginVal,hg-marginVal);mc.lineTo(marginVal,marginVal);
			mc.endFill();
		}
	)");

	action(R"(
		function strip_underscores(txt){
			var splitAr=txt.split('_');
			return splitAr.join(' ');
		}
	)");//is only because of _ images and swfs
	actionf(buf,R"(
		//url
		function load_extern(pos,extension){
			var ep_name=singleTraining[pos];
			%s
			return full_path(ep_name+'.'+extension);

			//trebuie asta 'within' alea mici dar la ruffle 'is not implemented'
			//System.security.allowDomain('game195780.konggames.com');
			//o solutie temporara ar fi _blank
		}
	)",load_extern);
	actionf(buf,R"(
		function start_scenario(pos){
	            //tm e ca pe kongregate nu vin fisierele noi cand se incearca updatarea
	            //e timpul fisierului si il ia pe cel nou
	            //dar se trimite in milisecunde dar parca acolo e in secunde
	            //var tm=new Date();
	            //game.scenario.loadMovie(ep_name+'.swf?'+tm.valueOf());
	            //game.scenario.loadMovie('http://universe-game.com/'+ep_name+'.swf');
	            //game.scenario.loadMovie(ep_name+'.swf');
	            //+'?9999999999999'
	            //game.scenario.loadMovie(ep_name+'.swf'+'?9999999999999');
	            //asta trebuie cand e cu www si de la kongregate
	            //89.35.62.10
	            //allow_domain('http://universe-game.com/test1.swf',game.scenario);
	            //game.scenario.loadMovie('http://universe-game.com/test1.swf');

			var nm=load_extern(pos,'swf');
			%s
				//!=undefined
				bar.extra_buttons_hide();
				game_clear();

				game['episode_number']=pos;
				//inca un level in jos ca altfel la load movie se elimina continutul actual
				game.createEmptyMovieClip('scenario',game.getNextHighestDepth());

				load_swf(nm,game.scenario);

				game.music=new Sound();
				game.music.onLoad=function(success){
					if(bar.is_sound)this.start();
				}
				game.music.onSoundComplete=function(){
					this.start();
				}
				//tm e ca pe kongregate nu vin fisierele noi cand se incearca updatarea
				//game.music.loadSound(ep_name+'.mp3?'+tm.valueOf(),false);
				//game.music.loadSound('http://'+gameDomain+'/'+ep_name+'.music',false);
				//game.music.loadSound('http://universe-game.com/'+ep_name+'.mp3',false);
				game.music.loadSound(load_extern(pos,'mp3'),false);

				start_time=new Date();
			%s
		}
	)",start_scenario_ante,start_scenario_post);
#define l_view "list_view"
	actionf(buf,R"(
		function list_loaded(){
			game.attachMovie('%s','%s',game.getNextHighestDepth());
			%s
		}
	)", l_view, l_view,list_loaded);
	actionf(buf,R"(
        //var gameDomain='ajob.atwebpages.com';
        //127.0.0.1
        function load_swf(swf_name,instance){
            instance.loadMovie(swf_name);
        }
		function env_set(){
			return game.attachMovie('main_space_env','space_env',game.getNextHighestDepth());
		}
		function home_set(){
			env_set();
			//
			game.createEmptyMovieClip('list_movie',game.getNextHighestDepth());

			//list_movie continut devine cel din list.swf
			//allow_domain('http://universe-game.com/list.swf',game.list_movie);
			//nu e vina mea ca se incarca de doua ori, aici e un t race, la list_loaded sunt doua. t race('aici?');
			load_swf(full_path('list.swf'),game.list_movie);
		}
		function game_clear(){
			var a;
			for(a in Key._listeners){
				//de la Enter de la Play(daca nu e key panel), ce mai ramane cand se iese fortzat din Cubes/Rooms+key panel, sau key panel
				//onUnload nu merge ca le puneam acolo
				Key.removeListener(Key._listeners[a]);
			}
			keys_panel.removeMovieClip();//ma incurca si oricum ii stric key listenerul, atentie ca removeListener sa se dea la cand exista ca altfel nu merge

			var d=game.getDepth();
			game.removeMovieClip();
			createEmptyMovieClip('game',d);
			game_xscale_set();//only if is defined
		}
		//game reset vine de la home press si end scenario
		function game_reset(){
			//doar cand e cazul
			if(automatics_func){
			//!=undefined
				automatics_func();
				automatics_func=undefined;
			}
			//
            game.music.stop();
            game_clear();
			//doar cand e cazul
            bar.counterBar.removeMovieClip();
			//
        }
        function homeBack(){
		if(!bar.intro)
		//mc. cel mai usor de testat, ca altfel ramane visibil rau, se apasa din prima Home 
			bar.extra_buttons_show();
		%s

		//asta e cel tare cu reset de automatics musica si counter
            game_reset();

            home_set();
        }
		//de la new sort, celalalt sort, de la linkuri externe
		function homeAgain(){
			game_clear();
			home_set();
		}
        function start_training(pos){
		if(bar.is_intro){
			game_clear();
			shared_level.space_env(game,'env',game.getNextHighestDepth());
			game.attachMovie('scenario_info','scenario_info',game.getNextHighestDepth());
			game.scenario_info['pos']=pos;
		}else start_scenario(pos);
        }
	)",bar_star_at_exit);
	action(R"(
		function full_path(a){
			// /dir/a.swf
			var c=_url.indexOf('//');
			c=_url.indexOf('/',c+2);
			var d=_url.lastIndexOf('/');
			var b=_url.substring(c,d+1);
			return b+a;

			//http://... now is full to also work multiple swf load in debugger, but still not working
			//var d=_url.lastIndexOf('/');
			//return _url.substring(0,d+1)+a;
		}
	)");
	action(R"(
	function mark_done(pos){
		//mai intai asta ca sa se observe daca este ultimul episod done ca sa se trimita game done
		var so=SharedObject.getLocal("levelIsDone");
		if(!so.data['lev'+pos]){//undefined sau false
			var i=0;var eps_done=1;
			for(;i<singleTraining.length;){
				if(so.data['lev'+i])eps_done++;
				i++;
			}
			//no Return value
			//kongregate.stats.submit('Universe',eps_done);
			flash.external.ExternalInterface.call('submitScore','Universe',eps_done);
		}
		so.data['lev'+pos]=true;
		so.flush();
	}
	function end_scenario(){
            var pos=game.episode_number;
			//game reset curata tot din "game"
            game_reset();

            shared_level.space_env(game,'env',game.getNextHighestDepth());

            game.attachMovie('scenario_done','scenario_done',game.getNextHighestDepth());
		game.scenario_done['game_name']=singleTraining[pos];

		//asta pentru restart
            game['episode_number']=pos;

		mark_done(pos);
	}
//	function call(n,mc){
//		sorter=_root['sorter'+n];
//		mc.first_page();
//	}
        //asta trebuie, ca _global e undefined in loadMovie
//        var kongregate=_global.kongregate;
        //if(kongregate){
//        kongregate.services.connect();
            //System.security.allowDomain('http://universe-game.com');
        //}
    )");

    //set keys
    //
    action("var text_names=new Array();var key_names=new Array();var key_default_code=new Array()");
    int key_nr=0;while(key_nr<keyboards_total){
        actionf(buf,"text_names[%u]='%s';key_names[%u]='%s'",key_nr,keyboards[key_nr]->disp_name,key_nr,keyboards[key_nr]->name);
        actionf(buf,"key_default_code[%u]=%s",key_nr,get_def_code(keyboards[key_nr]->defaultKey));
        key_nr++;
    }
    #define key_text_w 200
    #define key_w 2*key_text_w
    #define key_unit_h 20
    #define key_unit_w key_w
    int key_h=key_unit_h*key_nr;
    int key_x=(width-key_w)/2;
    int key_y=(height-key_h)/2;
    //
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"_x=%u;_y=%u",key_x,key_y);
    actionf_sprite(presprite,buf,"var key_unit_h=%u;var key_nr=%u",key_unit_h,key_nr);
    action_sprite(presprite,R"(
        var com=_root['shared_level'];
        var i;var key_nr=_root.text_names.length;
        i=0;while(i<key_nr){
		if(!com[_root.key_names[i]]){//undefined sau getCode, care e Number, '0' e 48, 0 e la inceput cand nu s-a apasat pe nimic
		// getCode This method returns 0 if no key was pressed or released, or if the key code is not accessible for security reasons.
			com[_root.key_names[i]]=_root.key_default_code[i];
		}
            i++;
        }
        //
        var new_key_mc;
        var yPos=0;
        i=0;while(i<key_nr){
            var d=getNextHighestDepth();
            var mc=attachMovie('keys_panel_entry','key'+d,d);mc._y=yPos;
            mc['text_name']=_root.text_names[i];
            mc['key_name']=_root.key_names[i];
            mc.onPress=function(){
                this.right_text='';
                new_key_mc=this;
            }
            yPos+=key_unit_h;i++;
        }
        var mc=attachMovie('keys_panel_ok','ok',getNextHighestDepth());mc._y=yPos;
	Key.removeListener(_root.game.list_view.container.list_pages.continue_play);
	Key.addListener(this);
	mc.onPress=function(){
		Key.removeListener(this._parent);
		_root.game.list_view.container.list_pages.continue_play.readd();
		removeMovieClip();
	}
	onKeyDown=function(){
		if(new_key_mc){
		//!=undefined
			var code=Key.getCode();
			var key_id=new_key_mc.key_name;
			com[key_id]=code;
			new_key_mc.right_text=new_key_mc.keycode_to_string(code);
			new_key_mc=undefined;
			//
			com.so_keys.data[key_id]=code;
			com.so_keys.flush();
		}
	}
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"keys_panel");
    //
    actionf(buf,"var set_key_w=%u;var set_key_h=%u",key_unit_w,key_unit_h);
    int keys_text_h=key_unit_h-5;
    int text_entry;
    EditText keys_text;keys_text.fontid=defFont;keys_text.font_height=keys_text_h;
    //ok
    #define ok_x key_unit_w/2
    EditText ok_text;
    ok_text.fontid=defFont;ok_text.font_height=keys_text_h;
	ok_text.rgba=0x0000ffFF;
    presprite=swf_sprite_new();
    text_entry=swf_text(key_unit_w-ok_x,key_unit_h,"ok_text",(HasFont|HasTextColor|ReadOnly|NoSelect),&ok_text);
    swf_sprite_placeobject_coords(presprite,text_entry,0,ok_x,0);
    action_sprite(presprite,"_root.set_key_fill(this);ok_text='OK'");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"keys_panel_ok");
    //
    presprite=swf_sprite_new();
        //
    text_entry=swf_text(key_text_w,key_unit_h,"left_text",(HasFont|ReadOnly|NoSelect),&keys_text);
    swf_sprite_placeobject(presprite,text_entry,0);
        //
    text_entry=swf_text(key_text_w,key_unit_h,"right_text",(HasFont|ReadOnly|NoSelect),&keys_text);
    swf_sprite_placeobject_coords(presprite,text_entry,1,key_text_w,0);
        //
    action_sprite(presprite,R"(
        function keycode_to_string(keycode){
            if(keycode==Key.BACKSPACE)return 'BACKSPACE';
            if(keycode==Key.CAPSLOCK)return 'CAPSLOCK';
            if(keycode==Key.CONTROL)return 'CONTROL';
            if(keycode==Key.DELETEKEY)return 'DELETEKEY';
            if(keycode==Key.DOWN)return 'DOWN';
            if(keycode==Key.END)return 'END';
            if(keycode==Key.ENTER)return 'ENTER';
            if(keycode==Key.ESCAPE)return 'ESCAPE';
            if(keycode==Key.HOME)return 'HOME';
            if(keycode==Key.INSERT)return 'INSERT';
            if(keycode==Key.LEFT)return 'LEFT';
            if(keycode==Key.PGDN)return 'PGDN';
            if(keycode==Key.PGUP)return 'PGUP';
            if(keycode==Key.RIGHT)return 'RIGHT';
            if(keycode==Key.SHIFT)return 'SHIFT';
            if(keycode==Key.SPACE)return 'SPACE';
            if(keycode==Key.TAB)return 'TAB';
            if(keycode==Key.UP)return 'UP';
            return String.fromCharCode(keycode);
        }
        var com=_root['shared_level'];
        _root.set_key_fill(this);
        //
        left_text=text_name;
        right_text=keycode_to_string(com[key_name]);
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"keys_panel_entry");

    //scenario_info
    #define scenario_info_w 400
    #define scenario_info_x (width-scenario_info_w)/2
    #define scenario_info_center scenario_info_w/2
    //
    #define scenario_info_unit_w scenario_info_w
    #define scenario_info_unit_h 40
    //
    //#define scenario_info_unit_w scenario_info_w
    presprite=swf_sprite_new();
	action_sprite(presprite,R"(
        //mc
        function add_mc_base(textH,txt,wd,hg){
            var d=getNextHighestDepth();
            var mc=attachMovie('info_text','info_text'+d,d);
            mc['textHeight']=textH;
            mc['txt']=txt;mc['wd']=wd;mc['hg']=hg;
            mc._y=total_h;
            return mc;
        }
        //mc
        function add_mc_top(txt,hg){
            add_mc_base(textHeight,txt,wd,hg);
            total_h+=hg;
        }
        //mc
        function add_mc_left(txt){
            add_mc_base(disp_textH,txt,wd,oneLine_h);
        }
        //mc
        function add_mc_right(txt){
            var mc=add_mc_base(disp_textH,txt,wd-centerX,oneLine_h);
            mc._x=centerX;
            total_h+=oneLine_h;
        }
	)");
	actionf_sprite(presprite,buf,"_x=%u",scenario_info_x);
	actionf_sprite(presprite,buf,"var height=%u;var wd=%u;var oneLine_h=%u",height,scenario_info_unit_w,scenario_info_unit_h);
	actionf_sprite(presprite,buf,"var centerX=%u",scenario_info_center);
	#define infoText_h scenario_info_unit_h/2
	actionf_sprite(presprite,buf,"var textHeight=%u",infoText_h);
	actionf_sprite(presprite,buf,R"(
		var total_h=0;var mc;
		//
		add_mc_top(_root.strip_underscores(_root.singleTraining[pos]),oneLine_h);
		//
		var dispKey=_root.singleTraining_dispKey[pos];
		//A positive integer that specifies the height of the new text field. oneLine_h=40
        var desc_hg=oneLine_h*4;
	//+(oneLine_h*3/4);
	if(dispKey.length==10){
		desc_hg-=oneLine_h;

		//this always happens at the moment
		//%%s
		//if(_root.singleTraining_mouse[pos])
		desc_hg-=oneLine_h;
	}
        add_mc_top(_root.singleTraining_desc[pos],desc_hg);
        //
        var dispKey=_root.singleTraining_dispKey[pos];var descKey=_root.singleTraining_descKey[pos];
        var disp_textH=textHeight-5;
        var i=0;while(i<dispKey.length){
            add_mc_left(_root.singleTraining_dispKey[pos][i]);
            //
            add_mc_right(_root.singleTraining_descKey[pos][i]);
            //
            i++;
        }
	if(_root.singleTraining_mouse[pos]){
	//!=undefined
		add_mc_left("%s");
		add_mc_right(_root.singleTraining_mouse[pos]);
	}
        //
        mc=attachMovie('info_button','info_button',getNextHighestDepth());
        mc._y=total_h;
        mc['txt']='Play';
        mc.onPress=function(){
            _root.start_scenario(pos);
        }
        total_h+=oneLine_h;
        //
        _y=(height-total_h)/2;
    )",mouse_disp_name);//,desc_height
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"scenario_info");
    //info_button
    EditText infoText;infoText.fontid=defFont;infoText.font_height=infoText_h;
    infoText.rgba=0xff0000FF;
    int infoTxt_entry;
    presprite=swf_sprite_new();
    infoTxt_entry=swf_text(scenario_info_unit_w-scenario_info_center,scenario_info_unit_h,"message",(HasFont|ReadOnly|NoSelect|HasTextColor),&infoText);
    swf_sprite_placeobject_coords(presprite,infoTxt_entry,0,scenario_info_center,0);
    actionf_sprite(presprite,buf,"_root.draw_list_entry(this,%u,%u);message=txt",scenario_info_unit_w,scenario_info_unit_h);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"info_button");
    //info_text
    presprite=swf_sprite_new();
    action_sprite(presprite,R"(
        _root.draw_list_entry(this,wd,hg);
        createTextField('txt_inf',getNextHighestDepth(),0,0,wd,hg);
        var fmt=new TextFormat();
        fmt.size=textHeight;
        txt_inf.multiline=true;
        txt_inf.wordWrap=true;
        txt_inf.text=txt;
        txt_inf.setTextFormat(fmt);
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"info_text");

    //counterBar
    int bar_sz=(bar_x<bar_y?bar_w:bar_h)-bar_startPos;
    presprite=swf_sprite_new();
    const char* cp_0=bar_x<bar_y?"current_pos":"0";
    const char* _0_cp=bar_x<bar_y?"0":"current_pos";
    actionf_sprite(presprite,buf,R"(
        function draw_progressBar(){
            var current_pos=bar_sz*barNr/barMax;
            clear();
            beginFill(0x00ffFF);
            moveTo(%s,%s);
            lineTo(%u,%s);//_root.bar._width aici nu am mai stat sa modific, e de la pointless, nici la pointless nu e in functie de care e mai mare
            lineTo(%u,%u);//_root.bar._width
            lineTo(%s,%u);//_root.bar._height
            endFill();
        }
    )",cp_0,_0_cp\
	,bar_w,_0_cp
	,bar_w,height-bar_startPos\
	,cp_0,height-bar_startPos);
    actionf_sprite(presprite,buf,"var bar_sz=%u",bar_sz);
	action_sprite(presprite,"draw_progressBar()");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"counterBar");

    //list
    dbl=swf_img("../tmp/root/list.dbl");
    swf_exports_add(dbl,"list_texture");
    actionf(buf,"var list_lineSz=%u",list_lineSz);
    action(R"(
        var list_matrix=new flash.geom.Matrix();
        var list_bmp=flash.display.BitmapData.loadBitmap('list_texture');
    )");

    int list_view_presprite=swf_sprite_new();
    EditText ed;ed.fontid=defFont;ed.font_height=text_height;
	action_sprite(list_view_presprite,R"(
        function put_page(page_nr){
            var d=container.getDepth();
            container.removeMovieClip();
            createEmptyMovieClip('container',d);
            //
            var list_start=page_nr*list_units;
            //                      ultima din pagina poate fi list_start+list_units sau ultimul level
            var list_end=Math.min(list_start+list_units,list_units_all);
            //
            var yPos=0;var mc;
            //la ultimele sa arate si dinainte sa fie pagina plina
            if((list_end-list_start)<list_units)list_start=list_units_all-list_units;
            //
            for(var j=list_start;j<list_end;){
			var i=_root.sorter[j];
			mc=container.attachMovie('list_entry',_root.singleTraining[i],container.getNextHighestDepth());
			mc._y=yPos;yPos+=list_unit_h;

			mc['pos']=i;
			mc.onPress=function(){
				_root.holder.removeMovieClip();
				_root.start_training(this.pos);
			}

			if(j==selLevel)mc['isSelected']=true;
			else if(so.data['lev'+i])mc['levelDone']=true;
			j++;
            }
            mc=container.attachMovie('list_pages','list_pages',container.getNextHighestDepth());
            mc._y=yPos;
            mc['page_info']=(page_nr+1)+'/'+(page_max+1);
            //remarcare: Click to play e pe bara unde e si "Go", bara e pe toata pagina, care se schimba
            if(selLevel<list_units_all)mc['continue_pos']=_root.sorter[selLevel];
        }
        function first_page(){
		selLevel=0;
		for(;selLevel<list_units_all;){
			if(!so.data['lev'+_root.sorter[selLevel]])break;//undefined sau false
			selLevel++;
		}
		//              aicea e player level sau ultimul index
		put_page(int(Math.min(selLevel,list_units_all-1)/list_units));
        }
	)");
    actionf_sprite(list_view_presprite,buf,"var list_inset=%u;var list_unit_w=%u;var list_unit_h=%u;var list_units=%u"
                                                          ,list_inset         ,list_unit_w       ,list_unit_h      ,episoade_pagina
    );
    //list_entry
    #define list_unit_play_inset_twice 2*5
    #define list_unit_play_h list_unit_h-(2*list_unit_play_inset_twice)
    #define list_unit_play_w list_unit_play_h
    presprite=swf_sprite_new();
	action_sprite(presprite,R"(
		function draw_check(color){
			lineStyle(1,color);
			var button_h_q=button_h/4;
			moveTo(xPos,yPos+button_h-button_h_q);
			lineTo(button_w/4+xPos,yPos+button_h);
			lineTo(xPos+button_w,yPos);
		}
	)");
    actionf_sprite(presprite,buf,"var list_unit_w=%u;var list_unit_h=%u",list_unit_w,list_unit_h);
    actionf_sprite(presprite,buf,"var play_inset_twice=%u;var button_w=%u;var button_h=%u",list_unit_play_inset_twice,list_unit_play_w,list_unit_play_h);
	actionf_sprite(presprite,buf, R"(
		if(isSelected)_root.draw_list_entry_ex(this,list_unit_w,list_unit_h,0xff0000);
		else _root.draw_list_entry(this,list_unit_w,list_unit_h);
		//
		attachMovie('list_entry_play','play',getNextHighestDepth());
		play._x=list_unit_w-play_inset_twice-button_w;
		var yPos=list_unit_h-button_h;
		//problema e ca la ffdec nu prea am paranteze asa simplu
		yPos/=2;
		play._y=yPos;
		//play['pos']=pos;//moved to all list_entry
		//
		var xPos=play._x-10-button_w;
		if(levelDone)draw_check(%u);
		%s
	)",!is_flashixy?color1:color2,location_mark);
    int list_txt=swf_text(rest_x,list_unit_h,"message",(HasFont|ReadOnly|NoSelect),&ed);
    swf_sprite_placeobject_coords(presprite,list_txt,0,text_x,0);
	actionf_sprite(presprite,buf,R"(
		message=_root.strip_underscores(_name);

		onRollOver=function(){
			if(_root.bar.is_pimg){
				var mcl=new MovieClipLoader();
				_root.createEmptyMovieClip('holder',_root.last_depth);
				//if inside game, remember about removeMovieClip

				var b=getBounds(_root);

				var listener=new Object();
				listener.onLoadComplete=function(mc){
					mc._x=_root._width-mc._width;
					if(b.yMin<%u)mc._y=_root._height-mc._height;
				}
				mcl.addListener(listener);

				mcl.loadClip(%s_name+'.jpg',_root.holder);
			}
		}
		onRollOut=function(){
			if(_root.bar.is_pimg)_root.holder.removeMovieClip();
		}
	)",height/2,preview_prefix);
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"list_entry");
    //list_entry_play
    presprite=swf_sprite_new();
    actionf_sprite(presprite,buf,"var play_w=%u;var play_h=%u",list_unit_play_w,list_unit_play_h);
    action_sprite(presprite,R"(
        lineStyle(1);
        beginFill(0x0000ff);
        lineTo(play_w,play_h/2);lineTo(0,play_h);lineTo(0,0);
        endFill();
		//this was moved to all list_entry
		//onPress=function(){
		//	_root.start_training(pos);
		//}
    )");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"list_entry_play");
    //
    //list_pages
    //
    #define go_width 40
    presprite=swf_sprite_new();
    EditText ed_go;ed_go.fontid=defFont;ed_go.font_height=text_height;
    ed_go.initialtext="Go";ed_go.rgba=0xff0000FF;
	ed_go.layout_align=layout_align_center;
	ed_go.layout_leftmargin=0;ed_go.layout_rightmargin=0;ed_go.layout_indent=0;ed_go.layout_leading=0;
    int list_go_txt=swf_text(go_width,list_unit_h,"",(HasFont|ReadOnly|NoSelect|HasText|HasTextColor|HasLayout),&ed_go);
    swf_sprite_placeobject(presprite,list_go_txt,0);
	action_sprite(presprite, R"(
		var page_max=_root.page_max;
		onPress=function(){
			var nr=Number(_parent.page_setNr);
			if(isNaN(nr))nr=page_max;
			else{
				nr=int(nr)-1;
				if(nr<0)nr=0;else if(page_max<nr)nr=page_max;
			}
			_root['list_mc'].put_page(nr);
		}
	)");
    //
    #define border_off 4
    actionf_sprite(presprite,buf,R"(
        lineStyle(1,0xff0000);
        var w_dist_to_underline=%u;
        var h_dist_to_underline=%u;
        var h_off=%u;
	beginFill(0xFfFF);
        moveTo(0,h_off);
        lineTo(w_dist_to_underline,h_off);
        lineTo(w_dist_to_underline,h_dist_to_underline);
        lineTo(0,h_dist_to_underline);
        lineTo(0,h_off);
	endFill()
    )",go_width,text_height+5,border_off);
    //
    swf_sprite_showframe(presprite);
    int go_button=swf_sprite_done(presprite);
    //continue_play
    presprite=swf_sprite_new();
	action_sprite(presprite,R"(
		function readd(){
			Key.addListener(this);
		}
	)");
    EditText ed_continue_play;ed_continue_play.fontid=defFont;ed_continue_play.font_height=text_height;
    ed_continue_play.initialtext="Play";ed_continue_play.rgba=0xff0000FF;
	#define continue_play_width 60
	ed_continue_play.layout_align=layout_align_center;
	ed_continue_play.layout_leftmargin=0;ed_continue_play.layout_rightmargin=0;ed_continue_play.layout_indent=0;ed_continue_play.layout_leading=0;
	int list_continue_play_txt=swf_text(continue_play_width,list_unit_h,"",(HasFont|ReadOnly|NoSelect|HasText|HasTextColor|HasLayout),&ed_continue_play);
    swf_sprite_placeobject(presprite,list_continue_play_txt,0);
    action_sprite(presprite,R"(
	onPress=function(){
		_root.start_training(pos);
	}
//la ruffle doar cu beginfill merge. asta nu useHandCursor=true
    )");
    //
    actionf_sprite(presprite,buf,R"(
        lineStyle(1,0xff0000);
        var w_dist_to_underline=%u;
        var h_dist_to_underline=%u;
        var h_off=%u;
	beginFill(0xFfFF);
        moveTo(0,h_off);
        lineTo(w_dist_to_underline,h_off);
        lineTo(w_dist_to_underline,h_dist_to_underline);
        lineTo(0,h_dist_to_underline);
	lineTo(0,h_off);
	endFill();
	readd();
	onKeyDown=function(){
		if(Key.getCode()==Key.ENTER)this.onPress();
	}
    )",continue_play_width,text_height+5,border_off);
    //
    swf_sprite_showframe(presprite);
    int continue_play=swf_sprite_done(presprite);
    swf_exports_add(continue_play,"list_continue_play");
	//reset button
	#define reset_width 75
	presprite=swf_sprite_new();
	EditText ed_reset;ed_reset.fontid=defFont;ed_reset.font_height=text_height;
	ed_reset.initialtext="Reset";ed_reset.rgba=0xff0000FF;
	ed_reset.layout_align=layout_align_center;
	ed_reset.layout_leftmargin=0;ed_reset.layout_rightmargin=0;ed_reset.layout_indent=0;ed_reset.layout_leading=0;
	int list_reset_txt=swf_text(reset_width,list_unit_h,"",(HasFont|ReadOnly|NoSelect|HasText|HasTextColor|HasLayout),&ed_reset);
	swf_sprite_placeobject(presprite,list_reset_txt,0);
	actionf_sprite(presprite,buf,R"(
		onPress=function(){
			var so=SharedObject.getLocal('levelIsDone');
			for(var i=0;i<%u;){
				so.data['lev'+i]=false;
				i++;
			}
			so.flush();
			_root['list_mc'].first_page();
		}
	)",get_iterator(is_flashixy));
	//
	actionf_sprite(presprite,buf,R"(
		lineStyle(1,0xff0000);
		var w_dist_to_underline=%u;
		var h_dist_to_underline=%u;
		var h_off=%u;
		beginFill(0xFfFF);
		moveTo(0,h_off);
		lineTo(w_dist_to_underline,h_off);
		lineTo(w_dist_to_underline,h_dist_to_underline);
		lineTo(0,h_dist_to_underline);
		lineTo(0,h_off);
		endFill()
	)",reset_width,text_height+5,border_off);
	//
	swf_sprite_showframe(presprite);
	int reset_button=swf_sprite_done(presprite);
    //
    presprite=swf_sprite_new();
    //
    EditText ed_inf;ed_inf.fontid=defFont;ed_inf.font_height=text_height;
    int list_inf_txt=swf_text(list_unit_w/4,list_unit_h,"page_info",(HasFont|ReadOnly|NoSelect),&ed_inf);
    swf_sprite_placeobject(presprite,list_inf_txt,0);
    //
    #define nr_size 40
    int setNr_x=list_unit_w/8;
    int go_x=setNr_x+nr_size;
    #define setNr_inset list_lineSz
    EditText ed_setnr;ed_setnr.fontid=defFont;ed_setnr.font_height=text_height-(2*setNr_inset);
    int list_nr_txt=swf_text(nr_size,list_unit_h-(2*setNr_inset),"page_setNr",(HasFont|Border),&ed_setnr);
    swf_sprite_placeobject_coords(presprite,list_nr_txt,1,setNr_x,setNr_inset);
    //
    swf_sprite_placeobject_coords(presprite,go_button,2,go_x+3,0);
    //
    actionf_sprite(presprite,buf,"_root.draw_list_entry(this,%u,%u)",list_unit_w,list_unit_h);
    actionf_sprite(presprite,buf,R"(
	if(continue_pos!=undefined){
	//0,1,...
		attachMovie('list_continue_play','continue_play',getNextHighestDepth());
		continue_play._x=%u;
		continue_play['pos']=continue_pos;
	}
    )",list_w/2-(continue_play_width/2));//go_x+go_width+100
    //
	swf_sprite_placeobject_coords(presprite,reset_button,3,list_unit_w-reset_width-5,0);
    //
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"list_pages");
    //
	action_sprite(list_view_presprite, "var list_units_all=_root.list_units_all;var page_max=_root.page_max");
    action_sprite(list_view_presprite,R"(
        _root['list_mc']=this;
    )");
    //sort
    #define sort_h list_unit_h
	int sort_w=list_unit_w/(!is_flashixy?sorts1_length:sorts2_length);
    //
    presprite=swf_sprite_new();
	action_sprite(presprite, R"(
        function paintings(){
            clear();
            beginBitmapFill(list_bmp,list_matrix,true);
            lineTo(w,0);lineTo(w,hg);lineTo(0,hg);
            endFill();
            if(shar.data.value==_name){
                beginBitmapFill(list_bmp,list_matrix,true);
                moveTo(0,hg);
                lineTo(w,hg);lineTo(w,hg+line);lineTo(0,hg+line);
                endFill();
                //
			//_root.call(_name,_parent);
			_parent.first_page();
            }
        }
	)");
    actionf_sprite(presprite,buf,"var wd=%u;var hg=%u;var w=%u;var line=%u",sort_w,sort_h,sort_w-4,list_lineSz);
	action_sprite(presprite, R"(
        var list_matrix=new flash.geom.Matrix();
        var list_bmp=flash.display.BitmapData.loadBitmap('list_texture');
        _y-=hg;
        _x=coef*wd;
        var shar=SharedObject.getLocal('listSort');
        paintings();
        onPress=function(){
            var prev=_parent[shar.data.value];
            shar.data.value=_name;
            shar.flush();
		_root.homeAgain();
        }
    )");
    #define sort_text_offX 10
    list_txt=swf_text(sort_w-sort_text_offX,sort_h,"message",(HasFont|ReadOnly|NoSelect),&ed);
    swf_sprite_placeobject_coords(presprite,list_txt,0,sort_text_offX,0);
    action_sprite(presprite,"message=_name");
    swf_sprite_showframe(presprite);
    sprite=swf_sprite_done(presprite);swf_exports_add(sprite,"list_sort");
    //put container then sorters above, to draw sorter over list
    action_sprite(list_view_presprite,"createEmptyMovieClip('container',getNextHighestDepth())");
	for(int i=0;i<(!is_flashixy?sorts1_length:sorts2_length);i++){
		actionf_sprite(list_view_presprite,buf,"var mov=attachMovie('list_sort','%s',getNextHighestDepth());mov['coef']=%u", \
			!is_flashixy?sorts1[i]:sorts_f[i],i);
	}
    //
	action_sprite(list_view_presprite,R"(
		_x=list_inset;_y=list_inset;
		//used at put_page and first_page
		var so=SharedObject.getLocal('levelIsDone');
		var selLevel;
	)");
    swf_sprite_showframe(list_view_presprite);
    sprite=swf_sprite_done(list_view_presprite);swf_exports_add(sprite, l_view);

    //

	actionf(buf,R"(
		var sharlistSort=SharedObject.getLocal('listSort');
		if(!sharlistSort.data.value){
			sharlistSort.data['value']='%s';//undefined sau 'Location',...
		}
		//if(sharlistSort.data.timeout==undefined)sharlistSort.data['timeout']=9;
		//timeout apare ciudat in fisierul sol, dar e corect
	)",!is_flashixy?sorts1[0]:sorts_f[0]);

	action("var start_time");

	submitform(defFont,NULL,R"(
		_root.start_scenario(_root.game.episode_number)
	)",NULL);//,is_flashixy t race(game_name);

	//com
	//asta era cand erau toate: shared_write();
	//if(!is_flashixy) chambers e la fel ca 13
	shared_write_ex(true,//13 22
			1,//13
			0,
			true,//space_env
			1    //7 13
	);
	//else shared_write_ex(false,0,0,true,1);

	action("createEmptyMovieClip('game',getNextHighestDepth());home_set();attachMovie('bar','bar',getNextHighestDepth())");
	if (bar_x != 0){
		actionf(buf,"bar_x=%u", bar_x);
		touch_or_mouse(true,is_flashixy);
		action("bar._x=bar_x");
	}
	if (bar_y != 0)actionf(buf, "bar._y=%u", bar_y);
	action("attachMovie('shared_level','shared_level',getNextHighestDepth())");
	action("var last_depth=getNextHighestDepth()");

    swf_exports_done();
	swf_showframe();
	swf_done();
	printf("done\n");
	return 0;
}
#ifdef __cplusplus
}
#endif
