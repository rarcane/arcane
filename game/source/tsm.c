typedef struct json_object_s JObject;
typedef struct json_value_s JValue;
typedef struct json_string_s JString;
typedef struct json_object_element_s JObjectE;
typedef struct json_array_s JArray;
typedef struct json_array_element_s JArrayE;

internal JObjectE *FindElementInJObject(JObject *object, char *element_name)
{
	for (JObjectE *next = object->start; next; next = next->next)
	{
		if (strcmp(next->name->string, element_name) == 0)
		{
			return next;
		}
	}
	
	return 0;
}

internal JArrayE *FindArrayElementAtIndex(JArray *array, i32 index)
{
	Assert(index >= 0 && index < array->length);
	
	JArrayE *next = array->start;
	for (i32 i = 0; i < index + 1; i++)
	{
		if (i == index)
		{
			return next;
		}
		
		next = next->next;
	}
	
	Assert(0);
	return 0;
}

internal v4 GetRotationFromObject(JObject *object)
{
	v4 rot = v4(1.0f, 0.0f, 0.0f, 0.0f);
	
	JObjectE *rot_e = FindElementInJObject(object, "rotation");
	if (rot_e)
	{
		JArray *rot_arr = rot_e->value->payload;
		rot.x = (f32)atof(json_value_as_number(FindArrayElementAtIndex(rot_arr, 0)->value)->number);
		rot.y = (f32)atof(json_value_as_number(FindArrayElementAtIndex(rot_arr, 1)->value)->number);
		rot.z = (f32)atof(json_value_as_number(FindArrayElementAtIndex(rot_arr, 2)->value)->number);
		rot.w = (f32)atof(json_value_as_number(FindArrayElementAtIndex(rot_arr, 3)->value)->number);
	}
	
	return rot;
}

internal v3 GetTranslationFromObject(JObject *object)
{
	v3 trans = v3(0.0f, 0.0f, 0.0f);
	
	JObjectE *trans_e = FindElementInJObject(object, "translation");
	if (trans_e)
	{
		JArray *trans_arr = trans_e->value->payload;
		trans.x = (f32)atof(json_value_as_number(FindArrayElementAtIndex(trans_arr, 0)->value)->number);
		trans.y = (f32)atof(json_value_as_number(FindArrayElementAtIndex(trans_arr, 1)->value)->number);
		trans.z = (f32)atof(json_value_as_number(FindArrayElementAtIndex(trans_arr, 2)->value)->number);
	}
	
	return trans;
}

internal void RecursivelyStoreBoneNodesInTSM(JArray *node_array, JObject *root, TSM *tsm, i32 parent_index)
{
	Bone *bone = &tsm->bones[tsm->bone_count];
	bone->id = tsm->bone_count;
	bone->parent_index = parent_index;
	strcpy(bone->name, json_value_as_string(FindElementInJObject(root, "name")->value)->string);
	tsm->bone_count++;
	
	bone->transform = M4TranslateV3(GetTranslationFromObject(root));
	bone->transform = M4MultiplyM4(bone->transform, M4RotateQuat(GetRotationFromObject(root)));
	
	JObjectE *children_e = FindElementInJObject(root, "children");
	if (children_e)
	{
		JArray *children = children_e->value->payload;
		for (JArrayE *next = children->start; next; next = next->next)
		{
			i32 child_index = atoi(json_value_as_number(next->value)->number);
			
			JObject *child_node = FindArrayElementAtIndex(node_array, child_index)->value->payload;
			
			RecursivelyStoreBoneNodesInTSM(node_array, child_node, tsm, bone->id);
		}
	}
}

internal u8 GetBoneIDFromName(const char *name, TSM *tsm)
{
	for (u8 i = 0; i < tsm->bone_count; i++)
	{
		if (strcmp(tsm->bones[i].name, name) == 0)
		{
			return i;
		}
	}
	
	return -1;
}

