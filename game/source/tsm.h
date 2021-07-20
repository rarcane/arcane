// NOTE(randy): TSM Specification. Can straight up include this and read in a .tsm file as a TSM struct
#define TSM_VERSION 0

typedef struct Vertex
{
	v3 position;
	v2 UVs;
    v3 normal;
	/*
		short bone_index_1;
		short bone_index_2;
		short bone_index_3;
		short bone_index_4;
		v4 bone_weights;
	 */
} Vertex;

typedef struct KeyPosition
{
	f64 time_stamp;
	v3 position;
} KeyPosition;

typedef struct KeyRotation
{
	f64 time_stamp;
	v4 rotation;
} KeyRotation;

typedef struct KeyScale
{
	f64 time_stamp;
	v3 scale;
} KeyScale;

#define MAX_KEY_FRAMES 256
typedef struct BoneKeyFrames
{
	KeyPosition key_positions[MAX_KEY_FRAMES];
	i32 key_position_count;
	KeyRotation key_rotations[MAX_KEY_FRAMES];
	i32 key_rotation_count;
	KeyScale key_scales[MAX_KEY_FRAMES];
	i32 key_scale_count;
} BoneKeyFrames;

#define MAX_ANIMATION_COUNT 32
typedef struct Animation
{
	char name[64];
	f32 duration;
	i32 ticks_per_second;
} Animation;

typedef struct BoneInfo BoneInfo;
struct BoneInfo
{
	i32 id; // ID can be implicit in the array it's sorted in?
	char name[64];
	m4 offset;
	m4 transform;
	i32 parent_index;
	
	BoneKeyFrames key_frames[MAX_ANIMATION_COUNT];
};

#define MAX_VERT_COUNT 16384
#define MAX_INDEX_COUNT 16384
#define MAX_BONE_COUNT 32
typedef struct Mesh
{
    Vertex vertices[MAX_VERT_COUNT];
	i32 vertex_count;
	u32 indices[MAX_INDEX_COUNT];
	i32 index_count;
	
	/*
		BoneInfo bone_infos[MAX_BONE_COUNT];
		i32 bone_count;
		
		Animation animations[MAX_ANIMATION_COUNT];
		i32 animation_count;
		
		i32 root_bone_index;
		m4 inverse_root_matrix;
	 */
} Mesh;

#define MAX_MESH_COUNT 4
typedef struct TSM
{
	i32 version;
	i32 vert_count;
	Vertex vertices[MAX_VERT_COUNT];
	
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