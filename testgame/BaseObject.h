#pragma once
#ifndef BASIC_OBJECT_H_
#define BASIC_OBJECT_H_

#include "CommonFunc.h"

class BaseObject
{
public:
	BaseObject();
	~BaseObject();
	virtual bool LoadImageFile(std::string path, SDL_Renderer* screen);
	void Render(SDL_Renderer* des, const SDL_Rect* clip = NULL);

	void Free();
	
	void SetRect(const int& x, const int& y) { rect_.x = x, rect_.y = y; };//Cai dat kich thuoc cho rect
	SDL_Rect GetRect() const { return rect_; } // Lay kich thuoc anh ra
	SDL_Texture* GetObject() { return p_object_; } //Lay Object ra
protected:
	SDL_Texture* p_object_; // Bien luu hinh anh
	SDL_Rect rect_;    //Bien dung de luu kich thuoc anh
};


#endif