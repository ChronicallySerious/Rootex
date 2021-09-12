#include "entity.h"

#include "event_manager.h"
#include "framework/ecs_factory.h"
#include "framework/component.h"
#include "framework/system.h"
#include "framework/systems/script_system.h"
#include "resource_loader.h"

Entity::Entity(Scene* scene)
    : m_Scene(scene)
{
}

Entity::~Entity()
{
	destroy();
}

JSON::json Entity::getJSON() const
{
	JSON::json j;
	j["components"] = {};
	for (auto&& [componentID, component] : m_Components)
	{
		j["components"][component->getName()] = component->getJSON();
	}
	return j;
}

bool Entity::onAllComponentsAdded()
{
	bool status = true;
	for (auto& component : m_Components)
	{
		status = status && component.second->setup();
	}
	return status;
}

bool Entity::onAllEntitiesAdded()
{
	bool status = true;

	for (auto& component : m_Components)
	{
		status = status && component.second->setupEntities();
	}

	return status;
}

void Entity::clear()
{
	for (auto& [id, component] : m_Components)
	{
		ECSFactory::RemoveComponent(*this, id);
	}
	m_Components.clear();
}

void Entity::destroy()
{
	call("destroy", { this });

	for (auto& [componentID, component] : m_Components)
	{
		ECSFactory::RemoveComponent(*this, componentID);
	}
	m_Components.clear();
}

bool Entity::addDefaultComponent(const String& componentName)
{
	return ECSFactory::AddDefaultComponent(*this, ECSFactory::GetComponentIDByName(componentName));
}

bool Entity::addComponent(const String& componentName, const JSON::json& componentData)
{
	return ECSFactory::AddComponent(*this, ECSFactory::GetComponentIDByName(componentName), componentData);
}

void Entity::registerComponent(Component* component)
{
	m_Components[component->getComponentID()] = component;
}

bool Entity::removeComponent(ComponentID toRemoveComponentID, bool hardRemove)
{
	Component* toRemoveComponent = getComponentFromID(toRemoveComponentID);
	if (!hardRemove)
	{
		for (auto& [componentID, component] : m_Components)
		{
			for (auto& dependency : component->getDependencies())
			{
				if (dependency->getID() == toRemoveComponentID)
				{
					WARN("Entity has other components depending on the to-be-removed component (" + toRemoveComponent->getName() + "). Component deletion was denied");
					return false;
				}
			}
		}
	}

	ECSFactory::RemoveComponent(*this, toRemoveComponentID);
	m_Components.erase(toRemoveComponentID);

	return true;
}

bool Entity::hasComponent(ComponentID componentID)
{
	return m_Components.find(componentID) != m_Components.end();
}

const HashMap<ComponentID, Component*>& Entity::getAllComponents() const
{
	return m_Components;
}

void Entity::bind(const Event::Type& event, const sol::function& function)
{
	m_Binder.bind(event, [this, function](const Event* e) -> Variant { return function.call<Variant>(); });
}

bool Entity::call(const String& function, const Vector<Variant>& args)
{
	bool status = false;
	return status;
}

const String& Entity::getName() const
{
	return m_Scene->getName();
}

const SceneID Entity::getID() const
{
	return m_Scene->getID();
}

const String& Entity::getFullName() const
{
	return m_Scene->getFullName();
}

void Entity::draw()
{
}
