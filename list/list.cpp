
//#include<windows.h>
#include<stdio.h>
#include <string.h>
#include <stdarg.h>

#include "../universe.hpp"

/*static int locatia_prima(){//daca schimb aici ! cu fara, trebuie si in sorts_loc la fel
	int i=0;
	for(; i<numarulEpisoadelor_total; i++){
		if(!is_flashix(episoade[i]))return i;
	}
	return i;
}
static int locatia_prima_totalul(){
	int j=0;
	for(int i=0; i<numarulEpisoadelor_total; i++){
		if(!is_flashix(episoade[i]))j++;
	}
	return j;
}
static int locatia_a_doua(){
	int i=0;
	for(; i<numarulEpisoadelor_total; i++){
		if(is_flashix(episoade[i]))return i;
	}
	return i;
}*/
/*static char strip_temp[100];
static char *strip(const char *p)
{
	char *out = strip_temp;
	while (*p) {// && out < strip_temp + sizeof(strip_temp) - 1) {
		*out++ = (*p == '_') ? ' ' : *p;
		p++;
	}
	*out = '\0';
	return strip_temp;
}*/

bool is_flashixy;
static void add_ep(int iteratar,episoade_pack*ep){
	char*nm=!is_flashixy?iteratar,ep->name:(!ep->id?ep->name:ep->designation);//strip(ep->name)
	actionf(buf,"_root['singleTraining'][%u]='%s'", iteratar,nm);
	actionf(buf,"_root['singleTraining_desc'][%u]='%s'", iteratar,ep->description);
	actionf(buf,"_root['singleTraining_dispKey'][%u]=new Array();_root['singleTraining_descKey'][%u]=new Array()", iteratar, iteratar);
	ep_keys* ep_k=ep->episod_chei;
	if(ep_k!=NULL){
		int key_nr=get_ep_nr(ep_k);
		for(int i=0;i<key_nr;i++){
			actionf(buf,"_root['singleTraining_dispKey'][%u][%u]='%s'", iteratar,i,ep_k[i].key->disp_name);
			actionf(buf,"_root['singleTraining_descKey'][%u][%u]='%s'", iteratar,i,ep_k[i].key_desc);
		}
	}
	char*mousePressDesc=ep->mousePress;
	if(mousePressDesc!=NULL)actionf(buf,"_root['singleTraining_mouse'][%u]='%s'", iteratar,mousePressDesc);
	if(is_flashixy){
		char*a=ep->id!=NULL?ep->id:(ep->base==kongregate?ep->designation:NULL);
		if(a){
			actionf(buf, "_root['singleTraining_ids'][%u]='%s'",iteratar,a);
		}
	}
	//actionf(buf, "_root['singleTraining_stat'][%u]='%u'", numarulEpisodului, ep->id_stat);
	//actionf(buf, "_root['singleTraining_%s'][%u]='%u'", pop,numarulEpisodului, ep->idpop);
	//actionf(buf, "_root['singleTraining_%s'][%u]='%u'", rat,numarulEpisodului, ep->idrat);
	//actionf(buf, "_root['singleTraining_%s'][%u]='%u'", rat_user, numarulEpisodului, ep->idratu);
}

#ifdef __cplusplus
extern "C" {
#endif

int main(int argc,char**argv){
//rooted_swf_path("list") "../test/list.swf"
	flags_macro
	char*nm=getenv("site");
	if(!nm){
		nm="";is_flashixy=false;
	}else is_flashixy=true;
	char nmbuf[100];sprintf(nmbuf,"../../swf_backup/%suniverse/list.swf",nm);
	swf_new_ex(nmbuf,width,height,0x112211,fps,flags);

    //securitate();
	int iterator=get_iterator(is_flashixy);
	int index_ultima_pagina = iterator - 1;
	index_ultima_pagina /= episoade_pagina;
	actionf(buf, "_root['list_units_all']=%u;_root['page_max']=%u", iterator, index_ultima_pagina);

    //list
    action("_root['singleTraining']=new Array()");
    action("_root['singleTraining_desc']=new Array()");
    action("_root['singleTraining_dispKey']=new Array()");
    action("_root['singleTraining_descKey']=new Array()");
    action("_root['singleTraining_mouse']=new Array()");
	if(is_flashixy)action("_root['singleTraining_ids']=new Array()");
//	action("_root['singleTraining_stat']=new Array()");
//	action("_root['singleTraining_scores']=new Array()");
	//actionf(buf, "_root['singleTraining_%s']=new Array();_root['singleTraining_%s']=new Array()", pop, rat);
	//actionf(buf, "_root['singleTraining_%s']=new Array();", rat_user);
	int iteratar=0;
	for(int numarulEpisodului=0; numarulEpisodului<numarulEpisoadelor_total; numarulEpisodului++){
		bool b=!is_flashixy?is_universe(episoade[numarulEpisodului]):true;
		if(b){
			add_ep(iteratar,episoade[numarulEpisodului]);
			iteratar++;
		}
	}

	//sorter
	//sprintf(buf, "%s", "function sort_the_list(){");
	sprintf(buf, R"(
		var n=_root.sharlistSort.data.value;
		var maximum=%u;
	)", iterator); //numarulEpisoadelor_total
	action(buf);
	char*sorts_chron=R"(
		var s=new Array();
		for(var i=0;i<maximum;){
			s[i]=i;
			i++;
		}
	)";
	char*sorts_new=R"(
		var s=new Array();
		var j=maximum;
		for(var i=0;i<maximum;){
			j--;s[i]=j;
			i++;
		}
	)";
	char*sorts_play=R"(
		var s=flash.external.ExternalInterface.call('sorts_play');
	)";
	char*s0;char*ffirst;char*flast;char*specific;
	if(!is_flashixy){
		s0=sorts1[0];
		ffirst=sorts_chron;
		buf2[0]='\0';
		flast=sorts_new;
		specific="";
	}else{
		s0=sorts2[0];
		ffirst=sorts_play;
		sprintf(buf2,"else if(n=='%s'){%s}",sorts2[1],sorts_chron);
		flast=sorts_new;
		specific="_root['sorter_play']=flash.external.ExternalInterface.call('sorts_plays');";
/*in clipa asta Location e identica cu Chronology
		actionf(buf,R"(
			function sorts_loc(){
				var s=new Array();
				var j=0;
				//primele sunt locale
				for(var i=%u;j<%u;){
					if(_root['singleTraining_ids'][i]){//string pentru http.../.. sau ''
						s[j]=i;
						j++;
					}
					i++;
				}
				//urmatoarele sunt externe
				//           aicea e j ca doar daca e gata j nu mai stam dupa i
				for(var i=%u;j<maximum;){
					if(!_root['singleTraining_ids'][i]){
					//!=''
						s[j]=i;
						j++;
					}
					i++;
				}
				return s;
			}
		)", locatia_prima(), locatia_prima_totalul(), locatia_a_doua());
		actionf(buf,R"(
			fsorts['%s']=sorts_loc;
		)",stable_sort[2]);
*/
	}
	actionf(buf,"if(n=='%s'){%s}%selse{%s}%s",s0,ffirst,buf2,flast,specific);
	action(R"(
		_root['sorter']=s;
		_root.list_loaded();
	)");

	//nu e nimic fara root.attach
	//actionf(buf, "_root.attachMovie('%s','%s',_root.getNextHighestDepth());_root.%s.%s=0;", information, information, information, information);

    swf_showframe();
    swf_done();
	printf("done\n");
    return 0;
}

#ifdef __cplusplus
}
#endif
