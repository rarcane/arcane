internal void TransformSkeleton(Ts2dSkeleton *skeleton)
{
	i32 animation_index = 0;
	
	for (i32 i = 0; i < TS2D_SKELETON_BONE_MAX; i++)
	{
		skeleton->bones[i].parent_index = -1;
	}
	
	Mesh *mesh = &core->tsm.meshes[0];
	for (i32 i = 0; i < mesh->bone_count; i++)
	{
		BoneInfo *bone_info = &mesh->bone_infos[i];
		skeleton->bones[i].parent_index = bone_info->parent_index;
		
		m4 transform = M4InitD(1.0f);
		
		BoneKeyFrames *key_frames = &bone_info->key_frames[animation_index];
		
		v3 position = key_frames->key_positions[0].position;
		
		if (bone_info == &mesh->bone_infos[mesh->root_bone_index])
		{
			position = V3MultiplyF32(position, .001f);
		}
		
		if (platform->key_down[KEY_t])
		{
			/*
						transform = M4MultiplyM4(transform, M4MultiplyM4(mesh->inverse_root_matrix, M4InitD(100.f)));
			 */
			transform = M4MultiplyM4(transform, M4TranslateV3(position));
		}
		
		// TODO(randy): Transform actin funni. Translation animation is in some weird space.
		// The transform on the spine is very large for some reason.
		
		
		/*
				v4 rot = key_frames->key_rotations[key_frames->key_rotation_count - 1].rotation;
				transform = M4MultiplyM4(transform, M4RotateQuat(rot));
		 */
		
		
		
		skeleton->bones[i].transform = transform;
	}
}