// NOTE(randy): TSM Specification. Can straight up include this and read in a .tsm file as a TSM struct
#define TSM_VERSION 0

typedef struct Vertex
{
	v3 position;
	v2 UVs;
    v3 normal;
	u8 bone_index_1;
	u8 bone_index_2;
	u8 bone_index_3;
	u8 bone_index_4;
	v4 bone_weights;
} Vertex;

typedef struct Transform
{
	v4 rotation;
	v3 translation;
	f32 scale;
} Transform;

internal m4 GetM4FromTransform(Transform t)
{
	return M4MultiplyM4(M4MultiplyM4(M4TranslateV3(t.translation), M4RotateQuat(t.rotation)), M4ScaleV3(v3(t.scale, t.scale, t.scale)));
}

typedef struct KeyRotation
{
	v4 rotation;
	f32 time_stamp;
} KeyRotation;

typedef struct KeyTranslation
{
	v3 translation;
	f32 time_stamp;
} KeyTranslation;

typedef struct KeyScale
{
	f32 scale;
	f32 time_stamp;
} KeyScale;

#define MAX_ANIMATION_COUNT 32
#define MAX_KEY_FRAME_COUNT 16
#define MAX_VERT_COUNT 16384
#define MAX_INDEX_COUNT 16384
#define MAX_BONE_COUNT 32
typedef struct Animation
{
	char name[64];
	f32 duration;
	
	KeyRotation rotations[MAX_BONE_COUNT][MAX_KEY_FRAME_COUNT];
	i32 rotation_count;
	KeyTranslation translations[MAX_BONE_COUNT][MAX_KEY_FRAME_COUNT];
	i32 translation_count;
	KeyScale scales[MAX_BONE_COUNT][MAX_KEY_FRAME_COUNT];
	i32 scale_count;
} Animation;

typedef struct Bone
{
	i32 id;
	char name[64];
	Transform local_transform;
	i32 parent_index;
	
	m4 inverse_bind_matrix;
	
	//BoneKeyFrames key_frames[MAX_ANIMATION_COUNT];
} Bone;

#define MAX_MESH_COUNT 4
typedef struct TSM
{
	i32 version;
	
	i32 vert_count;
	Vertex vertices[MAX_VERT_COUNT];
	
	u32 index_count;
	u32 indices[MAX_INDEX_COUNT];
	
	Bone bones[MAX_BONE_COUNT];
	u8 bone_count;
	
	m4 root_transform;
	
	Animation animations[MAX_ANIMATION_COUNT];
	i32 animation_count;
	
	/*
		Mesh meshes[MAX_MESH_COUNT];
		int mesh_count;
	 */
} TSM;

static void WriteTSMToFile(TSM *tsm, char *path)
{
	FILE *file = fopen(path, "wb");
	Assert(file);
	fwrite(tsm, 1, sizeof(TSM), file);
	fclose(file);
}

static TSM *ReadTSMFromFile(TSM *tsm, char *path)
{
	FILE *file = fopen(path, "rb");
	Assert(file);
	fread(tsm, 1, sizeof(TSM), file);
	fclose(file);
	
	return tsm;
}