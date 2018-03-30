#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include"Render.h"
extern int screen_exit;
extern int screen_keys[512];
int main()
{

	Render render;
	int states[] = { RENDER_STATE_TEXTURE, RENDER_STATE_COLOR, RENDER_STATE_WIREFRAME };
	int indicator = 0;
	int kbhit = 0;
	float alpha = 1;
	float pos = 3.5;

	TCHAR *title = _T("Mini3d (software render tutorial) - ")
		_T("Left/Right: rotation, Up/Down: forward/backward, Space: switch state");

	if (render.screen_init(800, 600, title))
		return -1;

	render.Render_init();


	while (screen_exit == 0 && screen_keys[VK_ESCAPE] == 0) {
		render.screen_dispatch();
		render.mDevice.device_clear( 1);
		render.camera_at_zero( pos, 0, 0);

		if (screen_keys[VK_UP]) pos -= 0.01f;
		if (screen_keys[VK_DOWN]) pos += 0.01f;
		if (screen_keys[VK_LEFT]) alpha += 0.01f;
		if (screen_keys[VK_RIGHT]) alpha -= 0.01f;

		if (screen_keys[VK_SPACE]) {
			if (kbhit == 0) {
				kbhit = 1;
				if (++indicator >= 3) indicator = 0;
				render.mDevice.render_state = states[indicator];
			}
		}
		else {
			kbhit = 0;
		}

		render.draw_box(alpha);
		render.screen_update();
		Sleep(1);
	}
	return 0;
}
