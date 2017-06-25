#include "DarkGDK.h"
#include <string.h>

// Constantes
const int height = 20;			// Altura do tabuleiro do jogo
const int width = 10;			// Comprimento do tabuleiro do jogo
const int sizeblock = 20;		
const int startplace = 5;
const int imgListLenght = 100;
int gameboard[height][width];
int score = 0;
int imglist[100];
int yspaceini = 40;
int xspaceini = 40;

// Definições da janela e de aspecto
void definitions()
{
	
	dbSetDisplayMode ( 500, 400, 32 );		// Resolução da janela
	dbSetWindowSize( 500, 400 );			// Tamanho da janela

	dbSetWindowLayout( 1, 1, 1 );
	dbSetWindowTitle( "Tetris Clone 0.8" );	// Nome da janela
	dbSetWindowOn();						
	
	//dbSyncOn();
	//dbSyncRate(60);
	dbSetTextFont( "book" );				// Tipo de fonte a usar
}

/*
	********************************************************************************
	Quando um objecto é criado, atribui automáticamente um número a este. Se o 
	objecto for apagado, liberta um espaço na lista que poderá ser usado 
	posteriormente por outro objecto. Suporta até ao número de elementos máximos do
	array atribuído.
	********************************************************************************
*/
int adImage()
{
	int id = 0;
	for(int i = 1; i < imgListLenght; i++)
	{
		if(imglist[i] == 0)
		{
			id = i;
			imglist[i] = 1;
			return id;
		}
	}
	return id;
}
/*
	********************************************************************************
	Verifica se existem alguma linha completa. Se for encontrado, apaga a linha, 
	move as peças todas que estavam nas linhas acima 1 linha para baixo e adiciona 
	um ponto á pontuação.
	********************************************************************************
*/
void checkRows()
{
	int count = 0;
	for( int i = 0; i < height; i++ )
	{
		for( int j = 0; j < width; j++ )
		{
			if( gameboard[i][j] == 1 )
				count ++;
		}
		if( count == width )
		{
			for( int k = 0; k < width; k++ )
				gameboard[i][k] = 0;
			for( int l = i; l > 1; l-- )
				for( int m = 0; m < width; m++ )
					gameboard[l][m] = gameboard[l-1][m];
			score++;
		}
		count = 0;
	}
}

void clearPieces()
{
	for( int j = 0; j < height; j++)
		for( int i = 0; i < width; i++ )
			if( gameboard[j][i] == 2 )
				gameboard[j][i] = 0;
}

class block
{
	int x;
	int y;
	int degree;
	int type;
	int piece[4][4];
public:
	block( int posx, int posy )
	{
		x = posx;
		y = posy;
		degree = 0;
		pieceCreate();
	}
	void setx(int newpos){x = newpos;}
	void sety(int newpos){y = newpos;}
	
	void pieceClear( int a[4][4] )
	{
		for( int j = 0; j < 4; j++)
		{
			for( int i = 0; i < 4; i++ )
			{
				a[j][i] = 0;
			}
		}
	}
	void pieceCreate()
	{
		pieceClear( piece );
		type = dbRnd( 6 );
		if( type == 0 ) 
		{
			piece[0][1] = 3;//*3*
			piece[1][1] = 3;//*1*
			piece[2][1] = 3;//*3*
			piece[3][1] = 3;//*3*
		}
		else if( type == 1) 
		{
			piece[0][1] = 3;
			piece[1][1] = 3;//*3*
			piece[2][1] = 3;//*1*
			piece[2][2] = 3;//*33
		}
		else if( type == 2) 
		{
			piece[0][1] = 3;
			piece[1][1] = 3;//*3*
			piece[2][1] = 3;//*1*
			piece[2][0] = 3;//33*
		}
		else if( type == 3) 
		{
			piece[0][1] = 3;
			piece[0][2] = 3;//*33
			piece[1][0] = 3;//31*
			piece[1][1] = 3;//***

		} 
		else if( type == 4) 
		{			
			piece[0][0] = 3;
			piece[0][1] = 3;//33*
			piece[1][1] = 3;//*13
			piece[1][2] = 3;//***

		}
		else if( type == 5) 
		{
			piece[0][0] = 3;
			piece[0][1] = 3;//33*
			piece[1][0] = 3;//31*
			piece[1][1] = 3;//***
		}
		else 
		{
			piece[0][1] = 3;
			piece[1][0] = 3;//*3*
			piece[1][1] = 3;//313
			piece[1][2] = 3;//***
		}
	}
	void pieceRotate()
	{
		int temp[4][4];
		if( type != 5 ) // Os cubos não precisam de rodar
		{
			pieceClear( temp );
			for( int i = 0; i < 3; i++ )
				for( int j = 0, k = 3; j < 3; j++, k-- )
					temp[k-1][i] = piece[i][j];
			for( int i = 0; i < 3; i++ )
				for( int j = 0; j < 3; j++ )
					piece[i][j] = temp[i][j];
		}
	}
	int posExact( int pospiece )
	{
		if( pospiece == 1 )
			return 0;
		else if( pospiece == 0 )
			return -1;
		else if( pospiece == 2)
			return 1;
		else
			return 2;
	}
	void checkDraw( int posy, int posx )
	{
		int cont = 0;
		int blkv = 0;
		int blkh = 0;
		// Verifica se colide com algum objecto ou parede ou fundo
		for( int j = 0; j < 4; j++)
		{
			for( int i = 0; i < 4; i++ )
			{
				if( piece[j][i] == 3 )
				{
					if( ( gameboard[posExact(j)][posExact(i) + posx] == 1 ) )
					{
						blkh++;
					}
					if( ( gameboard[posExact(j) + posy][x + posExact(i)] == 1 ) || ( ( posExact(j) + posy ) >= ( height ) ) )
					{
						blkv++;
					}
					if( ( gameboard[posExact(j) + posy][posExact(i) + posx] != 1 ) && ( (posExact(j) + posy) < (height) ) && ( (posExact(i) + posx) < width && (posExact(i) + posx) >= 0 ) )
					{
						cont++;
					}
				}
			}
		}
		if( cont == 4 ) // Se as 4 peças podem ser desenhadas, então desenha
		{
			clearPieces();
			for( int j = 0; j < 4; j++)
			{
				for( int i = 0; i < 4; i++ )
				{
					if( piece[j][i] == 3 )
					{
						gameboard[posExact(j) + posy][posExact(i) + posx] = 2;
					}
				}
			}
			x = posx;
			y = posy;
		}
		else if( blkv != 0 && blkh == 0) // Se colidir com um bloco na vertical e nenhum dos lados
		{
			clearPieces();
			for( int j = 0; j < 4; j++)
			{
				for( int i = 0; i < 4; i++ )
				{
					if( piece[j][i] == 3 )
					{
						gameboard[(y-1)+j][(x-1)+i] = 1;
					}
				}
			}
			x = startplace;
			y = 0;
			pieceCreate();
		}
	}
	void move(int posy, int posx)
	{
		int count = 0;
		if( posx != 0)
		{
			checkDraw( y+posy, x+posx );
		}
		if( posy != 0)
		{
			checkDraw( y+posy, x+posx );
		}
		checkRows();
	}

