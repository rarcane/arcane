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
		Bone *bone = &model->bones[i];
		skeleton->bones[i].parent_index = bone->parent_index;
		
		// TODO(randy): i hav no fucking clue what is happening.
		
		m4 global_joint_trans;
		if (platform->key_down[KEY_u])
			global_joint_trans = M4MultiplyM4(bone->transform, model->global_transform);
		else
			global_joint_trans = model->global_transform;
		if (bone->parent_index >= 0)
		{
			// NOTE(randy): Loop through parents, applying their transforms
			for (Bone *parent = &model->bones[bone->parent_index]; 1; parent = &model->bones[parent->parent_index])
			{
				if (platform->key_down[KEY_y])
					global_joint_trans = M4MultiplyM4(global_joint_trans, parent->transform);
				else
					global_joint_trans = M4MultiplyM4(parent->transform, global_joint_trans);
				
				if (parent->parent_index == -1)
				{
					break;
				}
			}
		}
		
		m4 inv_global = M4Inverse(model->global_transform);
		m4 offset = bone->offset;
		
		if (platform->key_down[KEY_t])
		{
			m4 transform = M4MultiplyM4(global_joint_trans, offset);
			transform = M4MultiplyM4(transform, inv_global);
			skeleton->bones[i].transform = transform;
		}
		else
		{
			m4 transform = M4MultiplyM4(global_joint_trans, offset);
			skeleton->bones[i].transform = transform;
		}
		
		
		
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