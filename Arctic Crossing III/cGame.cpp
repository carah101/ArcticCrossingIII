/*
==================================================================================
cGame.cpp
==================================================================================
*/
#include "cGame.h"

cGame* cGame::pInstance = NULL;
static cTextureMgr* theTextureMgr = cTextureMgr::getInstance();
static cFontMgr* theFontMgr = cFontMgr::getInstance();
static cSoundMgr* theSoundMgr = cSoundMgr::getInstance();
static cButtonMgr* theButtonMgr = cButtonMgr::getInstance();


/*
=================================================================================
Constructor
=================================================================================
*/
cGame::cGame()
{

}
/*
=================================================================================
Singleton Design Pattern
=================================================================================
*/
cGame* cGame::getInstance()
{
	if (pInstance == NULL)
	{
		pInstance = new cGame();
	}
	return cGame::pInstance;
}


void cGame::initialise(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	// Get width and height of render context
	SDL_GetRendererOutputSize(theRenderer, &renderWidth, &renderHeight);
	this->m_lastTime = high_resolution_clock::now();
	// Clear the buffer with a black background
	SDL_SetRenderDrawColor(theRenderer, 0, 0, 0, 255);
	SDL_RenderPresent(theRenderer);
	
	theTextureMgr->setRenderer(theRenderer);
	theFontMgr->initFontLib();
	theSoundMgr->initMixer();

	// Set filename
	theFile.setFileName("Data/usermap.dat");
	// Check file is available
	if (!theFile.openFile(ios::in)) //open file for input output
	{
		cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
		fileAvailable = false;
	}
	else
	{
		cout << "File '" << theFile.getFileName() << "' opened for input!" << endl;
		fileAvailable = true;
	}

	theAreaClicked = { 0, 0 };
	// Store the textures
	textureName = { "tile1", "tile2", "tile3", "tile4", "tile5","tile6", "theBackground", "theSeal", "Fish", "iceBlock" };
	texturesToUse = {"Images/Tiles/1.png", "Images/Tiles/2.png", "Images/Tiles/3.png", "Images/Tiles/4.png", "Images/Tiles/5.png", "Images/Tiles/6.png", "Images/Background.png", "Images/Seal.png", "Images/Fish.png", "Images/Ice.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	tempTextTexture = theTextureMgr->getTexture("tile1");
	aRect = { 0, 0, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
	aColour = { 228, 213, 238, 255 };
	// Store the textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", "settings_btn" };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", "Images/Buttons/button_settings.png" };
	btnPos = { { 400, 375 }, { 400, 300 }, { 400, 300 }, { 500, 500 }, { 400, 300 }, { 740, 500 }, { 400, 300 } };
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		theTextureMgr->addTexture(btnNameList[bCount], btnTexturesToUse[bCount]);
	}
	for (int bCount = 0; bCount < btnNameList.size(); bCount++)
	{
		cButton * newBtn = new cButton();
		newBtn->setTexture(theTextureMgr->getTexture(btnNameList[bCount]));
		newBtn->setSpritePos(btnPos[bCount]);
		newBtn->setSpriteDimensions(theTextureMgr->getTexture(btnNameList[bCount])->getTWidth(), theTextureMgr->getTexture(btnNameList[bCount])->getTHeight());
		theButtonMgr->add(btnNameList[bCount], newBtn);
	}
	theGameState = MENU;
	theBtnType = EXIT;
	// Create textures for Game Dialogue (text)
	fontList = { "ice" };
	fontsToUse = {"Fonts/ice_demon.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 20);
	}
	// Create text Textures
	gameTextNames = { "TitleTxt", "FishTxt", "DragDropTxt", "ThanksTxt", "SeeYouTxt"};
	gameTextList = { "Arctic Crossing", "Get the fish!", "Don't fall in the water!", "Thanks for playing!", "Save your score?"};
	for (int text = 0; text < gameTextNames.size(); text++)
	{
		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("ice")->createTextTexture(theRenderer, gameTextList[text], SOLID, { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	}
	// Load game sounds
	soundList = { "theme", "click" };
	soundTypes = { MUSIC, SFX };
	soundsToUse = { "Audio/Bit Quest.mp3", "Audio/SFX/ClickOn.wav"};
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	spriteBkgd.setSpritePos({ 0, 0 });
	spriteBkgd.setTexture(theTextureMgr->getTexture("theBackground"));
	spriteBkgd.setSpriteDimensions(theTextureMgr->getTexture("theBackground")->getTWidth(), theTextureMgr->getTexture("theBackground")->getTHeight());

	//initialise the ice blocks
	IceA.setSpritePos({ 300, 399 });	 IceA.setTexture(theTextureMgr->getTexture("iceBlock")); 	IceA.setSpriteDimensions(theTextureMgr->getTexture("iceBlock")->getTWidth(), theTextureMgr->getTexture("iceBlock")->getTHeight());
	IceB.setSpritePos({ 450, 699 });	 IceB.setTexture(theTextureMgr->getTexture("iceBlock"));	IceB.setSpriteDimensions(theTextureMgr->getTexture("iceBlock")->getTWidth(), theTextureMgr->getTexture("iceBlock")->getTHeight());
	IceC.setSpritePos({ 600, 399 });	 IceC.setTexture(theTextureMgr->getTexture("iceBlock"));	IceC.setSpriteDimensions(theTextureMgr->getTexture("iceBlock")->getTWidth(), theTextureMgr->getTexture("iceBlock")->getTHeight());

	IceA.setSpriteTranslation({ 0, 0 });
	IceB.setSpriteTranslation({ 0, 0 });
	IceC.setSpriteTranslation({ 0, 0 });

	iceBounce = false;

	//initialise the seal
	theSeal.setSpritePos({ 60, 450 });
	theSeal.setTexture(theTextureMgr->getTexture("theSeal"));
	theSeal.setSpriteDimensions(theTextureMgr->getTexture("theSeal")->getTWidth(), theTextureMgr->getTexture("theSeal")->getTHeight());
	//initialise fish
	Fish1.setSpritePos({ 200, 400 });	Fish1.setTexture(theTextureMgr->getTexture("Fish"));	Fish1.setSpriteDimensions(theTextureMgr->getTexture("Fish")->getTWidth(), theTextureMgr->getTexture("Fish")->getTHeight());
	Fish2.setSpritePos({ 750, 400 });	Fish2.setTexture(theTextureMgr->getTexture("Fish"));	Fish2.setSpriteDimensions(theTextureMgr->getTexture("Fish")->getTWidth(), theTextureMgr->getTexture("Fish")->getTHeight());
	Fish3.setSpritePos({ 100, 675 });	Fish3.setTexture(theTextureMgr->getTexture("Fish"));	Fish3.setSpriteDimensions(theTextureMgr->getTexture("Fish")->getTWidth(), theTextureMgr->getTexture("Fish")->getTHeight());
	Fish4.setSpritePos({ 850, 675 });	Fish4.setTexture(theTextureMgr->getTexture("Fish"));	Fish4.setSpriteDimensions(theTextureMgr->getTexture("Fish")->getTWidth(), theTextureMgr->getTexture("Fish")->getTHeight());

	
	/*theTileMap.setMapStartXY({ 100, 100 });
	theTilePicker.setTileListStartXY({ 740, 100 });*/
}

void cGame::run(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	loop = true;

	while (loop)
	{
		//We get the time that passed since the last frame
		double elapsedTime = this->getElapsedSeconds();

		loop = this->getInput(loop);
		this->update(elapsedTime);
		this->render(theSDLWND, theRenderer);
	}
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer)
{
	SDL_RenderClear(theRenderer);
	switch (theGameState)
	{
	case MENU:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		// Render the Title
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		scale = { 1, 1 };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("FishTxt");
		pos = { 300, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("DragDropTxt");
		pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case PLAYING:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 850, 10 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		
		//render ice blocks
		IceA.render(theRenderer, &IceA.getSpriteDimensions(), &IceA.getSpritePos(), IceA.getSpriteRotAngle(), &IceA.getSpriteCentre(), IceA.getSpriteScale());
		IceB.render(theRenderer, &IceB.getSpriteDimensions(), &IceB.getSpritePos(), IceB.getSpriteRotAngle(), &IceB.getSpriteCentre(), IceB.getSpriteScale());
		IceC.render(theRenderer, &IceC.getSpriteDimensions(), &IceC.getSpritePos(), IceC.getSpriteRotAngle(), &IceC.getSpriteCentre(), IceC.getSpriteScale());

		// render the seal & fish
		theSeal.render(theRenderer, &theSeal.getSpriteDimensions(), &theSeal.getSpritePos(), theSeal.getSpriteRotAngle(), &theSeal.getSpriteCentre(), theSeal.getSpriteScale());
		Fish1.render(theRenderer, &Fish1.getSpriteDimensions(), &Fish1.getSpritePos(), Fish1.getSpriteRotAngle(), &Fish1.getSpriteCentre(), Fish1.getSpriteScale());
		Fish2.render(theRenderer, &Fish2.getSpriteDimensions(), &Fish2.getSpritePos(), Fish2.getSpriteRotAngle(), &Fish2.getSpriteCentre(), Fish2.getSpriteScale());
		Fish3.render(theRenderer, &Fish3.getSpriteDimensions(), &Fish3.getSpritePos(), Fish3.getSpriteRotAngle(), &Fish3.getSpriteCentre(), Fish3.getSpriteScale());
		Fish4.render(theRenderer, &Fish4.getSpriteDimensions(), &Fish4.getSpritePos(), Fish4.getSpriteRotAngle(), &Fish4.getSpriteCentre(), Fish4.getSpriteScale());

		/*
		theTileMap.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTilePicker.render(theSDLWND, theRenderer, theTextureMgr, textureName);
		theTileMap.renderGridLines(theRenderer, aRect, aColour);*/
		/*theButtonMgr->getBtn("load_btn")->setSpritePos({ 675, 10 });
		theButtonMgr->getBtn("load_btn")->render(theRenderer, &theButtonMgr->getBtn("load_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("load_btn")->getSpritePos(), theButtonMgr->getBtn("load_btn")->getSpriteScale());
		theButtonMgr->getBtn("save_btn")->setSpritePos({ 850, 10 });
		theButtonMgr->getBtn("save_btn")->render(theRenderer, &theButtonMgr->getBtn("save_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("save_btn")->getSpritePos(), theButtonMgr->getBtn("save_btn")->getSpriteScale());
		if (theTilePicker.getTilePicked() > -1)
		{
			dragTile.render(theRenderer, &dragTile.getSpriteDimensions(), &dragTile.getSpritePos(), spriteBkgd.getSpriteScale());
		}*/
		
	}
	break;
	case END:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("ThanksTxt");
		pos = { 300, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		tempTextTexture = theTextureMgr->getTexture("SeeYouTxt");
		pos = { 300, 75, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 500, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 500, 575 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
	}
	break;
	case QUIT:
	{
		loop = false;
	}
	break;
	default:
		break;
	}
	SDL_RenderPresent(theRenderer);
}

void cGame::render(SDL_Window* theSDLWND, SDL_Renderer* theRenderer, double rotAngle, SDL_Point* spriteCentre)
{

	SDL_RenderPresent(theRenderer);
}

void cGame::update()
{

}

void cGame::update(double deltaTime)
{
	// CHeck Button clicked and change state
	switch (theGameState)
	{
		case MENU:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
			theGameState = theButtonMgr->getBtn("play_btn")->update(theGameState, PLAYING, theAreaClicked);
		}
		break;
		case PLAYING:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, END, theAreaClicked);
			theGameState = theButtonMgr->getBtn("load_btn")->update(theGameState, LOADMAP, theAreaClicked);
			if (fileAvailable && theGameState == LOADMAP)
			{
				theTileMap.initialiseMapFromFile(&theFile);
				theGameState = PLAYING;
				theAreaClicked = { 0, 0 };
			}
			theGameState = theButtonMgr->getBtn("save_btn")->update(theGameState, SAVEMAP, theAreaClicked);
			if (theGameState == SAVEMAP)
			{
				// Check file is available
				if (!theFile.openFile(ios::out)) //open file for output
				{
					cout << "Could not open specified file '" << theFile.getFileName() << "'. Error " << SDL_GetError() << endl;
				}
				else
				{
					cout << "File '" << theFile.getFileName() << "' opened for output!" << endl;
					theTileMap.writeMapDataToFile(&theFile);
				}

				//theTileMap.writeMapDataToFile(&theFile);
				theGameState = PLAYING;
				theAreaClicked = { 0, 0 };
			}

			//Update ice position

			if (!iceBounce)
			{
				IceA.setSpriteTranslation({ 0,  75 });
				IceB.setSpriteTranslation({ 0, -55 });
				IceC.setSpriteTranslation({ 0,  75 });
			}
			else
			{
				IceA.setSpriteTranslation({ 0, -55 });
				IceB.setSpriteTranslation({ 0,  75 });
				IceC.setSpriteTranslation({ 0, -55 });
			}
			if (IceB.getSpritePos().y <= 400)
			{
				iceBounce = true;
			}
			if (IceB.getSpritePos().y >= 700)
			{
				iceBounce = false;
			}
				IceA.update(deltaTime);
				IceB.update(deltaTime);
				IceC.update(deltaTime);

			//update seal postion
				if (theSeal.getSpritePos().y < 350)
				{
					
				}
			theSeal.update(deltaTime);

			//check for collisions
			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(Fish1.getBoundingRect())))
			{
				Fish1.setActive(false);
				Score += 100;
			}
			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(Fish2.getBoundingRect())))
			{
				Fish2.setActive(false);
				Score += 100;
			}
			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(Fish3.getBoundingRect())))
			{
				Fish3.setActive(false);
				Score += 100;
			}
			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(Fish4.getBoundingRect())))
			{
				Fish4.setActive(false);
				Score += 100;
			}

			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(IceA.getBoundingRect())) || theSeal.collidedWith(&(theSeal.getBoundingRect()), &(IceC.getBoundingRect())))
			{
					if (iceBounce)
					{
						theSeal.setSealMotion({ 0, -130 });
					}
					else
					{
						theSeal.setSealMotion({ 0, 130 });
					}
			}

			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(IceB.getBoundingRect())))
			{
					if (!iceBounce)
					{
						theSeal.setSealMotion({ 0, -130 });
					}
					else
					{
						theSeal.setSealMotion({ 0, 130 });
					}
			}


		}
		break;
		case END:
		{
			theGameState = theButtonMgr->getBtn("exit_btn")->update(theGameState, QUIT, theAreaClicked);
			theGameState = theButtonMgr->getBtn("menu_btn")->update(theGameState, MENU, theAreaClicked);
		}
		break;
		case QUIT:
		{
		}
		break;
		default:
			break;
	}
}

