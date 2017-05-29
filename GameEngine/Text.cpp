#include "Text.h"


Text::Text(void) : Image()
{
	file = NULL;                        // font texture
    graphics = NULL;                    // pointer to graphics
    color = graphicsNS::WHITE;          // default to white font
    backColor = TRANSCOLOR;             // default to transparent (no) fill
    align = textNS::LEFT;
    width = textNS::GRID_WIDTH - 3;     // -2 for transparent border and -1 for divider line
    height = textNS::GRID_HEIGHT - 3;
    fontHeight = textNS::FONT_HEIGHT;
    spriteData.width = 0;
    spriteData.height = 0;
    spriteData.rect.bottom = 1;         // rectangle to select parts of an image
    spriteData.rect.right = 1;
    for(int row=0; row<textNS::ROWS; row++)         // for each row of characters in font
    {
        for(int col=0; col<textNS::COLUMNS; col++)  // for each col of characters in font
        {
            fontData[row][col].left = MAXINT;
            fontData[row][col].right = 0;
        }
    }
    proportional = false;
    proportionalSpacing = textNS::PROPORTIONAL_SPACING;
    underline = false;
    bold = false;
    tabSize = textNS::TAB_SIZE;
}


Text::~Text(void)
{
}

//////////////////////////////////////////////////////////////////////////////
//initialize text
//define font image's each character left edge and right edge
//post:	return true if successfully, false otherwise
//fontData array is stored each character's left edge and right edge
//////////////////////////////////////////////////////////////////////////////
bool Text::initialize(Graphics *g, const char *file)
{
	try
	{
		graphics = g;	//pointer to graphics object

		//load font texture and check location (pixels) to specify each character's accurate position
		//load font texture system memory (can lock)
		UINT w,h;
		HRESULT result = graphics->loadTextureSystemMem(file, TRANSCOLOR, w, h, textureData);

		if(FAILED(result))
		{
			SAFE_RELEASE(textureData);
			return false;
		}

		//////////////////////////////////////////////////////////////////////////////
		//lock font texture to access pixel data
		//textureData.width and textureData.height is stored
		//all of font texture size
		//each character has frame that width 1 pixel
		//there are characters ROWS * COLS
		//lock font texture (need to access pixels data
		//////////////////////////////////////////////////////////////////////////////
		D3DLOCKED_RECT rect;

		result = textureData->LockRect(0, &rect, NULL, D3DLOCK_READONLY);
		//if can not lock
		if(FAILED(result))
		{
			SAFE_RELEASE(textureData);
			return false;
		}
		
		//////////////////////////////////////////////////////////////////////////////
		//each character process unit pixel and specify left edge and right edge
		//fontData array stored left edge and right edge
		//if specified each character's edge, unlock texture
		//finally, reload texture call initilize(), this is to render character
		//so, load to textureManager
		//////////////////////////////////////////////////////////////////////////////
		//process each line in font texture
		for(DWORD row = 0; row < textNS::ROWS; ++row)
		{
			//process each column in font texture
			for(DWORD col = 0; col < textNS::COLUMNS; ++col)
			{
				fontData[row][col].left = MAXINT;	//initialize fontData
				fontData[row][col].right = 0;

				//process one character
				//for y = top edge pixel; y <= bottom edge pixel; ++y
				for(DWORD y = row * textNS::GRID_HEIGHT + 1; y < (row + 1) * textNS::GRID_HEIGHT - 1; ++y)
				{
					//get pointer to head of scan line in texture
					DWORD* pBits = (DWORD*)((BYTE*)rect.pBits + y * rect.Pitch);
					//process this line
					for(DWORD x = col * textNS::GRID_WIDTH + 1; x < (col + 1) * textNS::GRID_WIDTH - 1; ++x)
					{
						//get this pixel
						DWORD dwPixel = pBits[x];
						//arpha is not transeparated
						if((dwPixel & 0xff000000) != 0x00)
						{
							//this pixel is more left
							if(x < fontData[row][col].left)
							{
								//store as left edge
								fontData[row][col].left = x;
							}
							//this pixel is more right
							if(x > fontData[row][col].right)
							{
								//store as right edge
								fontData[row][col].right;
							}
						}
					}
				}
			}
		}
		//when finished process texture, unlock
		textureData->UnlockRect(0);
		//release font texture
		SAFE_RELEASE(textureData);

		//////////////////////////////////////////////////////////////////////////////
		//load to textureManager to draw font image
		//////////////////////////////////////////////////////////////////////////////
		if(!fontTexture.initialize(graphics, file))
			return false;	//error of loading font texture

		//prepare font image
		if(!Image::initialize(graphics, textNS::FONT_WIDTH, textNS::FONT_HEIGHT, 0, &fontTexture))
			return false;	//failed
	}
	catch(...)
	{
		return false;
	}
	return true;	//successfully
}