// NOTE(randy): avert your eyes, child
internal InitTSMFromGLTFFile(TSM *tsm, char *path)
{
	FILE *f = 0;
	f = fopen(path, "r");
	Assert(f);
	
	fseek(f, 0, SEEK_END);
	i32 size = ftell(f);
	fseek(f, 0, SEEK_SET);
	i32 cur = ftell(f);
	
	void *data = MemoryArenaAllocateAndZero(core->frame_arena, size);
	ReadFromFile(f, data, size);
	
	JValue* root = json_parse(data, size);
	Assert(root->type == json_type_object);
	
	JObject* object = (struct json_object_s*)root->payload;
	
	JObject *armature = 0;
	
	JArray *nodes = FindElementInJObject(object, "nodes")->value->payload;
	for (JArrayE *next = nodes->start; next; next = next->next)
	{
		if (strcmp(json_value_as_string(FindElementInJObject(next->value->payload, "name")->value)->string,
				   "Armature") == 0)
		{
			armature = next->value->payload;
			break;
		}
	}
	Assert(armature);
	
	JArray *armature_children = FindElementInJObject(armature, "children")->value->payload;
	
	// NOTE(randy): Find mesh & skin
	JObject *mesh_0_node = FindArrayElementAtIndex(nodes, atoi(json_value_as_number(FindArrayElementAtIndex(armature_children, 0)->value)->number))->value->payload;
	Assert(mesh_0_node);
	
	JArray *mesh_array = FindElementInJObject(object, "meshes")->value->payload;
	i32 mesh_0_index = atoi(json_value_as_number(FindElementInJObject(mesh_0_node, "mesh")->value)->number);
	JObject *mesh_0 = FindArrayElementAtIndex(mesh_array, mesh_0_index)->value->payload;
	Assert(mesh_0);
	
	JArray *skin_array = FindElementInJObject(object, "skins")->value->payload;
	i32 skin_0_index = atoi(json_value_as_number(FindElementInJObject(mesh_0_node, "skin")->value)->number);
	JObject *skin_0 = FindArrayElementAtIndex(skin_array, skin_0_index)->value->payload;
	Assert(skin_0);
	
	// NOTE(randy): Find root bone and store him
	JObject *root_bone_node = FindArrayElementAtIndex(nodes, atoi(json_value_as_number(FindArrayElementAtIndex(armature_children, 1)->value)->number))->value->payload;
	RecursivelyStoreBoneNodesInTSM(nodes, root_bone_node, tsm, -1);
	
	
	tsm->global_transform = M4TranslateV3(GetTranslationFromObject(armature));
	
	/*
	i32 mesh_index = atoi(json_value_as_number(FindElementInJObject(next->value->payload, "mesh")->value)->number);
	mesh_0 = FindArrayElementAtIndex(mesh_array, mesh_index)->value->payload;
	
	i32 skin_index = atoi(json_value_as_number(FindElementInJObject(next->value->payload, "skin")->value)->number);
	skin_0 = FindArrayElementAtIndex(skin_array, skin_index)->value->payload;
*/
	
	JArray *accessor_array = FindElementInJObject(object, "accessors")->value->payload;
	JArray *buffer_views_array = FindElementInJObject(object, "bufferViews")->value->payload;
	
	// NOTE(randy): Mesh stuff
	// get name of mesh? if u want ;)
	JObject *mesh_0_prims = json_value_as_array(FindElementInJObject(mesh_0, "primitives")->value)->start->value->payload;
	JObject *attributes = FindElementInJObject(mesh_0_prims, "attributes")->value->payload;
	i32 index_accessor_index = atoi(json_value_as_number(FindElementInJObject(mesh_0_prims, "indices")->value)->number);
	JObject *index_accessor = FindArrayElementAtIndex(accessor_array, index_accessor_index)->value->payload;
	i32 index_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(index_accessor, "bufferView")->value)->number);
	tsm->index_count = atoi(json_value_as_number(FindElementInJObject(index_accessor, "count")->value)->number);
	JObject *index_buffer_view = FindArrayElementAtIndex(buffer_views_array, index_buffer_view_index)->value->payload;
	i32 index_byte_length = atoi(json_value_as_number(FindElementInJObject(index_buffer_view, "byteLength")->value)->number);
	i32 index_byte_offset = atoi(json_value_as_number(FindElementInJObject(index_buffer_view, "byteOffset")->value)->number);
	Assert(index_byte_length == sizeof(i16) * tsm->index_count);
	
	// NOTE(randy): Vertex positions
	i32 pos_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "POSITION")->value)->number);
	JObject *pos_accessor = FindArrayElementAtIndex(accessor_array, pos_accessor_index)->value->payload;
	i32 pos_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(pos_accessor, "bufferView")->value)->number);
	JObject *pos_buffer_view = FindArrayElementAtIndex(buffer_views_array, pos_buffer_view_index)->value->payload;
	i32 buffer_index = atoi(json_value_as_number(FindElementInJObject(pos_buffer_view, "buffer")->value)->number);
	i32 pos_byte_length = atoi(json_value_as_number(FindElementInJObject(pos_buffer_view, "byteLength")->value)->number);
	i32 pos_byte_offset = atoi(json_value_as_number(FindElementInJObject(pos_buffer_view, "byteOffset")->value)->number);
	tsm->vert_count = atoi(json_value_as_number(FindElementInJObject(pos_accessor, "count")->value)->number);
	Assert(pos_byte_length == sizeof(v3) * tsm->vert_count);
	
	// NOTE(randy): Vertex normals
	i32 normal_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "NORMAL")->value)->number);
	JObject *normal_accessor = FindArrayElementAtIndex(accessor_array, normal_accessor_index)->value->payload;
	i32 normal_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(normal_accessor, "bufferView")->value)->number);
	JObject *normal_buffer_view = FindArrayElementAtIndex(buffer_views_array, normal_buffer_view_index)->value->payload;
	i32 normal_byte_length = atoi(json_value_as_number(FindElementInJObject(normal_buffer_view, "byteLength")->value)->number);
	i32 normal_byte_offset = atoi(json_value_as_number(FindElementInJObject(normal_buffer_view, "byteOffset")->value)->number);
	Assert(normal_byte_length == sizeof(v3) * tsm->vert_count);
	
	// NOTE(randy): Vertex joints
	i32 joints_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "JOINTS_0")->value)->number);
	JObject *joints_accessor = FindArrayElementAtIndex(accessor_array, joints_accessor_index)->value->payload;
	i32 joints_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(joints_accessor, "bufferView")->value)->number);
	JObject *joints_buffer_view = FindArrayElementAtIndex(buffer_views_array, joints_buffer_view_index)->value->payload;
	i32 joints_byte_length = atoi(json_value_as_number(FindElementInJObject(joints_buffer_view, "byteLength")->value)->number);
	i32 joints_byte_offset = atoi(json_value_as_number(FindElementInJObject(joints_buffer_view, "byteOffset")->value)->number);
	Assert(joints_byte_length == sizeof(u8) * 4 * tsm->vert_count);
	
	i32 inverse_bind_accessor_index = atoi(json_value_as_number(FindElementInJObject(skin_0, "inverseBindMatrices")->value)->number);
	JObject *inverse_bind_accessor = FindArrayElementAtIndex(accessor_array, inverse_bind_accessor_index)->value->payload;
	i32 inverse_bind_count = atoi(json_value_as_number(FindElementInJObject(inverse_bind_accessor, "count")->value)->number);
	i32 inverse_bind_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(inverse_bind_accessor, "bufferView")->value)->number);
	JObject *inverse_bind_buffer_view = FindArrayElementAtIndex(buffer_views_array, inverse_bind_buffer_view_index)->value->payload;
	i32 inverse_bind_byte_length = atoi(json_value_as_number(FindElementInJObject(inverse_bind_buffer_view, "byteLength")->value)->number);
	i32 inverse_bind_byte_offset = atoi(json_value_as_number(FindElementInJObject(inverse_bind_buffer_view, "byteOffset")->value)->number);
	Assert(inverse_bind_byte_length == sizeof(m4) * inverse_bind_count);
	
	JArray *joints = FindElementInJObject(skin_0, "joints")->value->payload;
	u8 joint_map_of_bone_ids[MAX_BONE_COUNT] = {0};
	u8 joint_count = (u8)joints->length;
	for (JArrayE *next = joints->start; next; next = next->next)
	{
		i32 joint_index = atoi(json_value_as_number(next->value)->number);
		JObject *joint_obj = FindArrayElementAtIndex(nodes, joint_index)->value->payload;
		
		const char* joint_name = json_value_as_string(FindElementInJObject(joint_obj, "name")->value)->string;
		
		u8 id = GetBoneIDFromName(joint_name, tsm);
		Assert(id != -1 && joint_index >= 0);
		joint_map_of_bone_ids[joint_index] = id;
	}
	Assert(joint_count == tsm->bone_count && inverse_bind_count == joint_count);
	
	// NOTE(randy): Vertex weights
	i32 weights_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "WEIGHTS_0")->value)->number);
	JObject *weights_accessor = FindArrayElementAtIndex(accessor_array, weights_accessor_index)->value->payload;
	i32 weights_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(weights_accessor, "bufferView")->value)->number);
	JObject *weights_buffer_view = FindArrayElementAtIndex(buffer_views_array, weights_buffer_view_index)->value->payload;
	i32 weights_byte_length = atoi(json_value_as_number(FindElementInJObject(weights_buffer_view, "byteLength")->value)->number);
	i32 weights_byte_offset = atoi(json_value_as_number(FindElementInJObject(weights_buffer_view, "byteOffset")->value)->number);
	Assert(weights_byte_length == sizeof(v4) * tsm->vert_count);
	
	// NOTE(randy): Assuming that this is all in the same buffer, use accessors to get it
	JArray *buffer_array = FindElementInJObject(object, "buffers")->value->payload;
	JObject *buffer_obj = FindArrayElementAtIndex(buffer_array, buffer_index)->value->payload;
	i32 byte_length = atoi(json_value_as_number(FindElementInJObject(buffer_obj, "byteLength")->value)->number);
	// const char *buffer_uri = json_value_as_string(FindElementInJObject(buffer_obj, "uri")->value)->string;
	// TODO(randy): Unscuff this and make it actually relative from the .gltf uri
	char buffer_path[256] = "";
	strcat(strncpy(buffer_path, path, strlen(path) - 5), ".bin");
	
	// NOTE(randy): Close old .gltf file and read in binary buffer
	fclose(f);
	f = fopen(buffer_path, "rb");
	
	// NOTE(randy): Read in position data
	fseek(f, pos_byte_offset, SEEK_SET);
	for (i32 i = 0; i < tsm->vert_count; i++)
	{
		ReadFromFile(f, &tsm->vertices[i].position, sizeof(v3));
	}
	
	// NOTE(randy): Read in normal data
	fseek(f, normal_byte_offset, SEEK_SET);
	for (i32 i = 0; i < tsm->vert_count; i++)
	{
		ReadFromFile(f, &tsm->vertices[i].normal, sizeof(v3));
	}
	
	// NOTE(randy): joint/bone_id data
	fseek(f, joints_byte_offset, SEEK_SET);
	for (i32 i = 0; i < tsm->vert_count; i++)
	{
		Vertex *vert = &tsm->vertices[i];
		
		u8 joint_index;
		ReadFromFile(f, &joint_index, sizeof(u8));
		u8 bone_id = joint_map_of_bone_ids[joint_index];
		vert->bone_index_1 = bone_id;
		
		ReadFromFile(f, &joint_index, sizeof(u8));
		bone_id = joint_map_of_bone_ids[joint_index];
		vert->bone_index_2 = bone_id;
		
		ReadFromFile(f, &joint_index, sizeof(u8));
		bone_id = joint_map_of_bone_ids[joint_index];
		vert->bone_index_3 = bone_id;
		
		ReadFromFile(f, &joint_index, sizeof(u8));
		bone_id = joint_map_of_bone_ids[joint_index];
		vert->bone_index_4 = bone_id;
	}
	
	// NOTE(randy): weight data
	fseek(f, weights_byte_offset, SEEK_SET);
	for (i32 i = 0; i < tsm->vert_count; i++)
	{
		ReadFromFile(f, &tsm->vertices[i].bone_weights, sizeof(v4));
	}
	
	// NOTE(randy): Yoink index data
	fseek(f, index_byte_offset, SEEK_SET);
	for (u32 i = 0; i < tsm->index_count; i++)
	{
		u16 index;
		ReadFromFile(f, &index, sizeof(u16));
		tsm->indices[i] = (u32)index;
	}
	
	// NOTE(randy): Get the inverse bind matricies
	fseek(f, inverse_bind_byte_offset, SEEK_SET);
	for (JArrayE *next = joints->start; next; next = next->next)
	{
		i32 joint_index = atoi(json_value_as_number(next->value)->number);
		JObject *joint_obj = FindArrayElementAtIndex(nodes, joint_index)->value->payload;
		
		m4 mat;
		ReadFromFile(f, &mat, sizeof(m4));
		
		u8 bone_id = joint_map_of_bone_ids[joint_index];
		tsm->bones[bone_id].offset = mat;
	}
	
	fclose(f);
}