#include "Gfx.hpp"

int main()
{
	/* Get a random colour, ORed with 0xff000000 for 100% opacity */
	auto getRandom = []() {
		return (std::rand() % 0x1000000) | 0xff000000;
	};
	
	u32 xpos = 0, ypos = 0, speed = 1, col = getRandom();
	u64 key;
	bool dirUp = false, dirRight = true;

    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    PadState pad;
    padInitializeDefault(&pad);

	hidInitializeTouchScreen();

    s32 prev_touchcount=0;
	
	Gfx::Init();

	/* Seed rand() */
	std::srand(std::time(NULL));

	while (appletMainLoop())
	{
		padUpdate(&pad);
		u64 kDown = padGetButtonsDown(&pad);

		if (kDown & HidNpadButton_Plus) break;

		HidTouchScreenState state={0};
        if (hidGetTouchScreenStates(&state, 1)) {
            if (state.count != prev_touchcount)
            {
                prev_touchcount = state.count;
            } 
		}
        if (kDown & HidNpadButton_A)
		{
			/* Adjust speed */
			switch (speed)
			{
				case 1:
					speed = 3;
					break;
				case 3:
					speed = 6;
					break;
				case 6:
					speed = 24;
					break;
				case 24:
					speed = 1;
					break;
			}
		}
		/* Direction control */
		xpos += dirRight ?  speed : -speed;
		ypos += dirUp    ? -speed :  speed;

        for(s32 i=0; i<state.count; i++)
        {
			xpos = state.touches[i].x - LogoWidth  / 2;
			ypos = state.touches[i].y - LogoHeight / 2;

        }


		
		/* Check bounds */
		if (xpos <= 0)
		{
			xpos     = 0;
			dirRight = true;
			col      = getRandom();
		}
		
		if (ypos <= 0)
		{
			ypos  = 0;
			dirUp = false;
			col   = getRandom();
		}
		
		if (xpos >= ScreenWidth - LogoWidth)
		{
			xpos     = ScreenWidth - LogoWidth;
			dirRight = false;
			col      = getRandom();
		}
		
		if (ypos >= ScreenHeight - LogoHeight)
		{
			ypos  = ScreenHeight - LogoHeight;
			dirUp = true;
			col   = getRandom();
		}

		Gfx::Draw(xpos, ypos, col);
		consoleUpdate(NULL);
	}

	Gfx::Close();
    
	return 0;
}
