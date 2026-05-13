
#include "../universe.hpp"

bool a=false;
void score(long int value){
	if(!a)a=true;else putchar(' ');
	printf("%ld",value);
}

void game(char*name){
	long int value;
	char cmd[512];
	if(snprintf(cmd, sizeof(cmd), "python3 get_plays.py \"%s\"", name) >= sizeof(cmd))exit(1);

	FILE *fp = popen(cmd, "r");
	if(!fp)exit(1);
	if(fscanf(fp, "%ld", &value) != 1)exit(1);
	pclose(fp);

	score(value);
}
int main(){
	for(int numarulEpisodului=0; numarulEpisodului<numarulEpisoadelor_total; numarulEpisodului++){
		if(episoade[numarulEpisodului]->id)game(episoade[numarulEpisodului]->id);
		else if(episoade[numarulEpisodului]->designation)game(episoade[numarulEpisodului]->designation);
		else{
			long int value;
			FILE *fp = popen("python3 get_firestore.py", "r");
			fscanf(fp, "%ld", &value);
			pclose(fp);
			score(value);
		}
	}
	return 0;
}