//////////////////////////////////////////////////////////////////////////////
//output string location X,Y
//this function process proportional space and escape sequence
//pre:	spriteBegin()
//post:	spriteEnd()
//////////////////////////////////////////////////////////////////////////////
void Text::print(const std::string &str, int x, int y)
{
	UCHAR ch = 0;
	UCHAR chN = 0;	//character index that location in texture(left top 0)
	std::string str2;
	width = textNS::FONT_WIDTH;
	int scaledWidth = static_cast<int>(textNS::FONT_WIDTH * spriteData.scale);	//width of character that was scaled
	float saveY = 0;
	int tabX = 0, tabW = 0;
	spriteData.x = (float)x;
	spriteData.y = (float)y;
	doAlign(str);
	//get all characters
	for(UINT i = 0; i < str.length(); ++i)
	{
		ch = str.at(i);

		//renderable character
		if(ch > textNS::MIN_CHAR && ch <=textNS::MAX_CHAR)
		{
			chN = ch - textNS::MIN_CHAR;	//MIN_CHAR is index 0

			//////////////////////////////////////////////////////////////////////////////
			//spriteData.rect use as rectangle of draw area in texture
			//its top edge is
			//		character index(chN) / columns of texture(text::NS::COLUMNS) * height of line(textNS::GRID_HEIGHT) + 1
			//its bottom edge is top + font height
			//////////////////////////////////////////////////////////////////////////////
			spriteData.rect.top = chN / textNS::COLUMNS * textNS::GRID_HEIGHT + 1;
			spriteData.rect.bottom = spriteData.rect.top + textNS::FONT_HEIGHT;

			//////////////////////////////////////////////////////////////////////////////
			//if proportional output is effective,
			//spriteData.rect.left and right are loaded from fontData array that store data prefare
			//if character do not use width of font,
			//proportionalSpacing add width, make space between characters that render proportional
			//////////////////////////////////////////////////////////////////////////////
			if(proportional)
			{
				spriteData.rect.left = fontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].left;
				//DirectX needs right edge + 1
				spriteData.rect.right = fontData[chN / textNS::COLUMNS][chN % textNS::COLUMNS].right + 1;

				width = spriteData.rect.right - spriteData.rect.left;
				
				//if use width of font, do not make space
				if(width >= textNS::FONT_WIDTH)
				{
					width = textNS::FONT_WIDTH;	//limit width
					spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
					spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
				}
				//if do not ise width of font, make spece
				else
				{
					width += proportionalSpacing;
				}
				scaledWidth = static_cast<int>(width * spriteData.scale);
				drawChar(ch);
			}
			//////////////////////////////////////////////////////////////////////////////
			//if proportional output is not effective,
			//spriteData.rect.left is
			//		character index(chN) % columns of texture(textNS::COLUMNS) * width of one character(textNS::GRID_WIDTH) + 1
			//spriteData.rect.right is
			//		left edge + width of one character that do not contain frame width
			//rendering character is done by drawChar(ch)
			//now location in screen X add width of character size that scaled
			//////////////////////////////////////////////////////////////////////////////
			else
			{
				width = textNS::FONT_WIDTH;
				spriteData.rect.left = chN % textNS::COLUMNS * textNS::GRID_WIDTH + 1;
				spriteData.rect.right = spriteData.rect.left + textNS::FONT_WIDTH;
				drawChar(ch);
			}
			spriteData.x += scaledWidth;
		}
		//unrenderable character
		else
		{
			//check this character is escape sequence
			switch (ch)
			{
			case ' ':	//space
				//proportional space is effective
				if(proportional)
				{
					//calculate poroportional space width and scaledWidth
					width = textNS::FONT_WIDTH / 2;
					scaledWidth = static_cast<int>(width * spriteData.scale);
				}
				//draw
				drawChar(' ');
				//add location in screen X
				spriteData.x += scaledWidth;
				break;

				//new line is down a line
				//and set left edge to start positoin of X
			case '\n':	//new line
				spriteData.x = (float)x;
				spriteData.y = static_cast<int>(height * spriteData.scale);
				saveY = spriteData.y;
				str2 = str.substr(i, str.length());
				doAlign(str2);
				spriteData.y = saveY;
				break;

			case '\r':	//return start position of X
				spriteData.x = (float)x;
				str2 = str.substr(i, str.length());
				doAlign(str2);
				break;

			case '\t':	//tab
				width = textNS::FONT_WIDTH;
				scaledWidth = static_cast<int>(width * spriteData.scale);
				tabX = static_cast<int>(spriteData.x) / (scaledWidth * tabSize);
				tabX = (tabX + 1) * scaledWidth * tabSize;
				tabW = tabX - static_cast<int>(spriteData.x);
				while (tabW > 0)
				{
					if(tabW >= scaledWidth)
					{
						drawChar(' ');
						spriteData.x += scaledWidth;
					}
					else
					{
						//set tab position processing character fraction
						width = tabW;
						drawChar(' ');
						spriteData.x = tabW;
					}
					tabW -= scaledWidth;
				}
				break;

			case '\b':	//back space
				spriteData.x -= scaledWidth;
				//do not erace character
				if(spriteData.x < 0)
					spriteData.x = 0;
				break;

			case '\v':	//vertical tab
				//set location in screen Y down height of character
				spriteData.y += static_cast<int>(height * spriteData.scale);
				break;

			case 0x01:	//font signature
				spriteData.rect.top = 1;
				spriteData.rect.bottom = 1 + textNS::FONT_HEIGHT;
				spriteData.rect.left = 1;
				spriteData.rect.right = 1 + textNS::FONT_WIDTH;
				draw(spriteData);
				spriteData.x += scaledWidth;
				break;
			}
		}
	}
}

