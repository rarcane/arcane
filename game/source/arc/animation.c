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
			// TODO(randy): This transform should match the inverse bind matrix in order for it to cancel out.
			
			
			skeleton->bones[0].transform = transform;
		}
	 */
	
	/*
		v2 initial_pos = v2(200.f, 400.f);
		
		Ts2dPushLine(v4u(1.0f), , v2(100.f, 100.f));
	 */
	
	
	
	// TODO(randy): figure out how to do this for the entire hierarchy
	for (i32 i = 0; i < 1; i++)
	{
		Bone *bone = &model->bones[i];
		
		m4 inverse_bind_matrix = bone->inverse_bind_matrix;
		
		m4 current_global_transform = model->global_transform;
		
		//skeleton->bones[i].transform = M4MultiplyM4(M4MultiplyM4(bone->transform, inverse_bind_matrix), current_global_transform);
		
		// TODO(randy): use the right bones in this transform thingo
		skeleton->bones[i].transform = bone->transform;
	}
}

