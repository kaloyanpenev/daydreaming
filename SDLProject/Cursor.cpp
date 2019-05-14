#include "Cursor.h"


Cursor::Cursor(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h) :
	Sprite(_window, _renderer, _file, _x, _y, _w, _h)
{
	SDL_ShowCursor(0);
	SDL_WarpMouseInWindow(NULL, 1280 / 2, 720 / 2);
}


Cursor::~Cursor()
{
}

void Cursor::Draw()
{
	if (image)
	{
		if (!usingLastPos)
		{
			//if we are using the current update of the mouse motion, the position rectangle's origin is at mouseX,mouseY
			SDL_RenderCopy(renderer, image, NULL, &position);
		}
		else
		{
			//if we are using the saved last position, we have to offset the rectangle's position before rendering it
			position.x -= position.w / 2;
			position.y -= position.h / 2;
			SDL_RenderCopy(renderer, image, NULL, &position);
		}
	}
}

void Cursor::MouseLook(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION)
	{
		int MouseX, MouseY;
		SDL_GetMouseState(&MouseX, &MouseY);

		//offset position of cursor sprite so that its center is mouseX, mouseY
		position.x = MouseX - position.w / 2; 
		position.y = MouseY - position.h / 2;
		usingLastPos = false; //currently not using last stored position
	}
	else if(!usingLastPos)
	{
		//if we are not using last stored position, we have to store it(so we can use it when we need it, see line 61)
		lastPos.x = position.x - position.w / 2;
		lastPos.y = position.y - position.h / 2;
		usingLastPos = true;
	}
}
float Cursor::getAngle(int x1, int y1, int x2, int y2)
{
	//algorithm for finding angle between 2 points
	float angle = atan2(y1 - y2, x1 - x2) * (180 / PI);
	return angle;
}
float Cursor::getAngleBetweenMouseAndRect(SDL_Rect _rect)
{
	if (!usingLastPos) //if current event IS a mouse motion
	{
		//then already the position's rectangle 
		position.x = position.x + position.w / 2;
		position.y = position.y + position.h / 2;
	}
	else //if current event is not a mouse motion, we are using a stored position
	{
		//when we are using a stored position, the mousex and mousey position are saved at x0,y0 of the position rectangle
		//so we have to offset the position rectangle x+w/2 and y+h/2 to make sure mousex and mousey match the center of the rectangle
		position.x = lastPos.x + position.w / 2;
		position.y = lastPos.y + position.h / 2;
	}
	//offset origin of rectangle to the center of it
	_rect.x += _rect.w / 2;
	_rect.y += _rect.h / 2;
	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderDrawLine(renderer, _rect.x , _rect.y, position.x, position.y);
	//algorithm for finding angle between 2 points
	float angle = atan2(position.y - _rect.y, position.x - _rect.x) * (180 / PI);
	return angle;
}
void Cursor::DrawLineMouseToSprite(Sprite* sprite)
{
	//SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	///*SDL_RenderDrawLine(renderer, littleman->getPosition().x + littleman->getPosition().w / 2, littleman->getPosition().y + littleman->getPosition().h / 2, mouse->getPosition().x + mouse->getPosition().w / 2, mouse->getPosition().y + mouse->getPosition().h / 2);*/
	//SDL_RenderDrawLine(renderer, sprite->getPosition().x + rect.w/2, sprite->getPosition().y, position.x, position.y);
}
