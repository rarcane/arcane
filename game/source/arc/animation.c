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
		
		m4 parent_transform = M4InitD(1.0f);
		if (bone_info->parent_index >= 0)
		{
			// NOTE(randy): Loop through parents, applying their transforms
			for (BoneInfo *parent = &mesh->bone_infos[bone_info->parent_index]; 1; parent = &mesh->bone_infos[parent->parent_index])
			{
				parent_transform = M4MultiplyM4(parent->transform, parent_transform);
				
				if (parent->parent_index == -1)
				{
					break;
				}
			}
		}
		
		// TODO(randy): Rotations are being applied bc I have no clue how to not apply them yay
		// This shouldn't matter though, they're local rotations, once we slap on the inverse to get it back to the world origin, it rly shouldn't make a diff?
		
		BoneKeyFrames *key_frames = &bone_info->key_frames[animation_index];
		
		// this position is relative to the bone's parent transform
		v3 position = key_frames->key_positions[0].position;
		m4 current_transform = M4TranslateV3(position);
		
		m4 global_bone_transform;
		if (!platform->key_down[KEY_1])
			global_bone_transform = M4MultiplyM4(parent_transform, current_transform);
		else
			global_bone_transform = M4MultiplyM4(current_transform, parent_transform);
		
		//global_bone_transform = M4MultiplyM4(current_transform, parent_transform);
		
		if (platform->key_down[KEY_t])
		{
			if (!platform->key_down[KEY_2])
				transform = M4MultiplyM4(transform, global_bone_transform);
			else
				transform = M4MultiplyM4(global_bone_transform, transform);
			
			if (!platform->key_down[KEY_3])
				transform = M4MultiplyM4(transform, bone_info->offset);
			else
				transform = M4MultiplyM4(bone_info->offset, transform);
			
			/*
						if (!platform->key_down[KEY_4])
							transform = M4MultiplyM4(transform, mesh->inverse_root_matrix);
						else
							transform = M4MultiplyM4(mesh->inverse_root_matrix, transform);
			 */
			
			// TODO(randy): Figure out how tf to multiply this transform out so it's in the proper space
		}
		
		//v4 rot = key_frames->key_rotations[key_frames->key_rotation_count - 1].rotation;
		/*
				transform = M4MultiplyM4(transform, M4RotateQuat(rot));
		 */
		
		skeleton->bones[i].transform = transform;
	}
}