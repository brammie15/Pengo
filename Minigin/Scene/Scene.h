#pragma once
#include <functional>

#include "Event.h"
#include "SceneManager.h"

namespace fovy
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(const std::shared_ptr<GameObject>& object);
		void RemoveAll();

		void Load();

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderImgui();

		void CleanupDestroyedGameObjects();
		void Unload();
		void DestroyGameObjects();

		[[nodiscard]] bool IsBeingUnloaded() const { return m_BeingUnloaded; }

		void SetRegisterBindings(std::function<void()> registerBindings) {
			m_registerBindings = std::move(registerBindings);
		}
		void SetUnregisterBindings(std::function<void()> unregisterBindings) {
			m_unregisterBindings = std::move(unregisterBindings);
		}
		void UnloadBindings() {
			if (m_unregisterBindings) {
				m_unregisterBindings();
			}
		}
		void LoadBindings() {
			OnSceneLoaded.Invoke();
			if (m_registerBindings) {
				m_registerBindings();
			}
		}

		[[nodiscard]] const std::string& GetName() const { return m_name; }
		[[nodiscard]] unsigned int GetId() const { return m_idCounter++; }

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		Event<> OnSceneLoaded;


	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector <std::shared_ptr<GameObject>> m_objects{};
		std::vector<std::shared_ptr<GameObject>> m_pendingAdditions{};
		bool m_BeingUnloaded{ false };

		static unsigned int m_idCounter;

		bool m_renderImgui{ false };

		//Imgui vars
		bool m_ShowDemoWindow{ false };

		std::function<void()> m_registerBindings;
		std::function<void()> m_unregisterBindings;
	};

}
