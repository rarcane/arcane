TSDEVTERMINAL_COMMAND_PROC(SaveLevelCommand)
{
	if (core->run_data->world_name[0])
		SaveWorld();
	else
		LogError("No level is currently active.");
	
	return 0;
}

TSDEVTERMINAL_COMMAND_PROC(LoadLevelCommand)
{
	if (word_count == 0)
		LogError("Please provide a level name.");
	else if (!LoadWorld(words[0]))
		LogError("Failed to load world \'%s\'", words[0]);
	
	return 0;
}

TSDEVTERMINAL_COMMAND_PROC(DrawCollidersCommand)
{
	GetEditorData()->debug_flags ^= DEBUG_FLAGS_draw_collision;
	
	return 0;
}