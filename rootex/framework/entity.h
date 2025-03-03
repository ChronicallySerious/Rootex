#pragma once

#include "common/common.h"
#include "event_manager.h"

class Component;
class Scene;

typedef unsigned int ComponentID;
typedef unsigned int SceneID;

/// A collection of ECS style components that defines an ECS style entity.
class Entity
{
	EventBinder<Entity> m_Binder;

protected:
	Scene* m_Scene;
	HashMap<ComponentID, Component*> m_Components;

public:
	Entity(Scene* scene);
	Entity(Entity&) = delete;
	Entity(Entity&&) = delete;
	Entity& operator=(Entity&&) = delete;
	~Entity();

	bool onAllComponentsAdded();
	bool onAllEntitiesAdded();

	void registerComponent(Component* component);
	bool addDefaultComponent(const String& componentName);
	bool addComponent(const String& componentName, const JSON::json& componentData);
	bool removeComponent(ComponentID toRemoveComponentID, bool hardRemove = false);
	bool hasComponent(ComponentID componentID);

	/// Remove all components
	void clear();

	/// Destruct all components.
	void destroy();

	Scene* getScene() const { return m_Scene; }

	template <class ComponentType = Component>
	ComponentType* getComponent();

	template <class ComponentType = Component>
	ComponentType* getComponentFromID(ComponentID ID);

	JSON::json getJSON() const;
	const String& getName() const;
	const SceneID getID() const;
	const String& getFullName() const;
	const HashMap<ComponentID, Component*>& getAllComponents() const;

	bool call(const String& function, const Vector<Variant>& args);

	void draw();
};

template <class ComponentType>
inline ComponentType* Entity::getComponent()
{
	return getComponentFromID<ComponentType>(ComponentType::s_ID);
}

template <class ComponentType>
inline ComponentType* Entity::getComponentFromID(ComponentID ID)
{
	auto findIt = m_Components.find(ID);
	if (findIt != m_Components.end())
	{
		Component* baseTypeComponent = findIt->second;
		return dynamic_cast<ComponentType*>(baseTypeComponent);
	}

	return nullptr;
}
