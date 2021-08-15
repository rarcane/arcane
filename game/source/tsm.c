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

internal i32 GetIntFromJObjectE(JObjectE *object)
{
	return atoi(json_value_as_number(object->value)->number);
}

internal v4 GetRotationFromObject(JObject *object)
{
	v4 rot = v4(0.0f, 0.0f, 0.0f, 1.0f);
	
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
	Assert(tsm->bone_count + 1 < MAX_BONE_COUNT);
	Bone *bone = &tsm->bones[tsm->bone_count];
	bone->id = tsm->bone_count;
	bone->parent_index = parent_index;
	strcpy(bone->name, json_value_as_string(FindElementInJObject(root, "name")->value)->string);
	tsm->bone_count++;
	
	bone->local_transform.translation = GetTranslationFromObject(root);
	bone->local_transform.rotation = GetRotationFromObject(root);
	bone->local_transform.scale = 1.0f;
	
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
	fclose(f);
	
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
	tsm->root_transform = M4TranslateV3(GetTranslationFromObject(armature));
	
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
	
	// NOTE(randy): Find root bone, then recursively store all bone nodes
	i32 root_bone_index = atoi(json_value_as_number(FindArrayElementAtIndex(armature_children, 1)->value)->number);
	JObject *root_bone_node = FindArrayElementAtIndex(nodes, root_bone_index)->value->payload;
	RecursivelyStoreBoneNodesInTSM(nodes, root_bone_node, tsm, -1);
	
	
	JArray *accessors = FindElementInJObject(object, "accessors")->value->payload;
	JArray *buffer_views_array = FindElementInJObject(object, "bufferViews")->value->payload;
	
	// NOTE(randy): Mesh stuff
	// get name of mesh? if u want ;)
	JObject *mesh_0_prims = json_value_as_array(FindElementInJObject(mesh_0, "primitives")->value)->start->value->payload;
	JObject *attributes = FindElementInJObject(mesh_0_prims, "attributes")->value->payload;
	i32 index_accessor_index = atoi(json_value_as_number(FindElementInJObject(mesh_0_prims, "indices")->value)->number);
	JObject *index_accessor = FindArrayElementAtIndex(accessors, index_accessor_index)->value->payload;
	i32 index_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(index_accessor, "bufferView")->value)->number);
	tsm->index_count = atoi(json_value_as_number(FindElementInJObject(index_accessor, "count")->value)->number);
	JObject *index_buffer_view = FindArrayElementAtIndex(buffer_views_array, index_buffer_view_index)->value->payload;
	i32 index_byte_length = atoi(json_value_as_number(FindElementInJObject(index_buffer_view, "byteLength")->value)->number);
	i32 index_byte_offset = atoi(json_value_as_number(FindElementInJObject(index_buffer_view, "byteOffset")->value)->number);
	Assert(index_byte_length == sizeof(i16) * tsm->index_count);
	
	// NOTE(randy): Vertex positions
	i32 pos_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "POSITION")->value)->number);
	JObject *pos_accessor = FindArrayElementAtIndex(accessors, pos_accessor_index)->value->payload;
	i32 pos_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(pos_accessor, "bufferView")->value)->number);
	JObject *pos_buffer_view = FindArrayElementAtIndex(buffer_views_array, pos_buffer_view_index)->value->payload;
	i32 buffer_index = atoi(json_value_as_number(FindElementInJObject(pos_buffer_view, "buffer")->value)->number);
	i32 pos_byte_length = atoi(json_value_as_number(FindElementInJObject(pos_buffer_view, "byteLength")->value)->number);
	i32 pos_byte_offset = atoi(json_value_as_number(FindElementInJObject(pos_buffer_view, "byteOffset")->value)->number);
	tsm->vert_count = atoi(json_value_as_number(FindElementInJObject(pos_accessor, "count")->value)->number);
	Assert(pos_byte_length == sizeof(v3) * tsm->vert_count);
	
	// NOTE(randy): Vertex normals
	i32 normal_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "NORMAL")->value)->number);
	JObject *normal_accessor = FindArrayElementAtIndex(accessors, normal_accessor_index)->value->payload;
	i32 normal_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(normal_accessor, "bufferView")->value)->number);
	JObject *normal_buffer_view = FindArrayElementAtIndex(buffer_views_array, normal_buffer_view_index)->value->payload;
	i32 normal_byte_length = atoi(json_value_as_number(FindElementInJObject(normal_buffer_view, "byteLength")->value)->number);
	i32 normal_byte_offset = atoi(json_value_as_number(FindElementInJObject(normal_buffer_view, "byteOffset")->value)->number);
	Assert(normal_byte_length == sizeof(v3) * tsm->vert_count);
	
	// NOTE(randy): Vertex joints
	i32 joints_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "JOINTS_0")->value)->number);
	JObject *joints_accessor = FindArrayElementAtIndex(accessors, joints_accessor_index)->value->payload;
	i32 joints_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(joints_accessor, "bufferView")->value)->number);
	JObject *joints_buffer_view = FindArrayElementAtIndex(buffer_views_array, joints_buffer_view_index)->value->payload;
	i32 joints_byte_length = atoi(json_value_as_number(FindElementInJObject(joints_buffer_view, "byteLength")->value)->number);
	i32 joints_byte_offset = atoi(json_value_as_number(FindElementInJObject(joints_buffer_view, "byteOffset")->value)->number);
	Assert(joints_byte_length == sizeof(u8) * 4 * tsm->vert_count);
	
	i32 inverse_bind_accessor_index = atoi(json_value_as_number(FindElementInJObject(skin_0, "inverseBindMatrices")->value)->number);
	JObject *inverse_bind_accessor = FindArrayElementAtIndex(accessors, inverse_bind_accessor_index)->value->payload;
	i32 inverse_bind_count = atoi(json_value_as_number(FindElementInJObject(inverse_bind_accessor, "count")->value)->number);
	i32 inverse_bind_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(inverse_bind_accessor, "bufferView")->value)->number);
	JObject *inverse_bind_buffer_view = FindArrayElementAtIndex(buffer_views_array, inverse_bind_buffer_view_index)->value->payload;
	i32 inverse_bind_byte_length = atoi(json_value_as_number(FindElementInJObject(inverse_bind_buffer_view, "byteLength")->value)->number);
	i32 inverse_bind_byte_offset = atoi(json_value_as_number(FindElementInJObject(inverse_bind_buffer_view, "byteOffset")->value)->number);
	Assert(inverse_bind_byte_length == sizeof(m4) * inverse_bind_count);
	
	// NOTE(randy): Paranoia
	JArray *joints = FindElementInJObject(skin_0, "joints")->value->payload;
	//Assert(joint_index == tsm->bone_count);
	
	// NOTE(randy): Vertex weights
	i32 weights_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "WEIGHTS_0")->value)->number);
	JObject *weights_accessor = FindArrayElementAtIndex(accessors, weights_accessor_index)->value->payload;
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
	
	// NOTE(randy): Read in binary buffer
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
		vert->bone_index_1 = joint_index;//joint_map_of_bone_ids[joint_index];
		
		ReadFromFile(f, &joint_index, sizeof(u8));
		vert->bone_index_2 = joint_index;//joint_map_of_bone_ids[joint_index];
		
		ReadFromFile(f, &joint_index, sizeof(u8));
		vert->bone_index_3 = joint_index;//joint_map_of_bone_ids[joint_index];
		
		ReadFromFile(f, &joint_index, sizeof(u8));
		vert->bone_index_4 = joint_index;//joint_map_of_bone_ids[joint_index];
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
	i32 joint_index = 0;
	for (JArrayE *next = joints->start; next; next = next->next)
	{
		i32 joint_node_index = atoi(json_value_as_number(next->value)->number);
		JObject *joint_obj = FindArrayElementAtIndex(nodes, joint_node_index)->value->payload;
		
		const char* joint_name = json_value_as_string(FindElementInJObject(joint_obj, "name")->value)->string;
		u8 id = GetBoneIDFromName(joint_name, tsm);
		// NOTE(randy): assume there will be no mismatch between our janky reconstruction and the skins->joints gltf array
		Assert(id == joint_index);
		
		m4 mat;
		ReadFromFile(f, &mat, sizeof(m4));
		
		tsm->bones[joint_index].inverse_bind_matrix = mat;
		
		joint_index++;
	}
	
	// NOTE(randy): Animation stuff
	JArray *animation_array = FindElementInJObject(object, "animations")->value->payload;
	for (JArrayE *next = animation_array->start; next; next = next->next)
	{
		JObject *animation_obj = next->value->payload;
		
		Animation *animation = &tsm->animations[tsm->animation_count++];
		strcpy(animation->name, json_value_as_string(FindElementInJObject(animation_obj, "name")->value)->string);
		
		JArray *channels = FindElementInJObject(animation_obj, "channels")->value->payload;
		JArray *samplers = FindElementInJObject(animation_obj, "samplers")->value->payload;
		for (JArrayE *next_channel = channels->start; next_channel; next_channel = next_channel->next)
		{
			JObject *channel = next_channel->value->payload;
			
			JObject *target = FindElementInJObject(channel, "target")->value->payload;
			i32 target_node_index = GetIntFromJObjectE(FindElementInJObject(target, "node"));
			char path[64] = "";
			strcpy(path, json_value_as_string(FindElementInJObject(target, "path")->value)->string);
			
			JObject *channel_node = FindArrayElementAtIndex(nodes, target_node_index)->value->payload;
			char node_name[64] = "";
			strcpy(node_name, json_value_as_string(FindElementInJObject(channel_node, "name")->value)->string);
			i32 bone_id = GetBoneIDFromName(node_name, tsm);
			
			i32 sampler_index = GetIntFromJObjectE(FindElementInJObject(channel, "sampler"));
			JObject *sampler = FindArrayElementAtIndex(samplers, sampler_index)->value->payload;
			
			// NOTE(randy): Grab all the keyframe timings
			i32 input_accessor_index = GetIntFromJObjectE(FindElementInJObject(sampler, "input"));
			JObject *input_accessor = FindArrayElementAtIndex(accessors, input_accessor_index)->value->payload;
			
			i32 key_frame_count = GetIntFromJObjectE(FindElementInJObject(input_accessor, "count"));
			
			i32 input_buffer_view_index = GetIntFromJObjectE(FindElementInJObject(input_accessor, "bufferView"));
			JObject *input_buffer_view = FindArrayElementAtIndex(buffer_views_array, input_buffer_view_index)->value->payload;
			i32 input_byte_length = GetIntFromJObjectE(FindElementInJObject(input_buffer_view, "byteLength"));
			i32 input_byte_offset = GetIntFromJObjectE(FindElementInJObject(input_buffer_view, "byteOffset"));
			Assert(input_byte_length == sizeof(f32) * key_frame_count);
			
			fseek(f, input_byte_offset, SEEK_SET);
			f32 key_frame_timings[MAX_KEY_FRAME_COUNT] = {0};
			for (i32 i = 0; i < key_frame_count; i++)
			{
				ReadFromFile(f, &key_frame_timings[i], sizeof(f32));
			}
			
			// NOTE(randy): Grab all the transform data from the keyframes
			i32 output_accessor_index = GetIntFromJObjectE(FindElementInJObject(sampler, "output"));
			JObject *output_accessor = FindArrayElementAtIndex(accessors, output_accessor_index)->value->payload;
			
			i32 key_frame_transform_count = GetIntFromJObjectE(FindElementInJObject(output_accessor, "count"));
			
			i32 output_buffer_view_index = GetIntFromJObjectE(FindElementInJObject(output_accessor, "bufferView"));
			JObject *output_buffer_view = FindArrayElementAtIndex(buffer_views_array, output_buffer_view_index)->value->payload;
			i32 output_byte_length = GetIntFromJObjectE(FindElementInJObject(output_buffer_view, "byteLength"));
			i32 output_byte_offset = GetIntFromJObjectE(FindElementInJObject(output_buffer_view, "byteOffset"));
			if (strcmp(path, "rotation") == 0)
			{
				Assert(output_byte_length == sizeof(v4) * key_frame_transform_count);
				Assert(key_frame_transform_count / 3 == key_frame_count); // NOTE(randy): Weird buffer padding in gltf
				
				fseek(f, output_byte_offset, SEEK_SET);
				v4 rotation_buffer[MAX_KEY_FRAME_COUNT * MAX_BONE_COUNT * 3] = {0};
				ReadFromFile(f, rotation_buffer, sizeof(v4) * key_frame_transform_count);
				
				// NOTE(randy): The above buffer is wierdly padded for some reason.
				v4 key_frame_rotations[MAX_KEY_FRAME_COUNT * MAX_BONE_COUNT] = {0};
				for (i32 i = 0; i < key_frame_count; i++)
				{
					key_frame_rotations[i] = rotation_buffer[i * 3 + 1];
				}
				
				// NOTE(randy): Populate key frames
				for (i32 i = 0; i < key_frame_count; i++)
				{
					KeyRotation *key_rot = &animation->rotations[bone_id][i];
					key_rot->rotation = key_frame_rotations[i];
					key_rot->time_stamp = key_frame_timings[i];
					animation->rotation_count++;
				}
			}
			else if (strcmp(path, "translation") == 0)
			{
				Assert(output_byte_length == sizeof(v3) * key_frame_transform_count);
				Assert(key_frame_transform_count / 3 == key_frame_count); // NOTE(randy): Weird buffer padding in gltf
				
				fseek(f, output_byte_offset, SEEK_SET);
				v3 translation_buffer[MAX_KEY_FRAME_COUNT * MAX_BONE_COUNT * 3] = {0};
				ReadFromFile(f, translation_buffer, sizeof(v3) * key_frame_transform_count);
				
				// NOTE(randy): The above buffer is wierdly padded for some reason.
				v3 key_frame_translations[MAX_KEY_FRAME_COUNT * MAX_BONE_COUNT] = {0};
				for (i32 i = 0; i < key_frame_count; i++)
				{
					key_frame_translations[i] = translation_buffer[i * 3 + 1];
				}
				
				// NOTE(randy): Populate key frames
				for (i32 i = 0; i < key_frame_count; i++)
				{
					KeyTranslation *key_rot = &animation->translations[bone_id][i];
					key_rot->translation = key_frame_translations[i];
					key_rot->time_stamp = key_frame_timings[i];
					animation->translation_count++;
				}
			}
			else if (strcmp(path, "scale") == 0)
			{
				
			}
			else
				Assert(0);
		}
	}
	
	fclose(f);
}