	int getx(){return x;}
	int gety(){return y;}
};


void testshow()
{
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
		{
			dbText(j * 10, i * 10, dbStr(gameboard[i][j]));
		}
}

void draw()
{
	char temp[100] = "";
	for(int i = 0; i < height; i++)
		for(int j = 0; j < width; j++)
		{
			if( gameboard[i][j] == 2 )
				dbPasteImage(1, ( 20 + ( j * 15 ) ), ( 20 + ( i * 15 ) ) );
			else if( gameboard[i][j] == 1 )
				dbPasteImage(2, ( 20 + ( j * 15 ) ), ( 20 + ( i * 15 ) ) );
			else
				dbPasteImage(3, ( 20 + ( j * 15 ) ), ( 20 + ( i * 15 ) ) );
		}
	strcat(temp, "Score: ");
	strcat(temp, dbStr(score));
	dbText((45 + (width * 20)), 20, temp);
}

void gameover( void )
{
	for(int j = 0; j < height; j++)
		if( gameboard[-1][j] == 1 )
		{
			dbSetTextSize(40);
			dbCenterText(250, 200, "Game Over");
			dbWaitKey();
		}
}

void DarkGDK ( void )
{
	dbRandomize(dbTimer());
	definitions();
	
	dbLoadImage("cube.png", 1);
	dbLoadImage("hold.png", 2);
	dbLoadImage("blank.png", 3);
	block bloco(startplace, 0);
	int turn = 0;
	int time = 0;
	bool blockmoving = 0;
	int timekey = 0;
	int timenext = 750;
	int timekeymove = 100;
	bool keypress = 0;
	
	while ( LoopGDK ( ) ) // Ciclo Principal
	{
		dbCLS();
		if( turn == 0 )
		{
			time = dbTimer();
			turn = 1;
		}
		else
		{
			if( (dbTimer() - time) <= timenext )//Tempo até o bloco cair
			{
				if( dbKeyState(30) == 1 )//Tecla A
				{
					if( keypress == 0 )
					{
						keypress = 1;
						timekey = dbTimer();
					}
					if ( ( keypress == 1 ) && ( ( dbTimer() - timekey ) >= timekeymove ) )
					{
						bloco.move(0,-1); // Move a peça para a esquerda
						keypress = 0;
					}
				}
				if( dbKeyState(32) == 1 )// Tecla D
				{
					if( keypress == 0 )
					{
						keypress = 1;
						timekey = dbTimer();
					}
					if ( ( keypress == 1 ) && ( ( dbTimer() - timekey ) >= timekeymove ) )
					{
						bloco.move(0,1); // Move a peça para a direita
						keypress = 0;
					}	
				}
				if( dbKeyState(31) == 1 )// Tecla S
				{
					bloco.move(1,0); // Move a peça para a direita
				}
				if( dbKeyState(17) == 1 ) // Rodar a peça
				{
					if( keypress == 0 )
					{
						keypress = 1;
						timekey = dbTimer();
					}
					if ( ( keypress == 1 ) && ( ( dbTimer() - timekey ) >= timekeymove ) )
					{
						bloco.pieceRotate();
						keypress == 0;
					}
				}
			}
			else
			{
				bloco.move(1,0);
				turn = 0;
			}
		}
		//testshow();
		draw();
		dbSync();
	}
	return;
}
