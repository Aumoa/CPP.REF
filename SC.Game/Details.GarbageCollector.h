#pragma once

namespace SC::Game::Details
{
	class GarbageCollector
	{
		std::mutex lock;
		std::queue<object> garbages[2];

	public:
		void Add( const object& garbage );
		void Collect();
		void CollectAll();
	};
}