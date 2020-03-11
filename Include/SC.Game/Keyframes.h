#pragma once

namespace SC::Game
{
	/// <summary> 완성된 키프레임 목록을 표현합니다. </summary>
	struct Keyframes
	{
		/// <summary> 이동, 확대 축소 및 회전 키프레임이 모두 포함된 단일 개체를 표현합니다. </summary>
		struct KeyframePair : private std::tuple<Keyframe<Vector3>, Keyframe<Vector3>, Keyframe<Quaternion>>
		{
			/// <summary> 빈 키프레임임을 나타내는 값을 가져옵니다. </summary>
			const bool IsEmpty = false;

			/// <summary> <see cref="KeyframePair"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
			constexpr KeyframePair() : tuple()
			{

			}

			/// <summary> <see cref="KeyframePair"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
			constexpr KeyframePair( const Keyframe<Vector3>& trans, const Keyframe<Vector3>& scale, const Keyframe<Quaternion>& quat ) : tuple( trans, scale, quat )
			{

			}

			/// <summary> <see cref="KeyframePair"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
			constexpr KeyframePair( const tuple& tup ) : tuple( tup )
			{

			}

			/// <summary> 대상 키프레임과 보간을 수행합니다. </summary>
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

			/// <summary> (Visual Studio 전용) 이동 애니메이션 키프레임 값을 가져옵니다. </summary>
			vs_property_get( Keyframe<Vector3>, Translation );

			/// <summary> 이동 애니메이션 키프레임 값을 가져옵니다. </summary>
			Keyframe<Vector3> Translation_get() const
			{
				return std::get<0>( ( const tuple& )*this );
			}

			/// <summary> (Visual Studio 전용) 확대 축소 애니메이션 키프레임 값을 가져옵니다. </summary>
			vs_property_get( Keyframe<Vector3>, Scaling );

			/// <summary> 확대 축소 애니메이션 키프레임 값을 가져옵니다. </summary>
			Keyframe<Vector3> Scaling_get() const
			{
				return std::get<1>( ( const tuple& )*this );
			}

			/// <summary> (Visual Studio 전용) 회전 애니메이션 키프레임 값을 가져옵니다. </summary>
			vs_property_get( Keyframe<Quaternion>, Rotation );

			/// <summary> 회전 애니메이션 키프레임 값을 가져옵니다. </summary>
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

			/// <summary> 빈 키프레임 개체를 가져옵니다. </summary>
			static KeyframePair Empty;

		private:
			KeyframePair( bool isEmpty ) : tuple()
				, IsEmpty( isEmpty )
			{

			}
		};

		/// <summary> 이동 애니메이션 키프레임 목록을 설정합니다. </summary>
		std::vector<Keyframe<Vector3>> Translation;

		/// <summary> 확대 축소 애니메이션 키프레임 목록을 설정합니다. </summary>
		std::vector<Keyframe<Vector3>> Scaling;

		/// <summary> 회전 애니메이션 키프레임 목록을 설정합니다. </summary>
		std::vector<Keyframe<Quaternion>> Rotation;

		/// <summary> <see cref="Keyframes"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Keyframes() = default;

		/// <summary> <see cref="Keyframes"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Keyframes( const Keyframes& copy )
			: Translation( copy.Translation )
			, Scaling( copy.Scaling )
			, Rotation( copy.Rotation )
		{

		}

		/// <summary> <see cref="Keyframes"/> 구조체의 새 인스턴스를 초기화합니다. </summary>
		Keyframes( Keyframes&& mov )
			: Translation( std::move( mov.Translation ) )
			, Scaling( std::move( mov.Scaling ) )
			, Rotation( std::move( mov.Rotation ) )
		{

		}

		/// <summary> 변형 키프레임 목록에서 해당 시간과 일치하는 보간된 키프레임을 가져옵니다. </summary>
		/// <param name="timePos"> 시간 값을 전달합니다. </param>
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

		/// <summary> 위치 변형 키프레임 목록에서 해당 시간과 일치하는 보간된 키프레임을 가져옵니다. </summary>
		/// <param name="timePos"> 시간 값을 전달합니다. </param>
		Keyframe<Vector3> InterpolateTranslation( double timePos ) const
		{
			return InterpolateHelper<Vector3>( timePos, Translation.cbegin(), Translation.cend() );
		}

		/// <summary> 비례 변형 키프레임 목록에서 해당 시간과 일치하는 보간된 키프레임을 가져옵니다. </summary>
		/// <param name="timePos"> 시간 값을 전달합니다. </param>
		Keyframe<Vector3> InterpolateScaling( double timePos ) const
		{
			return InterpolateHelper<Vector3>( timePos, Scaling.cbegin(), Scaling.cend() );
		}

		/// <summary> 회전 변형 키프레임 목록에서 해당 시간과 일치하는 보간된 키프레임을 가져옵니다. </summary>
		/// <param name="timePos"> 시간 값을 전달합니다. </param>
		Keyframe<Quaternion> InterpolateRotation( double timePos ) const
		{
			return InterpolateHelper<Quaternion>( timePos, Rotation.cbegin(), Rotation.cend() );
		}

		/// <summary> (Visual Studio 전용) 키프레임 목록을 조회해 마지막 재생 시간을 계산합니다. </summary>
		vs_property_get( double, Duration );

		/// <summary> 키프레임 목록을 조회해 마지막 재생 시간을 계산합니다. </summary>
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

	/// <summary> 빈 키프레임 개체를 가져옵니다. </summary>
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