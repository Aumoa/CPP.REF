#pragma once

namespace SC::Game
{
	/// <summary> �ϼ��� Ű������ ����� ǥ���մϴ�. </summary>
	struct Keyframes
	{
		/// <summary> �̵�, Ȯ�� ��� �� ȸ�� Ű�������� ��� ���Ե� ���� ��ü�� ǥ���մϴ�. </summary>
		struct KeyframePair : private std::tuple<Keyframe<Vector3>, Keyframe<Vector3>, Keyframe<Quaternion>>
		{
			/// <summary> �� Ű���������� ��Ÿ���� ���� �����ɴϴ�. </summary>
			const bool IsEmpty = false;

			/// <summary> <see cref="KeyframePair"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
			constexpr KeyframePair() : tuple()
			{

			}

			/// <summary> <see cref="KeyframePair"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
			constexpr KeyframePair( const Keyframe<Vector3>& trans, const Keyframe<Vector3>& scale, const Keyframe<Quaternion>& quat ) : tuple( trans, scale, quat )
			{

			}

			/// <summary> <see cref="KeyframePair"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
			constexpr KeyframePair( const tuple& tup ) : tuple( tup )
			{

			}

			/// <summary> ��� Ű�����Ӱ� ������ �����մϴ�. </summary>
			KeyframePair Interpolate( const KeyframePair& right, double t ) const
			{
				KeyframePair pair;
				
				auto invt = 1.0 - t;

				auto& trans = std::get<0>( pair );
				trans.TimePos = Translation.TimePos * invt + right.Translation.TimePos * t;
				trans.Value = Translation.Value.Lerp( right.Translation.Value, t );

				auto& scale = std::get<1>( pair );
				scale.TimePos = Scaling.TimePos * invt + right.Scaling.TimePos * t;
				scale.Value = Scaling.Value.Lerp( right.Scaling.Value, t );

				auto& quat = std::get<2>( pair );
				quat.TimePos = Rotation.TimePos * invt + right.Rotation.TimePos * t;
				quat.Value = Rotation.Value.Slerp( right.Rotation.Value, t );

				return pair;
			}

			/// <summary> (Visual Studio ����) �̵� �ִϸ��̼� Ű������ ���� �����ɴϴ�. </summary>
			vs_property_get( Keyframe<Vector3>, Translation );

			/// <summary> �̵� �ִϸ��̼� Ű������ ���� �����ɴϴ�. </summary>
			Keyframe<Vector3> Translation_get() const
			{
				return std::get<0>( ( const tuple& )*this );
			}

			/// <summary> (Visual Studio ����) Ȯ�� ��� �ִϸ��̼� Ű������ ���� �����ɴϴ�. </summary>
			vs_property_get( Keyframe<Vector3>, Scaling );

			/// <summary> Ȯ�� ��� �ִϸ��̼� Ű������ ���� �����ɴϴ�. </summary>
			Keyframe<Vector3> Scaling_get() const
			{
				return std::get<1>( ( const tuple& )*this );
			}

			/// <summary> (Visual Studio ����) ȸ�� �ִϸ��̼� Ű������ ���� �����ɴϴ�. </summary>
			vs_property_get( Keyframe<Quaternion>, Rotation );

			/// <summary> ȸ�� �ִϸ��̼� Ű������ ���� �����ɴϴ�. </summary>
			Keyframe<Quaternion> Rotation_get() const
			{
				return std::get<2>( ( const tuple& )*this );
			}

			KeyframePair& operator=( const KeyframePair& pair )
			{
				using namespace std;

				get<0>( *this ) = pair.Translation;
				get<1>( *this ) = pair.Scaling;
				get<2>( *this ) = pair.Rotation;

				return *this;
			}

			KeyframePair& operator=( const tuple& tup )
			{
				using namespace std;

				( tuple& )*this = tup;

				return *this;
			}

			constexpr bool operator==( const KeyframePair& right ) const
			{
				return ( const tuple& )*this == ( const tuple& )right;
			}

			constexpr bool operator!=( const KeyframePair& right ) const
			{
				return ( const tuple& )*this != ( const tuple& )right;
			}

			/// <summary> �� Ű������ ��ü�� �����ɴϴ�. </summary>
			static KeyframePair Empty;

		private:
			KeyframePair( bool isEmpty ) : tuple()
				, IsEmpty( isEmpty )
			{

			}
		};

		/// <summary> �̵� �ִϸ��̼� Ű������ ����� �����մϴ�. </summary>
		std::vector<Keyframe<Vector3>> Translation;

		/// <summary> Ȯ�� ��� �ִϸ��̼� Ű������ ����� �����մϴ�. </summary>
		std::vector<Keyframe<Vector3>> Scaling;

		/// <summary> ȸ�� �ִϸ��̼� Ű������ ����� �����մϴ�. </summary>
		std::vector<Keyframe<Quaternion>> Rotation;

		/// <summary> <see cref="Keyframes"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Keyframes() = default;

		/// <summary> <see cref="Keyframes"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Keyframes( const Keyframes& copy )
			: Translation( copy.Translation )
			, Scaling( copy.Scaling )
			, Rotation( copy.Rotation )
		{

		}

		/// <summary> <see cref="Keyframes"/> ����ü�� �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Keyframes( Keyframes&& mov )
			: Translation( std::move( mov.Translation ) )
			, Scaling( std::move( mov.Scaling ) )
			, Rotation( std::move( mov.Rotation ) )
		{

		}

