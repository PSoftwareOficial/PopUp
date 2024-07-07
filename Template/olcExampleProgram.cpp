#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include "PopUp.h"



class Game : public olc::PixelGameEngine
{
public:
	Game()
	{
		sAppName = "Breakout Clone";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		selectFiles = std::make_shared<InputDouble>();
		selectFiles->SetTitle("Double Value");
		popUp = std::make_unique<PopUp::System>(this, selectFiles);
		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		Clear(olc::BLUE);
		if (popUp->GetStatus() == PopUp::ACTIVE) {
			popUp->Update();
		}
		else {
			DrawString({ 10, 10 }, selectFiles->result, olc::BLACK);
		}
		return true;
	}

private:
	std::unique_ptr<PopUp::System> popUp;
	std::shared_ptr<InputDouble> selectFiles;
};




int main()
{
	Game demo;
	if (demo.Construct(500,500, 1, 1))
		demo.Start();

	return 0;
}
