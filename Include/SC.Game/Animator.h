#pragma once

namespace SC::Game
{
	/// <summary> ���� ������ �����ϴ� ��ü�� ���� �ִϸ��̼� ������ �����մϴ�. �� Ŭ������ ��ӵ� �� �����ϴ�. </summary>
	class Animator final : public Component
	{
		struct tag_StateMachine
		{
			std::wstring Name;
			RefPtr<AnimationState> State;
			RefPtr<AnimationClip> Clip;
			double Duration = 0;
			std::map<std::string, Keyframes::KeyframePair, std::less<>> Keyframes;
			double TimePos = 0;

			void Interpolate( double timeDelta )
			{
				if ( Clip )
				{
					TimePos += timeDelta;
					Clip->Interpolate( TimePos, Keyframes );
				}
			}
		};

#pragma pack( push, 4 )
		struct tag_BoneTransform
		{
			sc_game_export_float( XMFLOAT4X4, 16 ) World;
		};

		struct tag_Bone
		{
			sc_game_export_float( XMFLOAT4X4, 16 ) Offset;
			sc_game_export_float( XMFLOAT4X4, 16 ) ToRoot;
		};
#pragma pack( pop )

		RefPtr<AnimatorController> controller;
		std::map<std::wstring, AnimVar, std::less<>> animVars;

		tag_StateMachine currentState;
		tag_StateMachine prevState;
		double blendTime = 0;

		std::vector<tag_Bone> boneTransform;
		std::array<sc_game_export_object( RefPtr<Details::CDynamicBuffer> ), 2> finalTransformBuffer;

	protected:
		void Render( RefPtr<Details::CDeviceContext>& deviceContext ) override;

	public:
		/// <summary> <see cref="Animator"/> Ŭ������ �� �ν��Ͻ��� �ʱ�ȭ�մϴ�. </summary>
		Animator();
		~Animator() override;

		/// <summary> (<see cref="ICloneable"/> �������̽����� ���� ��.) ���� �ν��Ͻ��� ���纻�� �� ��ü�� �����մϴ�. </summary>
		object Clone() override;

		/// <summary> (<see cref="Component"/> Ŭ�������� ��� ��.) ����� ��ü�� ���۵� �� ����˴ϴ�. </summary>
		void Start() override;

		/// <summary> (<see cref="Component"/> Ŭ�������� ��� ��.) ������Ʈ�� ���� ���� ������ �� ������ �����մϴ�. </summary>
		/// <param name="time"> ���� �ð��� �����մϴ�. </param>
		void FixedUpdate( Time& time ) override;

		/// <summary> (Visual Studio ����) <see cref="SC::Game::AnimatorController"/> Ŭ�������� ����ϴ� �ִϸ��̼� �Ű������� ���� �����ϰų� �����ɴϴ�. </summary>
		vs_property( AnimVar, Vars )[];

		/// <summary> <see cref="AnimatorController"/> Ŭ�������� ����ϴ� �ִϸ��̼� �Ű������� ���� �����ɴϴ�. </summary>
		AnimVar Vars_get( const std::wstring_view& param0 );

		/// <summary> <see cref="AnimatorController"/> Ŭ�������� ����ϴ� �ִϸ��̼� �Ű������� ���� �����մϴ�. </summary>
		void Vars_set( const std::wstring_view& param0, const AnimVar& value );

		/// <summary> (Visual Studio ����) �ִϸ��̼� ��Ʈ�ѷ� ��ü�� �����ϰų� �����ɴϴ�. </summary>
		vs_property( RefPtr<AnimatorController>, Controller );

		/// <summary> ����� �ִϸ��̼� Ŭ���� �����ɴϴ�. </summary>
		RefPtr<AnimatorController> Controller_get();

		/// <summary> ����� �ִϸ��̼� Ŭ���� �����մϴ�. </summary>
		void Controller_set( RefPtr<AnimatorController> value );

	private:
		void InitializeOffset( RefPtr<GameObject> gameObject, int parent = -1 );

		void UpdateToRoot( RefPtr<GameObject> gameObject, int parent = -1 );
		void ReplaceToRoot();

		void Transistor();
		void UpdateBlend( double timeDelta );
		tag_StateMachine MakeStateMachine( const std::wstring_view& name );
	};
}