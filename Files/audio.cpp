#include "audio.h"


bool Audio::initialize()
{
	 if( SDL_Init(SDL_INIT_AUDIO ) < 0 ) 
	 { 
		 LogCritical("SDL could not initialize!");
		 return false;
	 }

	if( Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) < 0 )
	{ 
		LogError(
			"SDL_mixer could not initialize!SDL_mixer Error : %s\n",
			Mix_GetError());
		return false;
	}

	std::regex	regex("^(.*).\\.(wav|mp3|ogg)$", std::regex_constants::icase);

	songs = new ResourceManager<Mix_Music*>(&Mix_LoadMUS, &Mix_FreeMusic);
	songs->setName("Songs");
	songs->loadMatchingFiles("./Assets/Songs/", regex);

	effects = new ResourceManager<Mix_Chunk*>([](const char* file) {return Mix_LoadWAV_RW(SDL_RWFromFile(file, "rb"), 1); }, &Mix_FreeChunk);
	effects->setName("Effects");
	effects->loadMatchingFiles("./Assets/Effects/", regex);


	this->setVolume(Config::audio.volume);

	
	 return true;
}

void Audio::shutdown()
{
	delete(songs);
	delete(effects);
	Mix_Quit();
}


void Audio::playEffect(std::string fileName)
{
	Mix_Chunk* tmp = effects->get(fileName);
	if (!tmp) return;
	Mix_PlayChannel(-1,tmp, 0);
}

bool Audio::playSong(std::string fileName)
{
	Mix_Music* tmp = songs->get(fileName);
	if (!tmp){
		return false;
	}
	else{
		Mix_PlayMusic(tmp, -1);
		return true;
	}
	
}

void Audio::stopMusic(){
	Mix_HaltMusic();
}

void Audio::pauseMusic(){
	Mix_PauseMusic();
}

void Audio::resumeMusic(){
	Mix_ResumeMusic();
}

void Audio::setVolume(int volume){
	if (volume > MIX_MAX_VOLUME) volume = MIX_MAX_VOLUME;
	if (volume < 0) volume = 0;

	Config::audio.volume = volume;
	Mix_VolumeMusic(volume);
	Mix_Volume(-1, volume);
}

int Audio::getVolume() const{
	return Mix_Volume(-1, -1);
}

