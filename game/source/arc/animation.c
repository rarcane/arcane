internal void TransformSkeleton(Ts2dSkeleton *skeleton)
{
	i32 animation_index = 0;
	
	for (i32 i = 0; i < TS2D_SKELETON_BONE_MAX; i++)
	{
		skeleton->bones[i].parent_index = -1;
	}
	
	for (i32 i = 0; i < core->tsm.meshes[0].bone_count; i++)
	{
		BoneInfo *bone_info = &core->tsm.meshes[0].bone_infos[i];
		skeleton->bones[i].parent_index = bone_info->parent_index;
		
		m4 transform = M4InitD(1.0f);
		
		BoneKeyFrames *key_frames = &bone_info->key_frames[animation_index];
		
		//v3 position = key_frames->key_positions[key_frames->key_position_count - 1].position;
		//v4 rot = key_frames->key_rotations[key_frames->key_rotation_count - 1].rotation;
		
		skeleton->bones[i].transform = transform;
	}
}