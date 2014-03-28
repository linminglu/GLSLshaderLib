//
//  CEmrysSprite.cpp
//  TestCC2
//
//  Created by linminglu on 14-3-27.
//
//

#include "CEmrysSprite.h"
CEmrysSprite::CEmrysSprite()
{
    m_use_hsl=false;

}
CEmrysSprite::~CEmrysSprite()
{
   // CC_SAFE_RELEASE(pProgram);
}
void CEmrysSprite::setTexture(CCTexture2D *texture)
{
    CCSprite::setTexture(texture);
    if(this->m_use_hsl)
    {
        m_use_hsl = false;
        initHSL();
    }
}
void CEmrysSprite::draw()
{
    if(m_use_hsl)
    {
        drawHSL();
        return;
    }
    CCSprite::draw();
}
void CEmrysSprite::updateBlendFunc()
{
    CCSprite::updateBlendFunc();
    if(m_use_hsl)
    {
        ccBlendFunc blend = getBlendFunc();
        if(m_color_l > 0)
            blend.src = GL_SRC_ALPHA;
        else
            blend.src = GL_ONE;
        blend.dst = GL_ONE_MINUS_SRC_ALPHA;
        setBlendFunc(blend);
    }
}
void CEmrysSprite::setUseHsl(bool flag)
{
    m_use_hsl=flag;
    initHSL();
}
void CEmrysSprite::initHSL()
{
	if(m_use_hsl==true)
	{
		GLchar * fragSource = (GLchar*) CCString::createWithContentsOfFile("jingling.fsh")->getCString();
        pProgram = new CCGLProgram();
        pProgram->initWithVertexShaderByteArray(ccPositionTextureColor_vert, fragSource);
        setShaderProgram(pProgram);
        pProgram->release();
        getShaderProgram()->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
        getShaderProgram()->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
        getShaderProgram()->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
        getShaderProgram()->addAttribute("a_position", 0);
        CHECK_GL_ERROR_DEBUG();
        
        getShaderProgram()->link();
        
        CHECK_GL_ERROR_DEBUG();
        getShaderProgram()->updateUniforms();
        
        CHECK_GL_ERROR_DEBUG();
        hLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "AddHue");
		sLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "AddSat");
		lLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "AddLig");
		rLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "AddRed");
		gLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "AddGreen");
		bLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "AddBlue");
		aLocation = glGetUniformLocation( getShaderProgram()->getProgram(), "AddAlpha");
	}
}
void CEmrysSprite::drawHSL()
{
    CC_PROFILER_START_CATEGORY(kCCProfilerCategorySprite, "CCSprite - draw");
    
    CCAssert(!m_pobBatchNode, "If CCSprite is being rendered by CCSpriteBatchNode, CCSprite#draw SHOULD NOT be called");
    
    CC_NODE_DRAW_SETUP();
    
    ccBlendFunc blend = getBlendFunc();
    ccGLBlendFunc(blend.src, blend.dst);
    
    getShaderProgram()->use();
    getShaderProgram()->setUniformsForBuiltins();
	getShaderProgram()->setUniformLocationWith1f(hLocation, m_color_h);
	getShaderProgram()->setUniformLocationWith1f(sLocation, m_color_s);
	getShaderProgram()->setUniformLocationWith1f(lLocation, m_color_l);
	getShaderProgram()->setUniformLocationWith1f(rLocation, _displayedColor.r/255.f);
	getShaderProgram()->setUniformLocationWith1f(gLocation, _displayedColor.g/255.f);
	getShaderProgram()->setUniformLocationWith1f(bLocation, _displayedColor.b/255.f);
	m_color_a = this->getOpacity() / 255.f;
	getShaderProgram()->setUniformLocationWith1f(aLocation, m_color_a);
    
    ccGLBindTexture2D( getTexture()->getName());
    ccGLEnableVertexAttribs(kCCVertexAttribFlag_PosColorTex );
    
#define kQuadSize sizeof(m_sQuad.bl)
    long offset = (long)&m_sQuad;
    
    // vertex
    int diff = offsetof( ccV3F_C4B_T2F, vertices);
    glVertexAttribPointer(kCCVertexAttrib_Position, 3, GL_FLOAT, GL_FALSE, kQuadSize, (void*) (offset + diff));
    
    // texCoods
    diff = offsetof( ccV3F_C4B_T2F, texCoords);
    glVertexAttribPointer(kCCVertexAttrib_TexCoords, 2, GL_FLOAT, GL_FALSE, kQuadSize, (void*)(offset + diff));
    
    // color
    diff = offsetof( ccV3F_C4B_T2F, colors);
    glVertexAttribPointer(kCCVertexAttrib_Color, 4, GL_UNSIGNED_BYTE, GL_TRUE, kQuadSize, (void*)(offset + diff));
    
    
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    
    CHECK_GL_ERROR_DEBUG();
    CC_INCREMENT_GL_DRAWS(1);
    
    CC_PROFILER_STOP_CATEGORY(kCCProfilerCategorySprite, "CEmrysSprite - drawHSL");
}
void CEmrysSprite::setHSL(float h, float s, float l)
{
    if(!m_use_hsl)
    {
        m_use_hsl=true;
        initHSL();
    }
	setH(h);
	setS(s);
	setL(l);
}
void CEmrysSprite::setH(float h)
{
    if(!m_use_hsl)
    {
        m_use_hsl=true;
        initHSL();
    }
	m_color_h = h;
}
void CEmrysSprite::setS(float s)
{
    if(!m_use_hsl)
    {
        m_use_hsl=true;
        initHSL();
    }
	m_color_s = s;
}
void CEmrysSprite::setL(float l)
{
    if(!m_use_hsl)
    {
        m_use_hsl=true;
        initHSL();
    }
	m_color_l = l;
	ccBlendFunc blend = getBlendFunc();
	if(m_color_l > 0)
		blend.src = GL_SRC_ALPHA;
	else
		blend.src = GL_ONE;
	blend.dst = GL_ONE_MINUS_SRC_ALPHA;
	setBlendFunc(blend);
}