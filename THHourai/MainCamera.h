#pragma once

namespace Touhou
{
	class MainCamera : public GameObject
	{
		static std::mutex locker;

	private:
		MainCamera();

	public:
		static RefPtr<MainCamera> GetInstance();
	};
}