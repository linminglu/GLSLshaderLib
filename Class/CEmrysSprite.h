//
//  CEmrysSprite.h
//  TestCC2
//
//  Created by linminglu on 14-3-27.
//
//

#ifndef __TestCC2__CEmrysSprite__
#define __TestCC2__CEmrysSprite__

#include <iostream>
#include "cocos2d.h"
using namespace std;
using namespace cocos2d;
class CEmrysSprite :public CCSprite
{
public:
    CEmrysSprite();
    ~CEmrysSprite();
    void initHSL();
	void drawHSL();
	void setHSL(float h, float s, float l);
	void setH(float h);
	void setS(float s);
	void setL(float l);
	float getH(void){ return m_color_h;}
	float getS(void){ return m_color_s;}
	float getL(void){ return m_color_l;}
    void updateBlendFunc();
    void setTexture(CCTexture2D *texture);
    void draw();
    void setUseHsl(bool flag);
private:
   	bool m_use_hsl;
	GLfloat m_color_h;
	GLfloat m_color_s;
	GLfloat m_color_l;
    GLfloat m_color_a;
    GLuint hLocation;
    GLuint sLocation;
    GLuint lLocation;
    GLuint rLocation;
    GLuint gLocation;
    GLuint bLocation;
    GLuint aLocation;
private:
    CCGLProgram* pProgram;
};

#endif /* defined(__TestCC2__CEmrysSprite__) */
