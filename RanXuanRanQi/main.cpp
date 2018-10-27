#include<Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tchar.h>
#include"Render.h"
#include"sdl.h"

#undef main


extern int screen_exit;
extern int screen_keys[512];
const int SCREEN_WIDTH = 600;
const int SCREEN_HEIGHT = 600;


int main()
{
	bool quit = false;
	bool sdl_ret = sdl_init(SCREEN_WIDTH, SCREEN_HEIGHT, "huangqi");

	//Start up SDL and create window
	if (sdl_ret == false)
	{
		printf("Failed to initialize!\n");
	}
	SDL_Event e;
	Render render;
	//空格锁

	float alpha = 1;
	bool firstMouse = true;
	float c_yaw = 0.0f;
	float c_pitch = 0.0f;
	point pos = {3.5, 0, 0};
	//定义窗口名称
	TCHAR* title = _T("软件渲染器");
	//设置窗口大小并初始化
	render.Render_init(pos, SCREEN_WIDTH, SCREEN_HEIGHT, title);

	float c_lastX = SCREEN_WIDTH >> 1, c_lastY = SCREEN_HEIGHT >> 1;
	while (!quit)
	{
		while (SDL_PollEvent(&e) != 0)
		{
			if (e.type == SDL_QUIT)
			{
				quit = true;
			}
				//User presses a key
			else if (e.type == SDL_KEYDOWN)
			{
				screen_keys[e.key.keysym.scancode] = 1;
			}
			else if (e.type == SDL_KEYUP)
			{
				screen_keys[e.key.keysym.scancode] = 0;
			}
			else if (e.type == SDL_MOUSEMOTION)
			{
				if (firstMouse)
				{
					c_lastX = e.motion.x;
					c_lastY = e.motion.y;
					firstMouse = false;
				}
				float xoffset = e.motion.x - c_lastX;
				float yoffset = e.motion.y - c_lastY;
				c_lastX = e.motion.x;
				c_lastY = e.motion.y;

				xoffset *= 0.1;
				yoffset *= 0.1;

				c_yaw += xoffset;
				c_pitch += yoffset;
				if (c_pitch > 89.0f)
					c_pitch = 89.0f;
				if (c_pitch < -89.0f)
					c_pitch = -89.0f;
			}
		}


		//位置转换
		if (screen_keys[SDL_SCANCODE_W]) render.Camera.Gofront(0.1);
		if (screen_keys[SDL_SCANCODE_S]) render.Camera.Goback(0.1);
		if (screen_keys[SDL_SCANCODE_A]) render.Camera.Goleft(0.1);
		if (screen_keys[SDL_SCANCODE_D]) render.Camera.GoRight(0.1);
		if (e.type == SDL_MOUSEMOTION)
		{
			c_lastX = e.motion.x;
			c_lastY = e.motion.y;
			float xoffset = e.motion.x - c_lastX;
			float yoffset = e.motion.y - c_lastY;
			c_lastX = e.motion.x;
			c_lastY = e.motion.y;

			xoffset *= 0.1;
			yoffset *= 0.1;

			c_yaw += xoffset;
			c_pitch += yoffset;
			if (c_pitch > 89.0f)
				c_pitch = 89.0f;
			if (c_pitch < -89.0f)
				c_pitch = -89.0f;
		}
		//进行渲染器更新
		SDL_SetRenderDrawColor(gRenderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(gRenderer);
		render.renderUpdate(pos);
		render.Camera.camera_init_by_euler(c_yaw, c_pitch);
		render.draw_box(alpha);
		Sleep(0.1);
		SDL_RenderPresent(gRenderer);
	}
	sdl_close();
	return 0;
}
