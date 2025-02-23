#pragma once
#include "SceneManager.h"

namespace dae
{
	class GameObject;
	class Scene final
	{
		friend Scene& SceneManager::CreateScene(const std::string& name);
	public:
		void Add(std::shared_ptr<GameObject> object);
		void Remove(const std::shared_ptr<GameObject>& object);
		void RemoveAll();

		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderImgui();

		void CleanupDestroyedGameObjects();
		void Unload();
		void DestroyGameObjects();

		[[nodiscard]] bool IsBeingUnloaded() const { return m_BeingUnloaded; }

		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

	private: 
		explicit Scene(const std::string& name);

		std::string m_name;
		std::vector <std::shared_ptr<GameObject>> m_objects{};
		bool m_BeingUnloaded{ false };

		static unsigned int m_idCounter; 
	};

}
