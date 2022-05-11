#include <stdio.h>

#define HOLECOUNT 12
#define MARB_PER_HOLE 4

typedef struct{
	
	unsigned char board[HOLECOUNT];
	unsigned short p1points;
	unsigned short p2points;
	
}gameboard;

typedef struct{
	
	int pathlen;
	int eval;
	char path[80];
	gameboard board;
	
}branchinfo;

void printboard(gameboard board){
	
	printf("\n");
	
	int r;
	
	if(board.p2points<10) printf(" ");
	printf("%d ",board.p2points);
	
	for(r=HOLECOUNT-1;r>(HOLECOUNT/2-1);r--){
		
		if(board.board[r]<10) printf(" ");
		printf("%d ",board.board[r]);
	}
	
	printf("\n   ");
	
	for(r=0;r<HOLECOUNT/2;r++){
		
		if(board.board[r]<10) printf(" ");
		printf("%d ",board.board[r]);
	}
	
	if(board.p1points<10) printf(" ");
	printf("%d ",board.p1points);
}

int playerhasmove(gameboard board,int player){
	
	int index;
	
	if(player==1) index=0;
	else index=HOLECOUNT/2;
	
	int r;
	
	for(r=0;r<(HOLECOUNT/2);r++) if(board.board[r+index]!=0) return 1;
	
	return 0;
}

branchinfo findmax(branchinfo* list,int itemcount){
	
	branchinfo highest;
	int r;
	
	highest=list[0];
	
	for(r=1;r<itemcount;r++){
		
		if(list[r].eval>highest.eval) highest=list[r];
	}
	
	return highest;
}

branchinfo findmin(branchinfo* list,int itemcount){
	
	branchinfo lowest;
	int r;
	
	lowest=list[0];
	
	for(r=1;r<itemcount;r++){
		
		if(list[r].eval<lowest.eval) lowest=list[r];
	}
	
	return lowest;
}

int makemove(gameboard *board,int space,int player){
	
	int count;
	int points;
	int endonpoint;
	int disfromscore;
	
	count=board->board[space];
	board->board[space]=0;
	disfromscore=0;
	endonpoint=0;
	
	if(player==1){
		
		if(space>5) disfromscore=(HOLECOUNT-space)+(HOLECOUNT/2);
		else disfromscore=(HOLECOUNT/2)-space;
	}
	else{
		
		disfromscore=HOLECOUNT-space;
	}
	
	int temp;
	
	temp=count-disfromscore;
	
	for(points=0;temp>-1;points++){
		
		temp-=HOLECOUNT+1;
	}
	
	if(temp==((HOLECOUNT+1)*-1)) endonpoint=1;
	
	if(player==1) board->p1points+=points;
	else board->p2points+=points;
	
	count-=points;
	
	int r;
	
	for(r=0;r<count;r++){
		
		space++;
		
		if(space==HOLECOUNT) space=0;
		
		board->board[space]++;
	}
		
	if(endonpoint) return 1;
	else if(board->board[space]>1) return makemove(board,space,player);

	return 0;
}


branchinfo branch(branchinfo board,int alpha,int beta,int player,int depth,int pathlen){
	
	if((!playerhasmove(board.board,player)) || depth==0){
		
		board.eval=board.board.p1points-board.board.p2points;
		board.pathlen=pathlen-1;
		return board;
	}
	
	int movecount;
	int index;
	char moves[HOLECOUNT/2];
	movecount=0;
	
	if(player==1) index=0;
	else index=HOLECOUNT/2;
	
	int r;
	
	for(r=0;r<(HOLECOUNT/2);r++){
		
		if(board.board.board[r+index]!=0){
			
			moves[movecount]=r+index;
			movecount++;
		}
	}
	
	int resultcount;
	int tempplayer;
	int dec;
	branchinfo results[HOLECOUNT/2];
	resultcount=0;
	
	branchinfo temp;
	
	if(player==1){
		
		for(r=0;r<movecount;r++){
			
			temp=board;
			
			temp.path[pathlen]=moves[r];
			
			if(!makemove(&temp.board,moves[r],player)){
				
				tempplayer=player*-1;
				dec=1;
			}
			else{
				
				tempplayer=player;
				dec=0;
			}
			
			results[r]=branch(temp,alpha,beta,tempplayer,depth-dec,pathlen+1);
			
			resultcount++;
			
			if(results[r].eval>alpha) alpha=results[r].eval;
			if(beta<=alpha) break;
		}
		
		return findmax(results,resultcount);
	}
	else{
		
		for(r=0;r<movecount;r++){
			
			temp=board;
			
			temp.path[pathlen]=moves[r];
			
			if(!makemove(&temp.board,moves[r],player)){
				
				tempplayer=player*-1;
				dec=1;
			}
			else{
				
				tempplayer=player;
				dec=0;
			}
			
			results[r]=branch(temp,alpha,beta,tempplayer,depth-dec,pathlen+1);
			
			resultcount++;
			
			if(results[r].eval<beta) beta=results[r].eval;
			if(beta<=alpha) break;
		}
		
		return findmin(results,resultcount);
	}
}

int aimove(gameboard board,int player){
	
	branchinfo result;
	
	result.board=board;
	
	result=branch(result,-100000,100000,player,5,0);
	
	return result.path[0];
}

void main(){
	
	int player;
	gameboard board;
	int input;
	int repeat;
	int index;
	int r;
	int plan;
	int aiisplayer;
	
	for(r=0;r<HOLECOUNT;r++) board.board[r]=MARB_PER_HOLE;
	
	board.p1points=0;
	board.p2points=0;
	
	player=1;
	
	printf("\nWho will move first?\n1 for human, 2 for ai\n\n");
	
	scanf("%d",&aiisplayer);
		
	if(aiisplayer==2) aiisplayer=1;
	else aiisplayer=-1;
	
	while(1){
		
		if(player==1) printf("\nplayer 1 to move\n");
		else printf("\nplayer 2 to move\n");
		
		printboard(board);

		if(playerhasmove(board,player)){
			if(player==aiisplayer){
			
				//scanf("%d",&input);
				//repeat=makemove(&board,input,player);
				plan=aimove(board,player);
				printf("|%d|",plan);
				repeat=makemove(&board,plan,player);
			}
			else{
				
				scanf("%d",&input);
				repeat=makemove(&board,input,player);
				//plan=aimove(board,player);
				//printf("|%d|",plan);
				//repeat=makemove(&board,aimove(board,player),player);
			}
			
			if(repeat==0) player*=-1;
		}
		else player*=-1;
		
		if(!(playerhasmove(board,player) || playerhasmove(board,(~player)+1))) break;
	}
	
	printboard(board);
	
	if(board.p1points==board.p2points) printf("\ndraw\n");
	if(board.p1points>board.p2points) printf("\nplayer 1 wins\n");
	if(board.p2points>board.p1points) printf("\nplayer 2 wins\n");
}
