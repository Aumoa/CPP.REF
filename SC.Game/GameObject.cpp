using namespace SC;
using namespace SC::Game;
using namespace SC::Game::Details;

using namespace std;

GameObject::GameObject( String name ) : Assets( name )
{
	transform = new Game::Transform();
	transform->gameObject = this;
}

RefPtr<IEnumerator<RefPtr<GameObject>>> GameObject::GetEnumerator()
{
	return new GameObjectEnumerator( gameObjects.begin(), gameObjects.end() );
}

object GameObject::Clone()
{
	RefPtr gameObject = new GameObject( String::Format( "{0} Clone", Name ) );
	gameObject->transform = transform->Clone().As<Game::Transform>();
	gameObject->transform->gameObject = gameObject;

	for ( int i = 0, count = ( int )components.size(); i < count; ++i )
	{
		auto clone = components[i].second->Clone().As<Component>();
		clone->gameObject = gameObject;
		gameObject->components.push_back( { components[i].first, clone } );
		clone->Awake();
	}

	for ( int i = 0, count = ( int )gameObjects.size(); i < count; ++i )
	{
		auto child = gameObjects[i]->Clone().As<GameObject>();
		child->Parent = gameObject;
	}

	return gameObject;
}

void GameObject::Start()
{
	for ( auto i : gameObjects )
	{
		i->Start();
	}
}

void GameObject::Update( Time& time, Input& input )
{
	for ( auto i : gameObjects )
	{
		i->Update( time, input );
	}

	for ( auto i : components )
	{
		if ( i.second->IsEnabled )
		{
			auto cmp = i.second;
			if ( cmp->isFirst )
			{
				cmp->Start();
				cmp->isFirst = false;
			}
			cmp->Update( time, input );
		}
	}
}

void GameObject::FixedUpdate( Time& time )
{
	for ( auto i : gameObjects )
	{
		i->FixedUpdate( time );
	}

	for ( auto i : components )
	{
		if ( i.second->IsEnabled )
		{
			auto cmp = i.second;
			if ( cmp->isFirst )
			{
				cmp->Start();
				cmp->isFirst = false;
			}
			cmp->FixedUpdate( time );
		}
	}
}

void GameObject::LateUpdate( Time& time, Input& input )
{
	transform->LateUpdate( time, input );

	for ( auto i : components )
	{
		if ( i.second->IsEnabled )
		{
			i.second->LateUpdate( time, input );
		}
	}

	for ( auto i : gameObjects )
	{
		i->LateUpdate( time, input );
	}
}

void GameObject::Render( RefPtr<CDeviceContext>& deviceContext )
{
	transform->SetGraphicsRootConstantBufferView( deviceContext );

	for ( auto i : components )
	{
		if ( i.second->IsEnabled )
			i.second->Render( deviceContext );
	}

	for ( auto i : gameObjects )
	{
		i->Render( deviceContext );
	}
}

RefPtr<Transform> GameObject::Transform_get()
{
	return transform;
}

RefPtr<GameObject> GameObject::Parent_get()
{
	return parent.TryResolveAs<GameObject>();
}

void GameObject::Parent_set( RefPtr<GameObject> value )
{
	if ( parent.IsValid )
	{
		auto par = parent.ResolveAs<GameObject>();

		for ( size_t i = 0, count = par->gameObjects.size(); i < count; ++i )
		{
			if ( par->gameObjects[i].Get() == this )
			{
				par->gameObjects.erase( par->gameObjects.begin() + i );
				break;
			}
		}
	}

	if ( value )
	{
		value->gameObjects.push_back( this );
		parent = value;
	}
}

int GameObject::NumChilds_get()
{
	return ( int )gameObjects.size();
}

RefPtr<GameObject> GameObject::Childs_get( int param0 )
{
	return gameObjects[param0];
}

void GameObject::AddComponent( size_t type_hash, RefPtr<Component> component )
{
	ComponentPair pair;
	pair.first = type_hash;
	pair.second = component;
	components.push_back( pair );
	component->gameObject = this;

	component->Awake();
}

RefPtr<Component> GameObject::GetComponent( size_t type_hash, std::function<bool( Component* )> caster )
{
	int dynamic_index = -1;

	if ( type_hash == typeid( Game::Transform ).hash_code() )
	{
		return transform;
	}

	for ( size_t i = 0, count = components.size(); i < count; ++i )
	{
		auto& comp = components[i];
		if ( comp.first == type_hash )
		{
			return comp.second;
		}

		if ( dynamic_index == -1 && caster( comp.second.Get() ) )
		{
			dynamic_index = ( int )i;
		}
	}

	if ( dynamic_index != -1 )
	{
		return components[dynamic_index].second;
	}
	else
	{
		return nullptr;
	}
}

void GameObject::RemoveComponent( size_t type_hash, function<bool( Component* )> caster )
{
	int dynamic_index = -1;

	if ( type_hash == typeid( Game::Transform ).hash_code() )
	{
		throw new Exception();
	}

	for ( size_t i = 0, count = components.size(); i < count; ++i )
	{
		auto& comp = components[i];
		if ( comp.first == type_hash )
		{
			components.erase( components.begin() + i );
			break;
		}
	}

	for ( size_t i = 0, count = components.size(); i < count; ++i )
	{
		auto& comp = components[i];
		if ( dynamic_index == -1 && caster( comp.second.Get() ) )
		{
			dynamic_index = ( int )i;
		}
	}

	if ( dynamic_index != -1 )
	{
		components.erase( components.begin() + dynamic_index );
	}
}