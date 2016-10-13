#ifndef INCLUDE_ENGINE
#define INCLUDE_ENGINE

class Engine : public CoreComponent
{
private:
	bool mRunning;
	static Engine* mpInstance;
	Engine();
	~Engine() {};
public:
	HRESULT run(TIME elapsed) override;
	HRESULT restore();
	HRESULT release() override { mRunning = false; delete this; };

	static Engine* get() { if (!mpInstance) { mpInstance = new Engine(); } };
};

#endif