bool cGame::getInput(bool theLoop)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			theLoop = false;
		}

		switch (event.type)
		{
			case SDL_MOUSEBUTTONDOWN:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					theAreaClicked = { event.motion.x, event.motion.y };
					if (theGameState == PLAYING)
					{
						theTilePicker.update(theAreaClicked);
						if (theTilePicker.getTilePicked() > -1)
						{
							dragTile.setSpritePos(theAreaClicked);
							dragTile.setTexture(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1]));
							dragTile.setSpriteDimensions(theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTWidth(), theTextureMgr->getTexture(textureName[theTilePicker.getTilePicked() - 1])->getTHeight());
						}
					}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEBUTTONUP:
				switch (event.button.button)
				{
				case SDL_BUTTON_LEFT:
				{
					if (theGameState == PLAYING)
					{
						theAreaClicked = { event.motion.x, event.motion.y };
						theTileMap.update(theAreaClicked, theTilePicker.getTilePicked());
						theTilePicker.setTilePicked(-1);
					}
				}
				break;
				case SDL_BUTTON_RIGHT:
					break;
				default:
					break;
				}
				break;
			case SDL_MOUSEMOTION:
			{
				//dragTile.setSpritePos({ event.motion.x, event.motion.y });
			}
			break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
				case SDLK_ESCAPE:
					theLoop = false;
					break;
				case SDLK_DOWN:
				{
					cout << "down ";
				}
				break;
				case SDLK_UP:
				{
					cout << "up ";
				}
				break;
				case SDLK_RIGHT:
				{
					theSeal.setSealMotion({ 200, 0 });
					cout << "right ";
				}
				break;

				case SDLK_LEFT:
				{
					theSeal.setSealMotion({ -200, 0 });
					cout << "left ";
				}
				break;
				case SDLK_SPACE:
				{
				}
				break;
				default:
					break;
				}
	
			default:
				break;
		}

	}
	return theLoop;
}

double cGame::getElapsedSeconds()
{
	this->m_CurrentTime = high_resolution_clock::now();
	this->deltaTime = (this->m_CurrentTime - this->m_lastTime);
	this->m_lastTime = this->m_CurrentTime;
	return deltaTime.count();
}

void cGame::cleanUp(SDL_Window* theSDLWND)
{
	// Delete our OpengL context
	SDL_GL_DeleteContext(theSDLWND);

	// Destroy the window
	SDL_DestroyWindow(theSDLWND);

	// Quit IMG system
	IMG_Quit();

	// Shutdown SDL 2
	SDL_Quit();
}

