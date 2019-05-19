#include "Cursor.h"


Cursor::Cursor(SDL_Window* _window, SDL_Renderer* _renderer, const char* _file, int _x, int _y, int _w, int _h) :
	Sprite(_window, _renderer, _file, _x, _y, _w, _h)
{
	m_MouseX = _x;
	m_MouseY = _y;
	SDL_ShowCursor(0);
	m_usingLastPos = false;
	//SDL_WarpMouseInWindow(NULL, 1280 / 2, 720 / 2);
}


Cursor::~Cursor()
{
}

void Cursor::Draw()
{
	if (texture)
	{
		if (m_usingLastPos)
		{
			//if we are using the saved last position, we have to offset the rectangle's position before rendering it up and left
			//this is because what used to be the center is now the origin(topleft) of the new rectangle
			position.x -= position.w / 2;
			position.y -= position.h / 2;
			SDL_RenderCopy(renderer, texture, NULL, &position);
		}
		else
		{
			//if we are using the current update of the mouse motion, the position rectangle's origin is already at the correct position
			SDL_RenderCopy(renderer, texture, NULL, &position);
		}
	}
}

void Cursor::MouseLook(SDL_Event* e)
{
	if (e->type == SDL_MOUSEMOTION)
	{
		SDL_GetMouseState(&m_MouseX, &m_MouseY);

		//offset position of cursor sprite so that its center is mouseX, mouseY
		position.x = m_MouseX - position.w / 2; 
		position.y = m_MouseY - position.h / 2;
		m_usingLastPos = false; //currently not using last stored position
	}
	else if(!m_usingLastPos)
	{
		//if we are not using last stored position, we have to store it(so we can use it when we need it, see line 61)
		//we subtract w/2 and h/2 to make sure our lastpos point is a half 
		m_lastPos.x = position.x;
		m_lastPos.y = position.y;
		m_usingLastPos = true;
	}
}
float Cursor::AngleBetweenMouseAndRect(SDL_Rect _rect)
{
	if (m_usingLastPos) //if current event is not a mouse motion, we are using a stored position
	{
		//assign position to the saved position(bear in mind here lastpos.x is the center of the position rectangle,
		//so we our new rectangle starts from the center of the previous one - we will need to offset it when rendering
		//however we need this exact position for our angle calculation
		position.x = m_lastPos.x;
		position.y = m_lastPos.y;
	}
	else //if current event IS a mouse motion
	{
		//then already x and y point to the center of the rectangle - 
		//however we need x and y to point to the origin of the triangle for our angle calculation
		//so we offset it positively(move it down and right)
		position.x = position.x + position.w / 2;
		position.y = position.y + position.h / 2;
	}
	//offset origin of rectangle to the center of it
	_rect.x += _rect.w / 2;
	_rect.y += _rect.h / 2;
	//algorithm for finding angle between 2 points
	double angle = atan2(position.y - _rect.y, position.x - _rect.x) * (180 / PI);
	return float(angle);
}
void Cursor::DrawLineMouseToSprite(Sprite* _sprite)
{
	int objX = _sprite->getObject().x + _sprite->getObject().w/2;
	int objY = _sprite->getObject().y + _sprite->getObject().h/2;
	SDL_SetRenderDrawColor(renderer, 255, 120, 120, 150);
	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_RenderDrawLine(renderer, m_MouseX, m_MouseY, objX, objY);
}
