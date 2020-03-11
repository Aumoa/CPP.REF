#pragma once

namespace SC::Game::Details
{
	class MDLParser : public TextParser
	{
		std::set<std::string> boneNameReplace;
		double beginProg;
		double endProg;
		double prog;

		RefPtr<IAsyncLoad> asyncLoad;

		vs_property( double, Progress );
		void Progress_set( double value )
		{
			double len = endProg - beginProg;
			prog = beginProg + len * value;
			asyncLoad->Progress = prog;
		}
		double Progress_get()
		{
			return prog;
		}

	public:
		struct tag_Interval
		{
			int Begin;
			int End;
		};

		struct tag_Sequence
		{
			std::string Anim;
			tag_Interval Interval;
			bool NonLooping = false;
		};

		struct tag_Version
		{
			int FormatVersion;
		};

		struct tag_Model
		{
			std::string Name;
			int NumGeosets;
			int NumGeosetAnims;
			int NumHelpers;
			int NumBones;
			int NumAttachments;
			int BlendTime;
			Vector3 MinimumExtent;
			Vector3 MaximumExtent;
		};

		struct tag_Group
		{
			int Indices[4];
		};

		struct tag_Geoset
		{
			std::vector<Vector3> Vertices;
			std::vector<Vector3> Normals;
			std::vector<Vector2> TVertices;
			std::vector<int> VertexGroup;
			std::vector<std::vector<int>> Faces;
			std::vector<tag_Group> Groups;
			int MaterialID;
		};

		struct tag_Texture
		{
			std::string Image;
			int ReplaceableId = 0;
		};

		struct tag_Layer
		{
			std::string FilterMode;
			int TextureID = -1;
		};

		struct tag_Material
		{
			bool ConstantColor = false;
			std::vector<tag_Layer> Layer;
		};

		template< class ValueType >
		struct tag_Key
		{
			int IntervalPos;
			ValueType Value;
		};

		template< class ValueType >
		struct tag_Keyframe
		{
			std::string Method;
			std::vector<tag_Key<ValueType>> Keyframes;
		};

		struct tag_Bone
		{
			std::string Name;
			int Parent = -1;
			tag_Keyframe<Vector3> Translation;
			tag_Keyframe<Vector3> Scale;
			tag_Keyframe<Quaternion> Rotation;

			void Export( tag_Interval interval, tag_Keyframe<Vector3>& translation, tag_Keyframe<Vector3>& scale, tag_Keyframe<Quaternion>& rotation )
			{
				int numKeyf = Translation.Keyframes.size();
				translation.Method = Translation.Method;
				for ( int i = 0; i < numKeyf; ++i )
				{
					if ( Translation.Keyframes[i].IntervalPos >= interval.Begin && Translation.Keyframes[i].IntervalPos <= interval.End )
					{
						translation.Keyframes.push_back( Translation.Keyframes[i] );
					}
				}

				numKeyf = Scale.Keyframes.size();
				scale.Method = Scale.Method;
				for ( int i = 0; i < numKeyf; ++i )
				{
					if ( Scale.Keyframes[i].IntervalPos >= interval.Begin && Scale.Keyframes[i].IntervalPos <= interval.End )
					{
						scale.Keyframes.push_back( Scale.Keyframes[i] );
					}
				}

				numKeyf = Rotation.Keyframes.size();
				rotation.Method = Rotation.Method;
				for ( int i = 0; i < numKeyf; ++i )
				{
					if ( Rotation.Keyframes[i].IntervalPos >= interval.Begin && Rotation.Keyframes[i].IntervalPos <= interval.End )
					{
						rotation.Keyframes.push_back( Rotation.Keyframes[i] );
					}
				}
			}
		};

		struct tag_GeosetAnim
		{
			int GeosetId;
			tag_Keyframe<bool> Keyframes;

			void Export( tag_Interval interval, tag_Keyframe<bool>& alpha )
			{
				int numKeyf = Keyframes.Keyframes.size();
				Keyframes.Method = Keyframes.Method;
				for ( int i = 0; i < numKeyf; ++i )
				{
					if ( Keyframes.Keyframes[i].IntervalPos >= interval.Begin && Keyframes.Keyframes[i].IntervalPos <= interval.End )
					{
						Keyframes.Keyframes.push_back( Keyframes.Keyframes[i] );
					}
				}
			}
		};

		tag_Version version;
		tag_Model model;
		std::vector<tag_Sequence> sequences;
		std::vector<tag_Texture> textures;
		std::vector<tag_Material> materials;
		std::vector<tag_Geoset> geosets;
		std::vector<tag_Bone> bones;
		std::vector<Vector3> pivots;
		std::vector<tag_GeosetAnim> geosetAnims;

	public:
		MDLParser( const char* block, int length, RefPtr<IAsyncLoad> asyncLoad, double beginProg, double endProg );
		MDLParser( std::filesystem::path filepath, RefPtr<IAsyncLoad> asyncLoad, double beginProg, double endProg );

		RefPtr<GameObject> CreateGameObject( String name, const std::filesystem::path& resourcePath );

	private:
		void TryParse();
		void ReadVersion();
		void ReadModel();
		tag_Geoset ReadGeoset();
		std::vector<int> ReadTriangles( int numIndexes );
		tag_Texture ReadTexture();
		tag_Material ReadMaterial();
		tag_Sequence ReadSequence();
		void ReadBone();
		void ReadGeosetAnim();

		Vector3 ReadVector3InBracket();
		Vector2 ReadVector2InBracket();
		Quaternion ReadVector4InBracket();
		void ReadVector2AsIntInBracket( int* out1, int* out2 );
		Vector3 ReadKeyframe3( const std::string& method );
		Quaternion ReadKeyframe4( const std::string& method );

		void ForEachContextsInBracket( std::function<void( std::string context )> contextFunctor );
		void ForEachContextsInBracket();
		void DiscardBracketInChildrens();

		std::vector<RefPtr<Texture2D>> LoadTextures( String name, const std::filesystem::path& resourcePath );
		std::vector<RefPtr<Material>> LoadMaterials( String name, const std::vector<RefPtr<Texture2D>>& txr );
		void LoadBones( String name, RefPtr<GameObject> head );
		void LoadMeshes( String name, RefPtr<GameObject> head, const std::vector<RefPtr<Material>>& mtr );
	};
}