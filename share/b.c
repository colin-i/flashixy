
#include "../universe.hpp"

bool a=false;
void put(float value){
	if(!a)a=true;else putchar(' ');
	printf("%.2f",value);
}

void game(char*name){
	char cmd[512];
	if(snprintf(cmd, sizeof(cmd), "python3 get_plays.py \"%s\" rating", name) >= sizeof(cmd))exit(1);

	FILE *fp = popen(cmd, "r");
	if(!fp)exit(1);
	float value;
	if(fscanf(fp, "%f", &value) != 1)exit(1);
	pclose(fp);

	put(value);
}
int main(){
	for(int numarulEpisodului=0; numarulEpisodului<numarulEpisoadelor_total; numarulEpisodului++){
		if(episoade[numarulEpisodului]->id)game(episoade[numarulEpisodului]->id);
		else if(episoade[numarulEpisodului]->designation)game(episoade[numarulEpisodului]->designation);
		else{
			float value;
			FILE *fp = popen("python3 get_firestore.py stars", "r");
			fscanf(fp, "%f", &value);
			pclose(fp);
			put(value);
		}
	}
	return 0;
}
