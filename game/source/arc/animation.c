internal void TransformSkeletonFromTSM(Ts2dSkeleton *skeleton, TSM *model)
{
	i32 animation_index = 0;
	
	for (i32 i = 0; i < TS2D_SKELETON_BONE_MAX; i++)
	{
		skeleton->bones[i].parent_index = -1;
		skeleton->bones[i].transform = M4InitD(1.0f);
	}
	
	// NOTE(randy): Take bone data from the model and put it into a neat lil Ts2dSkeleton
	for (i32 i = 0; i < model->bone_count; i++)
	{
		Bone *bone = &model->bones[i];
		skeleton->bones[i].parent_index = bone->parent_index;
	}
	
	/*
		{
			Bone *bone = &model->bones[0];
			
			m4 inverse_bind_matrix = bone->inverse_bind_matrix;
			m4 bone_transform = bone->transform;
			
			m4 transform = M4MultiplyM4(bone_transform, M4Inverse(global_transform));
			
skeleton->bones[0].transform = transform;
		}
	 */
	
	/*
		v2 initial_pos = v2(200.f, 400.f);
		
		Ts2dPushLine(v4u(1.0f), , v2(100.f, 100.f));
	 */
	
	
	
	for (i32 i = 0; i < model->bone_count; i++)
	{
		Bone *bone = &model->bones[i];
		
		m4 current_global_transform = M4MultiplyM4(model->root_transform, bone->local_transform);
		
		Bone *next_bone = bone;
		while (next_bone->parent_index != -1)
		{
			Bone *parent_bone = &model->bones[next_bone->parent_index];
			
			current_global_transform = M4MultiplyM4(parent_bone->local_transform, current_global_transform);
			
			next_bone = parent_bone;
		}
		
		// NOTE(randy): Pretty sure the inverse bind matrix from gltf is the same as this, chief.
		m4 inverse_bind_matrix = bone->inverse_bind_matrix;//M4Inverse(current_global_transform);
		
		m4 transform = M4MultiplyM4(current_global_transform, inverse_bind_matrix);
		
		skeleton->bones[i].transform = transform;
	}
}

