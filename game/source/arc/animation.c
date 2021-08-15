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
	
	i32 key_frame = 0;
	
	for (i32 i = 0; i < model->bone_count; i++)
	{
		Bone *bone = &model->bones[i];
		
		Transform anim_local = {
			model->animations[0].rotations[i][key_frame].rotation,
			model->animations[0].translations[i][key_frame].translation,
			1.0f
		};
		m4 anim_local_mat = GetM4FromTransform(anim_local);
		
		//m4 current_global_transform = M4MultiplyM4(model->root_transform, GetM4FromTransform(bone->local_transform));
		m4 current_global_transform = M4MultiplyM4(model->root_transform, anim_local_mat);
		
		Bone *next_bone = bone;
		while (next_bone->parent_index != -1)
		{
			Bone *parent_bone = &model->bones[next_bone->parent_index];
			
			Transform parent_anim_local = {
				model->animations[0].rotations[parent_bone->id][key_frame].rotation,
				model->animations[0].translations[parent_bone->id][key_frame].translation,
				1.0f
			};
			m4 parent_anim_local_mat = GetM4FromTransform(parent_anim_local);
			
			// TODO(randy): Figure out what the translation data looks like
			
			//current_global_transform = M4MultiplyM4(GetM4FromTransform(parent_bone->local_transform), current_global_transform);
			current_global_transform = M4MultiplyM4(parent_anim_local_mat, current_global_transform);
			
			next_bone = parent_bone;
		}
		
		m4 inverse_bind_matrix = bone->inverse_bind_matrix;
		
		m4 transform = M4MultiplyM4(current_global_transform, inverse_bind_matrix);
		
		skeleton->bones[i].transform = transform;
	}
}

