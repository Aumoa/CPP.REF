#pragma once

namespace SC::Game
{
	/// <summary> 개체의 변환 상태를 표현합니다. 이 클래스는 상속될 수 없습니다. </summary>
	class Transform final : public Component
	{
		friend class Camera;
		friend class GameObject;

		struct Constants
		{
			sc_game_export_float( XMFLOAT4X4, 16 ) World;
			sc_game_export_float( XMFLOAT4X4, 16 ) WorldInvTranspose;
		};

		Vector3 position;
		Vector3 scale;
		Quaternion rotation;

		std::array<sc_game_export_object( RefPtr<Details::CDynamicBuffer> ), 2> dynamicBuffer;

	private:
		void SetGraphicsRootConstantBufferView( RefPtr<Details::CDeviceContext>& deviceContext );

	public:
		/// <summary> <see cref="Transform"/> 클래스의 새 인스턴스를 초기화합니다. </summary>
		Transform();
		~Transform() override;

		/// <summary> (<see cref="ICloneable"/>) 현재 인스턴스의 복사본인 새 개체를 생성합니다. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> 클래스에서 상속 됨.) 변환 행렬을 생성합니다. 부모 변환의 영향을 받습니다. </summary>
		/// <param name="time"> 게임 시간을 전달합니다. </param>
		/// <param name="input"> 현재 프레임에서 입력 상태를 전달합니다. </param>
		virtual void LateUpdate( Time& time, Input& input );

		/// <summary> 대상을 바라보도록 설정합니다. </summary>
		/// <param name="target"> 대상을 지정합니다. </param>
		/// <param name="up"> 상향 벡터를 지정합니다. </param>
		void LookAt( RefPtr<Transform> target, Vector3 up = Vector3::Up );

		/// <summary> 대상을 바라보도록 설정합니다. </summary>
		/// <param name="target"> 대상을 지정합니다. </param>
		/// <param name="up"> 상향 벡터를 지정합니다. </param>
		void LookAt( Vector3 target, Vector3 up = Vector3::Up );

		/// <summary> 방향을 바라보도록 설정합니다. </summary>
		/// <param name="direction"> 방향을 지정합니다. </param>
		/// <param name="up"> 상향 벡터를 지정합니다. </param>
		void LookTo( Vector3 direction, Vector3 up = Vector3::Up );

		/// <summary> (Visual Studio 전용) 개체의 위치 변환을 설정하거나 가져옵니다. </summary>
		vs_property( Vector3, Position );

		/// <summary> 개체의 위치 변환을 가져옵니다. </summary>
		Vector3 Position_get();

		/// <summary> 개체의 위치 변환을 설정합니다. </summary>
		void Position_set( Vector3 value );

		/// <summary> (Visual Studio 전용) 개체의 크기 변환을 설정하거나 가져옵니다. </summary>
		vs_property( Vector3, Scale );

		/// <summary> 개체의 크기 변환을 가져옵니다. </summary>
		Vector3 Scale_get();

		/// <summary> 개체의 크기 변환을 설정합니다. </summary>
		void Scale_set( Vector3 value );

		/// <summary> (Visual Studio 전용) 개체의 회전 변환을 설정하거나 가져옵니다. </summary>
		vs_property( Quaternion, Rotation );

		/// <summary> 개체의 회전 변환을 가져옵니다. </summary>
		Quaternion Rotation_get();

		/// <summary> 개체의 회전 변환을 설정합니다. </summary>
		void Rotation_set( Quaternion value );

		/// <summary> (Visual Studio 전용) 개체가 현재 바라보는 방향을 가져옵니다. </summary>
		vs_property_get( Vector3, Forward );

		/// <summary> 개체가 현재 바라보는 방향을 가져옵니다. </summary>
		Vector3 Forward_get();
	};
}