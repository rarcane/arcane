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
		
		m4 transform = M4InitD(1.0f);
		
		m4 offset = bone->offset;
		m4 parent_trans = bone->transform;
		
		// TODO(randy): Try out the new proper M4RotateQuat. It should be good now to compute the whole matrix.
		
		skeleton->bones[i].transform = transform;
	}
}