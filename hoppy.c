
#include "neslib.h"

//basic variables

static unsigned char i;
static unsigned char pad,spr;
static unsigned char touch;
static unsigned char frame;

//coordinates

static unsigned char hoppy_x[2];
static unsigned char hoppy_y[2];


//first hopper

const unsigned char metaHoppy1[]={ 0,	0,	0x50,	0,
	8,	0,	0x51,	0,
	16,	0,	0x52,	0,
	0,	8,	0x60,	0,
	8,	8,	0x61,	0,
	16,	8,	0x62,	0,
	0,	16,	0x70,	0,
	8,	16,	0x71,	0,
	16,	16,	0x72,	0,
	128
};

//second hopper

const unsigned char metaHoppy2[]={
	0,	0,	0x50,	1,
	8,	0,	0x51,	1,
	16,	0,	0x52,	1,
	0,	8,	0x60,	1,
	8,	8,	0x61,	1,
	16,	8,	0x62,	1,
	0,	16,	0x70,	1,
	8,	16,	0x71,	1,
	16,	16,	0x72,	1,
	128
};



void main(void)
{
	ppu_on_all();//enable rendering

	//set coordinates
	
	hoppy_x[0]=52;
	hoppy_y[0]=100;
	hoppy_x[1]=180;
	hoppy_y[1]=100;

	//initialise other variables
	
	touch=0; //collision flag
	frame=0; //framecount

	//main/gameloop

	while(1)
	{
		ppu_wait_frame();//wait for next  frame

		//flashing if sprites touch

		i=frame&1?0x30:0x2a;

		pal_col(17,touch?i:0x21);//set first sprite palette
		pal_col(21,touch?i:0x26);//set second sprite palette

		//process data
		
		spr=0;

		for(i=0;i<2;++i)
		{
			//display hoppy
			
			spr=oam_meta_spr(hoppy_x[i],hoppy_y[i],spr,!i?metaHoppy1:metaHoppy2);

			//poll inputs 
			
			pad=pad_poll(i);

			if(pad&PAD_LEFT &&hoppy_x[i]>  0) hoppy_x[i]-=2;
			if(pad&PAD_RIGHT&&hoppy_x[i]<232) hoppy_x[i]+=2;
			if(pad&PAD_UP   &&hoppy_y[i]>  0) hoppy_y[i]-=2;
			if(pad&PAD_DOWN &&hoppy_y[i]<212) hoppy_y[i]+=2;
		}

		//testing collisions for 20x20 box??
		
		if(!(hoppy_x[0]+22< hoppy_x[1]+2 ||
		     hoppy_x[0]+ 2>=hoppy_x[1]+22||
	         hoppy_y[0]+22< hoppy_y[1]+2 ||
		     hoppy_y[0]+ 2>=hoppy_y[1]+22)) touch=1; else touch=0;

		frame++;
	}
}