#pragma once
#include <string>
#include <functional>
#include <filesystem>

namespace fovy {
	class Minigin final {
	private:
		double m_AccumulatedTime{};
		bool m_quit{};

	public:
		explicit Minigin(const std::filesystem::path& dataPath);
		~Minigin();
		void Run(const std::function<void()>& load);
		void RunOneFrame();

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;
	};
}