//////////////////////////////////////////////////////////////////////////////
//set spriteData.x, spriteData.y by now string and location
//default is LEFT
//////////////////////////////////////////////////////////////////////////////
void Text::doAlign(const std::string &str)
{
	if(spriteData.texture == NULL)	//if there is not texture
		return;

	UINT w, h;
	switch (align)
	{
	case textNS::LEFT:
		break;

		//X, Y is right edge
	case textNS::RIGHT:
		getWidthHeight(str, w, h);
		spriteData.x -= w;
		break;

		//X is center Y is top edge
	case textNS::CENTER:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		break;


		//X is center Y is center
	case textNS::CENTER_MIDDLE:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		spriteData.y -= h / 2;
		break;

		//X is center T is bottom edge
	case textNS::CENTER_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.x -= w / 2;
		spriteData.y -= h;
		break;

		//X is left edge Y is bottom edge
	case textNS::LEFT_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.y -= h;
		break;

		//X is right edge Y is bottom edge
	case textNS::RIGHT_BOTTOM:
		getWidthHeight(str, w, h);
		spriteData.x -= w;
		spriteData.y -= h;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////////
//get string width and height that current font size
//do not render string
//////////////////////////////////////////////////////////////////////////////
void Text::getWidthHeight(const std::string &str, UINT &w, UINT &h)
{
	if(spriteData.texture == NULL)	//there is not texture
		return;
}