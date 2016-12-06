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
	theAreaClicked = { 0, 0 };
	// Store the textures
	textureName = { "theBackground", "theSeal", "Fish", "iceBlock", "instruct", "title" };
	texturesToUse = { "Images/Background.png", "Images/Seal.png", "Images/Fish.png", "Images/Ice.png", "Images/Instructions.png", "Images/Title.png" };
	for (int tCount = 0; tCount < textureName.size(); tCount++)
	{	
		theTextureMgr->addTexture(textureName[tCount], texturesToUse[tCount]);
	}
	// Store the button textures
	btnNameList = { "exit_btn", "instructions_btn", "load_btn", "menu_btn", "play_btn", "save_btn", };
	btnTexturesToUse = { "Images/Buttons/button_exit.png", "Images/Buttons/button_instructions.png", "Images/Buttons/button_load.png", "Images/Buttons/button_menu.png", "Images/Buttons/button_play.png", "Images/Buttons/button_save.png", };
	btnPos = { { 400, 250 }, { 400, 200 }, { 400, 200 }, { 400, 500 }, { 400, 300 }, { 740, 500 } };
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
	fontsToUse = {"Fonts/ice_pixel-7.ttf" };
	for (int fonts = 0; fonts < fontList.size(); fonts++)
	{
		theFontMgr->addFont(fontList[fonts], fontsToUse[fonts], 48);
	}
	// Create text Textures
	gameTextNames = { "TitleTxt", "ThanksTxt", "ScoreTxt"};
	gameTextList = { "Arctic Crossing", "Thanks for playing!", "Score: 0" };
	for (int text = 0; text < gameTextNames.size(); text++)
	{
		theTextureMgr->addTexture(gameTextNames[text], theFontMgr->getFont("ice")->createTextTexture(theRenderer, gameTextList[text], SOLID , { 255, 255, 255, 255 }, { 0, 0, 0, 0 }));
	}
	// Load game sounds
	soundList = { "theme", "click", "pickup", "end" };
	soundTypes = { MUSIC, SFX, SFX, SFX };
	soundsToUse = { "Audio/Bit Quest.mp3", "Audio/Button.wav", "Audio/Pickup.wav", "Audio/GameOver.wav" };
	for (int sounds = 0; sounds < soundList.size(); sounds++)
	{
		theSoundMgr->add(soundList[sounds], soundsToUse[sounds], soundTypes[sounds]);
	}

	theSoundMgr->getSnd("theme")->play(-1);

	//initialise background
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
	theSeal.setSpritePos({ 80, 500 });
	theSeal.setTexture(theTextureMgr->getTexture("theSeal"));
	theSeal.setSpriteDimensions(theTextureMgr->getTexture("theSeal")->getTWidth(), theTextureMgr->getTexture("theSeal")->getTHeight());
	//initialise fish
	Fish1.setSpritePos({ 200, 400 });	Fish1.setTexture(theTextureMgr->getTexture("Fish"));	Fish1.setSpriteDimensions(theTextureMgr->getTexture("Fish")->getTWidth(), theTextureMgr->getTexture("Fish")->getTHeight());
	Fish2.setSpritePos({ 750, 400 });	Fish2.setTexture(theTextureMgr->getTexture("Fish"));	Fish2.setSpriteDimensions(theTextureMgr->getTexture("Fish")->getTWidth(), theTextureMgr->getTexture("Fish")->getTHeight());
	Fish3.setSpritePos({ 100, 675 });	Fish3.setTexture(theTextureMgr->getTexture("Fish"));	Fish3.setSpriteDimensions(theTextureMgr->getTexture("Fish")->getTWidth(), theTextureMgr->getTexture("Fish")->getTHeight());
	Fish4.setSpritePos({ 850, 675 });	Fish4.setTexture(theTextureMgr->getTexture("Fish"));	Fish4.setSpriteDimensions(theTextureMgr->getTexture("Fish")->getTWidth(), theTextureMgr->getTexture("Fish")->getTHeight());

	//initialise title and instruction panels
	instructions.setSpritePos({ 200, 500 });
	instructions.setTexture(theTextureMgr->getTexture("instruct"));
	instructions.setSpriteDimensions(theTextureMgr->getTexture("instruct")->getTWidth(), theTextureMgr->getTexture("instruct")->getTHeight());

	title.setSpritePos({ 200, 50 });
	title.setTexture(theTextureMgr->getTexture("title"));
	title.setSpriteDimensions(theTextureMgr->getTexture("title")->getTWidth(), theTextureMgr->getTexture("title")->getTHeight());
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
		//render  background, instructions and title card
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		instructions.render(theRenderer, &instructions.getSpriteDimensions(), &instructions.getSpritePos(), instructions.getSpriteRotAngle(), &instructions.getSpriteCentre(), instructions.getSpriteScale());
		title.render(theRenderer, &title.getSpriteDimensions(), &title.getSpritePos(), title.getSpriteRotAngle(), &title.getSpriteCentre(), title.getSpriteScale());
		// Render Button
		theButtonMgr->getBtn("play_btn")->render(theRenderer, &theButtonMgr->getBtn("play_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("play_btn")->getSpritePos(), theButtonMgr->getBtn("play_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 375 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
			}
	break;
	case PLAYING:
	{
		spriteBkgd.render(theRenderer, NULL, NULL, spriteBkgd.getSpriteScale());
		
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 850, 10 });
		theButtonMgr->getBtn("exit_btn")->render(theRenderer, &theButtonMgr->getBtn("exit_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("exit_btn")->getSpritePos(), theButtonMgr->getBtn("exit_btn")->getSpriteScale());
		tempTextTexture = theTextureMgr->getTexture("TitleTxt");
		pos = { 10, 10, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
		//render the score
	
		
		tempTextTexture = theTextureMgr->getTexture("ScoreTxt");
		pos = { 10, 50, tempTextTexture->getTextureRect().w, tempTextTexture->getTextureRect().h };
		tempTextTexture->renderTexture(theRenderer, tempTextTexture->getTexture(), &tempTextTexture->getTextureRect(), &pos, scale);
	


		//render the scor
		//render ice blocks
		IceA.render(theRenderer, &IceA.getSpriteDimensions(), &IceA.getSpritePos(), IceA.getSpriteRotAngle(), &IceA.getSpriteCentre(), IceA.getSpriteScale());
		IceB.render(theRenderer, &IceB.getSpriteDimensions(), &IceB.getSpritePos(), IceB.getSpriteRotAngle(), &IceB.getSpriteCentre(), IceB.getSpriteScale());
		IceC.render(theRenderer, &IceC.getSpriteDimensions(), &IceC.getSpritePos(), IceC.getSpriteRotAngle(), &IceC.getSpriteCentre(), IceC.getSpriteScale());

		// render the seal & fish
		Fish1.render(theRenderer, &Fish1.getSpriteDimensions(), &Fish1.getSpritePos(), Fish1.getSpriteRotAngle(), &Fish1.getSpriteCentre(), Fish1.getSpriteScale());
		Fish2.render(theRenderer, &Fish2.getSpriteDimensions(), &Fish2.getSpritePos(), Fish2.getSpriteRotAngle(), &Fish2.getSpriteCentre(), Fish2.getSpriteScale());
		Fish3.render(theRenderer, &Fish3.getSpriteDimensions(), &Fish3.getSpritePos(), Fish3.getSpriteRotAngle(), &Fish3.getSpriteCentre(), Fish3.getSpriteScale());
		Fish4.render(theRenderer, &Fish4.getSpriteDimensions(), &Fish4.getSpritePos(), Fish4.getSpriteRotAngle(), &Fish4.getSpriteCentre(), Fish4.getSpriteScale());
		theSeal.render(theRenderer, &theSeal.getSpriteDimensions(), &theSeal.getSpritePos(), theSeal.getSpriteRotAngle(), &theSeal.getSpriteCentre(), theSeal.getSpriteScale());
		
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

		theButtonMgr->getBtn("menu_btn")->setSpritePos({ 400, 500 });
		theButtonMgr->getBtn("menu_btn")->render(theRenderer, &theButtonMgr->getBtn("menu_btn")->getSpriteDimensions(), &theButtonMgr->getBtn("menu_btn")->getSpritePos(), theButtonMgr->getBtn("menu_btn")->getSpriteScale());
		theButtonMgr->getBtn("exit_btn")->setSpritePos({ 400, 575 });
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


			//check for collisions with pickups
			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(Fish1.getBoundingRect())))
			{
				Fish1.setActive(false);
				Score += 100;
				cout << Score << "\n";
				theSoundMgr->getSnd("pickup")->play(-1);
			}
			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(Fish2.getBoundingRect())))
			{
				Fish2.setActive(false);
				Score += 100;
				cout << Score << "\n";
				theSoundMgr->getSnd("pickup")->play(-1);
			}
			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(Fish3.getBoundingRect())))
			{
				Fish3.setActive(false);
				Score += 100;
				cout << Score << "\n";
				theSoundMgr->getSnd("pickup")->play(-1);
			}
			if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(Fish4.getBoundingRect())))
			{
				Fish4.setActive(false);
				Score += 100;
				cout << Score << "\n";
				theSoundMgr->getSnd("pickup")->play(-1);
			}
			//undate the fish
			Fish1.update(deltaTime);
			Fish2.update(deltaTime);
			Fish3.update(deltaTime);
			Fish4.update(deltaTime);

			//check if the seal is on the ice blocks
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
					onIce = true;
			}

			else if (theSeal.collidedWith(&(theSeal.getBoundingRect()), &(IceB.getBoundingRect())))
			{
					if (!iceBounce)
					{
						theSeal.setSealMotion({ 0, -130 });
					}
					else
					{
						theSeal.setSealMotion({ 0, 130 });
					}
					onIce = true;
			}
			else
			{
				onIce = false;
			}

			//check if the seal is in the water


			if ((theSeal.getSpritePos().x > 330) && (theSeal.getSpritePos().x < 800))
			{
				if (!onIce)
				{
					theGameState = END;
				}	
			}
			//update the score
			switch (Score)
			{
			case 100:
			{
						gameTextList[2] = { "Score: 100" }; break;
			}
			case 200:
			{
						gameTextList[2] = { "Score: 200" }; break;
			}
			case 300:
			{
						gameTextList[2] = { "Score: 300" }; break;
			}
			case 400:
			{
						gameTextList[2] = { "Score: 400" }; break;
			}
			}

			if (Score == 400)
			{
				//theGameState = END;
			}
		}
		break;
		case END:
		{
			theSeal.setActive(false);
			theSoundMgr->getSnd("end")->play(0);
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

				}
				break;
				case SDLK_UP:
				{

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

