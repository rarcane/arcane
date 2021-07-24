internal void TransformSkeletonFromTSM(Ts2dSkeleton *skeleton, TSM *model)
{
	i32 animation_index = 0;
	
	for (i32 i = 0; i < TS2D_SKELETON_BONE_MAX; i++)
	{
		skeleton->bones[i].parent_index = -1;
	}
	
	// NOTE(randy): Take bone data from the model and put it into a neat lil Ts2dSkeleton
	for (i32 i = 0; i < model->bone_count; i++)
	{
		BoneInfo *bone_info = &model->bone_infos[i];
		skeleton->bones[i].parent_index = bone_info->parent_index;
		
		m4 transform = M4InitD(1.0f);
		
		skeleton->bones[i].transform = transform;
		
		/*
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
		 */
		
		// TODO(randy): Rotations are being applied bc I have no clue how to not apply them yay
		// This shouldn't matter though, they're local rotations, once we slap on the inverse to get it back to the world origin, it rly shouldn't make a diff?
		
		/*
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
					
					// TODO(randy): Figure out how tf to multiply this transform out so it's in the proper space
				}
		 */
	}
}