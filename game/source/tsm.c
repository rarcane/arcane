typedef struct json_object_s JObject;
typedef struct json_value_s JValue;
typedef struct json_string_s JString;
typedef struct json_object_element_s JObjectE;
typedef struct json_array_s JArray;
typedef struct json_array_element_s JArrayE;

JObjectE *FindElementInJObject(JObject *object, char *element_name)
{
	for (JObjectE *next = object->start; next; next = next->next)
	{
		if (strcmp(next->name->string, element_name) == 0)
		{
			return next;
		}
	}
	
	Assert(0);
	return 0;
}

JArrayE *FindArrayElementAtIndex(JArray *array, i32 index)
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

InitTSMFromGLTFFile(TSM *tsm, char *path)
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
	
	// NOTE(randy): Skeleton
	// TODO(randy): Parse in bones and create a hierachy
	
	// NOTE(randy): Mesh stuff
	JArray *mesh_array = FindElementInJObject(object, "meshes")->value->payload;
	JArray *accessor_array = FindElementInJObject(object, "accessors")->value->payload;
	JArray *buffer_views_array = FindElementInJObject(object, "bufferViews")->value->payload;
	
	JObject *mesh_0 = FindArrayElementAtIndex(mesh_array, 0)->value->payload;
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
	
	// NOTE(randy): Vertex weights
	i32 weights_accessor_index = atoi(json_value_as_number(FindElementInJObject(attributes, "WEIGHTS_0")->value)->number);
	JObject *weights_accessor = FindArrayElementAtIndex(accessor_array, weights_accessor_index)->value->payload;
	i32 weights_buffer_view_index = atoi(json_value_as_number(FindElementInJObject(weights_accessor, "bufferView")->value)->number);
	
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
	
	// NOTE(randy): Yoink index data
	fseek(f, index_byte_offset, SEEK_SET);
	for (u32 i = 0; i < tsm->index_count; i++)
	{
		u16 index;
		ReadFromFile(f, &index, sizeof(u16));
		tsm->indices[i] = (u32)index;
	}
	
	fclose(f);
}