		/// <summary> ���� Ű������ ��Ͽ��� �ش� �ð��� ��ġ�ϴ� ������ Ű�������� �����ɴϴ�. </summary>
		/// <param name="timePos"> �ð� ���� �����մϴ�. </param>
		KeyframePair Interpolate( double timePos ) const
		{
			using namespace std;

			Keyframe<Vector3> trans;
			Keyframe<Vector3> scale;
			Keyframe<Quaternion> quat;

			if ( Translation.empty() )
			{
				trans.TimePos = timePos;
				trans.Value = Vector3::Zero;
			}
			else
			{
				trans = InterpolateTranslation( timePos );
			}

			if ( Scaling.empty() )
			{
				scale.TimePos = timePos;
				scale.Value = Vector3::One;
			}
			else
			{
				scale = InterpolateScaling( timePos );
			}

			if ( Rotation.empty() )
			{
				quat.TimePos = timePos;
				quat.Value = Quaternion::Identity;
			}
			else
			{
				quat = InterpolateRotation( timePos );
			}

			return tuple( trans, scale, quat );
		}

		/// <summary> ��ġ ���� Ű������ ��Ͽ��� �ش� �ð��� ��ġ�ϴ� ������ Ű�������� �����ɴϴ�. </summary>
		/// <param name="timePos"> �ð� ���� �����մϴ�. </param>
		Keyframe<Vector3> InterpolateTranslation( double timePos ) const
		{
			return InterpolateHelper<Vector3>( timePos, Translation.cbegin(), Translation.cend() );
		}

		/// <summary> ��� ���� Ű������ ��Ͽ��� �ش� �ð��� ��ġ�ϴ� ������ Ű�������� �����ɴϴ�. </summary>
		/// <param name="timePos"> �ð� ���� �����մϴ�. </param>
		Keyframe<Vector3> InterpolateScaling( double timePos ) const
		{
			return InterpolateHelper<Vector3>( timePos, Scaling.cbegin(), Scaling.cend() );
		}

		/// <summary> ȸ�� ���� Ű������ ��Ͽ��� �ش� �ð��� ��ġ�ϴ� ������ Ű�������� �����ɴϴ�. </summary>
		/// <param name="timePos"> �ð� ���� �����մϴ�. </param>
		Keyframe<Quaternion> InterpolateRotation( double timePos ) const
		{
			return InterpolateHelper<Quaternion>( timePos, Rotation.cbegin(), Rotation.cend() );
		}

		/// <summary> (Visual Studio ����) Ű������ ����� ��ȸ�� ������ ��� �ð��� ����մϴ�. </summary>
		vs_property_get( double, Duration );

		/// <summary> Ű������ ����� ��ȸ�� ������ ��� �ð��� ����մϴ�. </summary>
		double Duration_get() const
		{
			double dur = 0;

			for ( int i = 0, count = Translation.size(); i < count; ++i )
			{
				auto& key = Translation[i].TimePos;
				dur = std::max( dur, key );
			}

			for ( int i = 0, count = Scaling.size(); i < count; ++i )
			{
				auto& key = Scaling[i].TimePos;
				dur = std::max( dur, key );
			}

			for ( int i = 0, count = Rotation.size(); i < count; ++i )
			{
				auto& key = Rotation[i].TimePos;
				dur = std::max( dur, key );
			}

			return dur;
		}

		Keyframes& operator=( const Keyframes& copy )
		{
			Translation = copy.Translation;
			Scaling = copy.Scaling;
			Rotation = copy.Rotation;

			return *this;
		}

		Keyframes& operator=( Keyframes&& mov )
		{
			Translation = std::move( mov.Translation );
			Scaling = std::move( mov.Scaling );
			Rotation = std::move( mov.Rotation );

			return *this;
		}

		bool operator==( const Keyframes& right ) const
		{
			return
				Translation == right.Translation &&
				Scaling == right.Scaling &&
				Rotation == right.Rotation &&
				true;
		}

		bool operator!=( const Keyframes& right ) const
		{
			return
				Translation != right.Translation ||
				Scaling != right.Scaling ||
				Rotation != right.Rotation ||
				false;
		}

	private:
		template< class T >
		Keyframe<T> InterpolateHelper( double timePos, typename std::vector<Keyframe<T>>::const_iterator begin, typename std::vector<Keyframe<T>>::const_iterator end ) const
		{
			auto it = std::lower_bound( begin, end, timePos, []( const Keyframe<T>& left, double right ) -> bool
				{
					return left.TimePos < right;
				}
			);

			if ( it != end && it == begin ) ++it;
			if ( it == end ) return *( end - 1 );
			else
			{
				auto next = it--;

				auto& left = *it;
				auto& right = *next;

				return left.Interpolate( right, timePos );
			}
		}
	};

	/// <summary> �� Ű������ ��ü�� �����ɴϴ�. </summary>
	__declspec( selectany ) Keyframes::KeyframePair Keyframes::KeyframePair::Empty( true );
}

inline std::wostream& operator<<( std::wostream& wout, const SC::Game::Keyframes& keyframes )
{
	wout << L"Duration: " << keyframes.Duration;
	return wout << L", NumKeyframes: (" << keyframes.Translation.size() << L", " << keyframes.Scaling.size() << L", " << keyframes.Rotation.size() << L")";
}

inline std::wostream& operator<<( std::wostream& wout, const SC::Game::Keyframes::KeyframePair& pair )
{
	wout << "Translation: " << pair.Translation;
	wout << ", Scaling: " << pair.Scaling;
	return wout << ", Rotation: " << pair.Rotation;
}